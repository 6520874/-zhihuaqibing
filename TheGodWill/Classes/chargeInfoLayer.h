//
//  chargeInfoLayer.h
//  WarOfGod
//
//  Created by user on 15/5/4.
//
//

#ifndef __WarOfGod__chargeInfoLayer__
#define __WarOfGod__chargeInfoLayer__

#include <stdio.h>

#include "cocos2d.h"
#include "AllDefine.h"
#include "cocos-ext.h"
#include "BaseLayer.h"
USING_NS_CC;
USING_NS_CC_EXT;


enum{
    
    CHARGE_TAG_ONE = 5,
    CHARGE_TAG_TWO,
    CHARGE_TAG_THR,
    CHARGE_TAG_FOUR,
    CHARGE_TAG_FIVE,
    CHARGE_TAG_SIX,
    CHARGE_EXIT,
};

class chargeInfoLayer:public CBaseLayer, public ScrollViewDelegate
{
public:
    chargeInfoLayer();
    ~chargeInfoLayer();
    CREATE_FUNC(chargeInfoLayer);
    virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
    virtual void onTouchMoved(Touch* pTouch, Event* pEvent);
    virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
    virtual void onTouchCancelled(Touch* pTouch, Event* pEvent);
    bool init();
    void addInfo();
    void addBtnMenu();
    void onEnter();
    void onExit();
    void addStoneInfo(const char* nor, const char* sel, Menu* parent, Vec2 pos, int tag);
    void btnCallBack(Ref* pSender);
    void showGoodsInfo();
    void getChargeInfoBack(Ref* pSender);
    //解析字符串
    std::string getDigitFromBackStr(const char* str);
    std::string getStrFromBackStr(const char* str);
    void paymentClick(int userId,int mfscount,float price,float oPrice,int count,std::string productId,std::string productName,
                      std::string payDescription,std::string orderId, std::string goodsInfo);
    void payCallBack();
    void iapSendMessage();//ios支付回调
    void getMessgageBack(Ref* pSender);//服务器回调
private:
    cocos2d::Size m_winSize;
    LayerColor* m_pBgLayer;
    EventListenerTouchOneByOne* listener;
    ScrollView* m_scrollView;
    cocos2d::Vec2 m_start;
    bool m_bScrollView;
    Menu* m_stoneMenu;
    bool m_bStoneMenu;
    Menu* m_pExitMenu;
    bool m_btnMenu;
    Sprite* m_pBg;
    bool m_isMoved;
    std::vector<chargeInfo> m_chargeInfo;
    std::vector<IdAndNum> m_chargeNum;//充值次数
    
    std::vector<IdAndNum> m_tempValue;//临时保存商品信息
    std::string m_goodsInfo;
    chargeInfo* m_pInfo;
    std::string m_order;
};

#endif /* defined(__WarOfGod__chargeInfoLayer__) */
