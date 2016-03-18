//
//  CSharedPlist.cpp
//  GAME2
//
//  Created by user on 14-10-24.
//
//

#include "CSharedPlist.h"


CSharedPlist* CSharedPlist::m_pSharedPlist = NULL;
CSharedPlist::CSharedPlist()
{
    
}
CSharedPlist::~CSharedPlist()
{
    
}

CSharedPlist* CSharedPlist::getInstance()
{
    if(!m_pSharedPlist)
    {
        m_pSharedPlist = new CSharedPlist();
       
    }
    return m_pSharedPlist;
    
}

bool CSharedPlist::findPlist(std::string plistFile)
{
    std::map<std::string,int>::iterator it = m_mapPlist.find(plistFile.c_str());
    if(it == m_mapPlist.end()){
        return false;
    }
    return true;
}

void CSharedPlist::erasePlist(std::string plistFile)
{
    std::map<std::string,int>::iterator it = m_mapPlist.find(plistFile.c_str());
    if(it != m_mapPlist.end()){
        it->second -= 1;
        if(it->second <=0){
            m_mapPlist.erase(plistFile.c_str());
            SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(plistFile.c_str());
            
            
            int offset = plistFile.find(".plist");
            
            std::string pngName = plistFile.replace(offset, 6, ".pvr.ccz");
            
            Director::getInstance()->getTextureCache()->removeTextureForKey(pngName);
            
            
            CCLOG("delete plist ::::%s",pngName.c_str());
        }
    }
}

void CSharedPlist::pushPlist(std::string plistFile)
{
    std::map<std::string,int>::iterator it = m_mapPlist.find(plistFile.c_str());
    if(it == m_mapPlist.end()){
        m_mapPlist.insert(std::make_pair(plistFile.c_str(),1));
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistFile.c_str());
    }else{
        it->second += 1;
    }
}