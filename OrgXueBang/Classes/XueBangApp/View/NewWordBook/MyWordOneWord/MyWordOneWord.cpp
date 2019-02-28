//
//  MyWordOneWord.cpp
//  ReadingMate-mobile
//
//  Created by 高超 on 2018/3/27.
//

#include "MyWordOneWord.hpp"
#include "GameMacro.hpp"
#include "UserManager.hpp"
#include "UiBase.h"
#include "DownImg.h"
#include "TipManager.hpp"

#define WORD_LAYERCOLOR_NAME "ONE_WORD_LAYERCOLOR"

MyWordOneWord::MyWordOneWord():
m_word(""),
m_audioPath(""),
m_audioId(AudioEngine::INVALID_AUDIO_ID),
m_skeleton_loading(nullptr),
m_skeleton_word(nullptr)
{
}

MyWordOneWord *MyWordOneWord::create(cocos2d::Node* parent, string& word, string& audioPath)
{
    talkingInterface::pageBegin("生词解义");
    
    auto node = parent->getChildByName("MyWordOneWord");
    if (node) {
        return (MyWordOneWord *)node->getChildByName("MyWordOneWord");
    }
    
    MyWordOneWord *ret = new (std::nothrow) MyWordOneWord();
    if (ret && ret->init(word, audioPath)) {
        parent->addChild(ret->m_ui);
        ret->autorelease();
        return ret;
    } else {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

MyWordOneWord::~MyWordOneWord()
{
    if ( m_skeleton_word ) {
        m_skeleton_word->clearTracks();
        m_skeleton_word->removeFromParent();
        m_skeleton_word = nullptr;
    }
    if (m_skeleton_loading) {
        m_skeleton_loading->clearTracks();
        m_skeleton_loading->removeFromParent();
        m_skeleton_loading = nullptr;
    }
    talkingInterface::pageEnd("生词解义");
}

bool MyWordOneWord::OnBaseKeyBack()
{
    this->m_ui->removeFromParent();
    return true;
}

bool MyWordOneWord::init(string& word, string& audioPath)
{
    m_ui = CSLoader::createNodeWithVisibleSize("View/NewWordBook/layer_myWordOneWord.csb");
    UiBase::resizeBackImg(m_ui);
    this->setName("MyWordOneWord");
    m_ui->addChild(this, -1);
    m_ui->setName("MyWordOneWord");
    
    m_word = word;
    m_audioPath = audioPath;

    createWordSkeletonBg();

    initView();
    
    getDataFromSever(word);
    
    if (!m_audioPath.empty()) {
        DownImg::getInstance()->DownFile(audioPath.c_str(), nullptr);
    }
    
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* pTouch, Event* event) {
        Vec2 touchPos_ = pTouch->getLocation();
        Node *bg = m_ui->getChildByName(CENTER_PANEL_NAME);
        touchPos_ = bg->convertToNodeSpace(touchPos_);
        if (touchPos_.y <= 0 || touchPos_.y > bg->getContentSize().height ) {
            
            auto call = CallFunc::create([=](){
                this->m_ui->removeFromParent();
            });
            if ( m_skeleton_word ) {
                auto s1 = ScaleTo::create(0.24f, 0.01);
                auto f1 = FadeTo::create(0.24f, 0);
                auto c1 = CallFunc::create([=](){
                    if ( bg ) {
                        bg->runAction(Spawn::create(s1->clone(), f1->clone(), NULL));
                    }
                });
                m_skeleton_word->runAction(Sequence::create(Spawn::create(s1->clone(), f1->clone(), c1, NULL), call, NULL));
            }else {
                this->m_ui->runAction(Sequence::create(DelayTime::create(0.15f), call, NULL));
            }
        }
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, m_ui);
    
    return true;
}

void MyWordOneWord::onEnter()
{
    Node::onEnter();
}

void MyWordOneWord::initView()
{
    float delayTime = 0.2f;
    
    auto btn_sound = dynamic_cast<Button*>(m_ui->getChildByName(CENTER_PANEL_NAME)->getChildByName("btn_sound"));
    btn_sound->setOpacity(0);
    btn_sound->setScale(0.01);
    auto s1 = ScaleTo::create(delayTime, 1);
    auto f1 = FadeTo::create(delayTime, 255);
    auto call1 = CallFunc::create([=](){
        UiBase::addClickEventListener(btn_sound, [=](Ref* pSender){
            playWordAudio(m_audioPath);
        });
    });
    btn_sound->runAction(Sequence::create(Spawn::create(s1, f1, NULL), call1, NULL));

    auto panel_center = m_ui->getChildByName(CENTER_PANEL_NAME);
    auto txt_word = dynamic_cast<Text*>(panel_center->getChildByName("txt_word"));
    txt_word->setOpacity(0);
    
    if (!m_skeleton_loading) {
        m_skeleton_loading = SkeletonAnimation::createWithJsonFile("spine/loading_second.json", "spine/loading_second.atlas", 1.0f);
        m_skeleton_loading->setPosition(Vec2(panel_center->getContentSize().width*0.5, panel_center->getContentSize().height*0.5));
        panel_center->addChild(m_skeleton_loading);
    }
    auto call2 = CallFunc::create([=](){
        m_skeleton_loading->setAnimation(0, "animation", true);
    });
    m_skeleton_loading->runAction(Sequence::create(DelayTime::create(1.5f), call2, NULL));
}

void MyWordOneWord::createWordSkeletonBg()
{
    auto layerColor = m_ui->getChildByName(WORD_LAYERCOLOR_NAME);
    if ( !layerColor ) {
        layerColor = LayerColor::create(Color4B(0, 0, 0, 150), m_ui->getContentSize().width, m_ui->getContentSize().height);
        layerColor->setAnchorPoint(Vec2::ZERO);
        layerColor->setPosition(0, 0);
        layerColor->setName(WORD_LAYERCOLOR_NAME);
        m_ui->addChild(layerColor, -1);
        
        EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [this](Touch* pTouch, Event* event) {
            return true;
        };
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, layerColor);
    }
    
    if (!m_skeleton_word) {
        m_skeleton_word = SkeletonAnimation::createWithJsonFile("spine/second_menu.json", "spine/second_menu.atlas", 1.0f);
        m_skeleton_word->setPosition(Vec2(layerColor->getContentSize().width*0.5, layerColor->getContentSize().height*0.5));
        layerColor->addChild(m_skeleton_word);
    }
    m_skeleton_word->setAnimation(0, "stand", false);
}

void MyWordOneWord::playWordAudio(string path)
{
    string strPathSnd = DownImg::getInstance()->getLocalFilePath(path.c_str());
    
    if (FileUtils::getInstance()->isFileExist(strPathSnd.c_str())) {
        if (AudioEngine::INVALID_AUDIO_ID != m_audioId) {
            SoundManager::getInstance()->stopSound(m_audioId);
        }
        SoundManager::getInstance()->VolumeBackSound(1.0f);
        m_audioId = SoundManager::getInstance()->playSound(strPathSnd, false, [=](int, const std::string){
            m_audioId = AudioEngine::INVALID_AUDIO_ID;
        });
    }
    else {
        TipManager::getInstance()->showMsgTips("声音文件下载中...");
    }
}

void MyWordOneWord::updateWordInfo(string& word, string& enDesc, string& cnDesc)
{
    if (m_skeleton_loading) {
        m_skeleton_loading->clearTracks();
        m_skeleton_loading->removeFromParent();
        m_skeleton_loading = nullptr;
    }
    auto s1 = ScaleTo::create(0.2f, 1);
    auto f1 = FadeTo::create(0.2f, 255);
    
    auto txt_word = dynamic_cast<Text*>(m_ui->getChildByName(CENTER_PANEL_NAME)->getChildByName("txt_word"));
    txt_word->setOpacity(0);
    txt_word->setScale(0.01);
    txt_word->setString(word);
    txt_word->runAction(Spawn::create(s1->clone(), f1->clone(), NULL));

    auto scrollView_info = (cocos2d::ui::ScrollView*)m_ui->getChildByName(CENTER_PANEL_NAME)->getChildByName("scrollView_info");
    scrollView_info->setOpacity(0);
    scrollView_info->setScale(0.01);
#if 1
    auto scrollSize = scrollView_info->getContentSize();
    auto txt_word_desc = dynamic_cast<Text*>(scrollView_info->getChildByName("txt_word_desc"));
    string descStr = cnDesc + enDesc;
    int h = UiBase::setText(txt_word_desc, descStr);
    if (h < scrollSize.height) { //文字过短时，把显示位置上移
        txt_word_desc->setPositionY(scrollSize.height - h);
    }
    scrollSize.height = h;
    scrollView_info->setInnerContainerSize(Size(scrollSize.width, scrollSize.height));
    
    scrollView_info->runAction(Spawn::create(s1->clone(), f1->clone(), NULL));
#else
    auto scrollSize = scrollView_info->getContentSize();
    auto txt_word_desc = dynamic_cast<Text*>(scrollView_info->getChildByName("txt_word_desc"));
    txt_word_desc->setVisible(false);
    
    Label* txt_node = nullptr;
    TTFConfig ttfConfig1(txt_word_desc->getFontName(), txt_word_desc->getFontSize(), GlyphCollection::DYNAMIC);
    txt_node = Label::createWithTTF(ttfConfig1, enDesc + cnDesc, TextHAlignment::LEFT, scrollSize.width);
    txt_node->setTextColor(txt_word_desc->getTextColor());
    txt_node->setPosition(Vec2(scrollSize.width * 0.5, scrollSize.height * 0.5f));
    
    Node* container = Node::create();
    container->setContentSize(Size(scrollSize.width, MAX(txt_node->getContentSize().height, scrollSize.height)));
    
    float fHeight = container->getContentSize().height;
    txt_node->setPosition(Vec2(scrollSize.width * 0.5f, fHeight - txt_node->getContentSize().height * 0.5f));
    container->addChild(txt_node);
    scrollView_info->addChild(container);
    scrollView_info->setInnerContainerSize(container->getContentSize());
#endif
}

void MyWordOneWord::getDataFromSever(const std::string& word)
{
    if (word.length() < 1) {
        return;
    }
    
    HttpDataToString dataStr;
    dataStr.addData("token", UserManager::getInstance()->getUserModel()->getUserToken());
    dataStr.addData("word", word, true);
    
    CDataHttp::getInstance()->HttpGet(eHttpType::paraphrase, dataStr.getString());
}

void MyWordOneWord::endWithHttpData(eHttpType nOp, bool bSuccess, const stuJson& json)
{
    if (nOp == eHttpType::paraphrase) {
        eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
        if(eHttpStateBase::eHttpStateBase_Rok == e) {
            std::string return_code = json.getValue("return_code");
            
            stuJson returnData = json.getJson("return_data");
            stuJson wordData = returnData.getJson("word");
            stuJson url = returnData.getJson("url");

            string word = wordData.getValue("word");
            string enExplain = wordData.getValue("description_en");
            string cnExplain = wordData.getValue("description_cn");

            updateWordInfo(word, enExplain, cnExplain);
        }else if(e == eHttpStateBase::eHttpStateBase_Rerr)
        {
//            img_notfind->setVisible(true);
        }
    }
}
