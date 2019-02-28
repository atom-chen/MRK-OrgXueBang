//
//  BookStruct.cpp
//  ReadingMate
//
//  Created by 丁晶 on 2017/1/11.
//
//

#include "BookStruct.hpp"
#include "UiBase.h"

BookData::BookData()
:id("-1")           ,idnumber("-1")         ,name("-1")     ,intro("-1")    ,cover("-1")
,word_count("-1")   ,keywords("-1")         ,topics("-1")   ,read_flag(false),lexile("-1")   ,read_time("-1")
,page_count("-1")   ,add_time("-1")         ,paging("-1")   ,level("")      ,cohort("")
,latest_book_flg("n")       ,read_flg("n")  ,last_read_page_id(0)           ,isFree("n")

,sum_stay_time(0)   ,sum_listening_time(0)  ,sum_read_nuts(0)   ,sum_quiz_nuts(0)
,sum_nativ(0)       ,sum_acc(0)             ,sum_fluency(0)     ,sum_read_1(0)  ,sum_read_2(0)
,sum_read_3(0)      ,sum_quiz_1(0)          ,sum_quiz_2(0)      ,sum_quiz_3(0)  ,sum_quiz_4(0)
,sum_quiz_5(0)      ,sum_status(false)      ,cur_nativ(0)       ,cur_acc(0)     ,cur_fluency(0)
,cur_read_1(0)      ,cur_read_2(0)          ,cur_read_3(0)      ,cur_quiz_1(0)  ,cur_quiz_2(0)
,cur_quiz_3(0)      ,cur_quiz_4(0)          ,cur_quiz_5(0)      ,correct_pages("")
,sum_recording_time(0)//按次序挨着整理的，小手一抖，就丢失了一个初始化
{
}

//string BookData::cohort2Level(){
////    if (level.length() > 0) {
////        return level;
////    }
////    if (cohort.length() > 0) {
////        if (cohort == "RM-FREE" || cohort == "RM-GUEST") {
////            level = "FREE";
////        } else {
////            if (cohort.find("RM") == 0 && cohort.find("-001") == 3) {
////                char c = level.at(2);
////                level.push_back(c);
////            }
////        }
////    }
//    
//    return level;
//};

PageScore::PageScore()
:m_listent(0)
,m_modelTime(300.0f)//范读时长
,m_reocrd(0)        //录时长
,m_score(0)         //得分
,m_dummy(0)
,m_nuts(0)          //坚果
,m_nativ(0)         //地道程度
,m_acc(0)           //准确度
,m_fluency(0)       //流利程度
{
};

void PageScore::clearScore()
{
    m_listent   =0;
    m_reocrd    =0;
    //m_score     =0;//老丁那边要用到Score，保留成绩方便还原现场，其他可以清除
    m_nuts      =0;
    m_nativ     =0;
    m_acc       =0;
    m_fluency   =0;
    
    for(auto word : m_errWords)
    {
        CC_SAFE_DELETE(word);
    }
    
    m_errWords.clear();
}

OnePage::OnePage()
:m_pageID(-1)
,m_pageTitle("-1")
,m_pageMode("-1")
,m_pdfPic("-1")
,m_pdfSound("-1")
,m_pdfContent("-1")
,m_pageName("-1")
,m_sortIndex(-1)
,m_isQuiz(false)
,m_parameters("Parameters")
,m_courseID(-1)
,m_downPicFailed(false)
,m_downSndFailed(false)
,m_downDatFailed(false)
{
    m_pageScore = new PageScore;
}

WholeBook::WholeBook()
:m_favourite(false)
,m_folderPic("")
,m_folderSnd("")
,m_folderCnt("")
{
    m_bookPages = new BookPagesVec;
    m_bookData = new BookData;
}

string WholeBook::getFolder(int index)
{
    string folder = "";
    OnePage* onePage = m_bookPages->at(index);
    if (onePage == nullptr) {
        return folder;
    }
    folder = m_bookData->id + "/" + onePage->m_pageName;
    return folder;
}

void WholeBook::downPic(int index)
{
    OnePage* onePage = m_bookPages->at(index);
    if (onePage == nullptr) {
        return;
    }
    if (onePage->m_pdfPic != "-1") {
        onePage->m_downPicFailed = false;
        DownImg::getInstance()->DownBookFile((m_folderPic + onePage->m_pdfPic).c_str(), getFolder(index).c_str(), index, eBookResPng);
    }
}

void WholeBook::downSound(int index)
{
    OnePage* onePage = m_bookPages->at(index);
    if (onePage == nullptr) {
        return;
    }
    if (onePage->m_pdfSound != "-1") {
        onePage->m_downSndFailed = false;
        DownImg::getInstance()->DownBookFile((m_folderSnd + onePage->m_pdfSound).c_str(), getFolder(index).c_str(), index, eBookResSnd);
    }
}

void WholeBook::downXml(int index)
{
    OnePage* onePage = m_bookPages->at(index);
    if (onePage == nullptr) {
        return;
    }
    if (onePage->m_pdfContent != "-1" && onePage->m_pageTitle != "Copy Right") {//新书的CopyRight.txt不用下载
        onePage->m_downDatFailed = false;
        DownImg::getInstance()->DownBookFile((m_folderCnt + onePage->m_pdfContent).c_str(), getFolder(index).c_str(), index, eBookResOth);
    }
}

void WholeBook::downAll(int index)
{
    //SILog("下载资源", index)
    downPic(index);
    downSound(index);
    downXml(index);
}

string WholeBook::getPicPath(int index)
{
    string filePath = "";
    OnePage* onePage = m_bookPages->at(index);
    if (onePage == nullptr) {
        return filePath;
    }
    if (onePage->m_pdfPic == "-1") {
        return filePath;
    }
    filePath = DownImg::getInstance()->getBookFilePath((m_folderPic + onePage->m_pdfPic).c_str(), getFolder(index).c_str());
    return filePath;
}

string WholeBook::getSoundPath(int index)
{
    string filePath = "";
    OnePage* onePage = m_bookPages->at(index);
    if (onePage == nullptr) {
        return filePath;
    }
    if (onePage->m_pdfSound == "-1") {
        return filePath;
    }
    filePath = DownImg::getInstance()->getBookFilePath((m_folderSnd + onePage->m_pdfSound).c_str(), getFolder(index).c_str());
    return filePath;
}

string WholeBook::getXmlPath(int index)
{
    string filePath = "";
    OnePage* onePage = m_bookPages->at(index);
    if (onePage == nullptr) {
        return filePath;
    }
    if (onePage->m_pdfContent == "-1" || onePage->m_pageTitle == "Copy Right") {
        return filePath;
    }
    filePath = DownImg::getInstance()->getBookFilePath((m_folderCnt + onePage->m_pdfContent).c_str(), getFolder(index).c_str());
    return filePath;
}

bool WholeBook::haveDat(int index)
{
    return m_bookPages->at(index)->m_pdfContent != "-1";
}

bool WholeBook::haveSnd(int index)
{
    return m_bookPages->at(index)->m_pdfSound != "-1";
}

bool WholeBook::havePic(int index)
{
    return m_bookPages->at(index)->m_pdfPic != "-1";
}

void WholeBook::downQuizSound(int index, const string &sndName)
{
    OnePage* onePage = m_bookPages->at(index);
    if (onePage == nullptr) {
        return;
    }
    string fullName = sndName;
    if (sndName.find(".ogg") == string::npos) {
        fullName += ".ogg";
    }
//    auto n = sndName.find('_');
    string pagePath;
    //edit by yy 2017年09月19日13:53:55
//    if (n == string::npos) {//适用于<task audio="big" id="1">这种配置文件没有包含书的title_id，因此从json中单独取
        pagePath = onePage->m_paramJson.getValue("title_id");
//    }else{//适用于<task audio="OAD-014006_TASK1" id="1">这种配置资源名称包含title_id，手动截取即可以拼装下载url路径（建议干掉）
//        pagePath = sndName.substr(0, n);
//    }
    
    DownImg::getInstance()->DownBookFile((m_folderSnd + pagePath + "/OM/" + fullName).c_str(), getFolder(index).c_str(), index, eBookResSnd);

}
void WholeBook::downQuizSound1(int index, const string &sndName)
{
    OnePage* onePage = m_bookPages->at(index);
    if (onePage == nullptr) {
        return;
    }
    string fullName = sndName;
    if (sndName.find(".ogg") == string::npos) {
        fullName += ".ogg";
    }
    //    auto n = sndName.find('_');
    string pagePath;
    //edit by yy 2017年09月19日13:53:55
    //    if (n == string::npos) {//适用于<task audio="big" id="1">这种配置文件没有包含书的title_id，因此从json中单独取
    pagePath = onePage->m_paramJson.getValue("title_id");
    //    }else{//适用于<task audio="OAD-014006_TASK1" id="1">这种配置资源名称包含title_id，手动截取即可以拼装下载url路径（建议干掉）
    //        pagePath = sndName.substr(0, n);
    //    }
    
    DownImg::getInstance()->DownBookFile((m_folderSnd + pagePath + "/QAA/" + fullName).c_str(), getFolder(index).c_str(), index, eBookResSnd);
    
}

void WholeBook::downQuizPic(int index, const string &sndName)
{
    OnePage* onePage = m_bookPages->at(index);
    if (onePage == nullptr) {
        return;
    }
    string fullName = sndName;
//    auto n = sndName.find('_');
    string pagePath;
//    if (n == string::npos) {
        pagePath = onePage->m_paramJson.getValue("title_id");
//    }else{//适用于"OM/OAB-002000_M1.png"
//        pagePath = sndName.substr(3, n - 3);//去掉"OM/"
//    }
    DownImg::getInstance()->DownBookFile((m_folderSnd + pagePath + "/" + fullName).c_str(), getFolder(index).c_str(), index, eBookResPng);

}


string WholeBook::getQuizSoundPath(int index, const string &sndName)
{
    string filePath = "";
    OnePage* onePage = m_bookPages->at(index);
    if (onePage == nullptr) {
        return filePath;
    }
    string fullName = sndName;
    if (sndName.find(".ogg") == string::npos) {
        fullName += ".ogg";
    }
    string pagePath = onePage->m_paramJson.getValue("title_id");
    filePath = DownImg::getInstance()->getBookFilePath((m_folderSnd + pagePath + "/OM/" + fullName).c_str(), getFolder(index).c_str());
    return filePath;
}
string WholeBook::getQuizSoundPath1(int index, const string &sndName)
{
    string filePath = "";
    OnePage* onePage = m_bookPages->at(index);
    if (onePage == nullptr) {
        return filePath;
    }
    string fullName = sndName;
    if (sndName.find(".ogg") == string::npos) {
        fullName += ".ogg";
    }
    string pagePath = onePage->m_paramJson.getValue("title_id");
    filePath = DownImg::getInstance()->getBookFilePath((m_folderSnd + pagePath + "/QAA/" + fullName).c_str(), getFolder(index).c_str());
    return filePath;
}

string WholeBook::getQuizPicPath(int index, const string &sndName)
{
    string filePath = "";
    OnePage* onePage = m_bookPages->at(index);
    if (onePage == nullptr) {
        return filePath;
    }
    string fullName = sndName;
    string pagePath;
    pagePath = onePage->m_paramJson.getValue("title_id");
    filePath = DownImg::getInstance()->getBookFilePath((m_folderSnd + pagePath + "/" + fullName).c_str(), getFolder(index).c_str());
    return filePath;
}

bool WholeBook::gotFullRes(int index)
{
    string strPath1 = getPicPath(index);
    string strPath2 = getSoundPath(index);
    string strPath3 = getXmlPath(index);
    
    OnePage* onePage = m_bookPages->at(index);
    
    bool bGot1 = !(strPath1 != "" && !(FileUtils::getInstance()->isFileExist(strPath1) || onePage->m_downPicFailed));
    bool bGot2 = !(strPath2 != "" && !(FileUtils::getInstance()->isFileExist(strPath2) || onePage->m_downSndFailed));
    bool bGot3 = !(strPath3 != "" && !(FileUtils::getInstance()->isFileExist(strPath3) || onePage->m_downDatFailed));
    return bGot1
        &&bGot2
        &&bGot3
    ;
}

void WholeBook::downOnePageRes(const string& id, int index, const string& url, const string& res, int iTag, eBookResType resType)
{
    string folder = StringUtils::format("%s/%d", id.c_str(), index);
    DownImg::getInstance()->DownBookFile((url + res).c_str(), folder.c_str(), iTag, resType);
}

BookTopic::BookTopic()
:t_id("-1")         //话题id
,t_name("-1")       //话题名称
,t_img("-1")        //话题图片
,t_index(-1)        //话题索引
,t_isSelected(false)//是否用户选中  y 选中； n 未选中
{
}

TextScore::TextScore()
:ts_open(0)     //开书时间
,ts_listen(0)   //听时长
,ts_modelTime(300)//范读时长
,ts_reocrd(0)   //录时长
,ts_score(0)    //得分
,ts_nuts(0)     //坚果
,ts_nativ(0)    //地道程度
,ts_acc(0)      //准确度
,ts_fluency(0)  //流利程度
,ts_repeat(0)   //测试次数
{
}

void TextScore::clearScore()
{
    ts_open     =0;    //开书时间
    ts_listen   =0;  //听时长
    ts_reocrd   =0;   //录时长
    ts_score    =0;    //得分
    ts_nuts     =0;     //坚果
    ts_nativ    =0;    //地道程度
    ts_acc      =0;      //准确度
    ts_fluency  =0;  //流利程度
    ts_repeat   =0; //测试次数
    for(auto word : m_errWords)
    {
        CC_SAFE_DELETE(word);
    }
    m_errWords.clear();
}

TxtPge::TxtPge()
:tp_id("-1")    //书页ID
,tp_title("-1") //书页名称
,tp_mode("-1")  //书页类型
,tp_titlID("-1")//书页title_id
,tp_infUrl("-1")//书页信息文件
,tp_srtIdx("-1")//书页排序
,tp_bookID("-1")//教材图书ID
,tp_bokNum("-1")//教材图书编号
,tp_resTag(0)   //资源标记
,tp_titTxt("-1")//解析的页眉
,tp_screenShotSaved(false)  //已截屏
,tp_screenShotReady(false)  //滚屏居中
,tp_initDlgOvered(false)    //初始化对话列表
,tp_initActOvered(false)    //初始化角色列表
,tp_initAllOvered(false)    //全部资源到位
,tp_downInfFailed(false)    //文件下载状态
,tp_downActFailed(false)    //文件下载状态
,tp_downSndFailed(false)    //文件下载状态
,tp_downLryFailed(false)    //文件下载状态

,tp_sndMp3("-1")    //音效资源名字
,tp_actXml("-1")    //参与者信息
,tp_titXml("-1")    //歌词参数
,tp_audDef("-1")    //分段朗读可用
,tp_tilNum("-1")    //页Title码
,tp_level("-1")     //书的级别
{
    
}

TxtBok::TxtBok()
:tb_txtID("-1")
,tb_txtPgeNum(0)
,tb_txtLstRed(0)
,tb_txtCntUrl("")
{
    tb_txtPges  = new TxtPgeVec;
}

void TxtBok::downFile(int idx, eBookResType resType)
{
    TxtPge* onePage = tb_txtPges->at(idx);
    if (onePage == nullptr) {
        return;
    }
    if (resType == eBookResInf && onePage->tp_infUrl != "-1")
    {
        DownImg::getInstance()->DownBookFile((tb_txtCntUrl + onePage->tp_infUrl).c_str(), getFolder(idx).c_str(), idx, eBookResInf);
    }
    else if (resType == eBookResSnd && onePage->tp_sndMp3 != "-1")
    {
        DownImg::getInstance()->DownBookFile((tb_txtCntUrl + onePage->tp_sndMp3).c_str(), getFolder(idx).c_str(), idx, eBookResSnd);
    }
    else if (resType == eBookResAct && onePage->tp_actXml != "-1")
    {
        DownImg::getInstance()->DownBookFile((tb_txtCntUrl + onePage->tp_actXml).c_str(), getFolder(idx).c_str(), idx, eBookResAct);
    }
    else if (resType == eBookResLyr && onePage->tp_titXml != "-1")
    {
        DownImg::getInstance()->DownBookFile((tb_txtCntUrl + onePage->tp_titXml).c_str(), getFolder(idx).c_str(), idx, eBookResLyr);
    }
    /*
    else if (resType == eBookResPng && onePage->tp_actVec.size() > 0)
    {
        for (int i = 0; i < onePage->tp_actVec.size(); i++) {
            DownImg::getInstance()->DownFile((tb_txtCntUrl + onePage->tp_actVec.at(i)->act_img).c_str(), nullptr);
            DownImg::getInstance()->DownBookFile((tb_txtCntUrl + onePage->tp_actVec.at(i)->act_img).c_str(), getFolder(idx).c_str(), idx, eBookResPng);
        }
    }
     */
}

string TxtBok::getFolder(int idx)
{
    string folder = "";
    TxtPge* onePage = tb_txtPges->at(idx);
    if (onePage == nullptr) {
        return folder;
    }
    folder = tb_txtID + "/" + onePage->tp_srtIdx;
    return folder;
}

void TxtBok::readFile(int idx, eBookResType resType)
{
    TxtPge* onePage = tb_txtPges->at(idx);
    
    if (resType == eBookResInf)//info.xml
    {
        string strLocalPath = DownImg::getInstance()->getBookFilePath((tb_txtCntUrl+onePage->tp_infUrl).c_str(), getFolder(idx).c_str());
        
        if(!FileUtils::getInstance()->isFileExist(strLocalPath))
        {
            return;
        }
        
        string xmlString = FileUtils::getInstance()->getStringFromFile(strLocalPath);
//        tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument();
//        tinyxml2::XMLElement* rootNode = NULL;
//
//        do
//        {
//            xmlDoc->Parse((const char *)xmlString.c_str(), xmlString.size());
//            rootNode = xmlDoc->RootElement();
//            if (NULL == rootNode)
//            {
//                CCLOG("read root node error");
//                break;
//            }
//            if (rootNode->Attribute("name")) {
//                onePage->tp_titTxt = rootNode->Attribute("name");
//            }
//
//            tinyxml2::XMLElement* pNodeP = (rootNode)->FirstChildElement();
//            while (NULL != pNodeP)
//            {
//                if (!strcmp(pNodeP->Value(), "properties"))
//                {//找到properties
//                    tinyxml2::XMLElement* pNodeE = (pNodeP)->FirstChildElement();
//                    while (NULL != pNodeE)
//                    {
//                        if (!strcmp(pNodeE->Value(), "entry"))
//                        {//找到entry
//                            string str = pNodeE->FirstChild()->Value();
//                            if (!strcmp(pNodeE->Attribute("key"), "actors_xml_file")){
//                                Utils::string_replace(str, "\\", "/");
//                                onePage->tp_actXml = onePage->tp_titlID +"/"+ str;
//                            }
//                            else if (!strcmp(pNodeE->Attribute("key"), "audio_filename"))
//                            {
//                                onePage->tp_sndMp3 = onePage->tp_titlID +"/"+ str+".ogg";
//                            }
//                            else if (!strcmp(pNodeE->Attribute("key"), "title_xml_file")){
//                                onePage->tp_titXml = onePage->tp_titlID +"/"+ str;
//                            }
//                            else if (!strcmp(pNodeE->Attribute("key"), "title_number")){
//                                onePage->tp_tilNum = str;
//                            }
//                            else if (!strcmp(pNodeE->Attribute("key"), "audiotags_defined")){
//                                onePage->tp_audDef = str;
//                            }
//                            else if (!strcmp(pNodeE->Attribute("key"), "level")){
//                                onePage->tp_level = str;
//                            }
//                        }
//                        pNodeE = pNodeE->NextSiblingElement();
//                    }
//
//                }
//                pNodeP = pNodeP->NextSiblingElement();
//            }
//        }while (0);
//        delete xmlDoc;
    }else if (resType == eBookResSnd)
    {
        
    }
    else if (resType == eBookResAct)
    {
        string strLocalPath = DownImg::getInstance()->getBookFilePath((tb_txtCntUrl+onePage->tp_actXml).c_str(), getFolder(idx).c_str());
        
        if(!FileUtils::getInstance()->isFileExist(strLocalPath))
        {
            return;
        }
        
        onePage->tp_actVec.clear();
        
        //默认松鼠
        ActInf* oneAll = new ActInf;
        oneAll->act_name = "All";
        oneAll->act_img = "https://storage.metricschina.com/prod/js/idevice/1.13.2/Idevice/css/mc_china_crc/img/squirrelAlldialog.png";
        onePage->tp_actVec.push_back(oneAll);
        
        //解析角色
//        string xmlString = FileUtils::getInstance()->getStringFromFile(strLocalPath);
//        tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument();
//        tinyxml2::XMLElement* rootNode = NULL;
//
//        do
//        {
//            xmlDoc->Parse((const char *)xmlString.c_str(), xmlString.size());
//            rootNode = xmlDoc->RootElement();
//            if (NULL == rootNode)
//            {
//                CCLOG("read root node error");
//                break;
//            }
//
//            tinyxml2::XMLElement* pNodeP = (rootNode)->FirstChildElement();
//            while (NULL != pNodeP)
//            {
//                if (!strcmp(pNodeP->Value(), "actor"))
//                {//找到actor
//                    ActInf* oneAct = new ActInf;
//
//                    oneAct->act_name = pNodeP->Attribute("actor_id");
//                    //可能含空格或特殊字符，需要二次处理
//                    string str_url = pNodeP->Attribute("actor_img");
//                    oneAct->act_img = onePage->tp_titlID+"/"+str_url;
//                    Utils::string_replace(oneAct->act_img, "\\", "/");
//                    oneAct->act_img = Utils::UrlEncode(oneAct->act_img);
//                    /*
//                    oneAct->act_img = onePage->tp_titlID;
//                    vector<string> vecUrl;
//                    Utils::splitString(str_url.c_str(), '\\', vecUrl);
//                    vector<string>::iterator iter;
//                    for (iter = vecUrl.begin(); iter != vecUrl.end(); iter++) {
//                        oneAct->act_img += "/";
//                        oneAct->act_img += Utils::UrlEncode(*iter);
//                    }
//                    */
//                    onePage->tp_actVec.push_back(oneAct);
//                }
//                pNodeP = pNodeP->NextSiblingElement();
//            }
//        }while (0);
//        delete xmlDoc;
    }
    else if (resType == eBookResLyr)
    {
        string strLocalPath = DownImg::getInstance()->getBookFilePath((tb_txtCntUrl+onePage->tp_titXml).c_str(), getFolder(idx).c_str());
        
        if(!FileUtils::getInstance()->isFileExist(strLocalPath))
        {
            return;
        }
        
        onePage->tp_actDlg.clear();
        string xmlString = FileUtils::getInstance()->getStringFromFile(strLocalPath);
        
        //////////////////////////////如果不单独提供String，自己解析
//        tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument();
//        tinyxml2::XMLElement* rootNode = NULL;
//        do
//        {
//            xmlDoc->Parse((const char *)xmlString.c_str(), xmlString.size());
//            rootNode = xmlDoc->RootElement();
//            if (NULL == rootNode)
//            {
//                CCLOG("read root node error");
//                break;
//            }
//            tinyxml2::XMLElement* pNodeP = (rootNode)->FirstChildElement();
//            while (NULL != pNodeP)
//            {
//                if (!strcmp(pNodeP->Value(), "p"))
//                {//找到P
//                    ActStc actorSentence;
//                    //角色名字
//                    actorSentence.as_actID = pNodeP->Attribute("actor_id");
//                    actorSentence.as_vecWords.clear();
//                    
//                    //有“单句播放上限”的需求，需要增加每句的起止时间掐段
//                    bool bStartTimeSettled = false;//句首时间设置标记
//                    bool bEndTimeSettled = false;//句末时间设置标记
//                    int iStartTimeTag = 0;  //开始时间（可以不为零）
//                    int iEndTimeTag = 0;    //结束时间（系数100）
//                    
//                    tinyxml2::XMLElement* pNodeS = (pNodeP)->FirstChildElement();
//                    while (NULL != pNodeS)
//                    {
//                        if (!strcmp(pNodeS->Value(), "s"))
//                        {//找到S
//                            tinyxml2::XMLElement* pNodeW = (pNodeS)->FirstChildElement();
//                            while (NULL != pNodeW)
//                            {
//                                if (!strcmp(pNodeW->Value(), "w")
//                                    && pNodeW->Attribute("start_audio")
//                                    && pNodeW->Attribute("end_audio"))//找到w
//                                {
//                                    //log("%s,%s,%s",
//                                    std::string strOne = pNodeW->FirstChild()->Value();
//                                    strOne += " ";//加空格隔开单词
//                                    WordAttr word;
//                                    word.w_index = (int)actorSentence.as_vecWords.size();
//                                    word.w_start = (int)actorSentence.as_sentence.length();
//                                    
//                                    word.w_string = strOne;
//                                    word.w_state = eWordDefault;
//                                    
//                                    if (pNodeW->Attribute("variants")) {
//                                        word.w_word = pNodeW->Attribute("variants");
//                                    } else {
//                                        word.w_word = word.w_string;
//                                    }
//                                    if (pNodeW->Attribute("start_audio") && pNodeW->Attribute("end_audio")) {
//                                        word.w_voiceBegin = atoi(pNodeW->Attribute("start_audio"));
//                                        word.w_voiceEnd = atoi(pNodeW->Attribute("end_audio"));
//                                        
//                                        //记录开始时间
//                                        if (!bStartTimeSettled) {
//                                            bStartTimeSettled = true;
//                                            iStartTimeTag = word.w_voiceBegin;
//                                        }
//                                        //更新结束时间
//                                        iEndTimeTag = word.w_voiceEnd;
//                                        
//                                        word.w_wordID = atoi(pNodeW->Attribute("id"));
//                                        actorSentence.as_vecWords.push_back(word);
//                                        
//                                        actorSentence.as_sentence += strOne;
//                                        
//                                    }
//                                }
//                                else
//                                {
//                                    if (strcmp(pNodeW->Value(), "br") == 0) {
//                                        actorSentence.as_sentence += "\n";
//                                    }
//                                }
//                                pNodeW = pNodeW->NextSiblingElement();
//                            }
//                        }
//                        pNodeS = pNodeS->NextSiblingElement();
//                        if(!pNodeS && iEndTimeTag != iStartTimeTag)//单词结点空了，表示句子到头了，就可以找到截止时间了
//                        {
//                            actorSentence.as_modelTime = (iEndTimeTag - iStartTimeTag)/100.0f;
//                            SFLog("ModelTime===解析单句时长", actorSentence.as_modelTime)
//                        }
//                    }
//                    onePage->tp_actDlg.push_back(actorSentence);
//                }
//                pNodeP = pNodeP->NextSiblingElement();
//                
//            }
//        }while (0);
//        delete xmlDoc;
    }
}


bool TxtBok::getFileState(bool bReal,int idx, eBookResType resType)
{
    TxtPge* onePage = tb_txtPges->at(idx);
    if (onePage == nullptr) {
        return false;
    }
    
    if(bReal)//检测本地文件
    {
        string str_name = "";
        
        if (resType == eBookResInf && onePage->tp_infUrl != "-1")
        {
            str_name = onePage->tp_infUrl;
        }
        else if (resType == eBookResSnd && onePage->tp_sndMp3 != "-1")
        {
            str_name = onePage->tp_sndMp3;
        }
        else if (resType == eBookResAct && onePage->tp_actXml != "-1")
        {
            str_name = onePage->tp_actXml;
        }
        else if (resType == eBookResLyr && onePage->tp_titXml != "-1")
        {
            str_name = onePage->tp_titXml;
        }
        
        string strLocalPath = DownImg::getInstance()->getBookFilePath((tb_txtCntUrl+str_name).c_str(), getFolder(idx).c_str());
        
        if(str_name != "" && FileUtils::getInstance()->isFileExist(strLocalPath))
        {
            return true;//文件存在
        }
        return false;
    }
    else
    {
        if (resType == eBookResInf && onePage->tp_infUrl != "-1")
        {
            return onePage->tp_downInfFailed;
        }
        else if (resType == eBookResSnd && onePage->tp_sndMp3 != "-1")
        {
            return onePage->tp_downSndFailed;
        }
        else if (resType == eBookResAct && onePage->tp_actXml != "-1")
        {
            return onePage->tp_downActFailed;
        }
        else if (resType == eBookResLyr && onePage->tp_titXml != "-1")
        {
            return onePage->tp_downLryFailed;
        }
        return false;//下载成功的状态
    }
}

void TxtBok::setFileState(int idx, eBookResType resType,bool bFail)
{
    TxtPge* onePage = tb_txtPges->at(idx);
    if (onePage == nullptr) {
        return;
    }
    if (resType == eBookResInf && onePage->tp_infUrl != "-1")
    {
        onePage->tp_downInfFailed = bFail;
    }
    else if (resType == eBookResSnd && onePage->tp_sndMp3 != "-1")
    {
        onePage->tp_downSndFailed = bFail;
    }
    else if (resType == eBookResAct && onePage->tp_actXml != "-1")
    {
        onePage->tp_downActFailed = bFail;
    }
    else if (resType == eBookResLyr && onePage->tp_titXml != "-1")
    {
        onePage->tp_downLryFailed = bFail;
    }
}

bool TxtBok::checkFullRes(int idx)
{
    if(tb_txtPges->size() == 0 || idx > tb_txtPges->size())
    {
        return false;
    }
    TxtPge* onePage = tb_txtPges->at(idx);
    
    string strLocalPath1 = (onePage->tp_infUrl != "-1"?DownImg::getInstance()->getBookFilePath((tb_txtCntUrl+onePage->tp_infUrl).c_str(), getFolder(idx).c_str()):"");
    string strLocalPath2 = (onePage->tp_sndMp3 != "-1"?DownImg::getInstance()->getBookFilePath((tb_txtCntUrl+onePage->tp_sndMp3).c_str(), getFolder(idx).c_str()):"");
    string strLocalPath3 = (onePage->tp_actXml != "-1"?DownImg::getInstance()->getBookFilePath((tb_txtCntUrl+onePage->tp_actXml).c_str(), getFolder(idx).c_str()):"");
    string strLocalPath4 = (onePage->tp_titXml != "-1"?DownImg::getInstance()->getBookFilePath((tb_txtCntUrl+onePage->tp_titXml).c_str(), getFolder(idx).c_str()):"");
    
    bool bGot1 = !(strLocalPath1 != "" && !(FileUtils::getInstance()->isFileExist(strLocalPath1) || onePage->tp_downInfFailed));
    bool bGot2 = !(strLocalPath2 != "" && !(FileUtils::getInstance()->isFileExist(strLocalPath2) || onePage->tp_downSndFailed));
    bool bGot3 = !(strLocalPath3 != "" && !(FileUtils::getInstance()->isFileExist(strLocalPath3) || onePage->tp_downActFailed));
    bool bGot4 = !(strLocalPath4 != "" && !(FileUtils::getInstance()->isFileExist(strLocalPath4) || onePage->tp_downLryFailed));
    
    /*
    bool bGotActors = true;
    if (onePage->tp_actVec.size() > 0) {
        for (int i = 0; i < onePage->tp_actVec.size(); i++) {
            string strLocalPath5 = DownImg::getInstance()->getBookFilePath((tb_txtCntUrl+onePage->tp_actVec.at(i)->act_img).c_str(), getFolder(idx).c_str());
            bGotActors = bGotActors && FileUtils::getInstance()->isFileExist(strLocalPath5);
            if (!bGotActors) {
                break;
            }
        }
    }
    */
    
    
    return bGot1
    && bGot2
    && bGot3
    && bGot4
    //&& bGotActors
    ;
}
