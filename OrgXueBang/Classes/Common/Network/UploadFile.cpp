//
//  UploadFile.cpp
//  ReadingFriends
//
//  Created by liuwei on 16/7/6.
//
//

//#include "stdafx.h"
#include "UploadFile.h"

#include "cocos2d.h"
USING_NS_CC;

#include "StringUtil.hpp"
#include "HttpManager.h"
#include "assist.h"

#include "CDataHttp.hpp"

#include "../../cocos2d/external/curl/include/ios/curl/curl.h"

#define CC_BREAK_IFNOT(cond)           if(cond == false) break


static size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    size_t sizes = size * nmemb;
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
    return sizes;
}
static size_t writeHeaderData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    size_t sizes = size * nmemb;
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
    return sizes;
}


static int uploadProgressFunc(void *pData, double dltotal, double dlnow, double ultotal, double ulnow)
{
    auto callBack = (std::function<void(double)>*)pData;
    if (callBack && ultotal != 0)
    {
        (*callBack)(ulnow / ultotal * 100);
    }
    return 0;
}


class CURLWei
{
    CURL *_curl;
public:
    CURLWei()
    : _curl(curl_easy_init())
    {
    }
    ~CURLWei()
    {
        if (_curl)
            curl_easy_cleanup(_curl);
    }
    
    template <class T>
    bool setOption(CURLoption option, T data)
    {
        return CURLE_OK == curl_easy_setopt(_curl, option, data);
    }
    

    bool perform()
    {
        if (CURLE_OK != curl_easy_perform(_curl))
            return false;
        
        long tmp = 0;
        long *responseCode = &tmp;
        CURLcode code = curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, responseCode);
        if (code != CURLE_OK || !(*responseCode >= 200 && *responseCode < 300)) {
            CCLOGERROR("Curl curl_easy_getinfo failed: %s", curl_easy_strerror(code));
            return false;
        }
        return true;
    }
};


static bool configureCURL(stuUpload* pUpload, int timeout)
{
    do
    {
        CURLWei curl;
        
        //form
        curl_httppost* post = nullptr;
        curl_httppost* last = nullptr;
        vector<stuFileInfo>::iterator it_files;
        for (it_files = pUpload->m_vecFiles.begin(); it_files != pUpload->m_vecFiles.end(); it_files++) {
            stuFileInfo info = *it_files;
            
            curl_formadd(&post, &last,
                         CURLFORM_COPYNAME,     info.m_strFileCopyName.c_str(),
                         CURLFORM_FILE,         info.m_strFileName.c_str(),
                         CURLFORM_CONTENTTYPE,  info.m_strFileContentType.c_str(),
                         CURLFORM_END);
        }
        
        
        // form post data
        std::string strPost = "";
        std::string strMD5 = "";
        KVPairVec::iterator iter;
        for (iter = pUpload->m_mapPostValue->begin(); iter != pUpload->m_mapPostValue->end(); iter++) {
            KVPair* kv = *iter;
            
//            curl_formadd(&post, &last,
//                         CURLFORM_COPYNAME,
//                         kv->m_strKey.c_str(),
//                         CURLFORM_COPYCONTENTS,
//                         StringUtil::UrlEncode(kv->m_strValue).c_str(),
//                         CURLFORM_END);
//            strPost += kv->m_strKey;
//            strPost += '=';
//            strPost += StringUtil::UrlEncode(kv->m_strValue);
//            strPost += '&';
//            strMD5 += StringUtil::UrlEncode(kv->m_strValue);
            
            curl_formadd(&post, &last,
                         CURLFORM_COPYNAME,
                         kv->m_strKey.c_str(),
                         CURLFORM_COPYCONTENTS,
                         kv->m_strValue.c_str(),
                         CURLFORM_END);
            strPost += kv->m_strKey;
            strPost += '=';
            strPost += kv->m_strValue;
            strPost += '&';
            strMD5 += kv->m_strValue;
        }
        
        //单独对MD5串加post参
        HttpManager::getMd5str(strMD5);
        curl_formadd(&post, &last,
                     CURLFORM_COPYNAME, "signcode",
                     CURLFORM_COPYCONTENTS, strMD5.c_str(),
                     CURLFORM_END);
        
        strPost = strPost + "signcode=" + strMD5;
        
        //再用post数据来一次,随便他取
        if (!strPost.empty())
        {
            strPost = strPost.substr(0, strPost.length());
            CC_BREAK_IFNOT(curl.setOption(CURLOPT_POST,  1));
            CC_BREAK_IFNOT(curl.setOption(CURLOPT_POSTFIELDS,     strPost.c_str()));
            CC_BREAK_IFNOT(curl.setOption(CURLOPT_POSTFIELDSIZE,  strPost.size()));
        }
        
        
        //header
        struct curl_slist *headers = nullptr;
        for (const auto& it : pUpload->m_mapHeadValue)
        {
            std::string strContent = it.first + ":" + it.second;
            headers = curl_slist_append(headers, strContent.c_str());
        }
        //cookie
        std::string strCookie = CDataHttp::getInstance()->getCookie();
        if (!strCookie.empty())
        {
            strCookie.insert(0, "Cookie: ");
            headers = curl_slist_append(headers, strCookie.c_str());
        }
        CC_BREAK_IFNOT(curl.setOption(CURLOPT_HTTPHEADER,  headers));
        
        //url
//        convertUrlDataToString(pUpload->m_strUrl,                  pUpload->m_mapGetValue);
//        convertUrlDataToString(pUpload->m_strUrl,                  pUpload->m_mapPostValue);
        CC_BREAK_IFNOT(curl.setOption(CURLOPT_URL,                 pUpload->m_strUrl.c_str()));
        
        
        //opt
        CC_BREAK_IFNOT(curl.setOption(CURLOPT_TIMEOUT,             timeout));
        CC_BREAK_IFNOT(curl.setOption(CURLOPT_HTTPPOST,            post));
        CC_BREAK_IFNOT(curl.setOption(CURLOPT_WRITEFUNCTION,       writeData));
        CC_BREAK_IFNOT(curl.setOption(CURLOPT_WRITEDATA,           &pUpload->m_vecResponseData));
        CC_BREAK_IFNOT(curl.setOption(CURLOPT_HEADERFUNCTION,      writeHeaderData));
        CC_BREAK_IFNOT(curl.setOption(CURLOPT_HEADERDATA,          &pUpload->m_vecHeadData));
        CC_BREAK_IFNOT(curl.setOption(CURLOPT_PROGRESSFUNCTION,    uploadProgressFunc));
        CC_BREAK_IFNOT(curl.setOption(CURLOPT_PROGRESSDATA,        &(pUpload->m_callBackProgress)));
        
     
        //https
        std::string strTmp = pUpload->m_strUrl.substr(0, 5);
        if (strTmp == "https")
        {
            CC_BREAK_IFNOT(curl.setOption(CURLOPT_SSL_VERIFYPEER, 0));
            CC_BREAK_IFNOT(curl.setOption(CURLOPT_SSL_VERIFYHOST, 0));
        }
        
        
        CC_BREAK_IFNOT(curl.setOption(CURLOPT_VERBOSE,             1L));
        CC_BREAK_IFNOT(curl.setOption(CURLOPT_NOPROGRESS,          0L));
        
        CC_BREAK_IFNOT(curl.perform());
        
        return true;
    
    }while(0);
    return false;
}





UploadFile::UploadFile()
{
}

UploadFile::~UploadFile()
{
}



void UploadFile::uploadFile(uploadResponse callBack, const stuUpload& upload)
{
    m_dProgress = 0;
    
    stuUpload* pUpload = new stuUpload(upload.m_strUrl,
                                       upload.m_vecFiles,
                                       upload.m_mapHeadValue,
                                       upload.m_mapPostValue,
                                       upload.m_mapGetValue);
    
    pUpload->m_callBackProgress = CC_CALLBACK_1(UploadFile::setUploadProgress, this);
    pUpload->m_callBackFinsih   = [=](){
        
        std::string strHeader;
        strHeader.assign(pUpload->m_vecHeadData.begin(), pUpload->m_vecHeadData.end());
        std::string strResult;
        strResult.assign(pUpload->m_vecResponseData.begin(), pUpload->m_vecResponseData.end());
        callBack(strHeader, strResult);
    };
    
    std::thread(&UploadFile::_threadUpload, this, pUpload).detach();
}



void UploadFile::_threadUpload(stuUpload* pUploadData)
{
    if (!pUploadData)
    {
        return;
    }
    
    configureCURL(pUploadData, 30);
 
    //一定放到主线程callback
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([pUploadData](){
        pUploadData->m_callBackFinsih();
        delete pUploadData;
    });
}





void UploadFile::setUploadProgress(double dProgress)
{
    std::lock_guard<std::mutex> lock(m_mutexProgress);
    m_dProgress = dProgress;
    log("upload : %.2f", dProgress);
}





double UploadFile::getUploadProgress()
{
    std::lock_guard<std::mutex> lock(m_mutexProgress);
    return m_dProgress;
}
