//
//  MachinePageStdafx.h
//  ReadingMate
//
//  Created by hyp on 18/11/15.
//
//

//#include "MachinePageStdafx.h"
#include "MachineManager.h"

static MachineManager* g_MachineManager = nullptr;
/*单例*/
MachineManager* MachineManager::getInstance()
{
    if (g_MachineManager == nullptr) {
        g_MachineManager = new MachineManager;
        g_MachineManager->init();
    }
    return g_MachineManager;
    
}
void MachineManager::destroyInstance()
{
    if (g_MachineManager) {
        g_MachineManager->free();
        delete g_MachineManager;
        g_MachineManager = nullptr;
    }
}

/*初始化函数*/
bool MachineManager::init()
{
    m_machineData = new MachineData();
    m_machineData->init();
    m_exitCallBack=nullptr;
    return true;
}
void MachineManager::reset()
{
    m_curProcess = 1;
    m_machineData->reset();
}
void MachineManager::free()
{
    m_machineData->free();
    delete m_machineData;
    m_machineData = nullptr;
}
MachineData* MachineManager::getMachineData()
{
    return m_machineData;
}
void MachineManager::setOnExitCallBack(std::function< void () > exitCallBack)
{
    m_exitCallBack = exitCallBack;
}
void MachineManager::runOnExitCallBack()
{
    if(m_exitCallBack!=nullptr)
    {
        m_exitCallBack();
    }
    m_exitCallBack=nullptr;
}
void MachineManager::setNextCallBack(std::function< void () > nextCallBack)
{
     m_nextCallBack = nextCallBack;
}
void MachineManager::runNextCallBack()
{
    if(m_nextCallBack!=nullptr)
    {
        m_nextCallBack();
    }
    m_nextCallBack=nullptr;
}
Vec2 MachineManager::getCurProcess()
{
    return  Vec2(m_curProcess,m_machineData->getQuestionData()->size());
}
Vec2 MachineManager::getShowProcess()
{
    auto questions = m_machineData->getQuestionData();
    int audioIndex = -1;//默认没有录音题
    for (int i = 0;i<questions->size();i++) {
        if (atoi(questions->at(i).getType().c_str())==QuestionType::Type_12) {
            audioIndex = i;
            break;
        }
    }
    int cur = m_curProcess;
    long len = questions->size();
    if (audioIndex>-1) {//有录音题
        len--;
        cur = (cur < audioIndex+1?cur:cur-1);
    }
    return  Vec2(cur,len);
}
vector<QuestionInfo>* MachineManager::getQuestionData()
{
    return m_machineData->getQuestionData();
}

void MachineManager::cumulateProcess(int num)
{
    m_curProcess = m_curProcess + num;
}

void MachineManager::requestAllData(bool force)
{
    requestQuestionData();
    requestRankData();
}
void MachineManager::setSomeParameter(string optionID,string classID,string bookID,string dayID,string lessonName,string stageId)
{
    m_machineData->setBookID(bookID);
    m_machineData->setOptionID(optionID);
    m_machineData->setClassID(classID);
    m_machineData->setDayID(dayID);
    m_machineData->setLessonName(lessonName);
    m_machineData->setStageId(stageId);
}
void MachineManager::requestQuestionData()
{
    Vec2 curProcess =  MachineManager::getInstance()->getCurProcess();
    unordered_map<string, string>heads;
    unordered_map<string, string>values;
    values["option_id"] = m_machineData->getOptionID();
//    values["option_id"] = "420";
    CDataHttp::getInstance()->HttpGetNew(eHttpType::getMachineQuestion,heads,values);
}
void MachineManager::requestRankData()
{
    unordered_map<string, string>heads;
    unordered_map<string, string>values;
    values["day"] = m_machineData->getDayID();
    values["class_id"] = m_machineData->getClassID();
    CDataHttp::getInstance()->HttpGetNew(eHttpType::getMachineNo1,heads,values);
}

//联网操作回调
void MachineManager::endWithHttpData(eHttpType nOp,bool bSuccess,const stuJson& json)
{
    if (nOp != eHttpType::getMachineQuestion)
    {
        return;
    }
    eHttpStateBase e = CDataHttp::getInstance()->GetHttpOpState(nOp);
    if(eHttpStateBase::eHttpStateBase_Rok == e)
    {
        if (nOp == eHttpType::getMachineQuestion)
        {
            m_machineData->receiveQuestionData((stuJson&)json);
        }
        
    }
    else if(e == eHttpStateBase::eHttpStateBase_Rerr)
    {
        
    }
    else
    {
        
    }
    
    
}
