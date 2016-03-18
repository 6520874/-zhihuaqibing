//
//  DataHimi.cpp
//  Oh!MonsterMR
//
//  Created by hsx on 14-4-16.
//  Copyright (c) 2014年 Augustimpression. All rights reserved.
//

#include "HttpEncrypt.h"
#include "LYPersonalData.h"
#include "sysTime.h"
#include <time.h>
#include "cocos2d.h"
USING_NS_CC;


static const std::string dataChars = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static inline bool baseData(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string SaveData(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for(i = 0; (i <4) ; i++)
                ret += dataChars[char_array_4[i]];
            i = 0;
        }
    }
    
    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;
        
        for (j = 0; (j < i + 1); j++)
            ret += dataChars[char_array_4[j]];
        
        while((i++ < 3))
            ret += '=';
        
    }
    
    return ret;
    
}
std::string HttpEncrypt(std::string httpStr)
{
    httpStr = httpStr + "*!^$#&@4chs";
    std::string hStr = SaveData(reinterpret_cast<const unsigned char*>(httpStr.c_str()), httpStr.length());
    if (hStr.length()<=20) {
        return hStr;
    }
    hStr = hStr.substr(hStr.length()-20);
    hStr = hStr.substr(0,hStr.length()-5);
    return hStr;
}


std::string getOrderNumber()
{
    std::string str;
    
    int userid = CPersonalData::getInstance()->getUserInfo()->m_iUserId;
    time_t t = CGameTime::getNowTimeInSecond();
    std::string code = CPersonalData::getInstance()->getUserInfo()->m_sAuthCode;
    CCString *string = CCString::createWithFormat("%d%ld%s",userid,t,code.c_str());
    str = string->getCString();
    std::string strOrder = SaveData(reinterpret_cast<const unsigned char*>(str.c_str()), str.length());
    strOrder = strOrder.substr(0,32);
    return strOrder;
}

std::string ParseData(std::string const& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;
    
    while (in_len-- && ( encoded_string[in_] != '=') && baseData(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = dataChars.find(char_array_4[i]);
            
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            
            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }
    
    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;
        
        for (j = 0; j <4; j++)
            char_array_4[j] = dataChars.find(char_array_4[j]);
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        
        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }
    
    return ret;
}
