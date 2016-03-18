//
//  DrawCardSucess.h
//  WarOfGod
//
//  Created by prefer_shan on 15/6/8.
//
//

#ifndef __WarOfGod__DrawCardSucess__
#define __WarOfGod__DrawCardSucess__

#include <stdio.h>
#include "BaseLayer.h"
#include "AllDefine.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "LYFightData.h"
USING_NS_CC;


class CDrawCard;

class DrawCardSucess:public CBaseLayer
{

public:
    
    static DrawCardSucess * create(int iMagic,CDrawCard * Parent) \
    { \
        DrawCardSucess *pRet = new(std::nothrow) DrawCardSucess(iMagic,Parent); \
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
    
    void onExit();

    DrawCardSucess(int iMagic,CDrawCard * Parent);
    ~DrawCardSucess();
    bool init();
    
private:

    
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    void onTouchCancelled(Touch *Touch, Event *pEvent);
    void OnAction(Node *pSender);
    void OnStarRunAction(Ref *pSender);
    
    void OnCloseMenu(Ref *pSender);
    void AddCloseMenu();
    
      void  RunStarAnimate(int iNum,Sprite *pDebrisIcon);
    
    //ParticleSystemQuad* m_pEffect;
    
    int m_iMagic; //0为金币1连抽 1为金币10连抽  2为魔法石1连抽 3为魔法石10连抽
    Menu * m_pMenuClose;
    bool m_bMenuClose;
    cocos2d::Ref * m_pTarget;
    cocos2d::SEL_MenuHandler m_pHandler;
    EventListenerTouchOneByOne *m_pListener;
     DrawCardInfo m_CardInfos;
    CDrawCard*m_Parent;
    
};

#endif /* defined(__WarOfGod__DrawCardSucess__) */
