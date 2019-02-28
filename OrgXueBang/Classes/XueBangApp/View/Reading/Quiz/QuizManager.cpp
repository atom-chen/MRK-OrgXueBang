//
//  QuizManager.cpp
//  ReadingMate
//
//  Created by 丁晶 on 2017/1/5.
//
//

#include "QuizManager.h"
#include "PageQuizOral.hpp"
#include "PageQuizTemp.hpp"

using namespace std;

static QuizManager s_QuizManager;

QuizManager* QuizManager::getInstance()
{
    return &s_QuizManager;
}

eQuizType QuizManager::getQuizType(OnePage *page)
{
    if (page == nullptr) {
        return eQuizNone;
    }
    if (!page->m_isQuiz) {
        return eQuizNone;
    }
    //口语题
    if (page->m_pageMode == "SRS") {
        return eQuizOral;
    }
    
    //OMR 拖拽题, LRA ／ LAA 选择题
    if (page->m_pageMode != "OMR" && page->m_pageMode != "LRA" && page->m_pageMode != "LAA") {
        CCLOG("unknow quiz type , page id:%d pagemode = %s", page->m_pageID, page->m_pageMode.c_str());
        return eQuizNone;
    }
    
    string passage_type, target_type, match_type, task_type, layout;
    
    passage_type = page->m_paramJson.getValue("passage_type");
    target_type = page->m_paramJson.getValue("target_type");
    match_type = page->m_paramJson.getValue("match_type");
    task_type = page->m_paramJson.getValue("task_type");
    layout = page->m_paramJson.getValue("layout");

    //no 8
    if (page->m_pageMode == "OMR" && passage_type == "Image" && target_type == "Sentence" && match_type == "Text" && task_type == "None" && layout == "words") {
        return eQuizWordSelect;
    }
    //no 8
    if (page->m_pageMode == "OMR" && passage_type == "ImageAudio" && target_type == "Sentence" && match_type == "Text" && task_type == "None" && layout == "words") {
        return eQuizWordSelect;
    }
    
    //no 1
    if (page->m_pageMode == "OMR" && passage_type == "ImageAudio" && target_type == "Sentence" && match_type == "Text" && task_type == "None") {
        return eQuizWordDrag;
    }
    //no 4
    if (page->m_pageMode == "OMR" && passage_type == "Image" && target_type == "Sentence" && match_type == "Text" && task_type == "Audio") {
        return eQuizWordDrag;
    }
    //no 36
    if (page->m_pageMode == "OMR" && passage_type == "None" && target_type == "Sentence" && match_type == "Text" && task_type == "ImageAudio") {
        return eQuizWordDrag;
    }
    //no 5
    if (page->m_pageMode == "OMR" && passage_type == "Image" && target_type == "Sentence" && match_type == "Text" && task_type == "None") {
        return eQuizWordDrag;
    }
    //???
    if (page->m_pageMode == "OMR" && passage_type == "None" && target_type == "Sentence" && match_type == "Text" && task_type == "Audio") {
        return eQuizWordDrag;
    }
    //???无图
    if (page->m_pageMode == "OMR" && passage_type == "Audio" && target_type == "Sentence" && match_type == "Text" && task_type == "None") {
        return eQuizWordDrag;
    }
    
    //no 22
    if (page->m_pageMode == "OMR" && passage_type == "None" && target_type == "LineUp" && match_type == "TextAudio" && task_type == "Audio") {
        return eQuizSentence;
    }

    //no 23
    if (page->m_pageMode == "OMR" && passage_type == "None" && target_type == "Text" && match_type == "Image" && task_type == "None" && layout == "order") {
        return eQuizPicWordDrag;
    }
    //no 24
    if (page->m_pageMode == "OMR" && passage_type == "None" && target_type == "TextAudio" && match_type == "Image" && task_type == "None") {
        return eQuizPicWordDrag;
    }
    //no 38
    if (page->m_pageMode == "OMR" && passage_type == "None" && target_type == "Text" && match_type == "Image" && task_type == "None") {
        return eQuizPicWordDrag;
    }
    //???
    if (page->m_pageMode == "OMR" && passage_type == "None" && target_type == "Text" && match_type == "Image" && task_type == "Audio") {
        return eQuizPicWordDrag;
    }
    
    string question_type, answer_type;
    question_type = page->m_paramJson.getValue("question_type");
    answer_type = page->m_paramJson.getValue("answer_type");
    
    //no 7
    if (page->m_pageMode == "LAA" && passage_type == "Image" && question_type == "Text" && answer_type == "Text" && layout == "blank_picture") {
        return eQuizFillIn;
    }
    //no 41
    if (page->m_pageMode == "LRA" && passage_type == "Image" && question_type == "Text" && answer_type == "Text" && layout == "blank_picture") {
        return eQuizFillIn;
    }
    
    //no 21
    if (page->m_pageMode == "LAA" && passage_type == "Audio" && question_type == "Text" && answer_type == "Text" && layout == "blank") {
        return eQuizListenFill;
    }
    //no 43
    if (page->m_pageMode == "LRA" && passage_type == "Audio" && question_type == "Text" && answer_type == "Text" && layout == "blank") {
        return eQuizListenFill;
    }
    
    //no 9
    if (page->m_pageMode == "LAA" && passage_type == "None" && question_type == "TextAudio" && answer_type == "Text" ) {
        return eQuizListenSelect;
    }
    //no 42
    if (page->m_pageMode == "LRA" && passage_type == "None" && question_type == "TextAudio" && answer_type == "Text" ) {
        return eQuizListenSelect;
    }
    
    //no 10
    if (page->m_pageMode == "LAA" && passage_type == "Image" && question_type == "Text" && answer_type == "Text") {
        return eQuizPicText;
    }
    //no 46
    if (page->m_pageMode == "LRA" && passage_type == "Image" && question_type == "Text" && answer_type == "Text") {
        return eQuizPicText;
    }
    
    //no 11
    if (page->m_pageMode == "LAA" && passage_type == "Image" && question_type == "TextAudio" && answer_type == "Text") {
        return eQuizListen;
    }
    //no 47
    if (page->m_pageMode == "LRA" && passage_type == "Image" && question_type == "TextAudio" && answer_type == "Text") {
        return eQuizListen;
    }
    
    //no 25
    if (page->m_pageMode == "LAA" && passage_type == "None" && question_type == "Text" && answer_type == "Text" && layout == "long_question") {
        return eQuizTextSort;
    }
    //no 45
    if (page->m_pageMode == "LRA" && passage_type == "None" && question_type == "Text" && answer_type == "Text" && layout == "long_question") {
        return eQuizTextSort;
    }

    //参数少的放后面
    //no 25
    if (page->m_pageMode == "LAA" && passage_type == "None" && question_type == "Text" && answer_type == "Text" ) {
        return eQuizTextSelect;
    }
    //no 44
    if (page->m_pageMode == "LRA" && passage_type == "None" && question_type == "Text" && answer_type == "Text" ) {
        return eQuizTextSelect;
    }
    
    
    //no 6
    if (page->m_pageMode == "LAA" && passage_type == "Audio" && question_type == "None" && answer_type == "Image") {
        return eQuizListenSelectPic;
    }
    //no 40
    if (page->m_pageMode == "LRA" && passage_type == "Audio" && question_type == "None" && answer_type == "Image") {
        return eQuizListenSelectPic;
    }
    
    //???
    if (page->m_pageMode == "LAA" && passage_type == "None" && question_type == "Audio" && answer_type == "Image") {
        return eQuizListenSelectPic;
    }
    //???
    if (page->m_pageMode == "LRA" && passage_type == "None" && question_type == "Audio" && answer_type == "Image") {
        return eQuizListenSelectPic;
    }

    //???
    if (page->m_pageMode == "LAA" && passage_type == "Audio" && question_type == "Text" && answer_type == "Text") {
        return eQuizListenFill;
    }
    //???
    if (page->m_pageMode == "LRA" && passage_type == "Audio" && question_type == "Text" && answer_type == "Text") {
        return eQuizListenFill;
    }
    
    CCLOG("unknow quiz type , page id:%d pagemode = %s passagetype = %s", page->m_pageID, page->m_pageMode.c_str(), passage_type.c_str());
    
    return eQuizNone;
}

//PageQuizBase* QuizManager::createQuiz(eQuizType type, Widget* pageUI)
//{
//    PageQuizBase* quiz = nullptr;
//    switch (type) {
//        case eQuizOral:
//            quiz = PageQuizOral::create(pageUI);
//            break;
//        case eQuizSentence:             //单词组句
//        case eQuizFillIn:               //看图填空
//        case eQuizListen:               //看图文本听音选择
//        case eQuizPicText:             //看图文本选择
//        case eQuizListenFill:           //听音填空
//        case eQuizListenSelectPic:       //听音选图
//        case eQuizListenSelect:          //听音选择
//        case eQuizPicWordDrag:           //图片单词对应
//        case eQuizTextSort:            //文本排序
//        case eQuizTextSelect:            //文本选择
//        case eQuizWordDrag:             //字母拖拽
//            quiz = PageQuizTemp::create(pageUI);
//            break;
//        default:
//            CCAssert(false, "error, no this type!");
//            break;
//    }
//    return quiz;
//    
//}




