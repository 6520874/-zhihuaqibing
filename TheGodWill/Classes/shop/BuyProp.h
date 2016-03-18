//
//  BuyProp.h
//  WarOfGod
//
//  Created by prefer_shan on 15/5/4.
//
//

#ifndef __WarOfGod__BuyProp__
#define __WarOfGod__BuyProp__

#include <stdio.h>
#include "BaseLayer.h"
#include "AllDefine.h"
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
NS_CC_EXT_BEGIN
class CBuyProp:public CBaseLayer
{
public:
    
    //CREATE_FUNC(CBuyProp);
    
    static CBuyProp* create(int index,bool bBox) \
    { \
        CBuyProp *pRet = new(std::nothrow) CBuyProp(index,bBox); \
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
    
    
    //void addProp();
    void  BuProp(cocos2d::Ref *target, cocos2d::SEL_CallFuncO handler);

private:
    bool init();
    
    void soliderCallBack(Ref * sender, cocos2d::extension::Control::EventType controlEvent);
    
    void OnMinus(Ref *pSender);
    void UpdateCoinStone(Ref *pSedner);
    
    void OnPlus(Ref *pSender);
    void OnSure(Ref *pSender);
    void OnUserMcf(Ref *pSender);
    
    void OnCancel(Ref *pSender);
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    void onTouchCancelled(Touch *touch, Event *pEvent);
    EventListenerTouchOneByOne * m_pListener;

    void onExit();
    std::vector<ShopItemInfo*> m_pShopInfo;
    Sprite *m_arrow;
    
    int m_iPropPrize;
    int m_iPropNum;
    int m_index;
    bool m_bBox;
    int m_iCurValue;
    int m_iPropId;
    cocos2d::extension::ControlSlider * m_solider;
    
    bool m_bMagic; //false 金币 true 魔法石
    Label * m_PropNum;
    Label * m_pCostNum;
    Label * m_pSliderValue;
    
    cocos2d::SEL_CallFuncO m_pHandlerBuyProp;//获得的回调
    CC_SYNTHESIZE(cocos2d::Ref*,m_pTarget,Target);//回调函数的目标
    
    Menu * m_pSureMenu;
    Menu * m_pCancleMenu;
    Menu * m_pMenuMinus;
    Menu * m_pMenuPlus;
    
    bool m_bContinerMenu;
    bool m_bSureMenu;
    bool m_bCancleMenu;
    bool m_bMenuMinus;
    bool m_bMenuPlus;
    CBuyProp(int index,bool bBox);
    ~CBuyProp();
};
NS_CC_EXT_END


#endif /* defined(__WarOfGod__BuyProp__) */
