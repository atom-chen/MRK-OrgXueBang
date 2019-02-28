//
//  Reador.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/21.
//

#include "Reador.hpp"

static Reador* instance = nullptr;

Reador::Reador()
{
    init();
}

Reador::~Reador()
{
    instance = nullptr;
}

void Reador::init()
{
    
}
Reador* Reador::getInstance()
{
    if (instance) {
        return instance;
    }
    instance = new Reador();
    return instance;
}
void Reador::destroyInstance()
{
    delete instance;
    instance = nullptr;
}

void Reador::setCurrentBook(const string& book, const string task, const string wordClassId, const string qualityId)
{
    m_currentBook = book;
    m_currentTask = task;
    m_currentWordClassId = wordClassId;
    m_currentQualityId = qualityId;
}
std::string& Reador::getCurrentTask()
{
    return m_currentTask;
}
std::string& Reador::getCurrentBook()
{
    return m_currentBook;
}
bool Reador::getPOrRing()
{
    return bPOrRing;
}
void Reador::setPOrRing(bool bIng)
{
    bPOrRing = bIng;
}

const string& Reador::getCurrentWordClass()
{
    return m_currentWordClassId;
}

const string& Reador::getCurrentQualityId()
{
    return m_currentQualityId;
}
