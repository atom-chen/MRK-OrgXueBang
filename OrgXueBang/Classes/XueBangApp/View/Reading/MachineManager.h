//
//  MachinePageStdafx.h
//  ReadingMate
//
//  Created by hyp on 18/11/15.
//
//

#ifndef MachineManager_hpp
#define MachineManager_hpp

//#include "UtilsDefine.h"
#include "stdafx.h"
#include "MachineData.h"
#include "CDataHttp.hpp"

/*练习题管理类*/
class MachineManager {
public:
    /*单例*/
    static MachineManager* getInstance();
    static void destroyInstance();
    /*初始化函数*/
    bool init();
    /*释放对象*/
    void free();
    /*数据重置*/
    void reset();
public:
    /*设置结束回调方法*/
    void setOnExitCallBack(std::function< void () > exitCallBack);
    /*执行结束回调方法*/
    void runOnExitCallBack();
    /*设置下一题回调方法*/
    void setNextCallBack(std::function< void () > nextCallBack);
    /*执行下一题回调方法*/
    void runNextCallBack();
    /*设置需要记录使用的数据*/
    void setSomeParameter(string optionID,string classID,string bookID,string dayID,string lessonName,string stageId);
    /*计算进度*/
    void cumulateProcess(int num = 1);
    /*得到真实进度*/
    Vec2 getCurProcess();
    /*得到显示进度*/
    Vec2 getShowProcess();
    /*得到题库列表*/
    vector<QuestionInfo>* getQuestionData();
    /*得到数据对象*/
    MachineData* getMachineData();
public:
    /*请求网络数据*/
    void requestAllData(bool force = false);
    /*请求题库数据*/
    void requestQuestionData();
    /*请求排行数据*/
    void requestRankData();
    /*接受服务器返回数据*/
    void endWithHttpData(eHttpType nOp,bool bSuccess,const stuJson& json);
private:
    /*解析服务器返回数据*/
    bool receiveQuestionData(stuJson& booksJson);
private:
    /*数据对象*/
    MachineData* m_machineData;
    /*下一题回调对象*/
    std::function< void ()> m_nextCallBack;
    /*结束回调对象*/
    std::function< void ()> m_exitCallBack;
    /*当前进度*/
    int m_curProcess;
};
#endif /* MachineManager_hpp */
