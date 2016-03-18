//
//  AtkMonsterInfo.h
//  WarOfGod
//
//  Created by prefer_shan on 15/4/15.
//
//

#ifndef __WarOfGod__AtkMonsterInfo__
#define __WarOfGod__AtkMonsterInfo__

#include <stdio.h>
#include "BaseLayer.h"

class CAtkMonsterInfo:public CBaseLayer
{
    
    CAtkMonsterInfo();
    ~CAtkMonsterInfo();
     bool init();
    EventListenerTouchOneByOne * m_pListener;
    
    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchMoved(Touch *touch, Event *event) ;
    void onTouchEnded(Touch *touch, Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
  
    
    void onExit();
    Ref * m_pTarget;
    cocos2d::SEL_MenuHandler  m_pHandler;
    
public:
    void CreateActionStart(Ref * pTarget,cocos2d::SEL_MenuHandler pHandler)
    ;
    
    void OnRedBtn(Ref *pSender);
    void OnBlueBtn(Ref *pSender);
    
    CREATE_FUNC(CAtkMonsterInfo);
   
};
//class CAtkMonsterInfo:public
#endif /* defined(__WarOfGod__AtkMonsterInfo__) */
