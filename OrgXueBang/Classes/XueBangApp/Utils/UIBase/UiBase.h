//
//  UiBase.h
//  ReadingMate
//
//  Created by 丁晶 on 2016/11/30.
//
//

#ifndef UiBase_h
#define UiBase_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using namespace std;

#define TOP_PANEL_NAME "panel_top"
#define CENTER_PANEL_NAME "panel_center"
#define BOTTOM_PANEL_NAME "panel_bottom"

class UiBase
{
private:
    static UiBase* m_uiBase;
    int m_visibleHeight;
    int m_centerPanelHeight;
    int m_topPanelHeight;
    int m_bottomPanelHeight;
    
public:
    static UiBase* getInstance();
    int getVisibleHeight(){return m_visibleHeight;};
    void setVisibleHeight(int h){m_visibleHeight = h;};
    int getCenterPanelHeight(){return m_centerPanelHeight;};
    
    cocos2d::Node* loadCSB(const std::string& filename);
    
    static int setText(cocos2d::Node* text, std::string& str, int minHeight = 0);
    
    static void setChatText(cocos2d::Node* text, const std::string& str, int minHeight, int minWidth, int maxWidth);

    static string& replace_all_distinct(string& str, const string& old_value, const string& new_value);
    
    static void saveJsonFile(rapidjson::Document &doc, string& fileName);
    
    static void loadJsonFile(rapidjson::Document &doc, string& fileName);
    
    static void addClickEventListener(cocos2d::ui::Widget* node, const cocos2d::ui::Widget::ccWidgetClickCallback& callback);
    
    static void StringReplace(string &strBase, const string& strSrc, const string& strDes);
    
    static void LoadTexture(cocos2d::ui::ImageView* pImg, const string& textureName, cocos2d::Size panelSize);
    
    static void removeHtmlChar(string& str);
    static void removeUrlChar(string& str);
    
    static void addAudioEventListener(cocos2d::ui::Widget* node, const cocos2d::ui::Widget::ccWidgetClickCallback& callback);
    
    static void playVideo(const string& url);
    
    static cocos2d::Node* getRunningSceneAndReleaseRes();
    static cocos2d::Node* getRunningScene();
    
    static void resizeBackImg(cocos2d::Node* ui);
    
    static void resizeText(cocos2d::ui::Text* node, int maxW);
    
    static void setBottomBtnPos(cocos2d::Node* node_btns);
    
    static void resizeTextRect(cocos2d::ui::Text* node);
    
    static cocos2d::Node* createSpine(cocos2d::Node* nodeParent, float x, float y, const string& jsonFile, const string& atlasFile, const string& animationFile,
                            bool bLoop = false, float scale = 1.0f, float timeScale = 1.0f);
};


#endif /* UiBase_h */
