//
//  MyText.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/1/5.
//
//

#ifndef MyText_hpp
#define MyText_hpp

#include "PreIncludes.hpp"
#include "BookStruct.hpp"

//enum eLetterState
//{
//    eLetterNormal,
//    eLetterWrong,
//    eLetterOther,
//};
//
//typedef struct
//{
//    int             l_index;    //字母索引
//    std::string     l_string;   //字母内容
//    eLetterState    l_state;    //字母状态
//}LetterAttr;



typedef std::function<void(int, std::string, int)> MyTextTouchCallback;

class MyText : public Layer
{
public:
    static MyText* create(const std::string& text,const cocos2d::Color3B &color, float fontSize = 20,const Size& size = Size(0, 0));
    bool init(const std::string& text,const cocos2d::Color3B &color, float fontSize,const Size& size);
    /*重设文字，富文本格式*/
    void setString(const std::string& text);
    /*设置默认字体大小*/
    void setSizeDefault(float fontSize);
    /*设置所有字体大小，屏蔽富文本大小字段*/
    void setSizeAll(float fontSize);
    /*设置默认文字颜色*/
    void setColorDefault(const cocos2d::Color3B &color);
    /*设置所有文字颜色，屏蔽富文本中文字颜色*/
    void setColorAll(const cocos2d::Color3B &color);
    /*歌词模式开始*/
    void setFontFile(std::string fileName);
    /*设置字体样式*/
    void setAlignment(TextHAlignment h1, TextVAlignment v1);
    
    /*获取单个字符的指针*/
    Sprite* getLetter(int nIndex);
    /*注册点击回调*/
    void setTouchListener(MyTextTouchCallback callback);
    /*取消注册*/
    void removeTouchListener();
    
    /* 设置错误 */
    void setErrorIds(vector<WordResult*>& mapErrorIds);
    
    /*歌词模式开始*/
    void showXml(cocos2d::Color3B ColorVariety = Color3B(0xff,0xfc,0xc3), bool bTextMode = false);
    void showXmlOntimer(float dt);
    
    void setXmlString(std::string xml);
    //通过单词数组设置文本
    void setStrByWords(const vector<WordAttr> vecWords);
    
    const WordAttr& getWordAttr(int i){return m_vecWords.at(i);};
    int getWordCount(){ return m_vecWords.size(); };
private:
    //解析富文本
    void checkText();
    
    //刷新
    void Refresh();
    //点击回调
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    //根据坐标
    int GetTouchIndex(const Point& pos);
    //获取数目
    int getLetterCnt();
    //获取i的值对应的序号
    int getLetterIndex(int c);
    //当前位置的应该后跳几个
    int getLetterJump(int c);
    
    Label*                      m_pLabel        ;
    cocos2d::Color3B            m_ColorDefault  ;
    cocos2d::Color3B            m_ColorVariety  ;
    float                       m_fSizeDefault  ;
    Size                        m_Size          ;
    std::string                 m_textDisplay   ;      //解析好现实内容
    std::vector<WordAttr>       m_vecWords;
    MyTextTouchCallback         m_touchCallback ;
    std::vector<int>            m_vReadTime     ;
    float                       m_fXmlTime      ;
    int                         m_nCurChange    ;
    std::map<int, int>          m_mapIdStart    ;
    std::string                 m_strTTF;
    
    void checkColor(int iWord, int iLetter = -1, int iState = 1);
    void resetState();
    
    bool m_bXml;
    void wordRectWithStr();
    void wordRectWithXml();
    void drawWordRect();
    
    int m_iBegin;
    int m_totalSecs;    //范读的总时长
public:
    void resetColor();
    //统计播放时长时会用这个数值
    int getTotalSecs()
    {
        return m_totalSecs;
    }
};



#endif /* MyText_hpp */
