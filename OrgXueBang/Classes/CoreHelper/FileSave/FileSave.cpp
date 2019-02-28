#include "PreIncludes.hpp"
#include "FileSave.h"
#include "md5code.h"

#define MYDEF_KEY_NUM 3
#define MYDEF_FILESAVE_DIR "UserSave/"
#define MYDEF_UserInfoSave "stuUserInfo_t.save"

/*保存文件-加密*/
eFileSaveResult CFileSave::EncryptSave(const std::string& file1,const unsigned char* pDat,unsigned int nLen,const std::string key1)
{
    string file = file1;
    if (file1.length() == 0)
        file = MYDEF_UserInfoSave;
    string key = key1;
    if (key1.length() == 0) {
        key = MYDEF_UserInfoSave;
    }
    std::string path = FileUtils::getInstance()->getWritablePath() + MYDEF_FILESAVE_DIR + file;
    auto pFileUtils = FileUtils::getInstance();
    //将数据加密
    
    //生成KEY
    unsigned char nkey[MYDEF_KEY_NUM];
    memset(nkey, 0, sizeof(nkey));
    for (int i = 0 ; i < MYDEF_KEY_NUM * file.length(); i++)
    {
        nkey[i % MYDEF_KEY_NUM] += (unsigned char)file.c_str()[i % file.length()];
    }
    //数据加密
    
    unsigned char* p = (unsigned char*)malloc(nLen + 32);
    memcpy(p, pDat, nLen);
    for (int i = 0; i < nLen; i++)
    {
        p[i] += nkey[i % MYDEF_KEY_NUM];
    }
    //数据算MD5
    CMd5Code md5;
    md5.GenerateMD5((unsigned char*)pDat, nLen);
    std::string strMd5 = md5.ToString();
    
    //md5(key+数据md5)
    strMd5 = key + strMd5;
    md5.GenerateMD5((unsigned char*)strMd5.c_str(), (int)strMd5.length());
    strMd5 = md5.ToString();
    
    //存储数据
    memcpy(p + nLen, strMd5.c_str(), 32);
    Data data;
    data.fastSet(p, nLen + 32);
    if(pFileUtils->writeDataToFile(data,path))
        return eFileSaveResult::ok;
    return eFileSaveResult::error;
}

eFileSaveResult CFileSave::EncryptSave(const std::string& file,const Data& data,const std::string key)
{
    return  EncryptSave(file,data.getBytes(),(unsigned int)data.getSize(),key);
   
}
/*加载文件-加密*/
eFileSaveResult CFileSave::EncryptLoad(const std::string& file1,Data& data,const std::string key1)
{
    string file = file1;
    if (file1.length() == 0)
        file = MYDEF_UserInfoSave;
    string key = key1;
    if (key1.length() == 0) {
        key = MYDEF_UserInfoSave;
    }
    std::string path = FileUtils::getInstance()->getWritablePath() + MYDEF_FILESAVE_DIR + file;
    auto pFileUtils = FileUtils::getInstance();
    
    char szMd5[33];
    
    //生成KEY
    unsigned char nkey[MYDEF_KEY_NUM];
    memset(nkey, 0, sizeof(nkey));
    for (int i = 0 ; i < MYDEF_KEY_NUM * file.length(); i++)
    {
        nkey[i % MYDEF_KEY_NUM] += (unsigned char)file.c_str()[i % file.length()];
    }
    //读取文件
    Data dataFile = pFileUtils->getDataFromFile(path);
    if (dataFile.getSize() <= 32) {
        return eFileSaveResult::notExist;
    }
    //内容
    data.copy(dataFile.getBytes(), dataFile.getSize() - 32);
    //md5
    memset(szMd5, 0, 33);
    memcpy(szMd5, dataFile.getBytes() + dataFile.getSize() - 32, 32);
    //循环解密
    unsigned char* p = data.getBytes();
    int nLen = (int)dataFile.getSize() - 32;
    for (int i = 0; i < nLen; i++)
    {
        p[i] -= nkey[i % MYDEF_KEY_NUM];
    }
    
    
    //数据算MD5
    CMd5Code md5;
    md5.GenerateMD5((unsigned char*)p, nLen);
    std::string strMd5 = md5.ToString();
    
    //md5(key+数据md5)
    strMd5 = key + strMd5;
    md5.GenerateMD5((unsigned char*)strMd5.c_str(), (int)strMd5.length());
    strMd5 = md5.ToString();

    if (strMd5 == szMd5)
    {//MD5加密
        return eFileSaveResult::ok;
    }
    dataFile.clear();
    return eFileSaveResult::wrongKey;
}


/*保存文件-不加密*/
eFileSaveResult CFileSave::Save(const std::string& file1,const unsigned char* pDat,unsigned int nLen)
{
    string file = file1;
    if (file1.length() == 0)
        file = MYDEF_UserInfoSave;
    Data data;
    data.copy(pDat, nLen);
    return Save(file,data);
}

eFileSaveResult CFileSave::Save(const std::string& file1,const Data& data)
{
    string file = file1;
    if (file1.length() == 0)
        file = MYDEF_UserInfoSave;
    std::string path = FileUtils::getInstance()->getWritablePath() + MYDEF_FILESAVE_DIR + file;
    auto pFileUtils = FileUtils::getInstance();
    if(pFileUtils->writeDataToFile(data,path))
        return eFileSaveResult::ok;
    return eFileSaveResult::error;
    
}

/*加载文件-不加密*/
eFileSaveResult CFileSave::Load(const std::string& file1,Data& data)
{
    string file = file1;
    if (file1.length() == 0)
        file = MYDEF_UserInfoSave;
    std::string path = FileUtils::getInstance()->getWritablePath() + MYDEF_FILESAVE_DIR + file;
    data = FileUtils::getInstance()->getDataFromFile(path);
    if (data.getSize() <= 0)
        return eFileSaveResult::notExist;
    return eFileSaveResult::ok;
}

bool CFileSave::createSaveDir()
{
    std::string path = FileUtils::getInstance()->getWritablePath() + MYDEF_FILESAVE_DIR;
    CCLOG("save path = %s", path.c_str());
    auto pFileUtils = FileUtils::getInstance();
    
    pFileUtils->createDirectory(path);
//    while(!pFileUtils->isDirectoryExist(path))
//        ;
    return true;
}

void CFileSave::deleteSaveFile()
{
    std::string path = FileUtils::getInstance()->getWritablePath() + MYDEF_FILESAVE_DIR + MYDEF_UserInfoSave;
    if (FileUtils::getInstance()->isFileExist(path)) {
        auto ret = FileUtils::getInstance()->removeFile(path.c_str());
        if (ret) {
            CCLOG("deleteSaveFile ret = %d", ret);
        }
    }
    
}


