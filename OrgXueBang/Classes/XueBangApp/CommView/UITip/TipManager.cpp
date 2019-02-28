//
//  TipManager.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/7.
//
//

#include "TipManager.hpp"
#include "TestTip.hpp"
#include "TipLoader.hpp"
//#include "LoginSignReward.hpp"
#include "BoyWithMsg.hpp"
#include "TipTurnPage.hpp"
#include "TipHobbySucc.hpp"
#include "TipWaiting.hpp"

TipManager::TipManager():m_tipList(nullptr)
{
    m_tipString = "";
    m_tipView = nullptr;
    m_tipParent = nullptr;
    
    m_tipList = new TipList;
}

TipManager::~TipManager()
{
    CC_SAFE_DELETE(m_tipList);
}

void TipManager::onExitScene()
{
    TipList::iterator it = m_tipList->begin();
    while (it!=m_tipList->end()) {
        TipLayer* tip = *it;
        if (tip) {
            tip->removeFromParent();
        }
        it ++;
    }
    m_tipList->clear();
}

TipManager* TipManager::getInstance()
{
    static TipManager s_instance;
    return &s_instance;
}

TipLayer* TipManager::openTip(eTipType type, eZorder order /*= eZorderTipLayer*/)
{
    TipLayer* tip = nullptr;
    if(isOpenTip(type, tip) && tip)
    {
        return tip;
    }
    
    tip = createTipWithType(type);
    tip->setTipType(type);
    tip->setLocalZOrder(order);
    
    m_tipList->push_back(tip);
    UiBase::getInstance()->getRunningScene()->addChild(tip, eZorderPopupLayer);
    tip->openTip();
    return tip;
}

TipLayer* TipManager::showTip(eTipType type, TipLayer::eActionType action, float interval)
{
    TipLayer* tip = nullptr;
    if(isOpenTip(type, tip) && tip)
    {
        return tip;
    }
    
    tip = createTipWithType(type);
    tip->setTipType(type);
    tip->setLocalZOrder(eZorderTipLayer);
    tip->setActionType(action);
    
    m_tipList->push_back(tip);
    UiBase::getInstance()->getRunningScene()->addChild(tip, eZorderPopupLayer);
    tip->openTip();
    tip->runAction(Sequence::create(DelayTime::create(interval),CallFunc::create( CC_CALLBACK_0(TipLayer::closeTip, tip)), NULL));
    return tip;
}

bool TipManager::removeTip(TipLayer *tip)
{
    TipList::iterator it = m_tipList->begin();
    while (it != m_tipList->end()) {
        if (*it == tip) {
            tip->removeFromParent();
            m_tipList->erase(it);
            return true;
        }
        it ++;
    }
    return false;
}

bool TipManager::isEmpty()
{
    return m_tipList->empty();
}

int TipManager::tipListNum()
{
    return (int)m_tipList->size();
}

void TipManager::closeCurTip()
{
    TipLayer* cur = getCurTip();
    if (cur) {
        cur->closeTip();
    }
}

bool TipManager::isOpenTip(eTipType type, TipLayer *&tip)
{
    TipList::iterator it = m_tipList->begin();
    while (it!=m_tipList->end()) {
        TipLayer* m_tip = *it;
        if (m_tip && m_tip->getTipType() == type) {
            tip = m_tip;
            return true;
        }
        it ++;
    }
    return false;
}

TipLayer* TipManager::createTipWithType(eTipType type)
{
    TipLayer* tip = nullptr;
    switch (type) {
        case eTipTest:
            tip = TestTip::create();
            break;
        case eTipLoader:
//            tip = TipLoader::create();
            break;
        case eTipWaiting:
            tip = TipWaiting::create();
            break;
        case eTipSignReward:
//            tip = LoginSignReward::create();
            break;
        case eTipBoyWithMsg:
            tip = BoyWithMsg::create();
            break;
        case eTipTurnPage:
            tip = TipTurnPage::create();
            break;
            
        case eTipHobbySucc:
            tip = TipHobbySucc::create();
            break;
        default:
            CCAssert(false, "error, no this type!");
            break;
    }
    return tip;
}

void TipManager::showTipWithString(const char *str, float dt /*= 2.0f*/, bool visible /*=true*/)
{
#if 1
    showMsgTips(str);
#else
    Node *parent = UiBase::getInstance()->getRunningScene();
    
    Label* m_pLabelMsg = Label::create();
    m_pLabelMsg->setSystemFontSize(40);
    m_pLabelMsg->setString(str);
    auto size =  m_pLabelMsg->getContentSize();
    m_pLabelMsg->setPosition(size.width / 2,size.height/2);
    LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 125),size.width,size.height);
    color->addChild(m_pLabelMsg);
    
    
//    auto winsize = Director::getInstance()->getWinSize();
    auto winsize = parent->getContentSize();
    color->setPosition(Vec2(winsize.width/2-size.width/2, winsize.height/2-size.height/2));
    parent->addChild(color,(int)parent->getChildrenCount() + 100);
    color->runAction(Sequence::create(
                               DelayTime::create(2),
                               Spawn::create(MoveBy::create(0.5, Vec2(0, 100)),FadeOut::create(0.5), NULL),
                               RemoveSelf::create(), NULL));
//    m_pLabelMsg->runAction(Sequence::create(DelayTime::create(1),FadeOut::create(0.5), NULL));
#endif
}

void TipManager::removeTips()
{
    m_tipView->removeFromParent();
    m_tipView = nullptr;
    m_tipString = "";
    m_tipParent = nullptr;
}

TipLayer* TipManager::getCurTip()
{
    if(m_tipList->size() == 0)
    {
        return nullptr;
    }
    TipLayer* tip = m_tipList->back();
    return tip;
}

TipLayer* TipManager::getTipWithType(eTipType type)
{
    TipLayer* tip = nullptr;
    if (isOpenTip(type, tip) && tip) {
        return tip;
    }
    return nullptr;
}

eTipType TipManager::getCurTipType()
{
    TipLayer* cur = getCurTip();
    if (cur) {
        return cur->getTipType();
    }
    return eTipNone;
}

void TipManager::showMsgTips(std::string msg, int fontSize, Vec2 showPos)
{
    if (msg.empty()) {
        return;
    }
    
    auto lastNode = Director::getInstance()->getRunningScene()->getChildByName("showMsgTipsNode");
    if(lastNode){
        return;
    }
    auto winsize = Director::getInstance()->getWinSize();
    
    Node* node = Node::create();
    node->setName("showMsgTipsNode");
    if (showPos == Vec2::ZERO) {
        node->setPosition(Vec2(winsize.width * 0.5f, winsize.height * 0.5f + 100.0f));
    }else {
        node->setPosition(showPos);
    }
    
    TTFConfig ttfConfig;
    ttfConfig.fontFilePath = MYDEF_TTF_WDT_TIANNIU;
    ttfConfig.fontSize = fontSize;
    ttfConfig.distanceFieldEnabled = false;
    ttfConfig.outlineSize = 0;
    
    Label* label = Label::createWithTTF(ttfConfig, msg);
    CCAssert(NULL != label, "");
    
    label->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(Vec2(node->getContentSize().width*.5, node->getContentSize().height*.5));

    label->disableEffect();
    label->setTextColor(Color4B(250, 255, 203, 255));
    node->addChild(label);
    
    cocos2d::ui::Scale9Sprite *sp = cocos2d::ui::Scale9Sprite::create("SrcImg/tip_under.png");
    if ( sp ) {
        label->setMaxLineWidth(winsize.width - 80.0f);
        Size labelSize = label->getContentSize();
        sp->setContentSize(Size(labelSize.width + 40.0f, labelSize.height + 40.0f));

        sp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        sp->setPosition(Vec2(0.0f, 6.0f));
        node->addChild(sp, -1);
        Director::getInstance()->getRunningScene()->addChild(node, 999999);
    }else {
        return;
    }
    
    float time = 0.8f;
    Point p = node->getPosition();
    node->setRotation3D(Vec3(-80, 0, 0));
    node->setPosition(Vec2(p.x, p.y + 100));
    auto m1 = MoveTo::create(time * 0.85, p);
    auto r1 = RotateTo::create(time, Vec3::ZERO);
    auto action1 = Spawn::create(EaseElasticOut::create(m1, time + 0.5f), EaseElasticOut::create(r1, time + 0.5f), NULL);
    auto call = CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, node));
    node->runAction(Sequence::create(Hide::create(),
                                     DelayTime::create(0.15f),
                                     Show::create(),
                                     action1,
                                     DelayTime::create(0.8f),
                                     call, NULL));
}

void TipManager::showLoading(Node* parent, Vec2 pos)
{
    if (!parent) {
        return;
    }
    if (parent->getChildByName("skeleton_show_loading")) {
        parent->removeChildByName("skeleton_show_loading");
    }
    
    SkeletonAnimation* skeleton_loading = SkeletonAnimation::createWithJsonFile("spine/loading_second.json", "spine/loading_second.atlas", 1.0f);
    if (skeleton_loading) {
        if (pos != Vec2::ZERO) {
            skeleton_loading->setPosition(pos);
        }else {
            skeleton_loading->setPosition(Vec2(parent->getContentSize().width*0.5f, parent->getContentSize().height*0.5f));
        }
        skeleton_loading->setName("skeleton_show_loading");
        skeleton_loading->setAnimation(0, "animation", true);
        parent->addChild(skeleton_loading, 999);
        
        Text* txt_loading = Text::create("Loading...",MYDEF_TTF_CHINESE, 28);
        if (txt_loading) {
            txt_loading->setColor(Color3B(255,255,255));
            txt_loading->setPosition(Vec2(skeleton_loading->getContentSize().width*0.5f, skeleton_loading->getContentSize().height*0.5f));
            skeleton_loading->addChild(txt_loading);
        }
    }
}

void TipManager::removeLoading(Node* parent)
{
    if (!parent) {
        return;
    }
    SkeletonAnimation* skeletonNode = dynamic_cast<SkeletonAnimation*>(parent->getChildByName("skeleton_show_loading"));
    if (skeletonNode) {
        skeletonNode->clearTracks();
        skeletonNode->removeAllChildren();
        skeletonNode->removeFromParent();
        skeletonNode = NULL;
    }
}
