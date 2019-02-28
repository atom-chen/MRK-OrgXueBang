//
//  StringUtil.cpp
//  OrgXueBang-mobile
//
//  Created by liuwei on 2019/1/8.
//

#include "StringUtil.hpp"
#include <regex>
#include <regex.h>
using namespace std;

string StringUtil::UrlEncode(const string& szToEncode)
{
    string src = szToEncode;
    char hex[] = "0123456789ABCDEF";
    string dst;
    for (size_t i = 0; i < src.size(); ++i)
    {
        unsigned char cc = src[i];
        
        if (isalnum(cc) || (cc == '-') || (cc == '_') || (cc == '.') || (cc == '~'))
        {
            dst += cc;
        }
        else if (cc == ' ')
        {
            dst += "+";
        }
        else
        {
            unsigned char c = static_cast<unsigned char>(src[i]);
            dst += '%';
            dst += hex[c / 16];
            dst += hex[c % 16];
        }
    }
    return dst;
}

string StringUtil::UrlDecode(const std::string& szToDecode)
{
    std::string result;
    int hex = 0;
    for (size_t i = 0; i < szToDecode.length(); ++i)
    {
        switch (szToDecode[i])
        {
            case '+':
                result += ' ';
                break;
            case '%':
                if (isxdigit(szToDecode[i + 1]) && isxdigit(szToDecode[i + 2]))
                {
                    std::string hexStr = szToDecode.substr(i + 1, 2);
                    hex = strtol(hexStr.c_str(), 0, 16);
                    //字母和数字[0-9a-zA-Z]、一些特殊符号[$-_.+!*'(),] 、以及某些保留字[$&+,/:;=?@]
                    //可以不经过编码直接用于URL
                    if (!((hex >= 48 && hex <= 57) || //0-9
                          (hex >=97 && hex <= 122) ||   //a-z
                          (hex >=65 && hex <= 90) ||    //A-Z
                          //一些特殊符号及保留字[$-_.+!*'(),]  [$&+,/:;=?@]
                          hex == 0x21 || hex == 0x24 || hex == 0x26 || hex == 0x27 || hex == 0x28 || hex == 0x29
                          || hex == 0x2a || hex == 0x2b|| hex == 0x2c || hex == 0x2d || hex == 0x2e || hex == 0x2f
                          || hex == 0x3A || hex == 0x3B|| hex == 0x3D || hex == 0x3f || hex == 0x40 || hex == 0x5f
                          ))
                    {
                        result += char(hex);
                        i += 2;
                    }
                    else result += '%';
                }else {
                    result += '%';
                }
                break;
            default:
                result += szToDecode[i];
                break;
        }
    }
    return result;
}
void StringUtil::trim(string &s)
{
    int index = 0;
    if( !s.empty())
    {
        while( (index = s.find(' ',index)) != string::npos)
        {
            s.erase(index,1);
        }
    }
}

void StringUtil::MaskPhone(string& strPhone)
{
    //前三中五后三
    std::string phoneModle = "(\\d{3})(\\d{5})(\\d{3})";
    regex r(phoneModle);
    //保留首尾，中间用星号替换
    std::string fmt("$1*****$3");
    strPhone = regex_replace(strPhone, r, fmt, regex_constants::format_no_copy);
}

void StringUtil::splitString(const char * source,char splitChar, vector<string> &outputVec)
{
    //
    string sTemp(source);
    int i = 0;
    while(sTemp.length()>i && sTemp.at(i)!= '\0'  ){
        if (sTemp.at(i)==splitChar) {
            string s = sTemp.substr(0,i);
            outputVec.push_back(s);
            sTemp = sTemp.substr(i+1, sTemp.length()-1);
            i=0;
        }
        ++i;
    }
    
    outputVec.push_back(sTemp);
}
//判断是否是个正确的手机号
bool StringUtil::isPhoneNumber(const std::string & mobile)
{
    std::regex re("[^0-9]");
    std::smatch sm;
    
    if( regex_search(mobile, sm, re) )
    {
        return false;
    }
    
    if( mobile.find("1") != 0 )
    {
        return false;
    }
    
    if ( mobile.size() != 11 )
    {
        return false;
    }
    return true;
}

bool StringUtil::StringVerification(std::string& src, std::string& regular)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    regex pattern(regular.c_str());
    if ( !regex_match( src, pattern ) )
    {
        return false;
    }
    return true;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID||CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    regex_t reg;
    int retval = regcomp(&reg, regular.c_str(), REG_EXTENDED | REG_NEWLINE);
    
    
    retval = regexec(&reg, src.c_str(), 0, NULL, 0);
    //    CCLOG("%s use %s reg is %d", src.c_str(), regular.c_str(), retval);
    regfree(&reg);
    if (retval == 0) {
        return true;
    }
    else
    {
        return false;
    }
#endif
}
bool StringUtil::checkPhone(std::string &phone)
{
    std::string regular = std::string("^1[3|4|5|6|7|8|9][0-9]{9}$");
    return StringVerification(phone, regular);
}
