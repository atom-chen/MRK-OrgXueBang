//
//  QuizBase.cpp
//  ReadingMate
//
//  Created by 丁晶 on 2017/1/11.
//
//

#include "QuizBase.h"
#include "SoundManager.hpp"
#include "QuizOkEffect.hpp"

int QuizBase::getScore()
{
    if (m_wholeBook) {
        return m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageScore->m_score;
    }
    return 0;
}

void QuizBase::setScore(int score)
{
    if (m_wholeBook) {
        m_wholeBook->m_bookPages->at(m_pageIndex)->m_pageScore->m_score = score;
    }
    m_bShowEff = false;
}

void QuizBase::playAnswerAudio(bool bRight)
{
    if (!m_wholeBook) {
        return;
    }
    string lvl = m_wholeBook->m_bookData->level;
    if (lvl != "AA" && lvl != "A" && lvl != "B" && lvl != "C") {
        return;
    }
    string strPathSnd;
    if (bRight) {
        strPathSnd = FileUtils::getInstance()->fullPathForFilename("music/quiz_right.mp3");
    } else {
        strPathSnd = FileUtils::getInstance()->fullPathForFilename("music/quiz_wrong.mp3");
    }
    
    if (strPathSnd.length() > 10) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        AudioEngine::stopAll();
#endif
        SoundManager::getInstance()->VolumeBackSound(1.0f);
        SoundManager::getInstance()->playSound(strPathSnd);
    }
}

void QuizBase::outPage()
{
    if (m_soundId != AudioEngine::INVALID_AUDIO_ID) {
        SoundManager::getInstance()->stopSound(m_soundId);
    }
}

bool QuizBase::showOkEff()
{
    if (m_bShowEff) {
        return false;
    }
    if (getScore() != 100) {
        return false;
    }
    m_bShowEff = true;
    int nType = 1;
    if (m_errorCount <= 0) {
        nType = 3;
    }
    else if (m_errorCount <= m_optionCount * 0.5) {
        nType = 2;
    }

    return QuizOkEffect::create(UiBase::getRunningScene(), nType);
}

int QuizBase::inPage()
{
//    m_errorCount = 0;
//    m_optionCount = 0;
    m_bShowEff = true;
    m_soundId = AudioEngine::INVALID_AUDIO_ID;
    return m_soundId;
}

void QuizBase::setParam(std::string imgFile, std::string voiceFile, std::string xmlFile)
{
    m_bXmlRead = true;
//    m_bShowEff = false;
}

void QuizBase::setWholeBook(WholeBook* wholeBook, int index)
{
    m_wholeBook = wholeBook;
    m_pageIndex = index;
}
