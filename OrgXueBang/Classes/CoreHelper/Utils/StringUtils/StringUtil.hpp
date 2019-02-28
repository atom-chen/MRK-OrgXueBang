//
//  StringUtil.hpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#ifndef StringUtil_hpp
#define StringUtil_hpp

#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

class StringUtil
{
public:
    static string UrlEncode(const string& szToEncode);
    static string UrlDecode(const std::string& szToDecode);
    static bool isPhoneNumber(const std::string & mobile);
    static void MaskPhone(string& strPhone);
    static bool StringVerification(std::string& src, std::string& regular);
    static bool checkPhone(std::string& phone);
    
    static void splitString(const char * source,char splitChar, vector<string> &outputVec);
    static void trim(string &s);
private:
    
};

#endif /* StringUtil_hpp */
