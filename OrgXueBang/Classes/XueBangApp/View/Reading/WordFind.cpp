//
//  WordFind.cpp
//  ReadingMate
//
//  Created by 丁晶 on 2016/12/8.
//
//
#include "GameMacro.hpp"

#include "WordFind.h"
#include "Reador.hpp"
#include "UiBase.h"
#include "DownImg.h"
#include "TipManager.hpp"
#include "NewHandTip.hpp"
//#include "MyWordBook.h"
#include "json.h"
#include "MyWord.hpp"

WordFind *WordFind::create(Node* parent, int offy, string& word)
{
    WordFind *ret = new (std::nothrow) WordFind();
    ret->m_parent = parent;
    ret->m_offy = offy;
    ret->m_word = word;
    if (ret && ret->init())
    {
        parent->addChild(ret->m_ui, 100);
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}

bool WordFind::init()
{
    m_ui = CSLoader::createNode("View/Reading/layer_wordFind.csb");
    m_ui->addChild(this, -1);
    m_ui->setTag(eLayerBookDiction);
    
    Size parentSize = m_parent->getContentSize();
    parentSize.height -= 100;   //让开顶部栏
    m_ui->setContentSize(parentSize);
    Helper::doLayout(m_ui);
    m_ui->setPosition(Vec2(0, 0));
//    m_ui->setPosition(Vec2(0, -parentSize.height));
//    auto action = MoveTo::create(0.5, Vec2(0, 0));
    
    
    Size topSize = m_ui->getChildByName("panel_top")->getContentSize();
    parentSize.height = parentSize.height - topSize.height;
    cocos2d::ui::ScrollView* scroll_word = dynamic_cast<cocos2d::ui::ScrollView*>(m_ui->getChildByName("scroll_word"));
    scroll_word->setContentSize(parentSize);
    scroll_word->setScrollBarEnabled(false);
    
    auto children = scroll_word->getChildren();
    for (auto a : children) {
        a->setVisible(false);
    }
    
//    TextField* tf_input = (TextField*)m_ui->getChildByName("panel_top")->getChildByName("tf_input");
//    tf_input->setTextColor(Color4B(0, 0, 0, 255));

    auto btn_close = (Widget*)m_ui->getChildByName("panel_top")->getChildByName("panel_close");
    UiBase::addClickEventListener(btn_close, [=](Ref* pSender){
//        this->find(tf_input->getString());
        m_ui->removeFromParent();
    });

    ImageView* img_play = (ImageView*)(scroll_word->getChildByName("panel_sound")->getChildByName("img_play"));
    UiBase::addAudioEventListener(img_play, [=](Ref* pSender){
        this->playSound();
    });
    
    ImageView* img_newBook = (ImageView*)(scroll_word->getChildByName("panel_wordBook")->getChildByName("img_newBook"));
    UiBase::addClickEventListener(img_newBook, [=](Ref* pSender){
//        auto book = MyWordBook::create(UiBase::getInstance()->getRunningScene());
//        book->showTab(1);
        MyWord::create(UiBase::getInstance()->getRunningScene());
        m_ui->removeFromParent();
    });
    
    ImageView* btn_addWordBook = (ImageView*)(scroll_word->getChildByName("panel_wordBook")->getChildByName("img_addNewWord"));
    UiBase::addClickEventListener(btn_addWordBook, [=](Ref* pSender){
        this->addToBook();
        btn_addWordBook->setVisible(false);
//        img_newBook->setVisible(true);
        this->showAddEff();
    });


    ImageView* btn_showCn = (ImageView*)(scroll_word->getChildByName("panel_showBtn")->getChildByName("img_showCn"));
    UiBase::addClickEventListener(btn_showCn, [=](Ref* pSender){
        if (scroll_word->getChildByName("panel_chinese")->isVisible()) {
            scroll_word->getChildByName("panel_chinese")->setVisible(false);
//            btn_showCn->setTitleText("显示中文");
        } else {
            scroll_word->getChildByName("panel_chinese")->setVisible(true);
//            btn_showCn->setTitleText("隐藏中文");
        }
    });
    btn_showCn->setVisible(false);
//    m_ui->runAction(action);
    
    find(m_word);
    
    m_sp = nullptr;
    return true;
}


void WordFind::find(const std::string& word)
{
    if (word.length() < 1) {
        return;
    }
    
    HttpDataToString dataStr;
//    dataStr.addData("token", Reador::getInstance()->getUserToken());
    dataStr.addData("word", word, true);
    
    CDataHttp::getInstance()->HttpGet(eHttpType::paraphrase, dataStr.getString());
    
//    m_Find.word = word;
//    m_Find.imgUrl = "haha";
//    m_Find.enExplain = "adj.\n1. a unit of language that native speakers can identify\n2. information about recent and important events\n3. a secret word or phrase known only to a restricted group";
//    m_Find.cnExplain = "n.\n单词;话语;诺言;消息\nvt.\n措辞，用词;用言语表达\nvi.\n讲话";
//    
//    setResult(&m_Find);
}

void WordFind::setResult(stuWordFind *result)
{
    int y = 300, h;
    cocos2d::ui::ScrollView* scroll_word = dynamic_cast<cocos2d::ui::ScrollView*>(m_ui->getChildByName("scroll_word"));
    Size scrollSize = scroll_word->getContentSize();
    
    Layout* panel_chinese = dynamic_cast<Layout*>(scroll_word->getChildByName("panel_chinese"));
    panel_chinese->setVisible(true);
    Text* text_cnExplain = dynamic_cast<Text*>(panel_chinese->getChildByName("text_cnExplain"));
    
    h = UiBase::setText(text_cnExplain, result->enExplain);
    
    Size size5 = panel_chinese->getContentSize();
    size5.height = h;
    panel_chinese->setContentSize(size5);
    y += size5.height;
    panel_chinese->setPosition(Vec2(0, y));

    Layout* panel_showBtn = dynamic_cast<Layout*>(scroll_word->getChildByName("panel_showBtn"));
    panel_showBtn->setVisible(true);
    Size size4 = panel_showBtn->getContentSize();
    y += size4.height;
    panel_showBtn->setPosition(Vec2(0, y));
    
    Layout* panel_english = dynamic_cast<Layout*>(scroll_word->getChildByName("panel_english"));
    panel_english->setVisible(true);
    Text* text_enExplain = dynamic_cast<Text*>(panel_english->getChildByName("text_enExplain"));
    
    h = UiBase::setText(text_enExplain, result->cnExplain);
    
    Size size3 = panel_english->getContentSize();
    size3.height = h;
    panel_english->setContentSize(size3);
    y += size3.height;
    panel_english->setPosition(Vec2(0, y));
    
    Layout* panel_sound = dynamic_cast<Layout*>(scroll_word->getChildByName("panel_sound"));
    if (result->sound.length() > 4) {
        DownImg::getInstance()->DownFile(result->sound.c_str(), nullptr);
        panel_sound->setVisible(true);
    } else {
        if (m_oggFile.length() <= 4) {
            panel_sound->setVisible(false);
        } else {
            panel_sound->setVisible(true);
        }
    }
    if (panel_sound->isVisible()) {
        Size size2 = panel_sound->getContentSize();
        y += size2.height;
        panel_sound->setPosition(Vec2(0, y));
    }
    
    Layout* panel_img = dynamic_cast<Layout*>(scroll_word->getChildByName("panel_img"));
    if (result->imgUrl.length() > 4) {
        panel_img->setVisible(true);
        Size size2 = panel_img->getContentSize();
        y += size2.height;
        panel_img->setPosition(Vec2(0, y));
        
        ImageView* img_word = dynamic_cast<ImageView*>(panel_img->getChildByName("img_word"));
        img_word->setVisible(false);
        img_word->setUserData((void *)1);
        DownImg::getInstance()->DownFile(result->imgUrl.c_str(), img_word);
    } else {
        panel_img->setVisible(false);
    }
    
    Layout* panel_wordBook = dynamic_cast<Layout*>(scroll_word->getChildByName("panel_wordBook"));
    Size size1 = panel_wordBook->getContentSize();
    y += size1.height;
    panel_wordBook->setPosition(Vec2(0, y));
    
    panel_wordBook->setVisible(true);
    ((Text *)(panel_wordBook->getChildByName("text_word")))->setString(result->word);
    
    
    scroll_word->setInnerContainerSize(Size(scrollSize.width, y));
    
    ImageView* btn_addWordBook = (ImageView*)(scroll_word->getChildByName("panel_wordBook")->getChildByName("img_addNewWord"));
    btn_addWordBook->setVisible(!result->bNewWord);
    ImageView* img_newBook = (ImageView*)(scroll_word->getChildByName("panel_wordBook")->getChildByName("img_newBook"));
    img_newBook->setVisible(result->bNewWord);
    
    if ((y - 300) > scrollSize.height) {
        scroll_word->setScrollBarEnabled(true);
        scroll_word->setScrollBarAutoHideEnabled(false);
    }

    if (!result->bNewWord) {
        if (NewHandTip::isShow("WordFind")) {
            return;
        }
        NewHandTip::setShow("WordFind");
        
        auto node = NewHandTip::create2(UiBase::getInstance()->getRunningScene());
//        node->addImg("newhand_wordDetail.png", btn_addWordBook, Vec2(-170, 110));
        node->addImgAndHole("newhand_wordDetail.png", btn_addWordBook, Vec2(-170, 110), 0, Vec2(0, 0), Size(300, 150));
    }
    
}

void WordFind::addToBook()
{
    CCLOG("add %s to book", m_Find.word.c_str());
    
//    HttpDataToString dataStr;
//    dataStr.addData("token", Reador::getInstance()->getUserToken());
//    dataStr.addData("word", m_Find.word);
//
//    CDataHttp::getInstance()->HttpPost(eHttpType::addvocabulary, dataStr.getString());
    unordered_map<string, string>heads;

    Json::Value root;
    root["word"] = m_Find.word;
    string str = root.toStyledString();
    CDataHttp::getInstance()->HttpPostNew(eHttpType::addvocabulary, heads, str);
//    unordered_map<string, string>values;
//    values["platform"] = "0";
//    CDataHttp::getInstance()->HttpGetNew(eHttpType::getGemStoreProducts, heads, values);
}

void WordFind::playSound()
{
    CCLOG("play sound");
    if (m_Find.sound.length() > 4) {
        //音效路径
        string strPathSnd = DownImg::getInstance()->getLocalFilePath(m_Find.sound.c_str());
        
        if (FileUtils::getInstance()->isFileExist(strPathSnd.c_str())) {
            SoundManager::getInstance()->VolumeBackSound(1.0f);
            SoundManager::getInstance()->playSound(strPathSnd);
        } else {
            TipManager::getInstance()->showTipWithString("文件下载中，请稍后再试。");
        }
    } else if (m_oggFile.length() > 4){
        if (FileUtils::getInstance()->isFileExist(m_oggFile.c_str())) {
            SoundManager::getInstance()->playSound(m_oggFile, false, nullptr, m_vStart, m_vTotal);
        }
    }
}

void WordFind::endWithHttpData(eHttpType nOp,bool bSuccess,const stuJson& json)
{
    ImageView* img_notfind = (ImageView*)m_ui->getChildByName("panel_top")->getChildByName("img_back")->getChildByName("img_notfind");
    if (nOp == eHttpType::paraphrase)
    {
        eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
        if(eHttpStateBase::eHttpStateBase_Rok == e)
        {
            std::string return_code = json.getValue("return_code");
            
            stuJson returnData = json.getJson("return_data");
            stuJson word = returnData.getJson("word");
            stuJson url = returnData.getJson("url");
            m_Find.word = word.getValue("word");
            m_Find.enExplain = word.getValue("description_en");
            m_Find.cnExplain = word.getValue("description_cn");
            m_Find.imgUrl = word.getValue("image");
            m_Find.sound = word.getValue("audio");//单词使用ogg没有mp3资源 add by hyp
            if (m_Find.imgUrl.length() > 4) {
                m_Find.imgUrl = url.getValue("word_img_url") + m_Find.imgUrl;
            }
            if (m_Find.sound.length() > 4) {
                m_Find.sound = url.getValue("word_audio_url") + m_Find.sound;
            }

            
            string newWord = word.getValue("new_word");
            if (newWord == "true") {
                m_Find.bNewWord = true;
            } else {
                m_Find.bNewWord = false;
            }
            
            setResult(&m_Find);
            
            img_notfind->setVisible(false);
        }
        else if(e == eHttpStateBase::eHttpStateBase_Rerr)
        {
            img_notfind->setVisible(true);
        }
        else
        {
            
        }
    }
    else if (nOp == eHttpType::addvocabulary)
    {
        eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
        if(eHttpStateBase::eHttpStateBase_Rok == e)
        {
            std::string return_code = json.getValue("return_code");
            
            stuJson returnData = json.getJson("return_data");
            
//            string add_flag = returnData.getValue("add_flag");
//            if (add_flag == "true") {
                CCLOG("增加生词成功");
                TipManager::getInstance()->showTipWithString("添加生词成功！");
//            } else {
//                CCLOG("增加生词失败");
//            }
            
        }
        else if(e == eHttpStateBase::eHttpStateBase_Rerr)
        {
            
        }
        else
        {
            
        }
    }
}

void WordFind::setVoice(const string &ogg, int start, int total)
{
    m_oggFile = ogg;
    m_vStart = start;
    m_vTotal = total;
    
    playSound();
}


bool WordFind::OnBaseKeyBack()
{
    this->m_ui->removeFromParent();
    return true;
}

void WordFind::showAddEff()
{
    cocos2d::ui::ScrollView* scroll_word = dynamic_cast<cocos2d::ui::ScrollView*>(m_ui->getChildByName("scroll_word"));
    auto text_word = (Text *)(scroll_word->getChildByName("panel_wordBook")->getChildByName("text_word"));
    ImageView* img_newBook = (ImageView*)(scroll_word->getChildByName("panel_wordBook")->getChildByName("img_newBook"));
    
    m_sp = SkeletonAnimation::createWithJsonFile("spine/dictionary.json", "spine/dictionary.atlas", 1.0f);
    auto pos1 = img_newBook->getParent()->convertToWorldSpace(img_newBook->getPosition());
    auto parent = UiBase::getInstance()->getRunningScene();
    auto pos2 = parent->convertToNodeSpace(pos1);
    m_sp->setPosition(pos2);
    parent->addChild(m_sp, 200);
    m_sp->setVisible(false);
    
    auto oldPos = text_word->getPosition();
//    text_word->setAnchorPoint(Vec2(0.5, 0.5));
    text_word->runAction(Sequence::create(DelayTime::create(0.2), MoveBy::create(0.3, Vec2(550, 0)), CallFunc::create([=](){
        text_word->setVisible(false);
        
        m_sp->setVisible(true);
        m_sp->addAnimation(0, "animation", false);
        
    }), DelayTime::create(1), CallFunc::create([=](){
        text_word->setPosition(oldPos);
        text_word->setScale(1);
        text_word->setVisible(true);
        img_newBook->setVisible(true);
        m_sp->removeFromParent();
        m_sp = nullptr;
    }), NULL));
    
    text_word->runAction(Sequence::create(ScaleTo::create(0.2, 1.5), ScaleTo::create(0.3, 0.1), NULL));
}

WordFind::~WordFind()
{
    if (m_sp) {
        m_sp->removeFromParent();
    }
}
