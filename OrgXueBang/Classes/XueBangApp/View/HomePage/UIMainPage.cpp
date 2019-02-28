//
//  UIMainPage.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/15.
//

#include "UIMainPage.hpp"
#include "UIMainTopBar.hpp"
#include "UIMainBookCell.hpp"
#include "HomePageManager.hpp"
#include "TipManager.hpp"

void UIMainPage::onEnter()
{
    Layer::onEnter();
}
void UIMainPage::onExit()
{
    Layer::onExit();
}

bool UIMainPage::init()
{
    if(!Layer::init()){
        return false;
    }
    initUI();
    
    gettimeofday(&_lastUpdate, nullptr);
    _lastUpdate.tv_sec -= 10;
    
    return true;
}

UIMainPage::UIMainPage():isGoToCurCell(false)
,isReqLoading(false)
{
    MessageManager::getInstance()->addObserver(this, CC_CALLFUNCO_SELECTOR(UIMainPage::onRevMsgDoHandle),(int)eHttpType::homePageBookList);
}
UIMainPage::~UIMainPage()
{
    MessageManager::getInstance()->removeObserver(this);
    HomePageManager::getInstance()->getHomePageModel()->clearData();
}
void UIMainPage::initUI()
{
//    auto mUI = CSLoader::createNode("View/HomePage/MainPage.csb");
    auto mUI = CSLoader::createNodeWithVisibleSize("View/HomePage/MainPage.csb");
    
    this->addChild(mUI);
    
    vector<string> vecBtnName = {"btnTest"};
    vector<string> vecNodeName = {
        "PanelBook",
    };
    
    for (auto name : vecBtnName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        btn->addClickEventListener(CC_CALLBACK_1(UIMainPage::btnClickHandle, this));
        mapWidget[name] = btn;
    }
    for (auto name : vecNodeName){
        auto btn = Helper::seekWidgetByName(static_cast<Widget*>(mUI), name);
        mapWidget[name] = btn;
    }
    
//    mapWidget["imgBG"] = (Widget*)mUI->getChildByName("FileNodeBG")->getChildByName("imgBG");
//    auto screenSize = SceneManager::getInstance()->m_fullSceneSize;
//    mapWidget["imgBG"]->setContentSize(screenSize);
//    Helper::doLayout(mapWidget["imgBG"]);
    
    mapWidget["btnTest"]->setVisible(false);
    
    Node* bar = mUI->getChildByName("FileNodeToBar");
    mapWidget["FileNodeToBar"] = (Widget*)bar;
    UIMainTopBar* mUIMainTopBar = UIMainTopBar::create(bar);
    this->addChild(mUIMainTopBar);

    initTableView();
}

void UIMainPage::btnClickHandle(Ref* pSender)
{
    Node* sender = (Node*)pSender;
    const std::string& name = sender->getName();
    printf("btnClickHandle::%s\n",name.c_str());
    if(name == "btnTest")
    {
        //        int index = std::rand()%(20-4) + 4;
        //        goToCell(index);
        //        HomePageManager::getInstance()->getHomePageModel()->setBookReaded("4");
        //        reflashData();
    }
}

void UIMainPage::initTableView()
{
    //创建tableview
    auto size = mapWidget["PanelBook"]->getContentSize();
    auto tableview = mTableView = TableView::create(this, size);
    tableview->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
    tableview->setDelegate(this);
    tableview->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    mapWidget["PanelBook"]->addChild(tableview);
    mItemSize = Size(size.width,size.height/3.5);//每个tableCell的大小
    
    //请求数据
    if(HomePageManager::getInstance()->getHomePageModel()->getAllWeekBookListCount() == 0){
        isGoToCurCell = true;
        isReqLoading = true;
        HomePageManager::getInstance()->reqHomePageBookList(true);
    }
    else{
        reflashData();
    }
}

Size UIMainPage::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return mItemSize;
}
TableViewCell* UIMainPage::tableCellAtIndex(TableView *table, ssize_t idx)
{
    CCLOG("UIMainPage-----tableCellAtIndex index: %d", (int)idx);
    TableViewCell* cell = table->dequeueCell();
    if(!cell){
        cell = UIMainBookCell::create();
        dynamic_cast<UIMainBookCell*>(cell)->initUI();
    }
    auto data = HomePageManager::getInstance()->getHomePageModel()->getOneWeekBookListByIndex((int)idx);
    bool flag = HomePageManager::getInstance()->getHomePageModel()->isCurWeekID(data->_WeekID);
    dynamic_cast<UIMainBookCell*>(cell)->updateCell(data,flag);
    
    return cell;
}
ssize_t UIMainPage::numberOfCellsInTableView(TableView *table)
{
    int num = HomePageManager::getInstance()->getHomePageModel()->getAllWeekBookListCount();
    return num;
}

void UIMainPage::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
    CCLOG("UIMainPage-----scrollViewDidScroll#########   %f  *****ContentSize***** %f  *****ViewSize***** %f"
          ,view->getContentOffset().y
          ,view->getContentSize().height
          ,view->getViewSize().height);
    if(isReqLoading || isGoToCurCell){
        CCLOG("UIMainPage-----scrollViewDidScroll--isReqLoading");
        return;
    }
    if(view->getContentOffset().y > 80){
        //请求数据
        CCLOG("UIMainPage-----scrollViewDidScroll--up");

        bool acc = HomePageManager::getInstance()->reqHomePageBookList(true);
        if(acc == false){
            TipManager::getInstance()->showTipWithString("之后数据已全部加载");
        }else{
            isReqLoading = true;
        }
    }else{
        float dis = view->getContentSize().height - view->getViewSize().height;
        if(std::abs(view->getContentOffset().y) - std::abs(dis) > 80){
            CCLOG("UIMainPage-----scrollViewDidScroll--down");
            
            bool acc = HomePageManager::getInstance()->reqHomePageBookList(false);
            if(acc == false){
                TipManager::getInstance()->showTipWithString("之前数据已全部加载");
            }else{
                isReqLoading = true;
            }
        }
    }
}
void UIMainPage::scrollViewDidZoom(cocos2d::extension::ScrollView* view)
{
    
}
void UIMainPage::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("UIMainPage-----cell touched at index: %ld", cell->getIdx());
}

//接收到数据
void UIMainPage::onRevMsgDoHandle(Ref* ref)
{
    MsgParam* param = (MsgParam*)ref;
    if(param->getIsResOK()){
        CCLOG("UIMainPage::onRevMsgDoHandle::===-OK-====");
        if(isGoToCurCell){
            int index = HomePageManager::getInstance()->getHomePageModel()->getIndexByWeekID();
            CCLOG("UIMainPage::onRevMsgDoHandle::===--====%d",index);
            if(index > 2){//没找到-1，第0，1 不需要定位
                mTableView->reloadData();
                int num = HomePageManager::getInstance()->getHomePageModel()->getAllWeekBookListCount();
                if(num - index == 1){//当要定位到最后一个时，位置会偏空，则给定位到倒数第2个
                    index = index - 1;
                }
                goToCell(index);
            }else{
                reflashData();
            }
            isGoToCurCell = false;
        }else{
            reflashData();
        }
    }else{
    }
    isReqLoading = false;
}

void UIMainPage::reflashData()
{
    Vec2 oldOff = mTableView->getContentOffset();
    Size oldSize = mTableView->getContentSize();
    mTableView->reloadData();
    Size newSize = mTableView->getContentSize();
    oldOff.y -= newSize.height - oldSize.height;
    mTableView->setContentOffset(oldOff);
}

//跳转到指定的Cell
void UIMainPage::goToCell(int index)
{
    CCLOG("UIMainPage-----goToCell:ss--222--- %d", index);
    Vec2 offset = mTableView->getContentOffset();
    int sum = (int)this->numberOfCellsInTableView(mTableView);
    int idx = sum - index - 1;
    offset.y = -((idx - 1) * mItemSize.height);
    mTableView->setContentOffset(offset,false);
}
bool UIMainPage::OnBaseKeyBack()
{
    struct timeval now;
    gettimeofday(&now, nullptr);
    
    if(now.tv_sec - _lastUpdate.tv_sec > 0.5)
        TipManager::getInstance()->showMsgTips("连续两次返回退出应用！");
    else
        Director::getInstance()->end();
    gettimeofday(&_lastUpdate, nullptr);
    return true;
}
