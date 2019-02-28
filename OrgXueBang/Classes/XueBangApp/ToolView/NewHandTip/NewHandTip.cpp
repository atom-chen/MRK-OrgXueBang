//
//  NewHandTip.cpp
//  ReadingMate
//
//  Created by 丁晶 on 2017/6/28.
//
//

#include "NewHandTip.hpp"

#include "SceneManager.hpp"
#include "StringUtil.hpp"

#define SAVE_NEW_HAND_NAME "newHand"

NewHandTip *NewHandTip::create(Node* parent, Vec2 handPos, const string& msg)
{
    NewHandTip *ret = new (std::nothrow) NewHandTip();
    ret->m_posList.push_back(handPos);
    ret->m_msgList.push_back(msg);
    if (ret && ret->init())
    {
        parent->addChild(ret->m_ui, 10000000);
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool NewHandTip::init()
{
    m_ui = UiBase::getInstance()->loadCSB("View/ToolView/UINewHand.csb");
    m_ui->addChild(this, -1);
    m_ui->setTag(eLayerUserTeach);
    
    auto panel = (Layout*)m_ui->getChildByName(CENTER_PANEL_NAME);
    panel->addClickEventListener([=](Ref* pSender){
        if (this->m_posList.size() > 0) {
            setHandAndMsg();
        } else {
            m_ui->removeFromParent();
        }
    });
    
    auto img_hand = (ImageView*)panel->getChildByName("img_hand");
    
    img_hand->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5, 1.1), ScaleTo::create(1.0, 0.9), ScaleTo::create(0.5, 1.0), NULL)));
    
    setHandAndMsg();
    
    return true;
}

NewHandTip::~NewHandTip()
{
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Image/SubTexture/ToolView/UINewHand.plist");
}

NewHandTip *NewHandTip::create(Node* parent, Node* node, const string& msg)
{
    auto pos = node->getParent()->convertToWorldSpace(node->getPosition());
    auto pos1 = parent->convertToNodeSpace(pos);
    auto anchor = node->getAnchorPoint();
    auto nodeSize = node->getContentSize();
    pos1.x -= (anchor.x - 0.5) * nodeSize.width;
    pos1.y -= (anchor.y - 0.5) * nodeSize.height;
    return create(parent, pos1, msg);
}

void NewHandTip::setHandAndMsg()
{
    auto panel = (Widget*)m_ui->getChildByName(CENTER_PANEL_NAME);
    
    auto img_hand = (ImageView*)panel->getChildByName("img_hand");
    img_hand->setPosition(m_posList[0]);
    img_hand->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5, 1.1), ScaleTo::create(1.0, 0.9), ScaleTo::create(0.5, 1.0), NULL)));
    
    auto img_dlg = (ImageView*)panel->getChildByName("img_dlg");
    if (m_posList[0].y > 500) {
        img_dlg->setPositionY(m_posList[0].y - 250);
    } else {
        img_dlg->setPositionY(m_posList[0].y + 200);
    }
    
    auto txt_msg = (Text*)img_dlg->getChildByName("txt_msg");
    txt_msg->setString(m_msgList[0]);
//    CCLOG("len = %d", m_msgList[0].length());
    if (m_msgList[0].length() > 72) {
        txt_msg->setFontSize(28);
    } else {
        txt_msg->setFontSize(32);
    }
    
    m_posList.erase(m_posList.begin());
    m_msgList.erase(m_msgList.begin());
}

NewHandTip* NewHandTip::create(Node* parent, vector<Vec2>& posList, vector<string>& msgList)
{
    NewHandTip *ret = new (std::nothrow) NewHandTip();
    ret->m_posList = posList;
    ret->m_msgList = msgList;
    if (ret && ret->init())
    {
        parent->addChild(ret->m_ui, 100000000);
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

NewHandTip* NewHandTip::create(Node* parent, vector<Node*>& nodeList, vector<string>& msgList)
{
    vector<Vec2> posList;
    for (auto it : nodeList) {
        auto pos = it->getParent()->convertToWorldSpace(it->getPosition());
        auto pos1 = parent->convertToNodeSpace(pos);
        auto anchor = it->getAnchorPoint();
        auto nodeSize = it->getContentSize();
        pos1.x -= (anchor.x - 0.5) * nodeSize.width;
        pos1.y -= (anchor.y - 0.5) * nodeSize.height;
        posList.push_back(pos1);
    }
    
    return create(parent, posList, msgList);
}

bool NewHandTip::isShow(const string &sceneName)
{
    auto newHand = UserDefault::getInstance()->getStringForKey(SAVE_NEW_HAND_NAME, "");
    vector<string> nameList;
    StringUtil::splitString(newHand.c_str(), ',', nameList);
    for (auto it : nameList) {
        if (it == sceneName) {
            return true;
        }
    }
    return false;
}

void NewHandTip::setShow(const string &sceneName)
{
    auto newHand = UserDefault::getInstance()->getStringForKey(SAVE_NEW_HAND_NAME, "");
    if (newHand.length() < 1) {
        newHand = sceneName;
    } else {
        newHand += "," + sceneName;
    }
    
    UserDefault::getInstance()->setStringForKey(SAVE_NEW_HAND_NAME, newHand);
}

void NewHandTip::closeHandTip()
{
    if(m_ui)
    {
        m_ui->removeFromParent();
    }
}

NewHandTip* NewHandTip::create2(Node* parent)
{
    NewHandTip *ret = NewHandTip::create(parent, Vec2(0,0), "");
    
    if (ret == nullptr)
    {
        return nullptr;
    }
    auto panel = (Widget*)ret->m_ui->getChildByName(CENTER_PANEL_NAME);
    
    auto img_hand = (ImageView*)panel->getChildByName("img_hand");
    img_hand->setVisible(false);
    auto img_dlg = (ImageView*)panel->getChildByName("img_dlg");
    img_dlg->setVisible(false);
    
    ret->createClip();
    return ret;
}

void NewHandTip::addImg(const string &imgName, cocos2d::Vec2 imgPos, int iRotate){
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Image/SubTexture/ToolView/UINewHand.plist", "Image/SubTexture/ToolView/UINewHand.png");
    
    auto node = ImageView::create();
    node->loadTexture("Image/SubImg/ToolView/UINewHand/" + imgName, Widget::TextureResType::PLIST);
    node->setPosition(imgPos);
    if(iRotate != 0)
        node->setRotation(iRotate);
    m_ui->addChild(node, 10);
}

void NewHandTip::addImg(const string& imgName, Node* node, Vec2 imgPos, int iRotate)
{
    auto pos = node->getParent()->convertToWorldSpace(node->getPosition());
    auto pos1 = m_ui->convertToNodeSpace(pos);
    auto anchor = node->getAnchorPoint();
    auto nodeSize = node->getContentSize();
    pos1.x -= (anchor.x - 0.5) * nodeSize.width;
    pos1.y -= (anchor.y - 0.5) * nodeSize.height;
    pos1.add(imgPos);
    addImg(imgName, pos1, iRotate);
}

void NewHandTip::createClip()
{
    auto panel = (Widget*)m_ui->getChildByName(CENTER_PANEL_NAME);
    auto Panel_1 = panel->getChildByName("Panel_1");
    Panel_1->setVisible(false);
    
    auto screenSize = SceneManager::getInstance()->m_fullSceneSize;
    m_clipping = ClippingNode::create();
//    m_clipping->setPosition(screenSize.width / 2, screenSize.height / 2);
    m_ui->addChild(m_clipping, -1);
    
    m_clipping->setInverted(true);
    m_clipping->setAlphaThreshold(0.5);
    
    m_stencil = Node::create();
    m_clipping->setStencil(m_stencil);
    
    auto darkBack = Layout::create();
    darkBack->setBackGroundColor(Color3B::BLACK);
    darkBack->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    darkBack->setOpacity(200);
//    darkBack->setContentSize(screenSize);
//    darkBack->setPosition(Vec2(-screenSize.width / 2, -screenSize.height / 2));
    darkBack->setContentSize(Size(1500, 2000));
    darkBack->setAnchorPoint(Vec2(0.5, 0.5));
    darkBack->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2));
    m_clipping->addChild(darkBack, -1);
}

void NewHandTip::createHole(cocos2d::Vec2 holePos, cocos2d::Size holeSize)
{
//    auto screenSize = Game::sharedGame()->m_fullSceneSize;
    
    auto testNode = cocos2d::ui::Scale9Sprite::create("SrcImg/animal.jpg");
//    testNode->setScale(holeSize.width / 200.0, holeSize.height / 200.0);
    testNode->setContentSize(holeSize);
//    testNode->setPosition(Vec2(holePos.x - screenSize.width / 2, holePos.y - screenSize.height / 2));
    testNode->setPosition(holePos);
    m_stencil->addChild(testNode);
    
    auto testMask = CSLoader::createNode("View/ToolView/node_newhand_mask.csb");
    auto img = testMask->getChildByName("Image_1");
    if (holeSize.width == holeSize.height) {
        img->setScale(holeSize.width / 100.0);
    } else {
        if (holeSize.width > holeSize.height) {
            float r = holeSize.height / 100.0;
            img->setScale(r);
            img->setContentSize(Size(holeSize.width / r, holeSize.height / r));
        } else {
            float r = holeSize.width / 100.0;
            img->setScale(r);
            img->setContentSize(Size(holeSize.width / r, holeSize.height / r));
        }
    }
//    img->setContentSize(holeSize);
    img->setOpacity(200);
    testMask->setPosition(holePos);
    m_ui->addChild(testMask, 1);
}

void NewHandTip::createHole(cocos2d::Node *node, cocos2d::Vec2 offPos, cocos2d::Size holeSize)
{
    auto pos = node->getParent()->convertToWorldSpace(node->getPosition());
    pos.add(offPos);
    auto pos1 = m_ui->convertToNodeSpace(pos);
    auto anchor = node->getAnchorPoint();
    auto nodeSize = node->getContentSize();
    pos1.x -= (anchor.x - 0.5) * nodeSize.width;
    pos1.y -= (anchor.y - 0.5) * nodeSize.height;
    
    createHole(pos1, holeSize);
}

void NewHandTip::addImgAndHole(const string &imgName, cocos2d::Node *node, cocos2d::Vec2 imgPos, int iRotate, cocos2d::Vec2 offPos, cocos2d::Size holeSize)
{
    auto pos = node->getParent()->convertToWorldSpace(node->getPosition());
    auto pos1 = m_ui->convertToNodeSpace(pos);
    auto anchor = node->getAnchorPoint();
    auto nodeSize = node->getContentSize();
    pos1.x -= (anchor.x - 0.5) * nodeSize.width;
    pos1.y -= (anchor.y - 0.5) * nodeSize.height;
    
    auto pos2 = pos1 + imgPos;
    addImg(imgName, pos2, iRotate);
    
    auto pos3 = pos1 + offPos;
    createHole(pos3, holeSize);
}

void NewHandTip::addImgAndClip(const string& imgName, Node* node, Vec2 imgPos, Vec2 clipPos, Size sizeClip, bool bBlur)
{
    auto pos = node->getParent()->convertToWorldSpace(node->getPosition());
    auto pos1 = m_ui->convertToNodeSpace(pos);
    auto anchor = node->getAnchorPoint();
    auto nodeSize = node->getContentSize();
    pos1.x -= (anchor.x - 0.5) * nodeSize.width;
    pos1.y -= (anchor.y - 0.5) * nodeSize.height;
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Image/SubTexture/ToolView/UINewHand.plist", "Image/SubTexture/ToolView/UINewHand.png");
    
    //添加一个圆圈遮罩，跟背景拼合
    cocos2d::ui::Scale9Sprite* scale9Sprite2 = cocos2d::ui::Scale9Sprite::create("SrcImg/test_nine.png");
    scale9Sprite2->setPosition(pos1 + clipPos);
    /*
    if(bBlur){
        scale9Sprite2->setCapInsets(Rect(5, 5, 31, 31));
    }else{
        scale9Sprite2->setCapInsets(Rect(15, 15, 11, 11));
    }
     */
    scale9Sprite2->setCapInsets(Rect(19, 19, 2, 2));//Rect(38, 38, 4, 4)
    float fScale = (sizeClip.height+30)/40.0f;
    scale9Sprite2->setContentSize((sizeClip + Size(50,30))/fScale);
    scale9Sprite2->setOpacity(200);
    scale9Sprite2->setScale(fScale);
    m_ui->addChild(scale9Sprite2);
    
    //抠图区域
    cocos2d::ui::Scale9Sprite* scale9Sprite3 = cocos2d::ui::Scale9Sprite::create("SrcImg/baner.png");
    scale9Sprite3->setPosition(pos1 + clipPos);
    scale9Sprite3->setContentSize(sizeClip + Size(50,30));
    m_stencil->addChild(scale9Sprite3);
    
    //文字提示和箭头
    if(imgName != "")
    {
        ImageView* imgTip = ImageView::create();
        imgTip->loadTexture(imgName, Widget::TextureResType::PLIST);
        imgTip->setPosition(pos1 + imgPos);
        m_ui->addChild(imgTip);
    }
}
