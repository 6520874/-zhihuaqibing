//
//  LvIntroduce.h
//  WarOfGod
//
//  Created by prefer_shan on 15/4/10.
//
//

#ifndef __WarOfGod__LvIntroduce__
#define __WarOfGod__LvIntroduce__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
USING_NS_CC;

class  CLvIntroduce:public CBaseLayer
{
    CLvIntroduce();
    ~CLvIntroduce();
    Size  m_winSize;
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) ;
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
    void onExit();
    
public:
    bool init();
    void OnCloseMenu(Ref *pSender);
    
    CREATE_FUNC(CLvIntroduce);
    EventListenerTouchOneByOne * m_pListener;
};


#endif /* defined(__WarOfGod__LvIntroduce__) */
