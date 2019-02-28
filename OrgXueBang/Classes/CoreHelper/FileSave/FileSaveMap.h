#ifndef __FILE_SAVE_Json_H__
#define __FILE_SAVE_Json_H__

/*
 文件保存
 by：高超
 */

#define MYDEF_FILEJSONSAVE_DIR "FileMapSave/"
#define MYDEF_FILEJSON_NAME "FileMapTxt.save"

class CFileSaveMap
{
public:
    static CFileSaveMap* getInstance();
    static void destroyInstance();
    bool init();
    
    string readFromFile(string key,string file = MYDEF_FILEJSON_NAME);
    void writeValueFile(string key,string value,string file = MYDEF_FILEJSON_NAME);
    void eraseValueFile(string key,string file = MYDEF_FILEJSON_NAME);
};

#endif



