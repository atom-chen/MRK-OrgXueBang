//
//  MyWordGame.hpp
//  ReadingMate-mobile
//
//  Created by 高超 on 2018/3/15.
//

#ifndef MyWordGame_hpp
#define MyWordGame_hpp
#include "cocos2d.h"
#include "stdafx.h"
#include "CDataHttp.hpp"

typedef struct MonsterUnit{
public:
    MonsterUnit():_name(""), _monster(nullptr){
    }
    string _name;                   //怪物名
    SkeletonAnimation* _monster;    //spine
}monsterUnit;

typedef struct GameSubject
{
public:
    GameSubject():_id(-1), _name(""), _desc(""), _audioPath(""), _imgPath(""), _bRight(false){
    }
    int _id;            //单词ID
    string _name;       //名称
    string _desc;       //解释
    string _audioPath;  //音频路径
    string _imgPath;    //图片路径
    bool _bRight;       //是否正解
}gameSubject;

typedef struct UpgradeWordsStruct
{
public:
    UpgradeWordsStruct():_id(-1), _word(""), _skillLevel(0){
    }
    int _id;            //升级单词ID
    string _word;       //升级单词名称
    int _skillLevel;    //升级单词等级
}upgradeWordStruct;

typedef struct GameResultData
{
public:
    GameResultData():_monsterId(0), _monsterName(""), _monsterPoint(0), _monsterTotalPoint(0), _bUpgradeFlag(false), _correctRate(0), _usedTime(0){
        _upgradWordsList.clear();
    }
    int _monsterId;             //获取的怪物孵化ID
    string _monsterName;        //怪物孵化名称
    int _monsterPoint;          //怪物孵化现在点数
    int _monsterTotalPoint;     //怪物孵化总点数
    bool _bUpgradeFlag;         //是否获得怪物 （true  达到点数     false：未达点数）
    std::vector<UpgradeWordsStruct*> _upgradWordsList; //升级的单词列表
    
    int _correctRate; //正确率
    long _usedTime; //用时sec
}gameResultData;

typedef struct MyWordGameData
{
public:
    MyWordGameData():_payGem(0), _curSubjectId(0), _curSubjectCount(0), _rightSubject(0),
    _curEggProc(0), _totalEggProc(0), _bInitMonsterFlag(false)
    {
        _monsterNameList.clear();
        _monsterList.clear();
        _subjectList.clear();
        _rightWordVect.clear();
        _wrongWordVect.clear();
    }
    
    int _payGem;         //此轮游戏支付宝石
    int _curSubjectId;   //当前题目ID
    int _curSubjectCount;//总题数
    int _rightSubject;   //答对题数
    int _curEggProc;     //当前怪蛋进度
    int _totalEggProc;   //孵化怪蛋总数
    
    bool _bInitMonsterFlag; //已初始化过怪物列表
    std::vector<string> _monsterNameList; //怪物名称列表
    std::vector<monsterUnit*> _monsterList; //已获得怪物列表
    
    std::vector<gameSubject*> _subjectList; //题目列表
    
    std::vector<int> _rightWordVect;    //答对的单词列表，上传使用
    std::vector<int> _wrongWordVect;    //答错的单词列表，上传使用
}myWordGameData;

class MyWordGameSubjectItem;
class MyWordGame : public cocos2d::Node, public BaseKeyListener, public CDataHttpDelegate
{
public:
    enum ePlayGameStatus{
        eStatusNone = -1,
        eStatusInit,             //初始化
        eStatusStart,            //开始
        eStatusStartEffect,      //准备数据
        eStatusGame,             //游戏
        eStatusEndEgg,           //怪物蛋效果
        eStatusResult            //结果(包括单词升级)
    };
    enum eMonsterStatus{    //怪物状态
        sleep = 0,
        move,
        stand,
        erand,
        win,
        fail
    };
    
    static MyWordGame* create(cocos2d::Node* parent, void *data, bool bDefShowLetter = true);
    MyWordGame();
    ~MyWordGame();
    virtual bool init(void *data, bool bDefShowLetter = true);
    virtual void onEnter() override;
    virtual bool OnBaseKeyBack() override;
    virtual void onExit() override;
    
protected:
    void initData(void *data);
    void clearDataList();

    void statusInit();
    void statusStartView();
    void statusStartEffect(std::vector<GameSubject*> std);
    void statusStartGame();
    void statusEndEgg();
    void statusResult();

    void getDataFromSever(int flag);
    virtual void endWithHttpData(eHttpType nOp, bool bSuccess, const stuJson& json) override;
    
    void setGameStatus(ePlayGameStatus status);
    ePlayGameStatus getGameStatus() const {return m_gameStatus;}
    void updateUserGemInfo();
    void updateDoSubjectInfo();
    void updateEggProc();
    void updateMonsterZorder(float t);
    void updateEggStar(float t);
    bool haveEnoughGem(int spendGem);
    
    SkeletonAnimation* setMonster(string nameStr, string animationName);
    void setOneMonster(SkeletonAnimation* monster, eMonsterStatus status);
    Vec2 randomMonsterPos();
    void showOneEggMonster();
    
    MyWordGameSubjectItem* createMyWordGameSubjectItemLayer();
    std::vector<GameSubject*> getRandomSubject(int randomCount, std::vector<GameSubject*> srcList);
    void createAnimation(Node* startNode, Node* endNode);
    std::vector<GameSubject*> getTestSubjectList(std::vector<GameSubject*> vec, std::vector<GameSubject*> srcVec);
    
    void touchSubjectItemCallBack(int wordId, string rightDesc, bool isRight);
    void touchResultCallBack(int type, void *data = nullptr);
    
    void playAudio(string path);
    void enterGemStore();
    void enterMainPage();

    void touchBackCallFun();
private:
    cocos2d::Node* m_ui;
    bool m_bDefShowLetter;                      //默认显示生词本是熟练度排序还是字母排序
    float m_panel_subjectX;                     //题目显示时的x坐标，用于出题动画
    bool b_canTouched;                          //是否能被点击，在动画时设置状态，防止用户强制退出
    
    myWordGameData* m_gamePlayData;             //游戏数据
    
    SkeletonAnimation* m_skeleton_egg;          //蛋spine
    SkeletonAnimation* m_skeleton_star;         //孵化蛋时的飞星spine
    SkeletonAnimation* m_skeleton_senceLight;   //孵化怪物时背景spine

    ePlayGameStatus m_gameStatus;               //游戏状态

    MyWordGameSubjectItem* m_subjectItemLayer;  //出题的界面
    
    string m_testName;                              //当前出题的单词名称
    std::vector<GameSubject*> m_copySubjectList;    //用于出题
    std::vector<GameSubject*> m_testSubject;        //用于出题
    
    gameResultData* m_gameResultData;           //游戏结果数据
    bool m_isbeginTime;                         //一次游戏循环开始及结束标志，用于计算游戏时间
};

#endif /* MyWordGame_hpp */
