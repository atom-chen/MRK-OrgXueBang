//
//  HttpTag.h
//  OrgXueBang
//
//  Created by liuwei on 2019/1/8.
//

#ifndef HttpTag_h
#define HttpTag_h

enum class eHttpType
{
    none = 0,
    check,          //手机查重
    create,         //验证注册
    login,          //登录
    update,         //更新用户
    userInfo,       //用户基本信息
    sendcode,       //发送验证码
    themescourses,       //主题图书列表
    pagelist,       //图书页信息
    guest,          //获取游客账号
    homelevelbooks, //主页LEVEL图书列表
    banners,        //banner活动
    recognize,      //打分接口
    search,         //搜索图书
    search_review,  //搜索图书（审书专用）
    favourite,      //收藏图书列表
    booklist_reading,   //正在读列表
    booklist_finish,    //读取完成列表
    themes,         //主题列表
    addfavouritecourse,     //增加收藏@yy
    removefavouritecourse,  //移除收藏@yy
    testtime,               //连接速度测试
    
    noticehint,     //消息提醒
    noticelist,     //获取消息
    noticedetail,   //消息内容
    noticeupdatestatus,    //更新阅读状态
    paraphrase,     //取得单词释义
    getwordlist,    //提供单词列表
    addvocabulary,  //新增生词
    vocabularys,    //取单元和最新单元下单词
    removevocabularys,  //删除单元生词
    getnewwordlist,    //取生词列表
    
    signin,         //签到
    getlist,        //任务列表
    history,        //历史任务列表
    detail,         //历史详情列表
    
    mallluckdraw,   //用户大转轮抽奖(不弹青蛙，后台请求)
    postevent,      //用户行为上传
    postevent_end = 9999999,  //日志结束
    postevent_openbook,      //用户行为上传——打开图书，开书日志，单独监听，需要拿到用户读这本书的event_id
    postevent_closebook,      //用户行为上传——关闭图书,仅用于原版书关书日志，完整关书，才能在报告页有成绩，奖卡牌
    topics,         //24个话题列表
    addusertopic,   //增加用户偏好话题
    removeusertopic,//移除用户偏好话题
    updatepic,      //更新用户头像
    calscores,      //用户读书报告
    caltaskscores,  //任务读书报告
    calendar,       //用户20分钟记录
    stickers,       //用户贴纸概况
    bindstickers,   //用户手动贴纸
    
    usermedal,      //查询用户4个奖章指标
    classlist,      //根据手机号取得教师班级信息
    applyclass,     //申请换班
    leaveclass,     //学生退班
    dubanclass,     //读伴班级信息
    freecoin,       //免费宝石
    aboutus,        //关于我们
    feedback,       //意见反馈
    rechargeproducts,   //充值产品列表
    getorderid,     //生成订单
    rechargelist,   //用户充值记录
    postdevicetoken,//上传用户设备token
    createcommon,   //创建普通用户
    updatepassword, //修改密码
    mall,           //商城
    flowerranking,  //学校教师鲜花榜
    
    
    //二期
    textbookpagelist,//取得教材图书所有页信息
    textbookpagelist2,//取得教材图书所有页信息(读书笔记加载图鉴专用)
    hometextbooks,  //主页教材图书列表
    textbooklist,   //根据条件取得教材图书列表
    myquestionslist,    //用户考试列表
    postquestionslist,  //获取试题
    questionhint,   //获取考试提醒
    usercertificate,//用户成就
    
    cardlist,       //卡牌列表
    cardlistdetail, //卡牌详情列表
    cardexchange,   //兑换卡牌
    cardupdatedetail,   //更新用户卡牌使用状态
    cardbossprompt,   //boss卡牌提示
    topiclevelbooks,   //卡牌话题图书列表
    cardrewards,   //卡牌领取奖励
    myboxlist,      //我的宝箱
    openbox,        //打开宝箱
    savecards,      //卡牌收入卡包
    
    notegetlist,    //用户阅读笔记列表
    notegetdetail,  //用户阅读笔记详情
    notesave,       //保存阅读笔记
    notequestions,  //阅读笔记问题列表
    notedelete,     //删除阅读笔记
    
    mallwheel,      //抽奖大转轮信息
    luckdrawmessage,//转轮跑马灯消息
    mallproductlist,//通用商城产品列表
    mallrechargeflower,//兑换鲜花
    mallrecharge,   //兑换Q币话费
    exchangeRecord, //宝石兑换记录
    getgoodsorder,  //生成实物订单
    
    myWordUnitList, //获取生词游戏单元列表
    getnewwordlist1,    //取生词列表
    getgradewordunit,   //单词unit列表
    getgradewordlist,   //取游戏一10个单词列表
    submitscoreword,    //单词本提交打分
    submitscorenewword, //生词提交打分
    
    festivalcard,//节日卡牌
    
    create_share_url,   //创建分享链接
    
    getwxpayorder,      //微信预订单取得
    dealwxpay,          //微信支付后续处理
    getalipayorder,     //支付宝预订单取得
    dealalipay,         //支付宝支付后续处理
    newclasslist,       //取得教师班级列表
    
    wordAchieve,        //单词成就
    
    shareBox,           //分享获得宝箱
    uploadLog,          //上传log
    
    seerfree,           //弹出西禾免费课提示对话框
    
    videoSeriesList,    //录播课视频系列列表
    videoSeriesDetail,  //录播课视频系列详情
    videoDetail,        //录播课视频详情
    
    getNewWordsV2,      //新获取生词
    getNewWordsReportV2, //新生词报告
    
    uploadBookRecord,          //上传音频
    record_share_url,    //录音分享链接
    
    getNewWordsGameRule, //获取生词游戏规则数据
    getNewWordsGameStart, //获取生词游戏开始数据
    getNewWordsGameEnd, //获取生词游戏结束数据
    
    getGemStoreProducts,    //获取宝石充值产品列表
    preApplePayOrder,       //苹果内购预下单
    preWXPayOrder,          //微信预下单
    preAliPayOrder,         //支付宝预下单
    queryPayStatus,         //订单状态查询
    
    get_school_grade,
    set_school_grade,
    
    getWordClassList,   //获取课程列表
    getOneWordClass,    //获取某课程详情
    
    getBookRecommend,//得到图书推荐 add by hyp
    getMyCourse,//得到我的课程 add by hyp
    getCourseList,//得到课程列表 add by hyp
    getBooklist,       //根据条件取得图书列表 add by hyp php
    getBooklist_v2,       //根据条件取得图书列表 add by hyp  go
    getTasklist,        //任务列表 add by hyp
    getHomePageData, //首页整合接口 add by hyp
    getBanners,        //banner活动 add by hyp
    getActivity,    //得到发现活动列表 add by hyp
    commentAudio,   //活动语音评论 add by hyp
    commentText,    //活动文字评论 add by hyp
    getActivityPay,//发现请求订单 add by hyp
    getThirdPartyPayOrder,//android统一下单接口 add by hyp
    getMachineQuestion,//人机练习 add by hyp
    uploadMachineAnswer,//人机练习 add by hyp
    getMachineNo1,//人机练习得到第一名信息 add by hyp
    
    
    
    getOneWordClassDetail, //获取某课程某阶段详情
    getOneWordLessonDetail, //获取某课程某阶段具体详情
    sendOneWordLessonProc,  //上报单元进度
    sendOneWordLessonGameStar,  //单元单词游戏开始
    sendOneWordLessonGameAward,  //单元单词游戏奖励（戳泡泡）
    wordClassPreApplePayOrder,       //单词课苹果内购预下单
    wordClassPreWXPayOrder,          //单词课微信预下单
    wordClassPreAliPayOrder,         //单词课支付宝预下单
    wordClassQueryPayStatus,         //单词课订单状态查询
    commonApplePayCheck,          //单词课苹果支付后验证
    
    getWordPayList,   //获取课程购买列表
    
    readingindex,           //学习成就3个指标
    
    testingStart,           //开始定级评测
    testingEnd,           //定级评测普通题型结果上传
    testingRecognize,          //定级评测语音题打分接口
    testingEndAudio,          //定级评测语音题型结果上传
    
    testingCacheExist,          //检查用户是否有缓存的记录
    testingStartCache,          //开始定级评测-检查缓存版本
    testingCacheDel,          //删除用户缓存的记录
    
    testingRecords,         //定级评测历史列表
    testingPoster,          //定级评测报表接口
    
    getDiscovery,       //起屏页活动
    getDiscoveryList,   //活动列表
    
    sendPushMsgRegId,       //发送pushu消息 注册regid
    sendPushMsgClick,       //发送pushu消息 点击某个消息打开应用
    
    setUserAddress,     //order 设置收货地址
    getUserAddress,     //order 获取收货地址
    
    getClassDay3,                //3天课程数据
    getClassDayStage,           //x课程阶段
    getClassDayStageDetail,     //x课程阶段详情
    getClassDayDaily,           //获取课程某天信息
    getClassHomework,           //获取作业信息
    postClassHomework,          //上传作业
    getClassAnswerShare,        //作业分享
    getClassRankings,           //课程总排行
    getClassDailyRankings,      //课程日排行
    
    getGuides,                  //精读课-导学课题目列表
    getGuidesEnd,                  //精读课-导学课完结记录
    setGuidesStep,                  //精读课-导学课 上传学习记录
    getLiveInfo,                //直播详情
    
    getOneWordClassDetail_v1, //获取某课程某阶段详情v1
    
    setSchoolCode,              //设置学校码
    getSchoolCodeName,          //获取学校码对应的学校名称
    regUserBySchoolCode,        //学校码注册用户
    getSchoolNameByCode,        //获取学校名称
    
    getAclassQRcode,        //获取21天课程群二维码 add by zzl
    
    rebuy,                  //充值卡 add by zzl
    rebuyHistory,           //充值卡历史记录 add by zzl
    
    getBranchSchoolList,//获取分校列表
    setBranchSchoolID,//设置分校ID
    classListV2,//班级列表 通过教师手机号OR学校ID
    
    sendSmsCode,       //发送短信验证码
    changeMobile,       //用户修改手机号码
    
    readKingReward,      //获取读霸1000,宝石奖励
    readKingReport,       //获取读霸1000打卡报告
    readingKingCourseDay,//获取课程某天信息
    readingKingReadEnd,//读霸1000，完成阅读
    readingKingWordExplain,        //获取读霸1000单词讲解,随机
    readkingStageDetail,    //读霸1000阶段详情数据 add by zzl
    readkingWordPractice,   //读霸1000练习
    readkingAnswer,         //读霸1000上传
    readkingSmallReview,    //小复习
    readkingBigReview,      //大复习
    readKingWordBook,   //读霸1000单词本
    readingKingWordExplainEnd,     //读霸1000单词讲解完结
    
    homePageBookList, //首页书单
};

#endif /* HttpTag_h */
