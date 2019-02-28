//
//  DownImg.cpp
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/7.
//
//

#include "DownImg.h"

#include "BaseDownload.h"
#include "OneDownload.h"

#include "md5code.h"

#include "ui/CocosGUI.h"
//#include "editor-support/cocostudio/CocoStudio.h"

//#include "NetworkReachabilityManager.h"

#include "BaseUnZipFile.h"

using namespace cocos2d::ui;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
static bool s_useDownloader = true;
#else
static bool s_useDownloader = false;
#endif

CDownloadDelegate::CDownloadDelegate()
{
    DownImg::getInstance()->registCallback(this);
}

CDownloadDelegate::~CDownloadDelegate()
{
    DownImg::getInstance()->unregistCallback(this);
}


#define NET_DOWN_DIR    "net_img/"
#define NET_DOWN_BOOK   "net_book/"
#define NET_DOWN_CARD   "net_book/net_card/"

static DownImg* s_pDown = NULL;
DownImg* DownImg::getInstance()
{
    if (s_pDown == NULL) {
        s_pDown = new DownImg();
    }
    return s_pDown;
}

DownImg::DownImg()
{
    m_wPath = FileUtils::getInstance()->getWritablePath();
//    CCLOG("writeablepath = %s", m_wPath.c_str());
    
    createDir();
    
    
    m_downloader.reset(new network::Downloader());
    m_downloader->onTaskProgress = [this](const network::DownloadTask& task,
                                          int64_t bytesReceived,
                                          int64_t totalBytesReceived,
                                          int64_t totalBytesExpected)
    {
//        float percent = float(totalBytesReceived * 100) / totalBytesExpected;
//        CCLOG("id = %s downloading data: %.1f%%[total %d KB]", task.identifier.c_str(), percent, int(totalBytesExpected/1024));
    };
    
//    m_downloader->onDataTaskSuccess = [this](const cocos2d::network::DownloadTask& task, vector<unsigned char>& data)
//    {
//        CCLOG("id = %s download data ok", task.identifier.c_str());
//    };
    m_downloader->onFileTaskSuccess = [this](const cocos2d::network::DownloadTask& task)
    {
//        CCLOG("id = %s download task ok", task.identifier.c_str());
        
        this->onDownOk(task.identifier);
        this->onBookResOk(task.identifier);
    };
    m_downloader->onTaskError = [this](const cocos2d::network::DownloadTask& task, int errorCode, int errorCodeInteral, const string& errorStr)
    {
        CCLOG("id = %s download error", task.requestURL.c_str());
        
        string file = task.identifier;
        if (m_imgMap.count(file) > 0) {
            auto beg = m_imgMap.lower_bound(file);
            beg->second->oneDown = new COneDownload(task.requestURL, task.storagePath);
            return;
        }
        
        auto it = m_bookDownMap.find(file);
        if (it != m_bookDownMap.end()) {
            it->second->oneDown = new COneDownload(task.requestURL, task.storagePath);
            return;
        }
//        auto it = m_cardDownMap.find(file);
//        if (it == m_cardDownMap.end()) {
//            return;
//        }
    };
    
//    NetworkRecahabilityManager::shareManager()->startMonitoring();
    
//    Director::getInstance()->getScheduler()->schedule([=](float dt){
//        this->downFailFile();
//    }, this, 1.0f, false, "DownImg");
    
    Director::getInstance()->getScheduler()->schedule([=](float dt){
        this->onTimer();
    }, this, 0.3f, false, "DownImg");
    
//    DownCardFile("http://localhost/card/bathroom.zip", "");
}

void DownImg::DownFile(const char *url, Node *imgNode, const char *defFile)
{
    if (url == nullptr) {
        return;
    }
    CMd5Code md5;
    md5.GenerateMD5((unsigned char*)url, (int)strlen(url));
    string md5str = md5.ToString();
    string ext = FileUtils::getInstance()->getFileExtension(url);
    md5str += ext;
    string filePath = m_wPath + NET_DOWN_DIR + md5str;
    
    if (FileUtils::getInstance()->isFileExist(filePath)) {
        setImg(imgNode, filePath);
        return;
    } else {
        if (defFile) {
            auto img = dynamic_cast<ImageView*>(imgNode);
            if (img) {
                img->loadTexture(defFile, Widget::TextureResType::LOCAL);
//                img->loadTexture(defFile, Widget::TextureResType::PLIST);//add by hyp
            }
        }
    }
    
    if (m_imgMap.count(md5str) < 1) {
        if (s_useDownloader) {
            m_downloader->createDownloadFileTask(url, filePath, md5str);
            auto imgDown = new ImgResDown;
            imgDown->oneDown = nullptr;//new COneDownload(url, filePath);
            imgDown->node = imgNode;
            m_imgMap.insert(pair<string, ImgResDown*>(md5str, imgDown));
        } else {
//            m_downloader->createDownloadFileTask(url, filePath, md5str);
            auto imgDown = new ImgResDown;
            imgDown->oneDown = new COneDownload(url, filePath);
            imgDown->node = imgNode;
            m_imgMap.insert(pair<string, ImgResDown*>(md5str, imgDown));
        }
        
    } else {
        auto beg = m_imgMap.lower_bound(md5str);
        auto end = m_imgMap.upper_bound(md5str);
        for (auto m = beg; m != end; m++) {
            if (m->second->node == imgNode) {
                return;
            }
        }
        auto imgDown = new ImgResDown;
        imgDown->oneDown = nullptr;
        imgDown->node = imgNode;
        m_imgMap.insert(pair<string, ImgResDown*>(md5str, imgDown));
    }
    if (imgNode) {
//        CCLOG("set onExit node = %x", imgNode);
        imgNode->setOnExitCallback([=](){
//            CCLOG("run onExit node = %x", imgNode);
            auto beg = m_imgMap.lower_bound(md5str);
            auto end = m_imgMap.upper_bound(md5str);
            for (auto m = beg; m != end; m++) {
                if (m->second->node == imgNode) {
                    m->second->node = nullptr;
//                    m_imgMap.erase(m);
                    return;
                }
            }
        });
    }
}

DownImg::~DownImg()
{
    m_imgMap.clear();
}

void DownImg::onDownOk(const string &file)
{
    if (m_imgMap.count(file) < 1) {
        return;
    }
    string filePath = m_wPath + NET_DOWN_DIR + file;
    if (!FileUtils::getInstance()->isFileExist(filePath)) {
        return;
    }
    
    auto beg = m_imgMap.lower_bound(file);
    auto end = m_imgMap.upper_bound(file);
    for (auto m = beg; m != end; m++) {
        Node* imgNode = m->second->node;
        if (imgNode) {
            setImg(imgNode, filePath);
        }
        if (m->second->oneDown) {
            delete m->second->oneDown;
        }
    }
    
    m_imgMap.erase(beg, end);
}

void DownImg::DownBookFile(const char *url, const char *folder, int iTag, eBookResType resType)
{
    CCLOG("book down url = %s", url);
    string filePath = m_wPath + NET_DOWN_BOOK + folder + "/";
    string urlPath = url;
    string fileName = "";
    string filePathName = "";
//    size_t pos = urlPath.find_last_of("/");
//    if (pos != std::string::npos)
//    {
//        fileName = urlPath.substr(pos+1);
//    } else {
//        CCLOG("url = %s can not find file name", url);
//        return;
//    }
//    filePathName = filePath + fileName;
    filePathName = getBookFilePath(url, folder);
    if (filePathName.length() < 1) {
        CCLOG("url = %s can not find file name", url);
        return;
    }
    if (FileUtils::getInstance()->isFileExist(filePathName)) {
//        CCLOG("url = %s, file exist", url);
        for (auto it = m_callbackMap.begin(); it != m_callbackMap.end(); it++) {
            it->first->endDownload(urlPath, filePathName, iTag, resType);
        }
        return;
    }
    
    FileUtils::getInstance()->createDirectory(filePath);
    
    auto it = m_bookDownMap.find(urlPath);
    if (it != m_bookDownMap.end()) {
//        CCLOG("url = %s, is downloading", url);
        if (it->second->fileName != filePathName) {
            addSameBookFile(url, filePathName.c_str(), iTag, resType);
        }
        return;
    }
    
    if (s_useDownloader) {
        m_downloader->createDownloadFileTask(url, filePathName, urlPath);
        stuPageResDown* res = new stuPageResDown;
        res->tag = iTag;
        res->resType = resType;
        res->fileName = filePathName;
        res->oneDown = nullptr;//new COneDownload(url, filePathName);
        m_bookDownMap.insert(pair<string, stuPageResDown*>(urlPath, res));
    } else {
//        m_downloader->createDownloadFileTask(url, filePathName, urlPath);
        stuPageResDown* res = new stuPageResDown;
        res->tag = iTag;
        res->resType = resType;
        res->fileName = filePathName;
        res->oneDown = new COneDownload(url, filePathName);
        m_bookDownMap.insert(pair<string, stuPageResDown*>(urlPath, res));
    }

}

void DownImg::onBookResOk(const string &file)
{
    auto it = m_bookDownMap.find(file);
    if (it == m_bookDownMap.end()) {
        return;
    }
    string url = it->first;
    stuPageResDown* res = it->second;
    string filePathName = res->fileName;
    int iTag = res->tag;
    eBookResType resType = res->resType;
    if (res->oneDown) {
        delete res->oneDown;
    }
    delete res;
    m_bookDownMap.erase(it);
    
    if (!FileUtils::getInstance()->isFileExist(filePathName)) {
        return;
    }
    
//    CCLOG("url = %s, is download ok", url.c_str());
    for (auto it = m_callbackMap.begin(); it != m_callbackMap.end(); it++) {
        it->first->endDownload(url, filePathName, iTag, resType);
    }
    
    onSameBookFileOk(file, filePathName);
}

void DownImg::registCallback(CDownloadDelegate *p)
{
    m_callbackMap[p] = true;
}

void DownImg::unregistCallback(CDownloadDelegate *p)
{
    m_callbackMap.erase(p);
}

string DownImg::getBookFilePath(const char *url, const char *folder)
{
    string filePath = m_wPath + NET_DOWN_BOOK + folder + "/";
    string urlPath = url;
    string fileName = "";
    string filePathName = "";
    size_t pos = urlPath.find_last_of("/");
    if (pos != std::string::npos)
    {
        fileName = urlPath.substr(pos+1);
    } else {
        CCLOG("url = %s can not find file name", url);
        return filePathName;
    }
    filePathName = filePath + fileName;
    return filePathName;
}

string DownImg::getLocalFilePath(const char *url)
{
    string filePath = "";
    if (url == nullptr) {
        return filePath;
    }
    CMd5Code md5;
    md5.GenerateMD5((unsigned char*)url, (int)strlen(url));
    string md5str = md5.ToString();
    string ext = FileUtils::getInstance()->getFileExtension(url);
    filePath = m_wPath + NET_DOWN_DIR + md5str + ext;
    return filePath;
}

//void DownImg::downFailFile()
//{
////    CCLOG("DownImg::downFailFile()");
//
////    if (! NetworkRecahabilityManager::shareManager()->isReachable()) {
////        return;
////    }
//
//    for (auto it : m_failTaskMap) {
//        if (it.second->failCount > 3 || it.second->bDown) {
//            continue;
//        }
//        it.second->bDown = false;
////        m_downloader->createDownloadFileTask(it.second->requestURL, it.second->storagePath, it.second->identifier);
//        break;
//    }
//}

void DownImg::setImg(cocos2d::Node *imgNode, const string &filePath)
{
    if (imgNode) {
        auto img = dynamic_cast<ImageView*>(imgNode);
        if (img) {
            img->loadTexture(filePath);
            if (img->getUserData()) {
                img->setVisible(true);
            }
            Node* sizeNode = (Node*)img->getUserObject();
            if (sizeNode) {
                auto imgSize = dynamic_cast<Scale9Sprite*>(img->getVirtualRenderer())->getSprite()->getTextureRect().size;
                auto panelSize = sizeNode->getContentSize();
//                CCLOG("imgSize = %f,%f", imgSize.width, imgSize.height);
//                CCLOG("panelSize = %f,%f", panelSize.width, panelSize.height);
                if (imgSize.width < 1 || panelSize.width < 1) {
                    return;
                }
                if (imgSize.width / imgSize.height > panelSize.width / panelSize.height) {
                    imgSize.height = imgSize.height * panelSize.width / imgSize.width;
                    imgSize.width = panelSize.width;
                } else {
                    imgSize.width = imgSize.width * panelSize.height / imgSize.height;
                    imgSize.height = panelSize.height;
                }
                img->setContentSize(imgSize);
                
                Helper::doLayout(img);
                
                auto img_BookBack = img->getChildByName("img_BookBack");
                if (img_BookBack) {
                    img_BookBack->setVisible(true);
                    imgSize.width += 10;
                    imgSize.height += 6;
                    img_BookBack->setContentSize(imgSize);
                    img_BookBack->setPositionX(img_BookBack->getPositionX() + 2);
                }
            }
        }
    }
}

void DownImg::createDir()
{
    string wPath = FileUtils::getInstance()->getWritablePath();
    
    FileUtils::getInstance()->createDirectory(wPath + NET_DOWN_DIR);
    FileUtils::getInstance()->createDirectory(wPath + NET_DOWN_BOOK);
    FileUtils::getInstance()->createDirectory(wPath + NET_DOWN_CARD);
    
    FileUtils::getInstance()->createDirectory(wPath + NET_DOWN_CARD + "img/");
    FileUtils::getInstance()->createDirectory(wPath + NET_DOWN_CARD + "ui/");
    FileUtils::getInstance()->createDirectory(wPath + NET_DOWN_CARD + "ui/card/");
    
    FileUtils::getInstance()->addSearchPath(wPath + NET_DOWN_CARD, true);
}


void DownImg::onBookResFail(const string &file)
{
    auto it = m_bookDownMap.find(file);
    if (it == m_bookDownMap.end()) {
        return;
    }
    string url = it->first;
    stuPageResDown* res = it->second;
    string filePathName = res->fileName;
    int iTag = res->tag;
    eBookResType resType = res->resType;
    if (res->oneDown) {
        delete res->oneDown;
    }
    delete res;
    m_bookDownMap.erase(it);
    
    CCLOG("url = %s, is download fail", url.c_str());
    for (auto it = m_callbackMap.begin(); it != m_callbackMap.end(); it++) {
        it->first->endDownloadFail(url, filePathName, iTag, resType);
    }
    
    onBookResFail(file);
}


void DownImg::onTimer()
{
    vector<string> okList, failList;
    
    for (auto m = m_imgMap.begin(); m != m_imgMap.end(); m++) {
        if (m->second->oneDown) {
            auto r = m->second->oneDown->getStatus();
            if (r == eBaseDownloadStatus::finish )
            {
                okList.push_back(m->first);
                
            }
            else if (r == eBaseDownloadStatus::error)
            {
                failList.push_back(m->first);
            }
        }
    }
    
    for (auto m : okList) {
        onDownOk(m);
    }
    
    for (auto m : failList) {
        auto beg = m_imgMap.lower_bound(m);
        auto end = m_imgMap.upper_bound(m);
        for (auto m = beg; m != end; m++) {
            if (m->second->oneDown) {
                delete m->second->oneDown;
            }
        }

        m_imgMap.erase(beg, end);
    }
    
    okList.clear();
    failList.clear();
    
    for (auto m = m_bookDownMap.begin(); m != m_bookDownMap.end(); m++) {
        if (m->second->oneDown) {
            auto r = m->second->oneDown->getStatus();
            if (r == eBaseDownloadStatus::finish )
            {
                okList.push_back(m->first);
                
            }
            else if (r == eBaseDownloadStatus::error)
            {
                failList.push_back(m->first);
            }
        }
    }
    
    for (auto m : okList) {
        onBookResOk(m);
    }
    
    for (auto m : failList) {
        onBookResFail(m);
    }
    
    okList.clear();
    failList.clear();
    
    for (auto m = m_cardDownMap.begin(); m != m_cardDownMap.end(); m++) {
        if (m->second->oneDown) {
            auto r = m->second->oneDown->getStatus();
            if (r == eBaseDownloadStatus::finish )
            {
                okList.push_back(m->first);
                
            }
            else if (r == eBaseDownloadStatus::error)
            {
                failList.push_back(m->first);
            }
        }
    }
    
    for (auto m : okList) {
        onCardResOk(m);
    }
    
    for (auto m : failList) {
//        onBookResFail(m);
    }

    okList.clear();
    failList.clear();
    
    for (auto m : m_unzipList) {
        switch (CBaskUnZip::getInstance()->getStatus(m))
        {
            case eBaseUnZipStatus::none://,
                CBaskUnZip::getInstance()->start(m);
                break;
            case eBaseUnZipStatus::wait://,
                break;//不处理
            case eBaseUnZipStatus::run://,
                break;//不处理
            case eBaseUnZipStatus::finish://,
                okList.push_back(m);
                break;//
            case eBaseUnZipStatus::error://,
                failList.push_back(m);
                break;
        }
    }
    
    for (auto m : okList) {
        onCardUnzipOk(m);
    }
    
    for (auto m : failList) {
        onCardUnzipFail(m);
    }
    
    if (m_imgMap.size() == 0 && m_idleDownList.size() > 0) {
        auto it = m_idleDownList.begin();
        string url = *it;
//        CCLOG("down idle file = %s", url.c_str());
        m_idleDownList.erase(it);
        DownFile(url.c_str(), nullptr);
    }
}

void DownImg::DownCardFile(const char* url, const char* folder)
{
    string filePath = m_wPath + NET_DOWN_CARD;
    string urlPath = url;
    string fileName = "";
    string filePathName = "";
    size_t pos = urlPath.find_last_of("/");
    if (pos != std::string::npos)
    {
        fileName = urlPath.substr(pos+1);
    } else {
        CCLOG("url = %s can not find file name", url);
        return;
    }
    filePathName = filePath + fileName;
    
    if (FileUtils::getInstance()->isFileExist(filePathName)) {
        CBaskUnZip::getInstance()->start(NET_DOWN_CARD + fileName);
        m_unzipList.push_back(NET_DOWN_CARD + fileName);
        return;
    }
    
    if (m_bookDownMap.find(urlPath) != m_bookDownMap.end()) {
        return;
    }
    
    FileUtils::getInstance()->createDirectory(filePath);
    
    stuCardResDown* res = new stuCardResDown;
    res->zip = NET_DOWN_CARD + fileName;
    res->oneDown = new COneDownload(url, filePathName);
    m_cardDownMap.insert(pair<string, stuCardResDown*>(urlPath, res));
}

void DownImg::onCardResOk(const string &file)
{
    auto it = m_cardDownMap.find(file);
    if (it == m_cardDownMap.end()) {
        return;
    }
    stuCardResDown* res = it->second;
    string filePathName = res->zip;
    if (res->oneDown) {
        delete res->oneDown;
    }
    delete res;
    m_cardDownMap.erase(it);
    
    if (!FileUtils::getInstance()->isFileExist(filePathName)) {
        return;
    }
    
    CBaskUnZip::getInstance()->start(filePathName);
    m_unzipList.push_back(filePathName);
}

void DownImg::onCardResFail(const string &file)
{
    auto it = m_cardDownMap.find(file);
    if (it == m_cardDownMap.end()) {
        return;
    }
    stuCardResDown* res = it->second;
    if (res->oneDown) {
        delete res->oneDown;
    }
    delete res;
    m_cardDownMap.erase(it);
    
}

void DownImg::onCardUnzipOk(const string &file)
{
    for (int i = 0; i < m_unzipList.size(); i++) {
        if (m_unzipList[i] == file) {
            m_unzipList.erase(m_unzipList.begin() + i);
            
            CCLOG("unzip ok . file = %s", file.c_str());
            break;
        }
    }
}

void DownImg::onCardUnzipFail(const string &file)
{
    for (int i = 0; i < m_unzipList.size(); i++) {
        if (m_unzipList[i] == file) {
            m_unzipList.erase(m_unzipList.begin() + i);
            
            FileUtils::getInstance()->removeFile(file);
            CBaskUnZip::getInstance()->remove(file);
            break;
        }
    }
}

void DownImg::checkBookVersion(const string &bookId, const string &version)
{
    CCLOG("checkBookVersion: book = %s, version = %s", bookId.c_str(), version.c_str());
    string bookPath = m_wPath + NET_DOWN_BOOK + "/" + bookId;
    string verFile = bookPath + "/" + "version.txt";
    
    if (FileUtils::getInstance()->isFileExist(verFile)) {
        string ver = FileUtils::getInstance()->getStringFromFile(verFile);
        if (ver == version) {
            CCLOG("cache version is same !");
            return;
        } else {
            CCLOG("cache version is not same, = %s", ver.c_str());
        }
    } else {
        CCLOG("not find version file");
    }
    
    FileUtils::getInstance()->removeDirectory(bookPath);
    FileUtils::getInstance()->createDirectory(bookPath);
    FileUtils::getInstance()->writeStringToFile(version, verFile);
}

void DownImg::addSameBookFile(const char *url, const char *folder, int iTag, eBookResType resType)
{
    string urlPath = url;
    string filePathName = folder;
    if (m_bookDownSame.count(urlPath) < 1) {
        
    } else {
        auto beg = m_bookDownSame.lower_bound(urlPath);
        auto end = m_bookDownSame.upper_bound(urlPath);
        for (auto m = beg; m != end; m++) {
            if (m->second->fileName == filePathName) {
                return;
            }
        }
    }
    
    stuPageResDown* res = new stuPageResDown;
    res->tag = iTag;
    res->resType = resType;
    res->fileName = filePathName;
    res->oneDown = nullptr;//new COneDownload(url, filePathName);
    m_bookDownSame.insert(pair<string, stuPageResDown*>(urlPath, res));
}

void DownImg::onSameBookFileOk(const string &file, const string &filePathName)
{
    if (m_bookDownSame.count(file) < 1) {
        return;
    } else {
        auto data = FileUtils::getInstance()->getDataFromFile(filePathName);
        string url = file;
        auto beg = m_bookDownSame.lower_bound(file);
        auto end = m_bookDownSame.upper_bound(file);
        for (auto m = beg; m != end; m++) {
            FileUtils::getInstance()->writeDataToFile(data, m->second->fileName);
            for (auto it = m_callbackMap.begin(); it != m_callbackMap.end(); it++) {
                it->first->endDownload(url, m->second->fileName, m->second->tag, m->second->resType);
            }
            delete m->second;
        }
        m_bookDownSame.erase(beg, end);
    }
}

void DownImg::onSameBookFileFail(const string &file)
{
    if (m_bookDownSame.count(file) < 1) {
        return;
    } else {
        string url = file;
        auto beg = m_bookDownSame.lower_bound(file);
        auto end = m_bookDownSame.upper_bound(file);
        for (auto m = beg; m != end; m++) {
            for (auto it = m_callbackMap.begin(); it != m_callbackMap.end(); it++) {
                it->first->endDownloadFail(url, m->second->fileName, m->second->tag, m->second->resType);
            }
            delete m->second;
        }
        m_bookDownSame.erase(beg, end);
    }
}

void DownImg::DownFileIdle(const char *url)
{
    if (url == nullptr) {
        return;
    }
    if (m_imgMap.size() > 0) {
        string s = url;
        m_idleDownList.push_back(s);
    } else {
        DownFile(url, nullptr);
    }
}

void DownManyFile::endDownload(string& url, string& file, int iTag, eBookResType resType)
{
    bool bAll = true;
    auto it = m_urlList.find(url);
    if (it == m_urlList.end()) {
        return;
    }
    it->second = file;
    for (const auto& it : m_urlList) {
        if (it.second.length() < 4) {
            bAll = false;
            break;
        }
    }
    it->second = "-1";
    if (bAll && m_okCall) {
        m_okCall();
    }
}

void DownManyFile::endDownloadFail(string& url, string& file, int iTag, eBookResType resType)
{
    auto it = m_urlList.find(url);
    if (it == m_urlList.end()) {
        return;
    }
    if (m_errorCall) {
        m_errorCall(url);
    }
}

void DownManyFile::setDownFiles(const string& folder, const vector<string> urlList)
{
    string wPath = FileUtils::getInstance()->getWritablePath();
    m_folder = folder;
    FileUtils::getInstance()->createDirectory(wPath + m_folder);
    
    m_urlList.clear();
    for (const auto& url : urlList) {
        m_urlList[url] = "";
    }
}

void DownManyFile::startDown()
{
    for (const auto& urlItem : m_urlList) {
        DownImg::getInstance()->DownBookFile(urlItem.first.c_str(), m_folder.c_str(), 120, eBookResOth);
    }
}

void DownManyFile::setCallbackFunc(const function<void ()> okCall, const function<void (const string &)> errorCall)
{
    m_okCall = okCall;
    m_errorCall = errorCall;
}

const char * DownManyFile::getLocalFile(const string &url)
{
    auto it = m_urlList.find(url);
    if (it == m_urlList.end()) {
        return nullptr;
    }
    return it->second.c_str();
}
