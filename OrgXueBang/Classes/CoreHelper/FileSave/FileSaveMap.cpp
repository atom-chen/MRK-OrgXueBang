#include "PreIncludes.hpp"
#include "FileSaveMap.h"

CFileSaveMap* g_CFileSaveMap = nullptr;

CFileSaveMap* CFileSaveMap::getInstance()
{
    if(g_CFileSaveMap == nullptr)
    {
        g_CFileSaveMap  = new CFileSaveMap();
        g_CFileSaveMap->init();
    }
    return g_CFileSaveMap;
}

void CFileSaveMap::destroyInstance()
{
    if(g_CFileSaveMap != nullptr)
        delete g_CFileSaveMap;
    g_CFileSaveMap = nullptr;
}

bool CFileSaveMap::init()
{
    std::string path = FileUtils::getInstance()->getWritablePath() + MYDEF_FILEJSONSAVE_DIR;
    auto pFileUtils = FileUtils::getInstance();
    if(pFileUtils->isDirectoryExist(path) == false)
    {
        pFileUtils->createDirectory(path);
        while(!pFileUtils->isDirectoryExist(path))
            ;
    }
    return true;
}

string CFileSaveMap::readFromFile(string key,string file)
{
    string filePath =FileUtils::getInstance()->getWritablePath() + MYDEF_FILEJSONSAVE_DIR + file;
    ValueMap valueMap =  FileUtils::getInstance()->getValueMapFromFile(filePath);
    if(valueMap.find(key)==valueMap.end())
    {
        return "";
    }
    return valueMap[key].asString();
}

void CFileSaveMap::writeValueFile(string key,string value,string file)
{
    string filePath =FileUtils::getInstance()->getWritablePath() + MYDEF_FILEJSONSAVE_DIR + file;
    ValueMap valueMap =  FileUtils::getInstance()->getValueMapFromFile(filePath);
    valueMap[key] = value;
    FileUtils::getInstance()->writeToFile(valueMap,filePath);
}
void CFileSaveMap::eraseValueFile(string key,string file)
{
    string filePath =FileUtils::getInstance()->getWritablePath() + MYDEF_FILEJSONSAVE_DIR + file;
    ValueMap valueMap =  FileUtils::getInstance()->getValueMapFromFile(filePath);
    valueMap.erase(key);
    FileUtils::getInstance()->writeToFile(valueMap,filePath);
}


