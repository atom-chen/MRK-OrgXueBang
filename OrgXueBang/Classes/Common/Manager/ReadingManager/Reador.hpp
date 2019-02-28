//
//  Reador.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/21.
//

#ifndef Reador_hpp
#define Reador_hpp

#include <stdio.h>
#include <string>
using namespace std;

class Reador
{
private:
    Reador();
    ~Reador();
public:
    static Reador* getInstance();
    static void destroyInstance();
    
    void init();
    
    void setCurrentBook(const std::string& book, const string task = "0", const string wordClassId = "", const string qualityId = "");
    
    //获取任务书ID
    std::string& getCurrentTask();
    std::string& getCurrentBook();
    
    const string& getCurrentQualityId();
    const string& getCurrentWordClass();
    bool getPOrRing();
    void setPOrRing(bool bIng);
private:
    std::string m_currentBook;
    string m_currentTask;
    bool bPOrRing;
    string m_currentWordClassId;//当前单词课id
    //精读课
    string m_currentQualityId; //精读课阶段ID (0或空：正常读书 其他值：阶段ID)
};

#endif /* Reador_hpp */
