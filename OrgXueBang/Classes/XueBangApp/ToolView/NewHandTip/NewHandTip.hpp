//
//  NewHandTip.hpp
//  ReadingMate
//
//  Created by 丁晶 on 2017/6/28.
//
//

#ifndef NewHandTip_hpp
#define NewHandTip_hpp

#include "stdafx.h"

class NewHandTip : public cocos2d::Node
{
public:
    
    static NewHandTip* create(cocos2d::Node* parentNode, Vec2 handPos, const string& msg);
    ~NewHandTip();
    
    virtual bool init();
    
    static NewHandTip* create(cocos2d::Node* parentNode, Node* node, const string& msg);
    void setHandAndMsg();
    
    static NewHandTip* create(cocos2d::Node* parentNode, vector<Vec2>& posList, vector<string>& msgList);
    
    static NewHandTip* create(cocos2d::Node* parentNode, vector<Node*>& nodeList, vector<string>& msgList);
    
    static bool isShow(const string& sceneName);
    static void setShow(const string& sceneName);
    void closeHandTip();
    
    static NewHandTip* create2(cocos2d::Node* parentNode);
    void addImg(const string& imgName, Vec2 imgPos, int iRotate = 0);
    void addImg(const string& imgName, Node* node, Vec2 imgPos, int iRotate = 0);
    
    void createClip();
    void createHole(Vec2 holePos, Size holeSize);
    void createHole(Node* node, Vec2 offPos, Size holeSize);
    
    void addImgAndHole(const string& imgName, Node* node, Vec2 imgPos, int iRotate, Vec2 offPos, Size holeSize);
    
    void addImgAndClip(const string& imgName, Node* node, Vec2 imgPos, Vec2 clipPos, Size sizeClip, bool bBlur = true);
protected:
    
    vector<Vec2> m_posList;
    vector<string> m_msgList;
private:
    cocos2d::Node* m_ui;
    cocos2d::ClippingNode* m_clipping;
    cocos2d::Node* m_stencil;
};


#endif /* NewHandTip_hpp */
