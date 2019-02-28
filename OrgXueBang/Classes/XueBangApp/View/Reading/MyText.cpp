//
//  MyText.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/5.
//
//

#include "stdafx.h"
#include "MyText.hpp"
#include "BookDataManager.hpp"


#include "tinyxml2/tinyxml2.h"

#define DRAW_RECT 0

std::string TEXT = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-'";

static const char my_utf8_skip_data[256] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5,
    5, 5, 5, 6, 6, 1, 1
};



MyText* MyText::create(const std::string& text,const cocos2d::Color3B &color, float fontSize,const Size& size)
{
    MyText *ret = new (std::nothrow) MyText();
    if (ret && ret->init(text,color,fontSize,size))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool MyText::init(const std::string& text,const cocos2d::Color3B &color,float fontSize,const Size& size)
{
    m_bXml = false;
    //创建
    m_pLabel = Label::create();
    addChild(m_pLabel,0);
    
    //属性
    m_pLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    m_pLabel->setVerticalAlignment(TextVAlignment::TOP);
    m_pLabel->setOverflow(Label::Overflow::RESIZE_HEIGHT);
    
    m_strTTF = "Font/arial.ttf";
    //初始值
    m_fXmlTime = 0;
    m_nCurChange = 0;
    m_touchCallback = NULL;
    m_fSizeDefault = fontSize;
    m_Size = size;
    m_textDisplay = text;
    m_ColorDefault = color;
    m_totalSecs = 300;
    m_vecWords.clear();
    checkText();
    Refresh();
    
    //注册触摸
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    touchListener->onTouchBegan = CC_CALLBACK_2(MyText::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(MyText::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(MyText::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void MyText::setString(const std::string& text)
{
    //富文本解析
    m_textDisplay = text;
//    m_vReadTime.clear();
    checkText();
    Refresh();
}

void MyText::setSizeDefault(float fontSize)
{
    m_fSizeDefault = fontSize;
    Refresh();
}

void MyText::setSizeAll(float fontSize)
{
    m_fSizeDefault = fontSize;
    Refresh();
}

void MyText::setColorDefault(const cocos2d::Color3B &color)
{
    m_ColorDefault = color;
    resetState();
    resetColor();
}

void MyText::setColorAll(const cocos2d::Color3B &color)
{
    m_ColorDefault = color;
    resetState();
    resetColor();
}

void MyText::setFontFile(std::string fileName)
{
    m_strTTF = fileName;
    Refresh();
}
/*设置字体样式*/
void MyText::setAlignment(TextHAlignment h1, TextVAlignment v1)
{
    m_pLabel->setHorizontalAlignment(h1);
    m_pLabel->setVerticalAlignment(v1);
}

void MyText::checkText()
{
    
}

Sprite* MyText::getLetter(int nIndex)
{
    return m_pLabel->getLetter(getLetterIndex(nIndex));
    //  return m_pLabel->getLetter(nIndex);
}

void MyText::Refresh()
{
    //字体
    if (!FileUtils::getInstance()->isFileExist(m_strTTF))
        return ;
    TTFConfig ttfConfig(m_strTTF,m_fSizeDefault,GlyphCollection::DYNAMIC);
    if (!m_pLabel->setTTFConfig(ttfConfig))
        return ;
    
    m_pLabel->setDimensions(m_Size.width, m_Size.height);
    
    //设置文字
    m_pLabel->setString(m_textDisplay);
    
    //计算区域
    auto drawSize =  m_pLabel->getContentSize();
    setContentSize(drawSize);
    //m_Size = drawSize;
    m_pLabel->setPosition(drawSize.width/2, drawSize.height/2);
    this->setIgnoreAnchorPointForPosition(false);
    this->setAnchorPoint(Vec2(0,1));
   
    if (m_bXml) {
        wordRectWithXml();
    } else {
        wordRectWithStr();
    }
    drawWordRect();
    
    resetState();
    resetColor();
}

bool MyText::onTouchBegan(Touch* touch, Event* event)
{
    if(m_touchCallback == NULL)
        return false;
    
    Point touchLocation = touch->getLocation();
    Point local = convertToNodeSpace(touchLocation);
    if (getContentSize().width >= local.x && getContentSize().height >= local.y &&  local.x >= 0 &&  local.y >= 0 )
    {
        m_iBegin =  GetTouchIndex(m_pLabel->convertToNodeSpace(touchLocation));
        if (m_iBegin < 0) {
            touchLocation.x += m_fSizeDefault / 2;
            m_iBegin =  GetTouchIndex(m_pLabel->convertToNodeSpace(touchLocation));
        }
        return true;
    }
    return false;
}

int MyText::GetTouchIndex(const Point& pos)
{
    for (int i = 0 ; i < m_vecWords.size(); i++) {
        if(m_vecWords.at(i).w_rect.containsPoint(pos))
            return m_vecWords.at(i).w_index;
    }
    return -1;
}

void MyText::onTouchEnded(Touch* touch, Event* event)
{    
    //查找坐标，准备回调
    if(m_touchCallback == NULL)
        return ;
    
    Point touchLocation = touch->getLocation();
    Point local = convertToNodeSpace(touchLocation);
    if (getContentSize().width >= local.x && getContentSize().height >= local.y &&  local.x >= 0 &&  local.y >= 0 )
    {
        int iWord = GetTouchIndex(m_pLabel->convertToNodeSpace(touchLocation));
        if (m_iBegin >=0 && iWord < 0) {
            touchLocation.x -= m_fSizeDefault / 2;
            iWord =  GetTouchIndex(m_pLabel->convertToNodeSpace(touchLocation));
        }
        if (iWord < 0) {
            m_touchCallback(iWord, "-1", m_iBegin);
        } else {
            if (m_iBegin < 0) {
                m_iBegin = iWord;
            }
            if (m_iBegin >=0 && m_iBegin > iWord) {
                int t = iWord;
                iWord = m_iBegin;
                m_iBegin = t;
            }
            m_touchCallback(iWord, m_vecWords.at(iWord).w_word, m_iBegin);
            
//            if (m_vecWords.at(iWord).w_state == eWordNormal) {
//                m_vecWords.at(iWord).w_state = eWordWrong;
//            } else {
//                m_vecWords.at(iWord).w_state = eWordNormal;
//            }
            for (int i = m_iBegin; i <= iWord; i++) {
                m_vecWords.at(i).w_state = eWordNormal;
                checkColor(i);
            }
        }
        
    }
}

void MyText::onTouchMoved(Touch* touch, Event* event)
{
    //查找坐标，准备回调
    if(m_touchCallback == NULL)
        return ;
}

void MyText::setTouchListener(MyTextTouchCallback callback)
{
    m_touchCallback = callback;
    
}
//取消注册
void MyText::removeTouchListener()
{
    m_touchCallback = NULL;
}

void MyText::showXml(cocos2d::Color3B ColorVariety, bool bTextMode)
{
    m_ColorVariety = ColorVariety;
    if(bTextMode)//教材书分段取时间，取第一个单词的开始时间
    {
        m_fXmlTime = (m_vecWords.size() == 0 ? 0:m_vecWords[0].w_voiceBegin) / 100.0f;
    }
    else//原版书，从零开始计时就好
    {
        m_fXmlTime = 0.0f;
    }
    
    m_nCurChange = 0;
    unschedule(CC_SCHEDULE_SELECTOR(MyText::showXmlOntimer));
    schedule(CC_SCHEDULE_SELECTOR(MyText::showXmlOntimer));
}

void MyText::showXmlOntimer(float dt)
{
    float speed = BookDataManager::getInstance()->getBookSoundSpeed();
    m_fXmlTime += dt*speed;
    for ( ; m_nCurChange < m_vReadTime.size(); m_nCurChange++)
    {
        if (m_vReadTime[m_nCurChange] != -1)
        {
            if (m_vReadTime[m_nCurChange] <= m_fXmlTime * 100)
            {//应该变色
                auto p = m_pLabel->getLetter(getLetterIndex(m_nCurChange));
                //auto p = m_pLabel->getLetter(m_nCurChange);
                if (p && p->getColor() != m_ColorVariety) {
                    p->setColor(m_ColorVariety);
                }
            }
            else
                break;
        }
    }
    if(m_nCurChange >= m_vReadTime.size())
    {
        unschedule(CC_SCHEDULE_SELECTOR(MyText::showXmlOntimer));
        resetState();
        resetColor();
    }
}



/* 设置错误 */
void MyText::setErrorIds(vector<WordResult*>& mapErrorIds)
{
//    CCASSERT(mapErrorIds.size() <= m_vecWords.size(), "Error word can not be more over all !!!");
//    vector<WordResult*>::iterator iter;
//    for (iter = mapErrorIds.begin(); iter != mapErrorIds.end(); iter++) {
//         WordResult* oneWord = *iter;
//        m_vecWords.at(oneWord->w_id).w_state = oneWord->w_state;
//        checkColor(oneWord->w_id);
//    }
    
    int Count = MIN(mapErrorIds.size(), m_vecWords.size());
    for (int i = 0; i < Count; i++) {
        WordResult* oneWord = mapErrorIds.at(i);
        m_vecWords.at(i).w_state = oneWord->w_state;//教材的单词ID分阶，不能按索引了oneWord->w_id
        m_vecWords.at(i).w_bitvec = oneWord->w_bitvec;
        for (int j = 0; j < m_vecWords.at(i).w_string.length(); j++) {
            checkColor(i, j, m_vecWords.at(i).w_bitvec.test(j));
        }
    }
}

int MyText::getLetterCnt()
{
    int i = 0;
    int c =0;
    for(c = 0;c < m_textDisplay.length() ;c += getLetterJump(c))
    {
        i++;
    }
    return i;
}
//获取i的值对应的序号
int MyText::getLetterIndex(int c)
{
    if(c >= m_textDisplay.length() || c < 0)
        return 0;
    int r = 0;
    for(int i = 0;r <= m_textDisplay.length() ;i++)
    {
        
        r += getLetterJump(r);
        if(r > c)
            return i;
    }
    return 0;
}
//当前位置的应该后跳几个
int MyText::getLetterJump(int c)
{
    unsigned char p = (unsigned char)(m_textDisplay.c_str()[c]);
    return my_utf8_skip_data[p];
}

void MyText::checkColor(int iWord, int iLetter, int iState)
{
    int iLetterIndex = m_vecWords.at(iWord).w_start;
    if(iLetter == -1)//整个单词变色
    {
        for (int i = 0; i < m_vecWords.at(iWord).w_string.length(); i++) {
            if (iLetter >= 0 && i != iLetter) {//指定单个字母，不匹配的跳过颜色检测
                continue;
            }
            
            if (m_vecWords.at(iWord).w_state == eWordWrong) {
                this->getLetter(iLetterIndex+i)->setColor(Color3B::RED);
            } else if (m_vecWords.at(iWord).w_state == eWordNormal){
                this->getLetter(iLetterIndex+i)->setColor(Color3B(65,189,140));
            } else {
                this->getLetter(iLetterIndex+i)->setColor(Color3B(161,96,49));
            }
        }
    }
    else//单个字母变色
    {
        if (iState == 0)
        {
            this->getLetter(iLetterIndex + iLetter)->setColor(Color3B::RED);
        }
        else
        {
            this->getLetter(iLetterIndex + iLetter)->setColor(Color3B(65,189,140));
        }
    }
    
}

void MyText::resetState()
{
    for (int i = 0; i < m_vecWords.size(); i++) {
        m_vecWords.at(i).w_state = eWordDefault;
    }
}

void MyText::resetColor()
{
//    SLog("resetColor1")
//    m_pLabel->setTextColor(Color4B(m_ColorDefault.r, m_ColorDefault.g, m_ColorDefault.b, 255));
    unschedule(CC_SCHEDULE_SELECTOR(MyText::showXmlOntimer));
    for (int i = 0; i < getLetterCnt(); i++) {
        auto p = m_pLabel->getLetter(i);
        if (p) {
            p->setColor(m_ColorDefault);
        }
    }
//    SLog("resetColor2")
}

void MyText::setXmlString(std::string xml)
{
    m_bXml = true;
    
    m_vecWords.clear();
    //////////////////////////////如果不单独提供String，自己解析
    tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument();
    tinyxml2::XMLElement* rootNode = NULL;
    std::string sentence = "";
    do
    {
        xmlDoc->Parse((const char *)xml.c_str(), xml.size());
        rootNode = xmlDoc->RootElement();
        if (NULL == rootNode)
        {
            CCLOG("read root node error");
            break;
        }
        tinyxml2::XMLElement* pNodeP = (rootNode)->FirstChildElement();
        while (NULL != pNodeP)
        {
            if (!strcmp(pNodeP->Value(), "p"))//找到P
            {
                tinyxml2::XMLElement* pNodeS = (pNodeP)->FirstChildElement();
                while (NULL != pNodeS)
                {
                    if (!strcmp(pNodeS->Value(), "s"))
                    {//找到S
                        tinyxml2::XMLElement* pNodeW = (pNodeS)->FirstChildElement();
                        while (NULL != pNodeW)
                        {
                            if (!strcmp(pNodeW->Value(), "w")
//                                && pNodeW->Attribute("start_audio")
//                                && pNodeW->Attribute("end_audio")
                                )//找到w
                            {
                                //log("%s,%s,%s",
                                std::string strOne = pNodeW->FirstChild()->Value();
                                
                                WordAttr word;
                                word.w_index = (int)m_vecWords.size();
                                word.w_start = (int)sentence.length();
                                
                                word.w_string = strOne;
                                word.w_state = eWordDefault;
                                
                                if (pNodeW->Attribute("variants")) {
                                    word.w_word = pNodeW->Attribute("variants");
                                } else {
                                    word.w_word = word.w_string;
                                }
                                if (pNodeW->Attribute("start_audio") && pNodeW->Attribute("end_audio")) {
                                    word.w_voiceBegin = atoi(pNodeW->Attribute("start_audio"));
                                    word.w_voiceEnd = atoi(pNodeW->Attribute("end_audio"));
                                    
                                    m_vecWords.push_back(word);
                                    
                                    sentence += strOne;
                                    //句号、叹号、问号规则，特殊照顾
                                    string strEnd = strOne.substr(strOne.length()-1);
                                    if (strEnd == "."
                                        || strEnd == "!"
                                        || strEnd == "?") {
                                        sentence += " ";
                                    }
                                } else {
//                                    word.w_voiceBegin = 0;
//                                    word.w_voiceEnd = 0;
//                                    m_vecWords.push_back(word);
//
                                    sentence += strOne;
                                }
                                pNodeW = pNodeW->NextSiblingElement();
                                if (pNodeW) {//加空格隔开单词
                                    string strEnd = sentence.substr(sentence.length()-1);
                                    if(strEnd != " " && strEnd != "-"){
                                        sentence += " ";
                                    }
                                }
                            }
                            else
                            {
                                if (strcmp(pNodeW->Value(), "br") == 0) {
                                    sentence += "\n";
                                }
                                
                                pNodeW = pNodeW->NextSiblingElement();
                            }
                            
                        }
                    }
                    pNodeS = pNodeS->NextSiblingElement();
                    
                    if(pNodeS){
                        if(sentence.length() > 0)
                        {
                            string strEnd = sentence.substr(sentence.length()-1);
                            if(strEnd != " "){
                                sentence += " ";
                            }
                        }
                    }
                }
                
            }
            pNodeP = pNodeP->NextSiblingElement();
            if (pNodeP) {
                //段落之间要换行
                sentence += "\n";
            }
        }
    }while (0);
    delete xmlDoc;
    
    m_vReadTime.clear();
    m_vReadTime.resize(sentence.length(), -1);
    
    for (int i = 0; i < m_vecWords.size(); i++)
    {
        for (int j = 0; j < m_vecWords[i].w_string.length(); j++) {
            int index = m_vecWords[i].w_start + j;
            m_vReadTime[index] = m_vecWords[i].w_voiceBegin + (m_vecWords[i].w_voiceEnd - m_vecWords[i].w_voiceBegin) * j / m_vecWords[i].w_string.length();
        }
    }
    
    //最后一个单词截止时间，减去第一个单词的开始时间，即获得当前句子的范读总时长;//2018年01月15日11:56:02
    int iScaleTime = m_vecWords[m_vecWords.size()-1].w_voiceEnd - m_vecWords[0].w_voiceBegin;
    SILog("歌词时间==》", iScaleTime)
    m_totalSecs = ceil(iScaleTime/100.0f);
    
    setString(sentence);
}

void MyText::setStrByWords(const vector<WordAttr> vecWords)
{
    m_bXml = true;
    
    m_vecWords.clear();
    m_vecWords = vecWords;
    
    string sentence = "";
    int idx = 0;
    
    for (idx = 0; idx < m_vecWords.size(); idx++) {
        WordAttr oneWord = m_vecWords.at(idx);
        sentence += oneWord.w_string;
    }
    
    m_vReadTime.clear();
    m_vReadTime.resize(sentence.length(), -1);
    
    for (int i = 0; i < m_vecWords.size(); i++)
    {
        for (int j = 0; j < m_vecWords[i].w_string.length(); j++) {
            int index = m_vecWords[i].w_start + j;
            if (index >= m_vReadTime.size()) {
                break;//add by dj 会有数组越界的情况，先简单处理下。
            }
            m_vReadTime[index] = m_vecWords[i].w_voiceBegin + (m_vecWords[i].w_voiceEnd - m_vecWords[i].w_voiceBegin) * j / m_vecWords[i].w_string.length();
        }
    }
    
    
    setString(sentence);
}

void MyText::wordRectWithStr()
{
    //文本行数
    int nLines = m_pLabel->getStringNumLines();
    //整体大小
    Size sizeTotal = m_pLabel->getContentSize();
    //单行高度
    float fHight = sizeTotal.height / nLines;
    
    int iCurLine = -1;          //行索引
    int iWordIndex = -1;        //单词索引
    int iLetterStart = 0;       //单词开始索引
    Rect rect = Rect(0,0,0,0);  //单词Rect
    bool bWordEnd = false;      //标记单词结束

    m_vecWords.clear();
    std::string sWord = "";     //单词字串

    for(int i = 0 ; i < m_textDisplay.length(); i ++)
    {
        int idx = getLetterIndex(i);
        auto *p = m_pLabel->getLetter(idx);
        auto c = m_textDisplay.at(idx);
        if (!p) {
            if (c == '\n') {
                bWordEnd = true;
            }
            continue;
        }

        //获取位置
        auto origin = p->getPosition();
        auto size = p->getContentSize();

        //过滤标点符号分割
        std::string::size_type pos = TEXT.find(c);
        if ((size.width < 0.2 && size.height < 0.2)||(pos == std::string::npos)) {
            bWordEnd = true;
            continue;   //龟孙的空格
        }
        //获取所在行
        int n = origin.y / fHight;
        if (n != iCurLine || bWordEnd)//行有变化
        {
            if (bWordEnd)
            {
                WordAttr word;
                word.w_index = iWordIndex;
                word.w_start = iLetterStart;
                word.w_rect = rect;
                word.w_string = sWord;
                word.w_state = eWordDefault;
                m_vecWords.push_back(word);
            }
            bWordEnd = false;
            sWord = "";
            iLetterStart = idx;
            iWordIndex ++;
            rect.origin.x = origin.x - size.width / 2;
            rect.origin.y = n * fHight;
            rect.size.height = fHight;
            rect.size.width = size.width;
            iCurLine = n;
        }
        else
        {
            rect.size.width = origin.x + size.width / 2 -  rect.origin.x;
        }
        sWord += c;
    }
    //结尾一词，添加数组
    if (bWordEnd)
    {
        WordAttr word;
        word.w_index = iWordIndex;
        word.w_start = iLetterStart;
        word.w_rect = rect;
        word.w_string = sWord;
        word.w_state = eWordDefault;
        m_vecWords.push_back(word);
        bWordEnd = false;
    }

}

void MyText::wordRectWithXml()
{
    //文本行数
    int nLines = m_pLabel->getStringNumLines();
    //整体大小
    Size sizeTotal = m_pLabel->getContentSize();
    //单行高度
    float fHight = sizeTotal.height / nLines;
    
    for (int i = 0; i < m_vecWords.size(); i++) {
        Rect rect = Rect(0,0,0,0);  //单词Rect
        for (int j = 0; j < m_vecWords[i].w_string.length(); j++) {
            int idx = getLetterIndex(m_vecWords[i].w_start + j);
            auto *p = m_pLabel->getLetter(idx);
            if (!p) {
                continue;
            }
            
            auto origin = p->getPosition();
            auto size = p->getContentSize();
            if (j == 0) {
                int n = origin.y / fHight;  //获取所在行
                rect.origin.x = origin.x - size.width / 2;
                rect.origin.y = n * fHight;
                rect.size.height = fHight;
                rect.size.width = size.width;
            } else {
                rect.size.width = origin.x + size.width / 2 -  rect.origin.x;
            }
        }
        m_vecWords[i].w_rect = rect;
    }
}

void MyText::drawWordRect()
{
#if DRAW_RECT
    //画出单词矩形
    DrawNode* drawNode = (DrawNode*)m_pLabel->getChildByTag(3);
    if (!drawNode) {
        drawNode = DrawNode::create();
        drawNode->setTag(3);
        m_pLabel->addChild(drawNode);
    }
    drawNode->clear();
    
    for (int i = 0; i < m_vecWords.size(); i++)
    {
        Vec2 vertices[4]=
        {
            Vec2(m_vecWords.at(i).w_rect.origin.x, m_vecWords.at(i).w_rect.origin.y),
            Vec2(m_vecWords.at(i).w_rect.size.width + m_vecWords.at(i).w_rect.origin.x, m_vecWords.at(i).w_rect.origin.y),
            Vec2(m_vecWords.at(i).w_rect.size.width + m_vecWords.at(i).w_rect.origin.x, m_vecWords.at(i).w_rect.size.height + m_vecWords.at(i).w_rect.origin.y),
            Vec2(m_vecWords.at(i).w_rect.origin.x, m_vecWords.at(i).w_rect.size.height + m_vecWords.at(i).w_rect.origin.y)
        };
        drawNode->drawLine(vertices[0], vertices[1], Color4F(1.0, 0.0, 0.0, 1.0));
        drawNode->drawLine(vertices[0], vertices[3], Color4F(0.0, 1.0, 0.0, 1.0));
        drawNode->drawLine(vertices[2], vertices[3], Color4F(0.0, 0.0, 1.0, 1.0));
        drawNode->drawLine(vertices[1], vertices[2], Color4F(0.0, 0.0, 0.0, 1.0));
    }
#endif
}


