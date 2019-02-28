//
//  ConstantEnum.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/11/10.
//
//

#ifndef ConstantEnum_hpp
#define ConstantEnum_hpp

enum eGameState{
    eStateNone = -1,
    eStateLogo,             //Logo暂时没有
    eStateMain,            //书架场景
    eStateLoading,          //Loading
    eStateStart,
    eStateBook,             //图书场景
    eStateRecord,           //录音秀场景
    eStateReport,
    eStateText,             //原版阅读
    
    eStateCount,
    
};
enum eInterState
{
    eInterClose, //交互-静态关闭
    eInterManue, //交互-拖动
    eInterAuto,  //交互-自动
    eInterShow,  //交互-静态打开
};

enum eTopState
{
    eTopClose,   //Top-静态关闭
    eTopOpen,    //Top-打开动画
    eTopReady,   //Top-监听区域内
    eTopDrag,    //Top-拖动动画
    eTopHide,    //Top-隐藏动画
    eTopShow,    //Top-静态显示
};

enum eSetState
{
    eSetClose,   //设置-静态关闭
    eSetOpen,    //设置-打开动画
    eSetHide,    //设置-隐藏动画
    eSetShow,    //设置-静态显示
};

enum eVIPStatus
{
    eVIPNone = -1,
    eVIPLow,    //普通会员
    eVIPHigh,   //高级会员
};

#endif /* ConstantEnum_hpp */
