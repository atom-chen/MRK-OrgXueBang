//
//  QuizOkEffect.cpp
//  ReadingMate-mobile
//
//  Created by 丁晶 on 2018/3/20.
//

#include "QuizOkEffect.hpp"

QuizOkEffect *QuizOkEffect::create(Node* parent, int wordType)
{
    QuizOkEffect *ret = new (std::nothrow) QuizOkEffect();
    ret->m_type = wordType;
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

bool QuizOkEffect::init()
{
    m_ui = UiBase::getInstance()->loadCSB("ui/quiz/layer_quizOkEffect.csb");
    m_ui->addChild(this, -1);
    
    float timeScale = 0.7f;
    m_ui->runAction(Sequence::create(DelayTime::create(1 / timeScale), RemoveSelf::create(), NULL));
    
    auto skeleton_word = SkeletonAnimation::createWithJsonFile("spine/answer_word.json", "spine/answer_word.atlas", 1.8f);
    skeleton_word->setTimeScale(timeScale);
    switch(m_type){
        case 1:
            skeleton_word->setAnimation(0, "good", false);
            break;
        case 2:
            skeleton_word->setAnimation(0, "great", false);
            break;
        default:
            skeleton_word->setAnimation(0, "perfect", false);
            break;
    }
    m_ui->getChildByName("panel_center")->getChildByName("panel_book")->addChild(skeleton_word);
    
    auto skeleton_nut = SkeletonAnimation::createWithJsonFile("spine/answer_nut.json", "spine/answer_nut.atlas", 1.0f);
//    skeleton_nut->setAnimation(0, "nut", false);
    skeleton_nut->setVisible(false);
    m_ui->getChildByName("panel_center")->getChildByName("panel_nut")->addChild(skeleton_nut);
    skeleton_nut->runAction(Sequence::create(DelayTime::create(0.2 / timeScale), CallFunc::create([=](){
        skeleton_nut->setVisible(true);
        skeleton_nut->setTimeScale(timeScale);
        skeleton_nut->setAnimation(0, "nut", false);
    }), NULL));
    return true;
}


