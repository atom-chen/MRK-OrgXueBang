
#include "BaseLoad.h"

#define BOOKLOAD_JSON_BASIC				"book_basic"
#define BOOKLOAD_JSON_BOOKLESSON		"book_lesson"
#define BOOKLOAD_JSON_TYPE              "type"
#define BOOKLOAD_JSON_SOURCE			"source"
#define BOOKLOAD_JSON_Transition        "transition"
#define BOOKLOAD_JSON_LESSON            "lesson"

#define BOOKLOAD_JSON_GETSTRING(Jobj,Jkey)	(Jobj.HasMember(Jkey) ? Jobj[Jkey].GetString() : "-1")

static stuJson s_emptyJson;

bool CBaseLoad::LoadConfig(stuBookDef& stuBook,std::string path)
{
    //加载文件
    Data data =  FileUtils::getInstance()->getDataFromFile(path);
    if (data.getSize() <= 0)
    {
        CCLOG("CBookLoad::LoadConfig:load file %s fail.",path.c_str());
        return false;
    }
    //JSON解析
    rapidjson::Document json_doc;
    
    char* bytes = (char *)malloc(data.getSize()+1);
    memset(bytes, 0, data.getSize()+1);
    memcpy(bytes, data.getBytes(),data.getSize());

    json_doc.Parse<0>(bytes);

    free(bytes);
    bytes = nullptr;
    
    if (json_doc.HasParseError())
    {
        CCLOG("CBookLoad::LoadConfig:json_doc.HasParseError().");
        auto err = json_doc.GetParseError();
        CCLOG("CBookLoad::LoadConfig:err:%d,pos:%ld",err,json_doc.GetErrorOffset());
        return false;
    }
    if(!json_doc.IsObject())
        return false;
    
    //读Basic
    if(!json_doc.HasMember(BOOKLOAD_JSON_BASIC))
        return false;
    const rapidjson::Value &pBasic = json_doc[BOOKLOAD_JSON_BASIC];
  
    stuBook.m_stuBasic.m_strType = BOOKLOAD_JSON_GETSTRING(pBasic,BOOKLOAD_JSON_TYPE);
    stuBook.m_stuBasic.m_strPath = stuBook.m_strPath + BOOKLOAD_JSON_GETSTRING(pBasic,BOOKLOAD_JSON_SOURCE);
    stuBook.m_stuBasic.m_strTransition = BOOKLOAD_JSON_GETSTRING(pBasic,BOOKLOAD_JSON_Transition);
    //读lesson
    stuBook.m_AllLesson.clear();
    if(!json_doc.HasMember(BOOKLOAD_JSON_BOOKLESSON))
        return false;
    const rapidjson::Value &pArrayLesson = json_doc[BOOKLOAD_JSON_BOOKLESSON];
    if(!pArrayLesson.IsArray())
        return false;
    for (rapidjson::SizeType i = 0; i < pArrayLesson.Size(); i++)
    {
        stuLessonDef stuLesson;
        stuLesson.clear();
        const rapidjson::Value &pLesson = pArrayLesson[i];
        //是否有lesson字段
        if(!pLesson.HasMember(BOOKLOAD_JSON_LESSON))
            return false;
        //循环lesson 数组
        const rapidjson::Value &pArrayModule = pLesson[BOOKLOAD_JSON_LESSON];
        for (rapidjson::SizeType j = 0; j < pArrayModule.Size(); j++)
        {
            stuBookModuleDef stuBookModule;
            const rapidjson::Value &pModule = pArrayModule[j];
            stuBookModule.m_strType = BOOKLOAD_JSON_GETSTRING(pModule,BOOKLOAD_JSON_TYPE);
            stuBookModule.m_strPath = stuBook.m_strPath + BOOKLOAD_JSON_GETSTRING(pModule,BOOKLOAD_JSON_SOURCE);
            stuBookModule.m_strTransition = BOOKLOAD_JSON_GETSTRING(pModule,BOOKLOAD_JSON_Transition);

            stuLesson.push_back(stuBookModule);
        }
        stuBook.m_AllLesson.push_back(stuLesson);
    }
    
    return true;
}

bool CBaseLoad::LoadJson(stuJson& stuJson,std::string path)
{
    //加载文件
    Data data =  FileUtils::getInstance()->getDataFromFile(path);
    if (data.getSize() <= 0)
    {
        CCLOG("CBookLoad::LoadConfig:load file %s fail.",path.c_str());
        return false;
    }
    //JSON解析
    rapidjson::Document json_doc;
    
    char* bytes = (char *)malloc(data.getSize()+1);
    memset(bytes, 0, data.getSize()+1);
    memcpy(bytes, data.getBytes(),data.getSize());
    
    json_doc.Parse<0>(bytes);
    
    free(bytes);
    bytes = nullptr;
    
    if (json_doc.HasParseError())
    {
        CCLOG("CBookLoad::LoadConfig:HasParseError():%d,pos:%ld",json_doc.GetParseError(),json_doc.GetErrorOffset());
        return false;
    }
    if(!json_doc.IsObject())
        return false;
    
    stuJson.Load(json_doc);
    return  true;
}

/*加载一个JSON字符串*/
bool CBaseLoad::LoadJsonString(stuJson& stuJson, std::string strData)
{
    //JSON解析
    rapidjson::Document json_doc;
    json_doc.Parse<0>(strData.c_str());
    
    if (json_doc.HasParseError())
    {
        CCLOG("CBookLoad::LoadConfig:HasParseError():%d,pos:%ld",json_doc.GetParseError(),json_doc.GetErrorOffset());
        return false;
    }
    if(!json_doc.IsObject())
        return false;
    
    stuJson.Load(json_doc);
    return true;
}

bool stuJson::Load(const rapidjson::Value &pValue)
{
    init();
    if (pValue.IsString()) {
        m_strEmpty = pValue.GetString();
        if(m_strEmpty.length())
            return true;
        return false;
    }
    else if (pValue.IsInt())
    {
        m_strEmpty = StringUtils::format("%d", pValue.GetInt());
        return true;
    }
    else if (pValue.IsDouble())
    {
        m_strEmpty = StringUtils::format("%f", pValue.GetDouble());
        return true;
    }
    //////////////
    else if (pValue.IsBool())
    {
        m_strEmpty = StringUtils::format("%d", pValue.GetBool());
        return true;
    }
    /////////////////
    if(!pValue.IsObject())
        return  false;
    bool bOk = false;
    for (rapidjson::Document::ConstMemberIterator pos = pValue.MemberBegin(); pos != pValue.MemberEnd(); pos++)
    {
        bOk = true;
        if(pos->value.IsArray())
        {
            if (pos->value.Size())
            {
                std::vector<stuJson> stuArray;
                m_stuJsonArrayMap[pos->name.GetString()] = stuArray;
                auto it = m_stuJsonArrayMap.find(pos->name.GetString());
                for (rapidjson::SizeType i = 0; i < pos->value.Size(); i++)
                {
                    const rapidjson::Value &p = pos->value[i];
                    stuJson stu;
                    stu.Load(p);
                    it->second.push_back(stu);
                }
            }
        }
        else
        {
            stuJson stu;
            if(stu.Load(pos->value))
            {
                std::vector<stuJson> stuArray;
                stuArray.push_back(stu);
                m_stuJsonArrayMap[pos->name.GetString()] = stuArray;
            }
        }
    }
    return bOk;
    
}

/*获取一个value*/
const std::string& stuJson::getValue(const std::string& key) const
{
    return GetArrayValue(key,0);
}
/*获取一个结构*/
const stuJson& stuJson::getJson(const std::string& key) const
{
    return GetArrayJson(key,0);
}
/*获取数组大小*/
const int stuJson::GetArraySize(const std::string& key) const
{
    std::string keyCn = key + MYDEF_KEY_CHINESE;
    if(MYDEF_IS_CHINESE && m_stuJsonArrayMap.find(keyCn) != m_stuJsonArrayMap.end())
        return  (int)m_stuJsonArrayMap.find(keyCn)->second.size();
    return m_stuJsonArrayMap.find(key) == m_stuJsonArrayMap.end() ? 0 : (int)m_stuJsonArrayMap.find(key)->second.size();
}
/*获取数组中元素*/
const stuJson& stuJson::GetArrayJson(const std::string& key,int nPos) const
{
    if(nPos < 0 || nPos >= GetArraySize(key))
    {
        s_emptyJson.init();
        return s_emptyJson;
//        return *this;
    }
    std::string keyCn = key + MYDEF_KEY_CHINESE;
    if(MYDEF_IS_CHINESE && m_stuJsonArrayMap.find(keyCn) != m_stuJsonArrayMap.end())
        return  m_stuJsonArrayMap.find(keyCn)->second[nPos];
    return m_stuJsonArrayMap.find(key)->second[nPos];
}
/*获取数组中value*/
const std::string& stuJson::GetArrayValue(const std::string& key,int nPos) const
{
    if(nPos < 0 || nPos >= GetArraySize(key)) return this->GetString();
    std::string keyCn = key + MYDEF_KEY_CHINESE;
    if(MYDEF_IS_CHINESE && m_stuJsonArrayMap.find(keyCn) != m_stuJsonArrayMap.end())
        return  m_stuJsonArrayMap.find(keyCn)->second[nPos].GetString();
    return m_stuJsonArrayMap.find(key)->second[nPos].GetString();
}
/*获取string*/
const std::string& stuJson::GetString() const
{
    return m_strEmpty;
    
}

/* 是否有此结结构体 */
bool stuJson::isHasJson(const std::string& key) const
{
    std::string keyCn = key + MYDEF_KEY_CHINESE;
    if(MYDEF_IS_CHINESE && m_stuJsonArrayMap.find(keyCn) != m_stuJsonArrayMap.end())
        return  true;
    return m_stuJsonArrayMap.find(key) != m_stuJsonArrayMap.end();
}

