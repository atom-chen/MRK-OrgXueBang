//
//  UIDef.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/11/11.
//
//

#ifndef UIDef_hpp
#define UIDef_hpp

//图片

#define ui_b_play_png   "ui_b_play1.png"
#define ui_b_play1_png   "ui_b_play1.png"
#define ui_b_play2_png   "ui_b_play2.png"
#define ui_b_sound_png  "ui_b_sound.png"

#define ui_box_back_png "ui_box_back.png"
#define ui_pause_back_png  "ui_pause_back.png"
#define ui_b_go_png     "ui_b_go.png"
#define ui_b_home_png   "ui_b_home.png"
#define ui_b_next_png   "ui_b_next.png"
#define ui_b_pause_png  "ui_b_pause.png"
#define ui_b_back_png  "ui_back.png"

#define ui_l_right_png  "ui_l_right.png"
#define ui_l_wrong_png  "ui_l_wrong.png"
#define ui_b_right_png  "ui_b_right.png"
#define ui_b_wrong_png  "ui_b_wrong.png"


#define ui_record_png   "ui_record.png"
#define ui_music_png    "ui_music.png"

//字体路径
#define MYDEF_TTF_ARIAL  "Font/arial.ttf"
#define FONT_NAME  "Font/arial.ttf"
#define MYDEF_TTF_WORD   "Font/Comic Sans MS Bold.ttf"
#define MYDEF_TTF_CHINESE  "Font/WDTTianniu.ttf"
#define MYDEF_TTF_GILLSANS  "Font/GillSans.ttc"
#define MYDEF_TTF_TIANNIU  "Font/WDTTianniu.ttf"
#define MYDEF_TTF_WDT_TIANNIU "Font/WDTTianniu.ttf"
//XML
#define MYDEF_XMLFILE "config/others_res.xml"

#define COLOR_YELLOW    Color3B(230, 176, 35)

#define MSG_TURN_PAGE           "TurnPage"
#define MSG_BOOK_RELOAD         "reloadOnePage"
#define MSG_UPDATE_NUT          "showQuizNut"
#define MSG_CHECK_EVALUATE      "checkEvaluate"
#define MSG_UPDATE_PRODUCE      "showProduceBtn"
#define MSG_SHOW_CARD_END       "showCardEnd"
#define MSG_SELECT_CARDS_END    "selectCardsEnd"
#define MSG_CARD_MY_CHEST_END   "CardMyChestEnd"
#define MSG_CARD_SCENE_END      "CardSceneEnd"
#define MSG_BOOK_READ_END       "BookReadEnd"
#define MSG_UPDATE_TASK         "updateTaskAfterReport"
//在xml下载完回调后，刷新交互面板
#define MSG_UPDATE_INTERACT                 "initInterPanel"
//设置交互呼出按钮可见性
#define MSG_SET_INTERACT                    "setInterVisible"
//录音时长到上限，强制停止
#define MSG_RECORD_TIMEOUT                  "onRecordTimeOut"
//设置自动播放按钮高亮性
#define MSG_SET_AUTOBTN                     "setAutoBtnState"
//正文页自动播放齿轮控制
#define MSG_PAGE_AUTOPLAY                   "setPageAutoPlay"
//主页按钮
#define MSG_MAINPAGE_BUTTON                 "mainpage_button"
//主页按钮显示隐藏
#define MSG_MAINPAGE_BUTTON_VISIBLE         "mainpage_button_visible"
//单词课购买失败返回消息
#define MSG_BUY_WORDCLASS_FAILSE            "msg_buy_wordCalss_failse"
//宝石购买失败返回消息
#define MSG_BUY_GEMS_FAILSE                 "msg_buy_gems_failse"

//点击个人信息按钮弹出个人信息界面
#define MSG_SHOW_SELF_UI_BY_CLICK           "msg_show_self_ui_by_click"

//设置刷新声音速度
#define MSG_SET_SOUND_SPEED                 "set_sound_speed"
#define MSG_SET_SOUND_SPEED_NOTICE          "set_sound_speed_notice"

//添加一本读完书籍
#define MSG_NEW_BOOK_FINISH_READ            "msg_new_book_finish_read"

#define   DOWN_LEVEL 1
#define   HIGH_LEVEL 3
#define   PAGE_BOOK_COUNT  50

#endif /* UIDef_hpp */
