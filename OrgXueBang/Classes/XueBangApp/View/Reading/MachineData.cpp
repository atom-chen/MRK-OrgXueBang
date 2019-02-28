//
//  MachinePageStdafx.h
//  ReadingMate
//
//  Created by hyp on 18/11/15.
//
//
//#include "MachinePageStdafx.h"

#include "MachineData.h"

/*初始化函数*/
bool MachineData::init()
{
    m_FromMachine = false;
    return true;
}
void MachineData::reset()
{
    m_questions.clear();
    m_CurTime = 0;
    m_FromMachine = false;
}
void MachineData::free()
{
    reset();
}
vector<QuestionInfo>* MachineData::getQuestionData()
{
    return &m_questions;
}
bool MachineData::receiveQuestionData(stuJson& booksJson)
{
    m_questions.clear();
    std::string return_code = booksJson.getValue("return_code");
    stuJson returnData = booksJson.getJson("return_data");
    string audioUrl = returnData.getJson("urls").getJson("question_audio_url").GetString();
    string imageUrl =  returnData.getJson("urls").getJson("question_image_url").GetString();
    string xmlUrl = returnData.getJson("urls").getJson("page_content_folder").GetString();
    string soundUrl = returnData.getJson("urls").getJson("page_sound_folder").GetString();
    
    stuJson questionsData = returnData.getJson("question");
    //text_questions
    int n = questionsData.GetArraySize("text_questions");
    for (int i = 0; i < n; i++) {
        stuJson jsonInfo = questionsData.GetArrayJson("text_questions", i);
        QuestionInfo question;
        question.setID(jsonInfo.getValue("id"));
        question.setOptionID(jsonInfo.getValue("option_id"));
        question.setType(jsonInfo.getValue("type"));
        question.setImage(jsonInfo.getValue("image"));
        question.setAnswer(jsonInfo.getValue("answer"));
        question.setText(jsonInfo.getValue("content"));
        question.setAudio(jsonInfo.getValue("audio"));
        question.setGuideAudio(jsonInfo.getValue("guide_audio"));
        
        question.setImageUrl(imageUrl);
        question.setAudioUrl(audioUrl);
        
        //answer_list
        vector<string> array1;
        int num = jsonInfo.GetArraySize("answer_list");
        for (int j = 0; j < num; j++) {
            array1.push_back(jsonInfo.GetArrayJson("answer_list",j).GetString());
        }
        question.setAnswerList(array1);
        //answer_audio_list
        vector<string> array2;
        num = jsonInfo.GetArraySize("answer_audio_list");
        for (int j = 0; j < num; j++) {
            array2.push_back(jsonInfo.GetArrayJson("answer_audio_list",j).GetString());
        }
        question.setAnswerAudioList(array2);
        //question_list
        vector<string> array3;
        num = jsonInfo.GetArraySize("question_content_list");
        for (int j = 0; j < num; j++) {
            array3.push_back(jsonInfo.GetArrayJson("question_content_list",j).GetString());
        }
        question.setQuestionList(array3);
        //question_audio_list
        vector<string> array4;
        num = jsonInfo.GetArraySize("question_audio_list");
        for (int j = 0; j < num; j++) {
            array4.push_back(jsonInfo.GetArrayJson("question_audio_list",j).GetString());
        }
        question.setQuestionAudioList(array4);
        //add
        m_questions.push_back(question);
    }
    //audio_questions
    int m = questionsData.GetArraySize("audio_questions");
    if (m>0) {//增加一个等待页面
        QuestionInfo question;
        question.setType("20");
        m_questions.push_back(question);
    }
    for (int i = 0; i < m; i++) {
        stuJson jsonInfo = questionsData.GetArrayJson("audio_questions", i);
        QuestionInfo question;
        question.setID(jsonInfo.getJson("audio").getValue("id"));
        question.setType(jsonInfo.getJson("course").getValue("type"));
        string strSnd = jsonInfo.getJson("audio").getValue("pdf_sound");
        UiBase::StringReplace(strSnd,".ogg", ".mp3");//使用mp3的资源  add by hyp
        question.setAudio(strSnd);
        question.setXmlFile(jsonInfo.getJson("audio").getValue("pdf_content"));
        
        stuJson parameters;
        CBaseLoad::LoadJsonString(parameters, jsonInfo.getJson("audio").getValue("parameters"));
        question.setTitleID(parameters.getValue("title_id"));
        
        question.setXmlUrl(xmlUrl);
        question.setAudioUrl(soundUrl);
        //add
        m_questions.push_back(question);
    }
    return true;
}


