//
//  CostTime.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/10/18.
//
//

#ifndef CostTime_hpp
#define CostTime_hpp

#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
#define COST_TIME_START         do {} while (0);
#define COST_TIME_END(tag)      do {} while (0);
#define COST_TIME_TAG(tag,tip)  do {} while (0);
#else
#define COST_TIME_START CostTime_Start();
#define COST_TIME_END(tag) CostTime_End(tag);
#define COST_TIME_TAG(tag,tip) CostTime_Tag(tag,tip);
#endif

extern void CostTime_Start();
extern void CostTime_End(const char* express);

extern void CostTime_Tag(const char* tag, const char* tip = "");

#endif /* CostTime_hpp */
