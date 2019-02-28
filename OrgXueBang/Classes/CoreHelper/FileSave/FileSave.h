#ifndef __FILE_SAVE_H__
#define __FILE_SAVE_H__

/*
 文件保存
 by：高超
 */


enum class eFileSaveResult
{
    ok = 0,             //OK
    notExist,           //不存在
    wrongKey,           //key错误
    error,              //其他错误 
};

class CFileSave
{
public:
    /*保存文件-加密*/
    static eFileSaveResult EncryptSave(const std::string& file,const unsigned char* pDat,unsigned int nLen,const std::string key);
    static eFileSaveResult EncryptSave(const std::string& file,const Data& data,const std::string key);
    /*加载文件-加密*/
    static eFileSaveResult EncryptLoad(const std::string& file,Data& data,const std::string key);
    

    /*保存文件-不加密*/
    static eFileSaveResult Save(const std::string& file,const unsigned char* pDat,unsigned int nLen);
    static eFileSaveResult Save(const std::string& file,const Data& data);
    /*加载文件-不加密*/
    static eFileSaveResult Load(const std::string& file,Data& data);
    /*创建文件夹,不要用*/
    static bool createSaveDir();
    static void deleteSaveFile();
};

#endif

