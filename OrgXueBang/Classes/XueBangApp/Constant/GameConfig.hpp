//
//  GameConfig.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/11/11.
//
//

#ifndef GameConfig_hpp
#define GameConfig_hpp

enum eZorder
{
    eZorderAboveUI  = 1,    //UI默认Load到0层
    eZorder2DLayer  = 5,    //内容展示层
    eZorderUILayer  = 10,   //UI交互层
    eZorderTipLayer = 150,   //弹出提示
    eZorderPopupLayer = 180,
    eZorderOtherLayer,
    
};

enum eLayerTag
{
    //登录相关
    eLayerLoginMain = 0,     //登录首页
    eLayerLoginRegister,     //登录注册页
    eLayerLoginConfirm,      //登录确认密码页
    eLayerLoginPersonal,     //登录个人版
    eLayerLoginSchool,       //登录校园版
    eLayerLoginClass,        //绑定班级
    eLayerLoginHobby,        //登录爱好
    eLayerLoginFindpass,     //找回密码
    eLayerLoginSign7,        //七日签到
    eLayerLoginSign7Reward,  //七日签到奖励
    
    //读书相关
    eLayerTextBook,     //教材阅读主体
    eLayerTextGuide,    //教材书导读封面
    eLayerTextBody,     //教材书内容页
    eLayerTextResult,   //教材书报告页
    
    eLayerBookMenu,           //原版阅读
    eLayerBookGude,
    eLayerBookBody,
    eLayerBookQuiz,
    eLayerBookResult,
    eLayerBookInter,    //交互面板
    eLayerBookDiction,  //单词本
    eLayerBookRecord,  //录音秀
    eLayerRecordShare,    //录音分享面板
    //任务相关
    eLayerUserTask,
    eLayerTaskHistory,
    eLayerTaskDetail,
    
    //消息相关
    eLayerUserMsg,
    
    //个人中心相关
    eLayerUserCenter,
    eLayerUserDaily20M,
    eLayerUserSet,
    eLayerUserHeads,
    eLayerUserFreecoin,
    eLayerUserInfo,
    eLayerUserWordBook,
    eLayerUserShop,
    eLayerUserScore,
    eLayerUserHobby,
    eLayerUserReport,
    eLayerUserAchieve,
    eLayerUserChangePass,
    eLayerUserJoinDuban,
    eLayerUserJoinClass,
    
    //读书笔记相关
    eLayerNoteList,     //笔记列表
    eLayerNoteQuestion, //笔记问题
    eLayerNoteEdit,     //笔记编辑
    eLayerNoteDetail,   //笔记详情
    eLayerNoteGalary,   //笔记图鉴
    
    //商城相关
    eLayerStoreMain,    //商城主界面
    eLayerStoreWheel,   //商城大转轮
    eLayerStoreExchange,//商城兑换卡牌
    eLayerStoreBuycard, //商城卡牌列表
    eLayerStoreFlowers, //商城鲜花列表
    eLayerStoreHistory, //商城兑换记录
    
    //弹窗系列
    eLayerPopupReward20M,//每天达成20分钟奖励贴纸
    eLayerUserTeach,    //用户引导
};

//书页类型
enum ePageType
{
    ePageNone = -1,
    ePageGuide,     //导读
    ePageCover,     //封面
    ePageCopy,      //版权
    ePageContent,   //书页
    ePageQStart,    //测试开始
    ePageQuziOral,  //语音测试
    ePageQuziOther, //其他测试
    ePageQEnd,      //测试结束
    ePageReport,    //报告
    ePageTest,
    
    ePageTypeCount,
};

//quiz类型
enum eQuizType
{
    eQuizNone = -1,
    eQuizOral,                  //口语题
    eQuizSentence,              //单词组句
    eQuizFillIn,                //看图填空
    eQuizListen,                //看图文本听音选择
    eQuizPicText,               //看图文本选择
    eQuizListenFill,            //听音填空
    eQuizListenSelectPic,       //听音选图
    eQuizListenSelect,          //听音选择
    eQuizPicWordDrag,           //图片单词对应
    eQuizTextSort,              //文本排序
    eQuizTextSelect,            //文本选择
    eQuizWordDrag,              //字母拖拽
    eQuizWordSelect,            //单词选择 数据是单词组句，展现是选择题
   
};

enum eLanguageType
{
    eLanguage_en = 0,		//英文
    eLanguage_zh_cn = 1,	//简体
    eLanguage_zh_hart = 2   //繁体
};

//读书模式
enum eReadMode
{
    eReadNone = -1,
    eReadAuto,
    eReadManual,
    eReadInteract,
    
    eReadModeCount,
};

//界面模式
enum ePageMode
{
    ePageNSCO = 0,
    ePageSRS,
    ePageOMR,
    ePageFC,
    
    ePageModeCount,
};

enum ePhoneState
{
    ePhoneNone,     //默认
    ePhoneClean,    //空白号码
    ePhoneCheck,    //正在查询
    ePhoneUsed,     //已注册过
    ePhoneReady,    //可用于注册
    ePhoneVerifying,//正在验证
    ePhoneVerified, //重新验证
    
};

#define MYDEF_KEY_CHINESE   "_cn"
#define MYDEF_IS_SET_CHINESE  true
//(CDataUser::getInstance()->getAddressstuInfo()->getChinese())
#define MYDEF_KEY_ENGLISH   "_en"
#define MYDEF_IS_ENGLISH   false

#define UI_ACTION_INTERVAL  500 // 毫秒

//教材截屏的存图（bookID，pageIndex）
#define TEXT_SCREEN_SHOT "net_book/%s/sc_%d.png"

#endif /* GameConfig_hpp */
