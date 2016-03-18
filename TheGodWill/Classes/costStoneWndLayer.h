//
//  costStoneWndLayer.h
//  WarOfGod
//
//  Created by user on 15/5/29.
//
//

#ifndef __WarOfGod__costStoneWndLayer__
#define __WarOfGod__costStoneWndLayer__

#include <stdio.h>
#include "BaseLayer.h"
#include "cocos2d.h"

USING_NS_CC;

class costStoneCommonLayer: public CBaseLayer
{
public:
    costStoneCommonLayer();
    ~costStoneCommonLayer();
    
    CREATE_FUNC(costStoneCommonLayer);
    bool init();
    void initWndInfo(Ref* target, SEL_MenuHandler hander, std::string content, int num = 0);
    void btnCallBack(Ref* pSedner);
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onSureBtn(Ref* pSender);
    void onCancenBtn(Ref* pSender);
    
    
private:
    Size m_winSize;    cocos2d::Ref * m_pTarget;    //确认的回调
    cocos2d::SEL_MenuHandler m_pHandler;
    EventListenerTouchOneByOne* listen;
    
    cocos2d::Ref * m_pCancelTarget;   //取消的回调
    cocos2d::SEL_MenuHandler m_pCancelHandler;
};

#endif /* defined(__WarOfGod__costStoneWndLayer__) */
