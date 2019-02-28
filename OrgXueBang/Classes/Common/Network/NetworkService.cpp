//
//  NetworkService.cpp
//  ReadingMate
//
//  Created by 杨勇 on 2017/4/14.
//
//

#include "NetworkService.hpp"

NetworkService::NetworkService()
{
    init();
}
NetworkService::~NetworkService()
{
    
}

NetworkService* NetworkService::getInstance()
{
    static NetworkService instance;
    return &instance;
}

void NetworkService::setServerAddress(string address)
{
    m_ServerAddress = address;
}

string NetworkService::getServiceUrl(eHttpType type, string otherArgs /*= ""*/)
{
    string url = m_ServerAddress;
    if(type >= eHttpType::postevent && type < eHttpType::postevent_end)
    {
        url += m_serviceMap[eHttpType::postevent];
    }
    else
    {
        url += m_serviceMap[type];
    }
    
    if (!otherArgs.empty())
    {
        url += otherArgs;
    }
    return url;
}

void NetworkService::init()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//    m_ServerAddress = "http://library-android.dubaner.com";//正式服（Android没有证书）
    m_ServerAddress = "http://172.16.18.29:7075";//内网test
#else
    m_ServerAddress = "https://library-ios.dubaner.com";//正式服（Ios有证书）
//    m_ServerAddress = "https://b-ios.dubaner.net";//正式服（Ios有证书）
    m_ServerAddress = "https://xuebang-test.dubaner.net";//外网test
    m_ServerAddress = "http://172.16.18.29:7075";//内网test
//    m_ServerAddress = "http://172.16.18.238:7075";//内网test
#endif
    
    m_serviceMap.insert(make_pair(eHttpType::check,"/api/v2/user/check/mobile"));        //手机查重
    m_serviceMap.insert(make_pair(eHttpType::create,"/api/v1/user/create"));       //验证注册
    m_serviceMap.insert(make_pair(eHttpType::login,"/api/v1/user/login"));              //登录
    m_serviceMap.insert(make_pair(eHttpType::update,"/api/v1/user/update"));       //更新用户
    m_serviceMap.insert(make_pair(eHttpType::userInfo,"/api/v1/user/info"));       //用户基本信息
//    m_serviceMap.insert(make_pair(eHttpType::sendcode,"/sms/sendcode"));    //发送验证码
    m_serviceMap.insert(make_pair(eHttpType::themescourses,"/api/v1/course/themescourses")); //主题图书列表
    m_serviceMap.insert(make_pair(eHttpType::pagelist,"/api/v2/course/page/list")); //图书页信息
    m_serviceMap.insert(make_pair(eHttpType::guest,"/api/v1/user/guest"));              //获取游客账号
//    m_serviceMap.insert(make_pair(eHttpType::homelevelbooks,"/course/homelevelbooks_v1")); //主页LEVEL图书列表
    m_serviceMap.insert(make_pair(eHttpType::banners,"/api/v1/site/banners"));     //banner活动
    m_serviceMap.insert(make_pair(eHttpType::recognize,"/api/v1/activity/recognize")); //打分接口
    
    
    m_serviceMap.insert(make_pair(eHttpType::search_review,"/course/search_v2")); //搜索图书(审书专用)
    m_serviceMap.insert(make_pair(eHttpType::search,"/api/v1/course/search")); //搜索图书
    
    m_serviceMap.insert(make_pair(eHttpType::favourite,"/api/v1/course/favourite"));    //收藏图书列表
    m_serviceMap.insert(make_pair(eHttpType::booklist_reading,"/api/v2/book/readlist"));    //读ing未读列表
    m_serviceMap.insert(make_pair(eHttpType::booklist_finish,"/api/v2/book/readlist"));    //已读未读列表
//    m_serviceMap.insert(make_pair(eHttpType::themes,"/achievements/themes"));   //主题列表
    m_serviceMap.insert(make_pair(eHttpType::addfavouritecourse,"/api/v1/course/favourite/add"));   //增加收藏
    m_serviceMap.insert(make_pair(eHttpType::removefavouritecourse,"/api/v1/course/favourite/remove"));//移除收藏
//    m_serviceMap.insert(make_pair(eHttpType::testtime,"/tools/test_time")); //连接速度测试
    m_serviceMap.insert(make_pair(eHttpType::noticelist,"/api/v1/notice/list")); //消息列表
    m_serviceMap.insert(make_pair(eHttpType::noticehint,"/api/v1/notice/hint"));   //消息提醒
    m_serviceMap.insert(make_pair(eHttpType::noticedetail,"/api/v1/notice/detail"));   //消息内容
    m_serviceMap.insert(make_pair(eHttpType::noticeupdatestatus,"/api/v1/notice/updatestatus"));   //更新阅读状态
    m_serviceMap.insert(make_pair(eHttpType::paraphrase,"/api/v2/word/search"));   //取得单词释义
//    m_serviceMap.insert(make_pair(eHttpType::getwordlist,"/word/getlist")); //提供单词列表
    m_serviceMap.insert(make_pair(eHttpType::addvocabulary,"/api/v2/book/words/one"));//新增生词
//    m_serviceMap.insert(make_pair(eHttpType::vocabularys,"/word/vocabularys")); //取单元和最新单元下单词
//    m_serviceMap.insert(make_pair(eHttpType::removevocabularys,"/word/removevocabularys"));  //删除单元生词
//    m_serviceMap.insert(make_pair(eHttpType::getnewwordlist,"/word/getwordlist"));  //取生词列表
    m_serviceMap.insert(make_pair(eHttpType::signin,"/api/v1/user/signin"));       //签到
    m_serviceMap.insert(make_pair(eHttpType::getlist,"/api/v1/task/getlist"));     //任务列表
    m_serviceMap.insert(make_pair(eHttpType::history,"/api/v1/task/history"));     //历史任务列表
    m_serviceMap.insert(make_pair(eHttpType::detail,"/api/v2/task/detail"));       //历史详情列表
    m_serviceMap.insert(make_pair(eHttpType::postevent,"/api/v3/activity/event"));        //用户行为上传  add by hyp
    m_serviceMap.insert(make_pair(eHttpType::postevent_openbook,"/api/v1/activity/event"));//用户行为上传——打开图书
    m_serviceMap.insert(make_pair(eHttpType::postevent_closebook,"/api/v1/activity/event/end"));//用户行为上传——关闭图书
    m_serviceMap.insert(make_pair(eHttpType::topics,"/api/v1/achievements/topics"));//24个话题列表
    m_serviceMap.insert(make_pair(eHttpType::addusertopic,"/api/v1/achievements/user/topic/add"));//增加用户偏好话题
    m_serviceMap.insert(make_pair(eHttpType::removeusertopic,"/api/v1/achievements/user/topic/remove"));//移除用户偏好话题
    m_serviceMap.insert(make_pair(eHttpType::updatepic,"/api/v1/user/update/pic")); //更新用户头像
    m_serviceMap.insert(make_pair(eHttpType::calscores,"/api/v1/activity/score/normal")); //用户读书报告
    m_serviceMap.insert(make_pair(eHttpType::caltaskscores,"/api/v1/activity/score/task"));//任务读书报告
    m_serviceMap.insert(make_pair(eHttpType::calendar,"/report/calendar"));     //用户20分钟记录
    m_serviceMap.insert(make_pair(eHttpType::stickers,"/api/v1/report/stickers")); //用户贴纸概况
    m_serviceMap.insert(make_pair(eHttpType::bindstickers,"/api/v1/user/bind/stickers"));     //用户手动贴纸
    m_serviceMap.insert(make_pair(eHttpType::usermedal,"/api/v1/achievements/user/medal/second")); //查询用户4个奖章指标
    m_serviceMap.insert(make_pair(eHttpType::classlist,"/api/v1/school/classlist")); //根据手机号取得教师班级信息
    m_serviceMap.insert(make_pair(eHttpType::applyclass,"/api/v1/school/applyclass_v2"));//申请换班
    m_serviceMap.insert(make_pair(eHttpType::leaveclass,"/api/v1/school/leaveclass"));//学生退班
    m_serviceMap.insert(make_pair(eHttpType::dubanclass,"/api/v1/school/dubanclass"));//读伴班级信息
    m_serviceMap.insert(make_pair(eHttpType::freecoin,"/api/v1/achievements/free/coins"));//免费宝石
    m_serviceMap.insert(make_pair(eHttpType::aboutus,"/api/v1/site/about"));//关于我们
    m_serviceMap.insert(make_pair(eHttpType::feedback,"/api/v1/site/feedback"));//意见反馈
    m_serviceMap.insert(make_pair(eHttpType::rechargeproducts,"/api/v1/trade/recharge/products"));//充值产品列表
    m_serviceMap.insert(make_pair(eHttpType::getorderid,"/api/v1/trade/order/create"));//生成订单
    m_serviceMap.insert(make_pair(eHttpType::rechargelist,"/trade/rechargelist"));//用户充值记录
//    m_serviceMap.insert(make_pair(eHttpType::postdevicetoken,"/tools/postdevicetoken"));  //上传用户设备token
    m_serviceMap.insert(make_pair(eHttpType::createcommon,"/api/v1/user/create/common"));//创建普通用户
    m_serviceMap.insert(make_pair(eHttpType::updatepassword,"/api/v1/user/update/password"));//修改密码
    m_serviceMap.insert(make_pair(eHttpType::mall,"/mall?%s"));//商城
    
    //二期
    m_serviceMap.insert(make_pair(eHttpType::textbookpagelist,"/course/textbookpagelist"));//取得教材图书所有页信息
    m_serviceMap.insert(make_pair(eHttpType::textbookpagelist2,"/course/textbookpagelist"));//取得教材图书所有页信息(读书笔记加载图鉴专用)
    m_serviceMap.insert(make_pair(eHttpType::hometextbooks,"/course/hometextbooks"));//主页教材图书列表
    m_serviceMap.insert(make_pair(eHttpType::textbooklist,"/course/textbooklist"));//根据条件取得教材图书列表
//    m_serviceMap.insert(make_pair(eHttpType::myquestionslist,"/questions/myquestionslist"));//用户考试列表
//    m_serviceMap.insert(make_pair(eHttpType::postquestionslist,"/questions/postquestionslist"));//获取试题
//    m_serviceMap.insert(make_pair(eHttpType::questionhint,"/questions/hint"));//获取考试提醒
    m_serviceMap.insert(make_pair(eHttpType::usercertificate,"/api/v1/achievements/user/certificate"));//获取成就数据
    
    m_serviceMap.insert(make_pair(eHttpType::cardlist,"/api/v1/card/list"));//卡牌列表
    m_serviceMap.insert(make_pair(eHttpType::cardlistdetail,"/api/v1/card/list/detail"));//卡牌详情列表
    m_serviceMap.insert(make_pair(eHttpType::cardexchange,"/api/v1/card/exchange"));//兑换卡牌
    m_serviceMap.insert(make_pair(eHttpType::cardupdatedetail,"/api/v1/card/update/detail"));//更新用户卡牌使用状态
    m_serviceMap.insert(make_pair(eHttpType::cardbossprompt,"/api/v1/card/boss/prompt"));//boss卡牌提示
    m_serviceMap.insert(make_pair(eHttpType::topiclevelbooks,"/api/v1/card/topic/level/books_v1"));//卡牌话题图书列表
    m_serviceMap.insert(make_pair(eHttpType::cardrewards,"/api/v1/card/rewards"));//卡牌领取奖励
    m_serviceMap.insert(make_pair(eHttpType::myboxlist,"/api/v1/box/myboxlist"));//我的宝箱
    m_serviceMap.insert(make_pair(eHttpType::openbox,"/api/v1/box/openbox"));//打开宝箱
    m_serviceMap.insert(make_pair(eHttpType::savecards,"/api/v1/box/savecards"));//卡牌收入卡包
    
    m_serviceMap.insert(make_pair(eHttpType::notegetlist,"/api/v1/note/list"));//用户阅读笔记列表
//    m_serviceMap.insert(make_pair(eHttpType::notegetdetail,"/note/getdetail"));//用户阅读笔记详情
    m_serviceMap.insert(make_pair(eHttpType::notesave,"/api/v1/note/save"));//保存阅读笔记
    m_serviceMap.insert(make_pair(eHttpType::notequestions,"/api/v1/note/questions"));//阅读笔记问题列表
    m_serviceMap.insert(make_pair(eHttpType::notedelete,"/api/v1/note/delete"));//删除阅读笔记
    
    m_serviceMap.insert(make_pair(eHttpType::mallwheel,"/mall/wheel"));//抽奖大转轮信息
    m_serviceMap.insert(make_pair(eHttpType::mallproductlist,"/mall/productlist"));//通用商城产品列表
    m_serviceMap.insert(make_pair(eHttpType::mallrecharge,"/mall/recharge"));//兑换Q币话费
    m_serviceMap.insert(make_pair(eHttpType::mallrechargeflower,"/mall/rechargeflower"));//兑换鲜花
    
    m_serviceMap.insert(make_pair(eHttpType::flowerranking,"/mall/flowerranking"));//学校教师鲜花榜
    m_serviceMap.insert(make_pair(eHttpType::mallluckdraw,"/mall/luckdraw"));//用户大转轮抽奖
    m_serviceMap.insert(make_pair(eHttpType::luckdrawmessage,"/mall/luckdrawmessage"));//转轮中奖信息滚动
    m_serviceMap.insert(make_pair(eHttpType::exchangeRecord,"/mall/exchangeRecord"));//宝石兑换记录
    
//    m_serviceMap.insert(make_pair(eHttpType::myWordUnitList,"/word/myWordUnitList"));//获取生词游戏单元列表
//    m_serviceMap.insert(make_pair(eHttpType::getnewwordlist1,"/word/getwordlist")); //取生词列表
//    m_serviceMap.insert(make_pair(eHttpType::getgradewordunit,"/word/getunit")); //单词unit列表
//    m_serviceMap.insert(make_pair(eHttpType::getgradewordlist,"/word/getwordgamesone")); //取游戏一10个单词列表
//    m_serviceMap.insert(make_pair(eHttpType::submitscoreword,"/word/submitscoreword")); //单词本提交打分
//    m_serviceMap.insert(make_pair(eHttpType::submitscorenewword,"/word/submitscorenewword")); //生词提交打分
    m_serviceMap.insert(make_pair(eHttpType::getgoodsorder,"/mall/getgoodsorder")); //生成实物订单
    m_serviceMap.insert(make_pair(eHttpType::festivalcard,"/api/v1/achievements/festival/card")); //节日卡牌
    
    m_serviceMap.insert(make_pair(eHttpType::create_share_url,"/api/v1/share/url/create")); //创建分享链接
    
    m_serviceMap.insert(make_pair(eHttpType::getwxpayorder,"/api/v1/trade/wechat/order")); //微信预订单取得
    m_serviceMap.insert(make_pair(eHttpType::dealwxpay,"/api/v1/trade/deal/wechat/pay")); //微信支付后续处理
    m_serviceMap.insert(make_pair(eHttpType::getalipayorder,"/api/v1/trade/alipay/order")); //支付宝预订单取得
    m_serviceMap.insert(make_pair(eHttpType::dealalipay,"/api/v1/trade/deal/alipay")); //支付宝支付后续处理

    m_serviceMap.insert(make_pair(eHttpType::newclasslist,"/api/v1/school/newclasslist"));//取得教师班级列表
    
    m_serviceMap.insert(make_pair(eHttpType::wordAchieve,"/api/v2/word/achievement"));//单词成就
    
    m_serviceMap.insert(make_pair(eHttpType::shareBox,"/api/v1/share/box"));//分享获得宝箱
    m_serviceMap.insert(make_pair(eHttpType::uploadLog,"/note/up_log"));//上传前端log日志
    
    m_serviceMap.insert(make_pair(eHttpType::seerfree,"/api/v1/notice/seerfreeupdatestatus"));//弹出西禾免费课提示对话框
    
    m_serviceMap.insert(make_pair(eHttpType::videoSeriesList,"/api/v2/video/series/list"));//录播课视频系列列表
    m_serviceMap.insert(make_pair(eHttpType::videoSeriesDetail,"/api/v2/video/series"));//录播课视频系列详情
    m_serviceMap.insert(make_pair(eHttpType::videoDetail,"/api/v2/video/detail"));//录播课视频详情
    
    m_serviceMap.insert(make_pair(eHttpType::getNewWordsV2,"/api/v2/book/words"));//新获取生词
    m_serviceMap.insert(make_pair(eHttpType::getNewWordsReportV2,"/api/v2/book/game/poster"));//新生词报告
    
    m_serviceMap.insert(make_pair(eHttpType::uploadBookRecord,"/api/v2/reading/audio/upload"));//上传用户导读页分享的音频
    m_serviceMap.insert(make_pair(eHttpType::record_share_url,"/api/v2/reading/share"));//请求录音分享的链接
    
    m_serviceMap.insert(make_pair(eHttpType::getNewWordsGameRule,"/api/v2/wgame/rules"));//获取生词游戏规则数据
    m_serviceMap.insert(make_pair(eHttpType::getNewWordsGameStart,"/api/v2/wgame/start"));//获取生词游戏开始数据
    m_serviceMap.insert(make_pair(eHttpType::getNewWordsGameEnd,"/api/v2/wgame/settlement"));//获取生词游戏结束数据
    
    m_serviceMap.insert(make_pair(eHttpType::getGemStoreProducts,"/api/v2/trade/coin/getproducts"));    //获取宝石商店数据
    m_serviceMap.insert(make_pair(eHttpType::preApplePayOrder,"/api/v2/trade/coin/applepayorder"));     //苹果内购预下单
    m_serviceMap.insert(make_pair(eHttpType::preWXPayOrder,"/api/v2/trade/coin/wxpayorder"));           //微信预下单
    m_serviceMap.insert(make_pair(eHttpType::preAliPayOrder,"/api/v2/trade/coin/alipayorder"));         //支付宝预下单
    m_serviceMap.insert(make_pair(eHttpType::queryPayStatus,"/api/v2/trade/coin/paystatus"));           //订单状态查询
    
    
    
    m_serviceMap.insert(make_pair(eHttpType::get_school_grade,"/api/v2/school/grade"));           //获取年级
    m_serviceMap.insert(make_pair(eHttpType::set_school_grade,"/api/v2/school/grade"));           //更新年级
    
    m_serviceMap.insert(make_pair(eHttpType::getWordClassList,"/api/v3/curriculum/list")); //获取课程列表 新接口
    
    m_serviceMap.insert(make_pair(eHttpType::getOneWordClass,"/api/v2/curriculum/detail"));  //获取某课程详情
    m_serviceMap.insert(make_pair(eHttpType::getBookRecommend,"/api/v2/book/recommend"));  //获取推荐课程   add by hyp
    m_serviceMap.insert(make_pair(eHttpType::getMyCourse,"/api/v2/curriculum/getlist/pay_v2"));  //得到我的课程 add by hyp
    m_serviceMap.insert(make_pair(eHttpType::getCourseList,"/api/v2/curriculum/getlist"));  //得到课程列表 add by hyp
//    m_serviceMap.insert(make_pair(eHttpType::getBooklist,"/course/booklist_v1")); //根据条件取得图书列表 add by hyp
    m_serviceMap.insert(make_pair(eHttpType::getBooklist_v2,"/api/v2/book/more")); //根据条件取得图书列表 add by hyp
    m_serviceMap.insert(make_pair(eHttpType::getTasklist,"/api/v2/task/getlist_v2"));     //任务列表 add by hyp
    m_serviceMap.insert(make_pair(eHttpType::getHomePageData,"/api/v2/home/page_v2"));     //首页整合数据 add by hyp
    m_serviceMap.insert(make_pair(eHttpType::getBanners,"/api/v2/site/banners"));     //banner活动 add by hyp
    m_serviceMap.insert(make_pair(eHttpType::themes,"/api/v2/achievements/themes"));   //主题列表  add by hyp
    m_serviceMap.insert(make_pair(eHttpType::getActivity,"/api/v2/discovery/"));   //发现活动  add by hyp
    m_serviceMap.insert(make_pair(eHttpType::commentAudio,"/api/v2/discovery/comment/audio"));   //发现语音评论  add by hyp
    m_serviceMap.insert(make_pair(eHttpType::commentText,"/api/v2/discovery/comment/text"));   //发现文字评论  add by hyp
    m_serviceMap.insert(make_pair(eHttpType::getActivityPay,"/api/v2/discovery/applepayorder"));   //宝箱请求订单  add by hyp
    m_serviceMap.insert(make_pair(eHttpType::getThirdPartyPayOrder,"/api/v2/trade/payorder/android"));   //android统一下单  add by hyp
    m_serviceMap.insert(make_pair(eHttpType::getMachineQuestion,"/api/v2/quality/questions"));   //人机练习  add by hyp
    m_serviceMap.insert(make_pair(eHttpType::uploadMachineAnswer,"/api/v2/quality/questions/answer"));   //人机练习答案上传  add by hyp
    m_serviceMap.insert(make_pair(eHttpType::getMachineNo1,"/api/v2/quality/rank/first"));   //人机练习得到第一名信息  add by hyp
    
    m_serviceMap.insert(make_pair(eHttpType::getOneWordClassDetail,"/api/v2/curriculum/stage/detail"));  //获取某课程某阶段详情
    m_serviceMap.insert(make_pair(eHttpType::getOneWordLessonDetail,"/api/v2/curriculum/lesson/detail"));  //获取某课程某阶段详情
    m_serviceMap.insert(make_pair(eHttpType::sendOneWordLessonProc,"/api/v2/curriculum/lesson/progress"));  //上报单元进度
    m_serviceMap.insert(make_pair(eHttpType::sendOneWordLessonGameStar,"/api/v2/curriculum/lesson/wgame/start"));  //单元单词游戏开始
    m_serviceMap.insert(make_pair(eHttpType::sendOneWordLessonGameAward,"/api/v2/curriculum/lesson/wgame/awards"));  //单元单词游戏奖励（戳泡泡）
    m_serviceMap.insert(make_pair(eHttpType::wordClassPreApplePayOrder,"/api/v2/curriculum/word/applepayorder"));  //单词课苹果内购预下单
    m_serviceMap.insert(make_pair(eHttpType::wordClassPreWXPayOrder,"/api/v2/curriculum/word/wxpayorder"));  //单词课微信预下单
    m_serviceMap.insert(make_pair(eHttpType::wordClassPreAliPayOrder,"/api/v2/curriculum/word/alipayorder"));  //单词课支付宝预下单
    m_serviceMap.insert(make_pair(eHttpType::wordClassQueryPayStatus,"/api/v2/trade/paystatus"));  //单词课订单状态查询
    m_serviceMap.insert(make_pair(eHttpType::commonApplePayCheck,"/api/v2/trade/applepaycheck"));  //通用苹果支付后验证   add by hyp
    
    
    m_serviceMap.insert(make_pair(eHttpType::getWordPayList,"/api/v2/curriculum/getlist/pay_v2")); //获取购买课程列表

    m_serviceMap.insert(make_pair(eHttpType::readingindex,"/api/v1/achievements/reading/index"));  //学习成就3个指标
    
    m_serviceMap.insert(make_pair(eHttpType::testingStart,"/api/v2/testing/start"));    //开始定级评测
    m_serviceMap.insert(make_pair(eHttpType::testingEnd,"/api/v2/testing/end"));        //定级评测普通题型结果上传
    m_serviceMap.insert(make_pair(eHttpType::testingRecognize,"/api/v2/testing/recognize"));      //定级评测语音题打分接口
    m_serviceMap.insert(make_pair(eHttpType::testingEndAudio,"/api/v2/testing/end/audio"));      //定级评测语音题型结果上传
    
    m_serviceMap.insert(make_pair(eHttpType::testingCacheExist,"/api/v2/testing/cache/exist"));      //检查用户是否有缓存的记录
    m_serviceMap.insert(make_pair(eHttpType::testingStartCache,"/api/v2/testing/start/cache"));      //开始定级评测-检查缓存版本
    m_serviceMap.insert(make_pair(eHttpType::testingCacheDel,"/api/v2/testing/cache/del"));      //删除用户缓存的记录

    m_serviceMap.insert(make_pair(eHttpType::testingRecords,"/api/v2/testing/records"));      //定级评测历史列表
    m_serviceMap.insert(make_pair(eHttpType::testingPoster,"/api/v2/testing/poster"));      //定级评测报表接口

    m_serviceMap.insert(make_pair(eHttpType::getDiscovery,"/api/v3/site/home/pop"));      //起屏页活动
    m_serviceMap.insert(make_pair(eHttpType::getDiscoveryList,"/api/v2/discovery/"));      //发现活动列表
    
    m_serviceMap.insert(make_pair(eHttpType::sendPushMsgRegId,"/api/push/user/reg"));     //发送pushu消息 注册regid
    m_serviceMap.insert(make_pair(eHttpType::sendPushMsgClick,"/api/push/record/status"));     //发送pushu消息 点击某个消息打开应用
    
//    m_serviceMap.insert(make_pair(eHttpType::setUserAddress,"/api/v2/address/create")); //order 设置收货地址
    m_serviceMap.insert(make_pair(eHttpType::setUserAddress,"/api/v2/address/modify")); //order 设置收货地址
    m_serviceMap.insert(make_pair(eHttpType::getUserAddress,"/api/v2/address/user")); //order 获取收货地址
    
    m_serviceMap.insert(make_pair(eHttpType::getClassDay3,"/api/v2/curriculum/quality/info/3"));              //3天课程数据
    m_serviceMap.insert(make_pair(eHttpType::getClassDayStage,"/api/v2/curriculum/quality/stage/list"));         //x课程阶段
    m_serviceMap.insert(make_pair(eHttpType::getClassDayStageDetail,"/api/v2/curriculum/quality/info"));   //x课程阶段详情
    m_serviceMap.insert(make_pair(eHttpType::getClassDayDaily,"/api/v2/curriculum/quality/daily"));   //获取课程某天信息
    m_serviceMap.insert(make_pair(eHttpType::getClassHomework,"/api/v2/curriculum/quality/homework/info"));   //获取作业信息
    m_serviceMap.insert(make_pair(eHttpType::postClassHomework,"/api/v2/curriculum/quality/homework/answer/upload"));   //上传作业
    m_serviceMap.insert(make_pair(eHttpType::getClassAnswerShare,"/api/v2/curriculum/quality/answer/share"));   //作业分享
    m_serviceMap.insert(make_pair(eHttpType::getClassRankings,"/api/v2/quality/rank/class"));   //课程总排行
    m_serviceMap.insert(make_pair(eHttpType::getClassDailyRankings,"/api/v2/quality/rank/class/day"));   //课程日排行
    
    m_serviceMap.insert(make_pair(eHttpType::getLiveInfo,"/api/v2/curriculum/quality/live/info")); //直播详情
    m_serviceMap.insert(make_pair(eHttpType::getGuides,"/api/v2/quality/guides/list/first")); //精读课-导学课题目列表
    m_serviceMap.insert(make_pair(eHttpType::getGuidesEnd,"/api/v2/quality/guides/end")); //精读课-导学课完结记录
    m_serviceMap.insert(make_pair(eHttpType::setGuidesStep,"/api/v2/quality/guides/step")); //精读课-导学课 上传学习记录
    
    m_serviceMap.insert(make_pair(eHttpType::getOneWordClassDetail_v1,"/api/v2/curriculum/stage/detail_v1"));  //获取某课程某阶段详情v1
    
    m_serviceMap.insert(make_pair(eHttpType::getSchoolNameByCode,"/api/v2/user/school"));
    m_serviceMap.insert(make_pair(eHttpType::regUserBySchoolCode,"/api/v2/user/create/school"));
    
    m_serviceMap.insert(make_pair(eHttpType::setSchoolCode,"/api/v2/user/school/add"));  //设置学校码
    m_serviceMap.insert(make_pair(eHttpType::getSchoolCodeName,"/api/v2/user/school"));  //获取学校码对应的学校名称

    m_serviceMap.insert(make_pair(eHttpType::getAclassQRcode,"/api/v2/curriculum/quality/class/qrcode")); //获取21天课程群二维码 add by zzl
    
    m_serviceMap.insert(make_pair(eHttpType::rebuy,"/api/v2/trade/card/recharge"));                 //充值卡 add by zzl
    m_serviceMap.insert(make_pair(eHttpType::rebuyHistory,"/api/v2/trade/card/recharge/list"));       //充值卡历史记录 add by zzl
    
    m_serviceMap.insert(make_pair(eHttpType::getBranchSchoolList,"/api/v2/school/branch"));
    m_serviceMap.insert(make_pair(eHttpType::setBranchSchoolID,"/api/v2/school/new"));
    m_serviceMap.insert(make_pair(eHttpType::classListV2,"/api/v2/school/class"));
    
    m_serviceMap.insert(make_pair(eHttpType::sendSmsCode,"/api/v1/sms/send/code"));         //发送短信验证码
    m_serviceMap.insert(make_pair(eHttpType::changeMobile,"/api/v2/user/mobile/change"));   //用户修改手机号码

    //读霸1000
    m_serviceMap.insert(make_pair(eHttpType::readKingReward,"/api/v3/quality/readking/reward"));
    m_serviceMap.insert(make_pair(eHttpType::readKingReport,"/api/v3/quality/readking/report"));
    m_serviceMap.insert(make_pair(eHttpType::readingKingCourseDay,"/api/v3/curriculum/quality/daily"));
    m_serviceMap.insert(make_pair(eHttpType::readingKingWordExplain,"/api/v3/quality/word/list"));                 //获取读霸1000单词讲解,随机
    m_serviceMap.insert(make_pair(eHttpType::readingKingReadEnd,"/api/v3/quality/readking/read/end"));
    m_serviceMap.insert(make_pair(eHttpType::readkingStageDetail,"/api/v3/quality/readking/info"));     //读霸1000阶段详情数据
    
    m_serviceMap.insert(make_pair(eHttpType::readkingWordPractice,"/api/v3/quality/word/questions"));     //单词练习
    m_serviceMap.insert(make_pair(eHttpType::readkingAnswer,"/api/v3/quality/word/questions/answer"));     //上传答案
    m_serviceMap.insert(make_pair(eHttpType::readkingSmallReview,"/api/v3/quality/review/small"));     //大复习
    m_serviceMap.insert(make_pair(eHttpType::readkingBigReview,"/api/v3/quality/review/big"));     //小复习
    m_serviceMap.insert(make_pair(eHttpType::readKingWordBook,"/api/v3/quality/readking/stage/words"));     //读霸1000单词本
    m_serviceMap.insert(make_pair(eHttpType::readingKingWordExplainEnd,"/api/v3/quality/word/end"));     //读霸1000单词讲解完结
    
    m_serviceMap.insert(make_pair(eHttpType::homePageBookList,"/api/v1/homepage_booklist"));
    
}
