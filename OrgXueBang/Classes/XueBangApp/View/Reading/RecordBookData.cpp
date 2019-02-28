//
//  RecordBookData.cpp
//  ReadingMate-mobile
//
//  Created by metrics001 on 2018/3/20.
//

#include "RecordBookData.hpp"
static RecordBookData* g_RecordBookData = nullptr;
RecordBookData* RecordBookData::getInstance()
{
    if (g_RecordBookData == nullptr) {
        g_RecordBookData = new RecordBookData;
        g_RecordBookData->init();
    }
    return g_RecordBookData;
}

void RecordBookData::destroyInstance()
{
    if (g_RecordBookData) {
        delete g_RecordBookData;
        g_RecordBookData = nullptr;
    }
}

/*初始化函数*/
bool RecordBookData::init()
{
    m_recordMode = eRecordBookMode::eRecord;
    return true;
}
