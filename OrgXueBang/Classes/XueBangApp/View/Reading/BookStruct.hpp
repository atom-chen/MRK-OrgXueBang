//
//  BookStruct.hpp
//  ReadingMate
//
//  Created by 杨勇 on 2016/12/15.
//
//

#ifndef BookStruct_hpp
#define BookStruct_hpp

#include "BaseLoad.h"
//#include "Constants.hpp"
#include <map>
#include <string>
#include <vector>
using namespace std;

#include <bitset>
using std::bitset;

#include "DownImg.h"
#include "StringUtil.hpp"
#include "GameConfig.hpp"

struct BookData
{
    
    string id;          //图书ID(有查询记录时)
    string idnumber;    //图书编号(有查询记录时)
    string name;        //图书名称(有查询记录时)
    string intro;       //图书简介(有查询记录时)
    string cover;       //封面
    string word_count;  //单词数(有查询记录时)
    string keywords;    //关键词
    string topics;      //话题
    bool   read_flag;   //1代表已翻完阅读页  0或其他值代表没有
    string lexile;      //蓝思值(有查询记录时)
    string page_count;  //图书页数(有查询记录时)
    string add_time;    //图书创建时间(有查询记录时)
    string read_time;   //用户最近阅读时间(有查询记录时)
    string paging;      //页数信息
    int last_read_page_id;//上次阅读记录
    
    string cohort;                  //所属分组 RM-FREE 免费；RM-GUEST 游客; RMA-001级别A
    string latest_book_flg;         //level最近阅读图书标识 y:是最近阅读 n:不是最近阅读
    string read_flg;                //是否已读 y:已读 n:未读
    string level;           //图书所属级别
    string isFree;          //是否免费
    string isNew;           //是否新书
    string booksale_flag;    //学校特有的标示 add by hyp
    string enroll; //存储变量 add by sck
    
    /////////服务器减压，客户端来统计，不安全，容易被破解/////////
    int     sum_stay_time;          //开书时长-每一页停留时间总和
    int     sum_listening_time;     //听音时长-阅读页面的听音时间总和
    int     sum_recording_time;     //录音时长-阅读页面的录音时间总和
    int     sum_read_nuts;          //历史朗读坚果数
    int     sum_quiz_nuts;          //历史答题坚果数
    int     sum_nativ;              //地道程度-3道语音题的得分总和
    int     sum_acc;                //准确程度-3道语音题的得分总和
    int     sum_fluency;            //流利程度-3道语音题的得分总和
    int     sum_read_1;             //朗读页1-第1道语音题的最好得分（综合）
    int     sum_read_2;             //朗读页2-第2道语音题的最好得分（综合）
    int     sum_read_3;             //朗读页3-第3道语音题的最好得分（综合）
    int     sum_quiz_1;             //答题页1-第1道测试题的最好得分（综合）
    int     sum_quiz_2;             //答题页2-第2道测试题的最好得分（综合）
    int     sum_quiz_3;             //答题页3-第3道测试题的最好得分（综合）
    int     sum_quiz_4;             //答题页4-第4道测试题的最好得分（综合）
    int     sum_quiz_5;             //答题页5-第5道测试题的最好得分（综合）
    bool    sum_status;             //是否阅读完成的状态
    
    int     cur_nativ;              //地道程度-3道语音题的得分总和(当前)
    int     cur_acc;                //准确程度-3道语音题的得分总和(当前)
    int     cur_fluency;            //流利程度-3道语音题的得分总和(当前)
    int     cur_read_1;             //朗读页1-第1道语音题的最好得分(当前)
    int     cur_read_2;             //朗读页2-第2道语音题的最好得分(当前)
    int     cur_read_3;             //朗读页3-第3道语音题的最好得分(当前)
    int     cur_quiz_1;             //答题页1-第1道测试题的最好得分(当前)
    int     cur_quiz_2;             //答题页2-第2道测试题的最好得分(当前)
    int     cur_quiz_3;             //答题页3-第3道测试题的最好得分(当前)
    int     cur_quiz_4;             //答题页4-第4道测试题的最好得分(当前)
    int     cur_quiz_5;             //答题页5-第5道测试题的最好得分(当前)
    string  correct_pages;          //答对题的页ID，逗号分隔
    //////////////////
    
    BookData();
    
//    string cohort2Level();
};

class PageBase;
typedef struct
{
    int                     m_pageIndex;        //页面编号
    ePageType               m_pageType;         //页面类型
    PageBase*               m_pageUI;           //Page指针
    NodeGrid*               m_grid;             //grid指针
}PageCache;

enum eWordState
{
    eWordDefault,   //-1    黑
    eWordNormal,    //0|1   绿
    eWordWrong,     //2|3   红
    eWordOther,
};

struct WordResult
{
    int         w_id;
    string      w_text;
    eWordState  w_state;
    bitset<45>  w_bitvec; //45位，全为0。
    
    WordResult()
    :w_id(0)
    ,w_text("")
    ,w_state(eWordDefault)
    {
        
    }
};

//一个单词的结构体
typedef struct
{
    int             w_index;    //单词索引
    std::string     w_string;   //单词内容
    int             w_start;    //开始索引
    eWordState      w_state;    //单词状态
    Rect            w_rect;     //矩形
    
    std::string     w_word;     //查单词
    int             w_voiceBegin;
    int             w_voiceEnd;
    int             w_wordID;   //单词ID，教材打分用
    
    bitset<45>      w_bitvec;   //45位，全为0。
}WordAttr;

struct PageScore
{
    int     m_listent;  //听时长
    float   m_modelTime;//范读时长
    int     m_reocrd;   //录时长
    int     m_score;    //得分
    int     m_dummy;    //临时显示
    int     m_nuts;     //坚果
    int     m_nativ;    //地道程度
    int     m_acc;      //准确度
    int     m_fluency;  //流利程度
    
    vector<WordResult*> m_errWords;
    
    PageScore();
    
    void clearScore();
    
};

//单页结构体
struct OnePage
{
    int     m_pageID;
    string  m_pageTitle;
    string  m_pageMode;
    string  m_pdfPic;
    string  m_pdfSound;
    string  m_pdfContent;
    string  m_pageName;
    int     m_sortIndex;
    bool     m_isQuiz;
    string  m_parameters;
    int     m_courseID;
    stuJson m_paramJson;
    string  m_properties;
    
    bool    m_downPicFailed;
    bool    m_downSndFailed;
    bool    m_downDatFailed;
    
    PageScore* m_pageScore;
    
    OnePage();
    
};

//整本书总页数组
typedef vector<OnePage*> BookPagesVec;

//分享数据结构
struct BookShareData
{
    string m_imgUrl;
    string m_subjects[6];
    int m_days;
    int m_books;
    int m_words;
};

class WholeBook
{
public:
    std::unordered_map<int, PageCache*>     m_mapPageViews;
    BookPagesVec*   m_bookPages;    //全部书页
    bool            m_favourite;    //收藏状态
    std::string     m_folderPic;    //图片资源根目录
    std::string     m_folderSnd;    //听读资源根目录
    std::string     m_folderCnt;    //数组资源根目录
    BookData*       m_bookData;     //书本概要信息
    
    std::unordered_map<int, std::string>    m_mapRecordPath;//录音保存路径
    std::unordered_map<int, stuJson>        m_mapRecordResult;
    
    BookShareData m_shareData;      //书分享数据结构

    WholeBook();
    
    string getFolder(int index);
    void downPic(int index);
    void downSound(int index);
    void downXml(int index);
    void downAll(int index);
    
    void downQuizSound(int index, const string& sndName);//拖拽题在/OM/
    void downQuizSound1(int index, const string& sndName);//选择题在/QAA/
    void downQuizPic(int index, const string& sndName);
    
    string getPicPath(int index);
    string getSoundPath(int index);
    string getXmlPath(int index);
    
    string getQuizSoundPath(int index, const string &sndName);//拖拽题在/OM/
    string getQuizSoundPath1(int index, const string &sndName);//选择题在/QAA/
    string getQuizPicPath(int index, const string &sndName);
    
    bool haveSnd(int index);
    bool havePic(int index);
    bool haveDat(int index);
    
    bool gotFullRes(int index);
    
    void clearWholeBook()
    {
        m_mapRecordPath.clear();
        m_mapRecordResult.clear();
        m_mapPageViews.clear();
        for(auto item : *m_bookPages)
        {
            OnePage* page = item;
            page->m_pageScore->clearScore();
            CC_SAFE_DELETE(page->m_pageScore);
            CC_SAFE_DELETE(item);
        }
        m_bookPages->clear();
    }
    
    static void downOnePageRes(const string& id, int index, const string& url, const string& res, int iTag, eBookResType resType);
    
};

//书的话题
struct BookTopic
{
    string  t_id;   //话题id
    string  t_name; //话题名称
    string  t_img;  //话题图片
    int     t_index;//话题索引
    bool    t_isSelected;//是否用户选中  y 选中； n 未选中
    
    BookTopic();
};

typedef vector<BookTopic*> BookTopicVec;

//角色信息
struct ActInf
{
    string act_name;
    string act_img;
    
    ActInf()
    :act_name("-1")
    ,act_img("-1")
    {
        
    }
};

typedef vector<ActInf*> ActVec;

//actor sentence单个角色的说话信息
struct ActStc
{
    string              as_actID;    //角色ID
    vector<WordAttr>    as_vecWords; //角色说的话
    string              as_sentence; //整句话
    float               as_modelTime;//单句话的范读时长
    
    void clearActStc()
    {
        as_vecWords.clear();
    }
};

struct TextScore
{
    time_t  ts_open;    //开书时间
    int     ts_listen;  //听时长
    int     ts_modelTime;//范读时长
    int     ts_reocrd;   //录时长
    int     ts_score;    //得分
    int     ts_nuts;     //坚果
    int     ts_nativ;    //地道程度
    int     ts_acc;      //准确度
    int     ts_fluency;  //流利程度
    int     ts_repeat;   //测试次数，取平均值用
    
    std::unordered_map<std::string, std::string> ts_files;  //录音文件
    vector<WordResult*> m_errWords;
    TextScore();
    
    void clearScore();
    
};

//原版教材的页信息
struct TxtPge
{
    string  tp_id;           //书页ID
    string  tp_title;        //书页名称
    string  tp_mode;         //书页类型
    string  tp_titlID;      //书页title_id
    string  tp_infUrl;      //书页信息文件
    string  tp_srtIdx;      //书页排序
    string  tp_bookID;      //教材图书ID
    string  tp_bokNum;      //教材图书编号
    
    unsigned char   tp_resTag;  //资源标记
    string  tp_titTxt;      //页眉的文字
    
    bool    tp_screenShotSaved;//已截屏
    bool    tp_screenShotReady;//滚屏居中
    
    bool    tp_initDlgOvered;
    bool    tp_initActOvered;
    bool    tp_initAllOvered;
    
    bool    tp_downInfFailed;
    bool    tp_downActFailed;
    bool    tp_downSndFailed;
    bool    tp_downLryFailed;
    
    string  tp_sndMp3;      //音效资源名字
    string  tp_actXml;      //参与者信息
    string  tp_titXml;      //歌词参数
    string  tp_audDef;      //分段朗读可用
    string  tp_tilNum;      //页Title码
    string  tp_level;       //书的级别
    
    ActVec  tp_actVec;      //角色列表
    
    vector<ActStc>  tp_actDlg;   //全部角色的会话
    TextScore       tp_score;   //该页的成绩
    
    void clearTxtPge()
    {
        for(auto act : tp_actVec)
        {
            CC_SAFE_DELETE(act);
        }
        tp_actVec.clear();
        for(auto dlg : tp_actDlg)
        {
            dlg.clearActStc();
        }
        tp_actDlg.clear();
    }
    
    TxtPge();
};

typedef vector<TxtPge*> TxtPgeVec;//原版书页数组

struct TxtBok
{
    TxtPgeVec*  tb_txtPges;     //页数组
    
    string      tb_txtID;       //书的ID
    int         tb_txtPgeNum;   //总页数
    int         tb_txtLstRed;   //最后记录
    
    string      tb_txtCntUrl;   //资源仓库
    
    TxtBok();
    
    void downFile(int idx, eBookResType resType);
    void readFile(int idx, eBookResType resType);
    string getFolder(int idx);
    
    //检测文件的配置或存在状态
    bool getFileState(bool real,int idx, eBookResType resType);
    //设置文件下载状态
    void setFileState(int idx, eBookResType resType,bool bFail);
    
    bool checkFullRes(int index);
    
    void clearTxtBok()
    {
        for(auto page : *tb_txtPges)
        {
            page->clearTxtPge();
            CC_SAFE_DELETE(page);
        }
        tb_txtPges->clear();
    }
};

struct BookNote
{
    /*
     "id": "1",
     "user_id": "257",
     "course_id": "1",
     "type": "high",
     "textbook_id": null,
     "book_name": "aaa",
     "note_question_id": "1",
     "note_question_title": "sdf",
     "cover_img": "257/t1.jpg",
     "bind_page_id": "0",
     "sound": "257/t1.png"
     "add_time": "2017/5/5 12:22:25"
     */
    
    string  bn_id;              //笔记ID
    string  bn_userID;          //用户ID
    string  bn_courseID;        //原版图书ID
    string  bn_idnumber;        //书的ID Number
    bool    bn_levelLow;        //低级书
    string  bn_textbookID;      //教材图书ID
    string  bn_bookName;        //图书名称
    string  bn_questionID;      //问题ID
    string  bn_questionTitle;   //问题名称
    string  bn_coverImg;        //阅读笔记封面图片
    string  bn_bindPageID;      //绑定书页ID
    string  bn_sound;           //阅读笔记语音
    string  bn_addTime;         //笔记更新时间
    
    BookNote()
    :bn_id("")//笔记ID
    ,bn_userID("")//用户ID
    ,bn_courseID("")//原版图书ID
    ,bn_idnumber("")//书的ID Number
    ,bn_levelLow(true)//级别
    ,bn_textbookID("")//教材图书ID
    ,bn_bookName("")//图书名称
    ,bn_questionID("")//问题ID
    ,bn_questionTitle("") //问题名称
    ,bn_coverImg("")//阅读笔记封面图片
    ,bn_bindPageID("")//绑定书页ID
    ,bn_sound("")//阅读笔记语音
    ,bn_addTime("")//笔记更新时间
    {
    }
};

typedef vector<BookNote> BookNoteVec;//读书笔记列表

struct NoteData
{
    int     nd_totalNum;
    int     nd_PageCount;
    
    string  nd_urlAudio;
    string  nd_urlImage;
    
    BookNoteVec nd_vecNotes;
    
    NoteData()
    :nd_totalNum(0)
    ,nd_PageCount(0)
    ,nd_urlAudio("")
    ,nd_urlImage("")
    {
    }
};

 struct NoteTopic
{
    string  nt_id;
    string  nt_title;
    string  nt_audio;
    
    NoteTopic()
    :nt_id("")
    ,nt_title("")
    ,nt_audio("")
    {
    }
};


#endif /* BookStruct_hpp */
