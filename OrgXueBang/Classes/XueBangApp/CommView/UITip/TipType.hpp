//
//  TipType.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/7.
//
//

#ifndef TipType_hpp
#define TipType_hpp

enum eTipType
{
    eTipNone = -1,
    eTipTest,
    eTipLoader,
    eTipWaiting,
    eTipSignReward,
    
    eTipBoyWithMsg, //松鼠旁边加一句话气泡
    eTipFreeCache,  //释放缓存
    eTipTurnPage,   //翻书动画
    eTipHobbySucc,  //选择偏好成功
    
    eTipCount,
};

#endif /* TipType_hpp */
