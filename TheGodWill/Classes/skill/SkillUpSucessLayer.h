//
//  SkillUpSucessLayer.h
//  WarOfGod
//
//  Created by prefer_shan on 15/3/26.
//
//

#ifndef __WarOfGod__SkillUpSucessLayer__
#define __WarOfGod__SkillUpSucessLayer__

#include <stdio.h>

#include "cocos2d.h"
#include "AllDefine.h"

USING_NS_CC;

class CSKillUpSucessLayer:public cocos2d::Layer
{
public:
    
    static CSKillUpSucessLayer* create(std::string strSkillName,std::string strImg,bool bHero=false)
    {
        CSKillUpSucessLayer *pRet = new CSKillUpSucessLayer(strSkillName,strImg,bHero);
        if (pRet && pRet->init())
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = NULL;
            return NULL;
        }
    }

    bool init();

    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) ;
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
    
    
private:
    void OnRemoveAll();
    void onExit();

    Size  m_winSize;
    std::string m_strImg;
    std::string m_strSKillName;
    bool m_bHero;
    EventListenerTouchOneByOne * m_plistener;
    
    CSKillUpSucessLayer(std::string strSkillName,std::string strImg,bool bHero);
    ~CSKillUpSucessLayer();
};

#endif /* defined(__WarOfGod__SkillUpSucessLayer__) */
