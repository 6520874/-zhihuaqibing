//
//  LoginIntro.h
//  WarOfGod
//
//  Created by prefer_shan on 15/4/14.
//
//

#ifndef __WarOfGod__LoginIntro__
#define __WarOfGod__LoginIntro__

#include <stdio.h>

#include "BaseLayer.h"
#include "BeName.h"
class  CLoginIntro:public CBaseLayer
{

public:
    CREATE_FUNC(CLoginIntro);
    
    static cocos2d::Scene* createScene();
    
    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchMoved(Touch *touch, Event *event) ;
    void onTouchEnded(Touch *touch, Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
     void update(float ft);
    
private:
    
    bool init();
    CLoginIntro();
    ~CLoginIntro();
    void onExit();
    
    int m_iIndex;
    Label * m_pTextInfo;
    
    std::string m_strText;
    
    float m_fBeginTime;
    
    CBeNameLayer *m_pBeNameLayer;
    EventListenerTouchOneByOne * m_pListener;
};
#endif /* defined(__WarOfGod__LoginIntro__) */
