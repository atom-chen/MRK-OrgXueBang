#ifndef __BOOK_LOAD_H__
#define __BOOK_LOAD_H__

/*
 图书加载
 by：高超
 */

#include "PreIncludes.hpp"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace  rapidjson;

/*一个模块的结构*/
typedef struct stuBookModuleDef
{
    std::string     m_strType;
    std::string     m_strPath;
    std::string     m_strTransition;
} stuBookModuleDef;

/*一课就是一个数组*/
typedef std::vector<stuBookModuleDef> stuLessonDef;

/*一本书*/
typedef struct stuBookDef
{
    std::string m_strBookId;        //书本id
    std::string m_strPath;          //文件夹路径
    //base
    stuBookModuleDef m_stuBasic;
    //3个lesson
    std::vector<stuLessonDef> m_AllLesson;
} stuBookDef;


#define MYDEF_KEY_CHINESE   "_cn"
#define MYDEF_IS_CHINESE   false

typedef struct stuJson
{
//    std::map<std::string,std::string>           m_strValueMap;
//    
//    std::map<std::string,stuJson>               m_stuJsonMap;
    
    std::map<std::string,std::vector<stuJson>>  m_stuJsonArrayMap;
//    stuJson*          m_pSelf;
    std::string       m_strEmpty;
    /*获取一个value*/
    const std::string& getValue(const std::string& key) const;
    /*获取一个结构*/
    const stuJson& getJson(const std::string& key) const;
    /*获取数组大小*/
    const int GetArraySize(const std::string& key) const;
    /*获取数组中元素*/
    const stuJson& GetArrayJson(const std::string& key,int nPos) const;
    /*获取数组中value*/
    const std::string& GetArrayValue(const std::string& key,int nPos) const;
    /*获取string*/
    const std::string& GetString() const;
    /* 是否有此结结构体 */
    bool isHasJson(const std::string& key) const;
//    /*获取一个value*/
//    const std::string& getValue(const std::string& key) const
//    {
//        return GetArrayValue(key,0);
//    }
//    /*获取一个结构*/
//    const stuJson& getJson(const std::string& key) const
//    {
//        return GetArrayJson(key,0);
//    }
//    /*获取数组大小*/
//    const int GetArraySize(const std::string& key) const
//    {
//        std::string keyCn = key + MYDEF_KEY_CHINESE;
//        if(MYDEF_IS_CHINESE && m_stuJsonArrayMap.find(keyCn) != m_stuJsonArrayMap.end())
//            return  (int)m_stuJsonArrayMap.find(keyCn)->second.size();
//        return m_stuJsonArrayMap.find(key) == m_stuJsonArrayMap.end() ? 0 : (int)m_stuJsonArrayMap.find(key)->second.size();
//    }
//    /*获取数组中元素*/
//    const stuJson& GetArrayJson(const std::string& key,int nPos) const
//    {
//        if(nPos < 0 || nPos >= GetArraySize(key)) return *this;
//        std::string keyCn = key + MYDEF_KEY_CHINESE;
//        if(MYDEF_IS_CHINESE && m_stuJsonArrayMap.find(keyCn) != m_stuJsonArrayMap.end())
//            return  m_stuJsonArrayMap.find(keyCn)->second[nPos];
//        return m_stuJsonArrayMap.find(key)->second[nPos];
//    }
//    /*获取数组中value*/
//    const std::string& GetArrayValue(const std::string& key,int nPos) const
//    {
//        if(nPos < 0 || nPos >= GetArraySize(key)) return this->GetString();
//        std::string keyCn = key + MYDEF_KEY_CHINESE;
//        if(MYDEF_IS_CHINESE && m_stuJsonArrayMap.find(keyCn) != m_stuJsonArrayMap.end())
//            return  m_stuJsonArrayMap.find(keyCn)->second[nPos].GetString();
//        return m_stuJsonArrayMap.find(key)->second[nPos].GetString();
//    }
//    /*获取string*/
//    const std::string& GetString() const
//    {
//        return m_strEmpty;
//    }
//    
//    /* 是否有此结结构体 */
//    bool isHasJson(const std::string& key) const
//    {
//        std::string keyCn = key + MYDEF_KEY_CHINESE;
//        if(MYDEF_IS_CHINESE && m_stuJsonArrayMap.find(keyCn) != m_stuJsonArrayMap.end())
//            return  true;
//        return m_stuJsonArrayMap.find(key) != m_stuJsonArrayMap.end();
//    }
    
    ///////////////////////////////////////////
    //以下接口不对外
    
    /*初始化*/
    void init()
    {
        m_stuJsonArrayMap.clear();
        m_strEmpty = "-1";
    };
    bool Load(const rapidjson::Value &pValue);
    
}stuJson;




class CBaseLoad
{
public://要求绝对路径
    /*加载一本书的定义部分*/
    static bool LoadConfig(stuBookDef& stuBook,std::string path);
    /*加载一个JSON文件*/
    static bool LoadJson(stuJson& stuJson,std::string path);
    /*加载一个JSON字符串*/
    static bool LoadJsonString(stuJson& stuJson, std::string strData);

};

#endif // __BOOK_LOAD_H__
