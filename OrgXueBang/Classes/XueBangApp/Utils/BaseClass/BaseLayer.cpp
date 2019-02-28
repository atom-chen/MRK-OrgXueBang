//
//  BaseLayer.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/11/11.
//
//

#include "BaseLayer.hpp"
#include "CostTime.hpp"
#include "ResourceManager.hpp"
#include "SceneManager.hpp"

BaseLayer::BaseLayer()
{
    
}

BaseLayer::~BaseLayer()
{
    
}

bool BaseLayer::init()
{
    bool bRet = Layer::init();
    ::gettimeofday(&m_actionTime, NULL);
    return bRet;
}

void BaseLayer::onEnter()
{
    Layer::onEnter();
}

void BaseLayer::onExit()
{
    Layer::onExit();
}

void BaseLayer::cleanup()
{
    Layer::cleanup();
}
bool BaseLayer::load(const char* gameUIFile)
{
    COST_TIME_TAG(gameUIFile, "")
    Widget* gameUI = ResourceManager::getInstance()->getUIFromJson(gameUIFile);
    
    m_rootWidget = gameUI;
    
    if(gameUI->getParent())
    {
        return true;
    }
    
    this->addChild(gameUI,1);
    
    addEvents();
    COST_TIME_TAG(gameUIFile, "add event over !")
    return gameUI != NULL;
}

void BaseLayer::resizeUI(Layout* json_root)
{
    json_root->setClippingEnabled(false);
    CCAssert(json_root!=NULL, "can't find panel_root");
    //float scale = SIZE_SCALE_2D;
    Size oriSize = json_root->getContentSize();
    //    Size visibleSize = Director::getInstance()->getVisibleSize();
    //    Size visSize = Size(DESIGN_RESOLUTION_WIDTH, visibleSize.height / visibleSize.width * DESIGN_RESOLUTION_WIDTH);
    Size visSize = SceneManager::getInstance()->m_curSceneSize;
    //    Size designSize = Game::sharedGame()->getDesignSize();
    //    Size winSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    
    json_root->setContentSize(Size(oriSize.width,visSize.height));
    Helper::doLayout(json_root);
    
    //    Point position = json_root->getPosition();
    //    position.x = (winSize.width-SIZE_SCALE_2D*designSize.width)/2;
    //    json_root->setPosition(Vec2::ZERO);
    //    Widget* bg = (Widget*)json_root->getChildByName("Sprite_Bg");
    //    if(bg)
    //    {
    //        const Size &bgImageSize = bg->getContentSize();
    //        bg->setIgnoreAnchorPointForPosition(false);
    //        bg->setAnchorPoint(Vec2(0.5f, 0.5f));
    //        bg->setScaleX(winSize.width/bgImageSize.width);
    //        bg->setScaleY(winSize.height/bgImageSize.height);
    //        bg->setPosition(Vec2(winSize.width/2, winSize.height/2));
    //
    //        // 若背景是layout即panel，则强制设置其透明度为75%
    //        Layout* la = dynamic_cast<Layout*>(bg);
    //        if (la)
    //        {
    //            // 临时处理下,为了解决弹出框的问题
    //            la->setBackGroundColorOpacity(0);
    //        }
    ////        bg->setVisible(false);
    //    }
    
    Widget *leftLayout = (Widget*)json_root->getChildByName("panel_left");
    Widget *topLayout = (Widget*)json_root->getChildByName("panel_top");
    Widget *rightLayout = (Widget*)json_root->getChildByName("panel_right");
    Widget *bottomLayout = (Widget*)json_root->getChildByName("panel_bottom");
    Widget *centerLayout = (Widget*)json_root->getChildByName("panel_center");
    Widget *panel_popup = (Widget*)json_root->getChildByName("panel_popup");
    if(leftLayout){
        leftLayout->setAnchorPoint(Point(0,0.5));
        leftLayout->setPosition(json_root->convertToNodeSpace(Point(0,visSize.height/2)));
        //leftLayout->addChild(Utils::TestColor(leftLayout,Color4B(0,0,0,1)));
    }
    if(rightLayout){
        rightLayout->setAnchorPoint(Point(1,0.5));
        rightLayout->setPosition(json_root->convertToNodeSpace(Point(visSize.width,visSize.height/2)));
        //rightLayout->addChild(Utils::TestColor(rightLayout,Color4B(0,0,0,1)));
    }
    if(topLayout){
        topLayout->setAnchorPoint(Point(0.5,1));
        topLayout->setPosition(json_root->convertToNodeSpace(Point(visSize.width/2,visSize.height)));
        //topLayout->addChild(Utils::TestColor(topLayout,Color4B(230,176,35,255)));
    }
    if(bottomLayout){
        bottomLayout->setAnchorPoint(Point(0.5,0));
        bottomLayout->setPosition(json_root->convertToNodeSpace(Point(visSize.width/2,0)));
        //bottomLayout->addChild(Utils::TestColor(bottomLayout,Color4B(255,255,255,255)));
    }
    if(centerLayout){
        
        if (topLayout && bottomLayout)//上下对称
        {
            centerLayout->setAnchorPoint(Point(0,0));
            centerLayout->setPosition(json_root->convertToNodeSpace(Point(0,bottomLayout->getContentSize().height)));
            centerLayout->setContentSize(Size(visSize.width, visSize.height - topLayout->getContentSize().height - bottomLayout->getContentSize().height));
            
        }
        else if (topLayout && !bottomLayout)//有上无下
        {
            centerLayout->setAnchorPoint(Point(0,0));
            centerLayout->setPosition(json_root->convertToNodeSpace(Vec2::ZERO));
            centerLayout->setContentSize(Size(visSize.width, visSize.height - topLayout->getContentSize().height));
        }
        else if (!topLayout && bottomLayout)//有下无上
        {
            centerLayout->setAnchorPoint(Point(0,0));
            centerLayout->setPosition(json_root->convertToNodeSpace(Vec2(0,bottomLayout->getContentSize().height)));
            centerLayout->setContentSize(Size(visSize.width, visSize.height - bottomLayout->getContentSize().height));
        }
        else if (!topLayout && !bottomLayout)//上下全无
        {
            centerLayout->setAnchorPoint(Point(0,0));
            centerLayout->setPosition(json_root->convertToNodeSpace(Vec2::ZERO));
            centerLayout->setContentSize(Size(visSize.width, visSize.height));
        }
        Helper::doLayout(centerLayout);
    }
    if (panel_popup) {
        panel_popup->setContentSize(visSize);
    }
}
