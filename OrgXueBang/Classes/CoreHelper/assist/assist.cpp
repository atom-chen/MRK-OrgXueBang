//#include "StdAfx.h"

#include "assist.h"
#include "GameMacro.hpp"
#include "HttpManager.h"
#include "StringUtil.hpp"

#include "tinyxml2/tinyxml2.h"

#include "t_sprintf.h"

std::string stringf(const char* format, ...)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
    va_list arg_list;
    
    va_start(arg_list, format);
    
    // SUSv2 version doesn't work for buf NULL/size 0, so try printing
    // into a small buffer that avoids the double-rendering and alloca path too...
    char short_buf[256];
    auto needed = t_snprintf(short_buf, sizeof short_buf,
                             format, arg_list) + 1;
    if (needed <= sizeof short_buf)
        return short_buf;
    
    // need more space...
    char* p = static_cast<char*>(alloca(needed));
    t_snprintf(p, needed, format, arg_list);
    
    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
    va_list arg_list;
    va_start(arg_list, format);
    char* p = static_cast<char*>(alloca(512));
    t_sprintf(p, format, arg_list);
#endif

    return p;
}

//Vec2 assist_getConfigPos(const stuJson& Json)
//{
//    auto pos = Vec2(atof(Json.getValue("x").c_str()), atof(Json.getValue("y").c_str()))  ;
//    return assist_getConfigPos(pos);
//}

Vec2 assist_getConfigPos(Vec2 pos)
{
    auto size = Director::getInstance()->getWinSize();
    pos = pos + Vec2((size.width - MY_DESIGNSIZE_W) / 2, (size.height - MY_DESIGNSIZE_H) / 2);
    return pos;
}

//Rect assist_getConfigRect(const stuJson& Json)
//{
//    Rect r = Rect(atof(Json.getValue("x").c_str()), atof(Json.getValue("y").c_str()),atof(Json.getValue("w").c_str()), atof(Json.getValue("h").c_str()))  ;
//    r.origin = assist_getConfigPos(r.origin);
//    return r;
//}

Rect calculateRectInGL(Node* pNode)
{
    Size sizeNode = pNode->getContentSize();
    Vec2 anchorNode = pNode->getAnchorPoint();
    Vec2 posNode = pNode->getPosition();
    Vec2 posLeftButtom = Vec2(posNode.x - anchorNode.x * sizeNode.width * pNode->getScaleX(), posNode.y - anchorNode.y * sizeNode.height * pNode->getScaleY());
    Vec2 posRightTop = Vec2(posLeftButtom.x + sizeNode.width * pNode->getScaleX(), posLeftButtom.y + sizeNode.height * pNode->getScaleY());
    
    Rect rectNodeInGL;
    if(pNode->getParent())
    {
        posLeftButtom = pNode->getParent()->convertToWorldSpace(posLeftButtom);
        posRightTop = pNode->getParent()->convertToWorldSpace(posRightTop);
    }
    
    rectNodeInGL.origin = posLeftButtom;
    rectNodeInGL.size   = Size(posRightTop.x - posLeftButtom.x, posRightTop.y - posLeftButtom.y);
    
    return rectNodeInGL;
}

const char* GetXmlStringByKey(const std::string& key)
{
//    const char *pKey = key.c_str();
//
//    __String *pstr = NULL;
//    const char* value = NULL;
//    tinyxml2::XMLElement* rootNode = NULL;
//    tinyxml2::XMLDocument* doc = NULL;
//    tinyxml2::XMLElement* curNode = NULL;
//    do
//    {
//        tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument();
//        doc = xmlDoc;
//
//        std::string xmlBuffer = FileUtils::getInstance()->getStringFromFile(MYDEF_XMLFILE);
//
//        if (xmlBuffer.empty())
//        {
//            CCLOG("can not read xml file");
//            break;
//        }
//        xmlDoc->Parse(xmlBuffer.c_str(), xmlBuffer.size());
//        // get root node
//        rootNode = xmlDoc->RootElement();
//        if (NULL == rootNode)
//        {
//            CCLOG("read root node error");
//            break;
//        }
//        // find the node
//        curNode = (rootNode)->FirstChildElement();
//        while (NULL != curNode)
//        {
//            const char* nodeName = curNode->Value();
//            if (!strcmp(nodeName, pKey))
//            {
//                break;
//            }
//            curNode = curNode->NextSiblingElement();
//        }
//    } while (0);
//    // find the node
//    if (curNode && curNode->FirstChild())
//    {
//        value = (const char*)(curNode->FirstChild()->Value());
//    }
//
//    if (value)
//    {
//        pstr = __String::createWithFormat("%s",value);
//    }
//
//    if(doc)
//    {
//        delete doc;
//    }
//    if (pstr != NULL)
//    {
//        return pstr->getCString();
//    }
//    return nullptr;
}

const char* GetXmlString(const std::string& key)
{
   
//    if(MYDEF_IS_ENGLISH)
//    {
//        std::string keyEn = key + MYDEF_KEY_ENGLISH;
//        auto r = GetXmlStringByKey(keyEn);
//        if(r != nullptr)
//            return r;
//    }
//
//    if(MYDEF_IS_SET_CHINESE)
//    {
//        std::string keyCn = key + MYDEF_KEY_CHINESE;
//        auto r = GetXmlStringByKey(keyCn);
//        if(r != nullptr)
//            return r;
//    }
//
//    auto r = GetXmlStringByKey(key);
//    if(r != nullptr)
//        return r;
    
    return key.c_str();
}


/* 转化颜色到color3b */
Color3B convertColor(std::string strColor)
{
    Color3B color(0,0,0);
    if (strColor.length() != 7)
    {
        return color;
    }
    
    strColor.erase(0,1);
    int a[6];
    for (int i = 0 ; i < 6; i++)
    {
        if (strColor[i] >= '0' && strColor[i] <= '9' )
            a[i] = strColor[i] - '0';
        else if (strColor[i] >= 'A' && strColor[i] <= 'F' )
            a[i] = strColor[i] - 'A' + 10;
        else if (strColor[i] >= 'a' && strColor[i] <= 'f' )
            a[i] = strColor[i] - 'a' + 10;
    }
    color = Color3B(a[0] * 16 + a[1],a[2] * 16 + a[3],a[4] * 16 + a[5]);
    return color;
}


/* 转化header数据 */
std::vector<std::string> convertHeaderDataToVector(const std::map<std::string, std::string>& value)
{
    std::vector<std::string> vecResult;
    
    for (auto& it : value)
    {
        std::string strT = it.first + ":" + it.second;
        vecResult.push_back(strT);
    }
    
    return vecResult;
}


/* 转化url数据 */
void convertUrlDataToString(std::string& strUrl, KVPairVec* value)
{
    strUrl += '?';
    std::string strMD5;
    KVPairVec::iterator iter;
    for (iter = value->begin(); iter != value->end(); iter++) {
        KVPair* kv = *iter;

        std::string strTmp = kv->m_strKey + "=" + StringUtil::UrlEncode(kv->m_strValue) + "&";
        strUrl += strTmp;
        strMD5 += StringUtil::UrlEncode(kv->m_strValue);
        
//        std::string strTmp = kv->m_strKey + "=" + kv->m_strValue + "&";
//        strUrl += strTmp;
//        strMD5 += kv->m_strValue;
    }
    HttpManager::getMd5str(strMD5);
    strUrl = strUrl + "signcode=" + strMD5;
}




/* 移动距离是否有效 */
#define MOVE_INCH            7.0f/160.0f
bool isMoveDistanceValid(float fDistance)
{
    auto glview = Director::getInstance()->getOpenGLView();
    float factor = ( glview->getScaleX() + glview->getScaleY() ) / 2;
    float fValue = fDistance * factor / Device::getDPI();
    if(fabs(fValue) < MOVE_INCH)
    {
        return false;
    }
    return true;
}
