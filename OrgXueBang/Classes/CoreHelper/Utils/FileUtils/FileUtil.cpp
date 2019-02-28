//
//  FileUtil.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#include "FileUtil.hpp"

#include <unistd.h>
#include <dirent.h>
#include "sys/stat.h"
#include "base/ccUTF8.h"
#include "platform/CCFileUtils.h"
USING_NS_CC;

long FileUtil::fileSizeTotal = 0l;
void FileUtil::getDirSize(const char* dir, int depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    
    if((dp = opendir(dir)) == NULL){
        fprintf(stderr, "can't open directory %s\n", dir);
        return;
    }
    
    chdir(dir);
    while((entry = readdir(dp)) != NULL){
        lstat(entry->d_name, &statbuf);
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
        int index = depth;
        //while(index--) printf("|  ");
        //printf("|--%s\n", entry->d_name);
        index = depth;
        if(S_ISDIR(statbuf.st_mode)){
            getDirSize(entry->d_name, depth + 1);
        }else if(S_ISREG(statbuf.st_mode)){
            char path[256];
            string srtFilePath = cocos2d::StringUtils::format("%s/%s",getcwd(path,256),entry->d_name);
            fileSizeTotal += cocos2d::FileUtils::getInstance()->getFileSize(srtFilePath);
            //SLLog(srtFilePath.c_str(), total)
        }
    }
    chdir("..");
    closedir(dp);
}
void FileUtil::clearDir(const char* dir)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    
    if((dp = opendir(dir)) == NULL){
        fprintf(stderr, "can't open directory %s\n", dir);
        return;
    }
    
    chdir(dir);
    while((entry = readdir(dp)) != NULL){
        lstat(entry->d_name, &statbuf);
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
        if(S_ISDIR(statbuf.st_mode) && strcmp(entry->d_name, "UserSave")){
            char path[256];
            string srtFilePath = StringUtils::format("%s/%s",getcwd(path,256),entry->d_name);
            FileUtils::getInstance()->removeDirectory(srtFilePath);
        }else if(S_ISREG(statbuf.st_mode) && strcmp(entry->d_name, "pay.txt")){
            char path[256];
            string srtFilePath = StringUtils::format("%s/%s",getcwd(path,256),entry->d_name);
            FileUtils::getInstance()->removeFile(srtFilePath);
        }
    }
    chdir("..");
    closedir(dp);
}
