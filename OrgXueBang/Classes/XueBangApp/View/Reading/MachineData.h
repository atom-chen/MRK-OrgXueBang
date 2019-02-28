//
//  MachinePageStdafx.h
//  ReadingMate
//
//  Created by hyp on 18/11/15.
//
//

#ifndef MachineData_hpp
#define MachineData_hpp

//#include "UtilsDefine.h"
#include "stdafx.h"
#include "BaseLoad.h"

//题的类型
typedef enum {
    Type_1 = 9, //练习题一
    Type_2 = 10,//练习题二
    Type_3 = 11,//练习题三
    Type_4 = 12,//练习题四
    Type_5 = 13,//练习题五
    Type_6 = 14,//练习题六
    Type_7 = 15,//练习题七
    Type_8 = 16,//练习题八
    Type_9 = 17,//练习题九
    Type_10 = 18,//练习题十
    Type_11 = 19,//练习题十一
    Type_12 = 20//等待页面
}QuestionType;


//每道题所包含的信息
struct QuestionInfo
{
    MYDEF_GETSETVALUE(string,ID);
    MYDEF_GETSETVALUE(string,OptionID);
    MYDEF_GETSETVALUE(string,Type);
    MYDEF_GETSETVALUE(string,Image);
    MYDEF_GETSETVALUE(string,ImageUrl);
    MYDEF_GETSETVALUE(string,Audio);
    MYDEF_GETSETVALUE(string,GuideAudio);
    MYDEF_GETSETVALUE(string,AudioUrl);
    MYDEF_GETSETVALUE(string,Intro);
    MYDEF_GETSETVALUE(string,Text);
    MYDEF_GETSETVALUE(string,XmlUrl);
    MYDEF_GETSETVALUE(string,XmlFile);
    MYDEF_GETSETVALUE(string,Answer);
    MYDEF_GETSETVALUE(string,TitleID);
    MYDEF_GETSETVALUE(vector<string>,AnswerList);
    MYDEF_GETSETVALUE(vector<string>,AnswerAudioList);
    MYDEF_GETSETVALUE(vector<string>,QuestionList);
    MYDEF_GETSETVALUE(vector<string>,QuestionAudioList);
    ~QuestionInfo(){
        
    }
    
};

/*练习题数据类*/
class MachineData {
public:
    /*初始化函数*/
    bool init();
    /*释放对象*/
    void free();
    /*数据重置*/
    void reset();
public:
    /*定义书的ID，课程ID，日期ID，课程名字等*/
    MYDEF_GETSETVALUE(string,BookID);
    MYDEF_GETSETVALUE(string,OptionID);
    MYDEF_GETSETVALUE(string,ClassID);
    MYDEF_GETSETVALUE(string,DayID);
    MYDEF_GETSETVALUE(string,LessonName);
    MYDEF_GETSETVALUE(string,StageId);
    /*定义来源 是否是从练习题进来的*/
    MYDEF_GETSETVALUE(bool,FromMachine);
    /*定义当前使用时间*/
    MYDEF_GETSETVALUE(int,CurTime);
    /*定义结束的数据*/
    MYDEF_GETSETVALUE(stuJson,EndData);
    /*得到题库列表*/
    vector<QuestionInfo>* getQuestionData();
    /*解析服务器返回数据*/
    bool receiveQuestionData(stuJson& booksJson);
private:
    /*定义题库列表*/
    vector<QuestionInfo> m_questions;
};
#endif /* MachineData_hpp */
