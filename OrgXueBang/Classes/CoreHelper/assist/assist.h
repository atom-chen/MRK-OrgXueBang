#ifndef __INCLUDE_ASSIST_H__
#define __INCLUDE_ASSIST_H__
/*
 *这个文件写一些基础的函数
 by：高超
 */

#include "cocos2d.h"
#include "BaseLoad.h"
#include "UploadFile.h"
USING_NS_CC;

/*格式化字符串*/
std::string stringf(const char* format, ...);


/* 获取世界坐标系下的rect */
cocos2d::Rect calculateRectInGL(Node* pNode);

/*获取配置坐标*/
cocos2d::Vec2 assist_getConfigPos(const stuJson& Json);
cocos2d::Vec2 assist_getConfigPos(Vec2 pos);
cocos2d::Rect assist_getConfigRect(const stuJson& Json);
/*获取XML字符串*/
const char* GetXmlString(const std::string& key);

/*获取XML数字*/
//int GetXmlInt(const char *pKey);

/* 转化颜色到color3b */
Color3B convertColor(std::string strColor);

/* 转化header数据 */
std::vector<std::string> convertHeaderDataToVector(const std::map<std::string, std::string>& value);

/* 转化url数据 */
void convertUrlDataToString(std::string& strUrl, KVPairVec* value);

/* 移动距离是否有效 */
bool isMoveDistanceValid(float fDistance);

#endif //__INCLUDE_ASSIST_H__
