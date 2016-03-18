//
//  CSharedPlist.h
//  GAME2
//
//  Created by user on 14-10-24.
//
//

#ifndef __GAME2__CSharedPlist__
#define __GAME2__CSharedPlist__
#include "cocos2d.h"


USING_NS_CC;

class CSharedPlist : public Node{
    
public:
    CSharedPlist();
    ~CSharedPlist();
    static CSharedPlist* getInstance();
    bool findPlist(std::string plistFile);
    void pushPlist(std::string plistFile);
    void erasePlist(std::string plistFile);
private:
    std::map<std::string,int> m_mapPlist;
    static CSharedPlist* m_pSharedPlist;
};

#endif /* defined(__GAME2__CSharedPlist__) */
