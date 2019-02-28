//
//  FileUtil.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#ifndef FileUtil_hpp
#define FileUtil_hpp

#include <stdio.h>
using namespace std;

class FileUtil
{
public:
    static void getDirSize(const char* dir, int depth);
    static void clearDir(const char* dir);
    static long fileSizeTotal;
private:
    
};

#endif /* FileUtil_hpp */
