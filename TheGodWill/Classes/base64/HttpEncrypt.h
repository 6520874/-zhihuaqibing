//
//  DataHimi.h
//  Oh!MonsterMR
//
//  Created by hsx  on 14-4-16.
//  Copyright (c) 2014年 Augustimpression. All rights reserved.
//

#ifndef Oh_MonsterMR_DataHimi_h
#define Oh_MonsterMR_DataHimi_h
#include <string>

std::string SaveData(unsigned char const* , unsigned int len);
std::string ParseData(std::string const& s);
std::string HttpEncrypt(std::string httpStr);
std::string getOrderNumber();
#endif
