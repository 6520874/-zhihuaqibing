//
//  SkillTouchDetail.h
//  WarOfGod
//
//  Created by prefer_shan on 15/5/6.
//
//

#ifndef __WarOfGod__SkillTouchDetail__
#define __WarOfGod__SkillTouchDetail__

#include <stdio.h>
#include "cocos2d.h"
#include "AllDefine.h"
#include "BaseLayer.h"


class CSKillTouchDetail:public CBaseLayer
{

public:
    
    //CREATE_FUNC(CSKillTouchDetail);
    static CSKillTouchDetail* create(FighterInfo* pInfo,int iSkillIndex) \
    { \
        CSKillTouchDetail *pRet = new(std::nothrow) CSKillTouchDetail(pInfo,iSkillIndex); \
        if (pRet && pRet->init()) \
        { \
            pRet->autorelease(); \
            return pRet; \
        } \
        else \
        { \
            delete pRet; \
            pRet = NULL; \
            return NULL; \
        } \
    }
    
private:
    bool init();
    
    CSKillTouchDetail(FighterInfo* pInfo,int iSkillIndex);
    ~CSKillTouchDetail();
    
    int m_iSkillIndex;
    
    FighterInfo* m_pfighterInfo;

    
};


#endif /* defined(__WarOfGod__SkillTouchDetail__) */
