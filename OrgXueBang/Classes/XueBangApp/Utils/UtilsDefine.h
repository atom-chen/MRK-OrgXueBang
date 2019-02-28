//
//  UtilsDefine.hpp
//  ReadingFriends
//
//  Created by metrics001 on 16/10/9.
//
//

#ifndef UtilsDefine_hpp
#define UtilsDefine_hpp

#include "BaseHeader.h"
#include "BaseLoad.h"
#include <regex>

//内存操作
#define MEMSET          memset
#define MEMCPY          memcpy
#define MEMMOVE         memmove

//#define        MALLOC(s)        malloc(s)
//#define        FREE(p)            free(p)
#define        FREEIF(p)        if (p) { FREE((void*)p); (p) = 0; }

//获取和设置一个值的宏
#define MYDEF_GETSETVALUE(type,name)    \
type m_##name;\
void set##name(type value)\
{\
m_##name = value;\
}\
type get##name()\
{\
return m_##name;\
}\
type* getAddress##name()\
{\
return &m_##name;\
}


//获取和设置数组的宏
#define MYDEF_GETSETVALUEARRAY(type,name,len)    \
type m_##name[len];\
bool set##name(const type* value,int nLen)\
{\
if(nLen < len)\
{\
memset(m_##name,0,sizeof(type)*len);\
memcpy(m_##name,value,nLen);\
return true;\
}\
return false;\
}\
type* get##name()\
{\
return m_##name;\
}

/*检查array里面是否有value*/
template<typename T>
bool CHECK_EXIST_FROM_ARRAY(T value,vector<T>* arr)
{
    for (int i = 0; i<arr->size(); i++) {
        if (value == arr->at(i)) {
            return true;
        }
    }
    return false;
}
/*得到value在arr的位置*/
template<typename T>
int GET_INDEX_FROM_ARRAY(T value,vector<T>* arr)
{
    for (int i = 0; i<arr->size(); i++) {
        if (value == arr->at(i)) {
            return i;
        }
    }
    return -1;
}

/*输出显示*/
#define kMaxStringLen (1024*100)
static void DEBUG_LOG(const char * format, ...)
{
#if not defined(COCOS2D_DEBUG) ||  (COCOS2D_DEBUG == 0)
    return;
#endif

    va_list args;
    va_start(args, format);

    char* pBuf = (char*)malloc(kMaxStringLen);
    if (pBuf != nullptr)
    {
        vsnprintf(pBuf, kMaxStringLen, format, args);
        log("%s",pBuf);
        free(pBuf);
    }
    va_end(args);
}
/*替换字符串*/
static string STRING_REPLACE_ALL(const string &str, string old, string replace)
{
/*
    string str_new = str;
    int size = (int)old.size();
    while (str_new.find(old)!=-1) {
        str_new = str_new.replace(str_new.find(old), size, replace);
    }
    return str_new;
*/
    std::regex r(old);
    std::string fmt(replace);
    return regex_replace(str, r, fmt);
}
/*分割字符串为int数组*/
static void splitString2Int(const char * source ,char splitChar,vector <int> & outputVec)
{
    char * temp = (char *)source;
    int i = 0;
    while(*(temp+i)!= '\0')
    {
        if(*(temp+i)==splitChar)
        {
            string s(temp,i);
            outputVec.push_back(atoi(s.c_str()));
            temp = temp+(i+1);
            i=0;
        }
        i++;
    }
    //最后一个也要加进去， just for test, test again
    if( i > 0 ){
        string s(temp,i);
        outputVec.push_back(atoi(s.c_str()));
    }
}

/*分割字符串为string数组*/
static void splitString2String(const char * source ,char splitChar,vector <string> & outputVec)
{
    char * temp = (char *)source;
    int i = 0;
    while(*(temp+i)!= '\0')
    {
        if(*(temp+i)==splitChar)
        {
            string s(temp,i);
            outputVec.push_back(s);
            temp = temp+(i+1);
            i=0;
        }
        i++;
    }
    //最后一个也要加进去， just for test, test again
    if( i > 0 ){
        string s(temp,i);
        outputVec.push_back(s);
    }
}

/*json与vec转换*/
static void jsonToVecStr(vector<string>&vec,const stuJson& json,string key)
{
    for (int i = 0 ; i < json.GetArraySize(key); i++)
    {
        auto value = json.GetArrayJson(key, i);
        vec.push_back(value.GetString());
    }
}
/*json与vec转换*/
static void vecToJsonInt(rapidjson::Document& doc1,vector<string>& vec,string& key)
{
    rapidjson::Document::AllocatorType& allocator = doc1.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    for (auto iter = vec.begin(); iter!=vec.end(); iter++) {
        array.PushBack(atoi((*iter).c_str()), allocator);
    }
    rapidjson::Value jsonStr(rapidjson::kStringType);
    jsonStr.SetString(key.c_str(), (int)key.size(),allocator);
    doc1.AddMember(jsonStr, array, allocator);
}
/*json与vec转换*/
static void vecToJsonStr(rapidjson::Document& doc1,vector<string>& vec,string& key)
{
    rapidjson::Document::AllocatorType& allocator = doc1.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
    for (auto iter = vec.begin(); iter!=vec.end(); iter++) {
        rapidjson::Value jsonStr(rapidjson::kStringType);
        jsonStr.SetString((*iter).c_str(), (int)(*iter).size(),allocator);
        array.PushBack(jsonStr, allocator);
    }
    rapidjson::Value jsonStr(rapidjson::kStringType);
    jsonStr.SetString(key.c_str(), (int)key.size(),allocator);
    doc1.AddMember(jsonStr, array, allocator);
}


static stuJson readJsonData(string path)
{
    stuJson jsonData;
    CBaseLoad::LoadJson(jsonData, path.c_str());
    return jsonData;
}

static bool strIsNull(string str)
{
    if(str==""||str=="-1"){
        return true;
    }
    return false;
}


static bool BUT_TOUCH_ENABLE = true;
static Node* notDestroyNode= NULL;
static bool BOOL_BUT_TOUCH_ENABLE()
{
    if(!notDestroyNode)
    {
        notDestroyNode = Node::create();
        notDestroyNode->retain();
        notDestroyNode->onEnter();
    }
    if(!BUT_TOUCH_ENABLE){
        return false;
    }
    BUT_TOUCH_ENABLE = false;
    notDestroyNode->scheduleOnce([=](float t)
                                            {
                                            BUT_TOUCH_ENABLE = true;
                                       }, 0.5f, "BOOL_BUT_TOUCH_ENABLE");
    
    return true;
}


#endif /* UtilsDefine_hpp */
