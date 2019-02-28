//
//  DownImg.h
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/7.
//
//

#ifndef DownImg_h
#define DownImg_h

#include "network/CCDownloader.h"
#include "cocos2d.h"
USING_NS_CC;

using namespace std;

class COneDownload;

enum eBookResType
{
    eBookResNaN=-1, //无效资源
    eBookResPng,    //Pdf图片
    eBookResSnd,    //朗读音效
    eBookResOth,    //其他资源
    eBookResInf,    //信息概览-二期原版阅读
    eBookResAct,    //角色信息-二期原版阅读
    eBookResLyr,    //歌词信息-二期原版阅读
    
    eBookResCard,   //卡牌
    
    eBookResCnt,
};

typedef struct PageResDown
{
    int tag;
    eBookResType resType;
    string fileName;
    COneDownload* oneDown;
}stuPageResDown;

typedef struct ImgResDown
{
    Node* node;
    COneDownload* oneDown;
}stuImgResDown;

typedef struct CardResDown
{
    string p_id;
    string zip;
    COneDownload* oneDown;
}stuCardResDown;


class CDownloadDelegate
{
public:
    CDownloadDelegate();
    virtual ~CDownloadDelegate();
    virtual void endDownload(string& url, string& file, int iTag = -1, eBookResType resType = eBookResNaN){};
    virtual void endDownloadFail(string& url, string& file, int iTag = -1, eBookResType resType = eBookResNaN){};
};


class DownImg
{
//    typedef struct DownTask
//    {
//        string requestURL;
//        string storagePath;
//        string identifier;
//        int failCount;
//        bool bDown;
//    }stuDownTask;
    
public:
    static DownImg* getInstance();
    
    DownImg();
    ~DownImg();
    void DownFile(const char* url, Node* imgNode, const char* defFile = nullptr);
    void onDownOk(const string &file);
    string getLocalFilePath(const char* url);
    
    void DownBookFile(const char* url, const char* folder, int iTag = -1, eBookResType resType = eBookResNaN);
    void onBookResOk(const string &file);
    void onBookResFail(const string &file);
    
    string getBookFilePath(const char* url, const char* folder);
    
    void registCallback(CDownloadDelegate *p);
    void unregistCallback(CDownloadDelegate *p);
    
//    void downFailFile();
    
    void setImg(Node* imgNode, const string& filePath);
    
    static void createDir();
    
    void onTimer();
    
    void DownCardFile(const char* url, const char* folder);
    
    void onCardResOk(const string &file);
    void onCardResFail(const string &file);

    void onCardUnzipOk(const string &file);
    void onCardUnzipFail(const string &file);
    
    void checkBookVersion(const string &bookId, const string &version);
    
    void addSameBookFile(const char *url, const char *folder, int iTag, eBookResType resType);
    void onSameBookFileOk(const string &file, const string &filePathName);
    void onSameBookFileFail(const string &file);
    
    void DownFileIdle(const char *url);
private:
    unique_ptr<network::Downloader> m_downloader;
    multimap<string, ImgResDown*> m_imgMap;
    string m_wPath;
    
    string m_bookRoot;
    map<string, stuPageResDown*> m_bookDownMap;
    multimap<string, stuPageResDown*> m_bookDownSame;
    
    map<CDownloadDelegate*, bool>m_callbackMap;
    
//    map<string, stuDownTask*>m_failTaskMap;
    
    map<string, stuCardResDown*> m_cardDownMap;
    vector<string> m_unzipList;
    
    vector<string> m_idleDownList;
};

class DownManyFile : public CDownloadDelegate
{
//private:
//    struct DownFileStatus {
//        string url;
//        int status;// -1: err, 0:downing, 1:ok
//        string localFile;
//    };
public:
    virtual void endDownload(string& url, string& file, int iTag = -1, eBookResType resType = eBookResNaN);
    virtual void endDownloadFail(string& url, string& file, int iTag = -1, eBookResType resType = eBookResNaN);
public:
    void setDownFiles(const string& folder, const vector<string> urlList);
    void startDown();
    void setCallbackFunc(const function<void ()> okCall, const function<void (const string &)> errorCall = nullptr);
    const char * getLocalFile(const string& url);
private:
    function<void ()> m_okCall;
    function<void (const string &)> m_errorCall;
    string m_folder;
    map<string, string> m_urlList;
};

#endif /* DownImg_h */
