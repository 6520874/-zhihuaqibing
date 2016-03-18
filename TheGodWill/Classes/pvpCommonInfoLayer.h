//
//  pvpCommonInfoLayer.h
//  WarOfGod
//
//  Created by user on 15/6/15.
//
//

#ifndef __WarOfGod__pvpCommonInfoLayer__
#define __WarOfGod__pvpCommonInfoLayer__

#include <stdio.h>
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "AllDefine.h"
#include "PublicDefine.h"


USING_NS_CC;
USING_NS_CC_EXT;
enum MSGTYPE
{
    PVP_COMMSG_DETAIL = 0,//详情
    PVP_COMMSG_UPGRADE,
    PVP_COMMSG_NOT_UPGRADE,//不能升级提示
    PVP_COMMSG_TRAIN,
    PVP_COMMSG_UPGRADE_TIP,
    PVP_COMMSG_CANCEL_UPGRADE,//取消升级提示
    PVP_COMMSG_ACQUIRE_AWARD,//领取奖励
    PVP_COMMSG_PRORUCT,//熔炼
    PVP_COMMSG_DINGSHI, //定时设置
    PVP_COMMSG_EVOLUTION,//进化
    PVP_COMMSG_PROTECTTIME,//取消保护时间提示
    PVP_COMMSG_NOFIND,//未搜到玩家提示
};

enum{
    BTN_EXIT_TAG = 0,
    BTN_FINISH_TAG,
    BTN_UPGRADE_TAG,
    BTN_CANCEL_UPGRADE,//退出取消升级
    BTN_OK_UPGRADE,//确定取消升级
    BTN_GET_AWARD,//领取奖励
    BTN_GET_AWARD_EXIT,//领取奖励退出
    PRODUCT_START_TAG,//开始熔炼
    PRODUCT_ADD_TAG,
    PRODUCT_MINUS_TAG,
    BTN_EVOLUTION_TAG,//进化
};

class pvpCommonInfoLayer:public CBaseLayer
{
public:
    pvpCommonInfoLayer();
    ~pvpCommonInfoLayer();
    static pvpCommonInfoLayer* create(int buildingId, MSGTYPE type, std::string content = "");//最后一个参数表示主基地等级，只在升级时用于判断
    static pvpCommonInfoLayer* create(int buildingId, int userBuildingId, MSGTYPE type);
    static pvpCommonInfoLayer* create(MSGTYPE type);
    bool init();
    void initUI();
    void detailAndUpgradeUI();
    void showDingshiInfo();
    
    void OnOkDinshi(Ref *pSender);
    
    void addJiTanPro();//添加祭坛巡逻者的属性
    void showTowerDetail();//显示建筑基本详情
    void showUpgradeBtn();//显示升级信息
    void showProductInfo();//显示熔炼情况
    void productInfoBack(Ref* pSender);//开始熔炼之前界面
    void addProductProcess();
    void checkBtnEnabled(ControlSlider* slider);
    void addProvessInfo();//显示开始熔炼情况
    void soliderCallBack(Ref * sender, Control::EventType controlEvent);
    void addCostStoneTip(Ref* pTarget, SEL_MenuHandler phander, std::string content, int stone);//花费魔法石提示
    void addCancelUpgrade(Ref* pTarget, SEL_MenuHandler phander);//取消升级提示
    void addAcquireAward(Ref* pTarget, SEL_MenuHandler phander, int staminaNum);//取消升级提示
    
    void addEvolutionInfo();//显示进化信息
    void clearProtectTime(Ref* pTarget, SEL_MenuHandler phander);//清空保护时间
    void showSearchResult(Ref* pTarget, SEL_MenuHandler pHander);//未搜到玩家的提示
    
    bool onTouchBegan(Touch* pTouch, Event* pEvent);
    void onTouchMoved(Touch* pTouch, Event* pEvent);
    void onTouchEnded(Touch* pTouch, Event* pEvent);
    
    void btnCallBack(Ref* pSender);
    void updateUserData(Ref *pSender);//更新用户金币等
    void startProductBack(Ref *pSender);
    void isEndProductBack(Ref *pSender);//判断是否熔连结束回调
    void gotoShop(Ref* pSender);//进入充值界面
    
    void OnSure(Ref *pSender);
    void OnCancel(Ref *pSender);
    void onExit();
    void showGuideInfo();
    void soliderCallBackDinshi(Ref * sender, cocos2d::extension::Control::EventType controlEvent);
    
    void update(float dt);//熔炼计时
    std::string calculateNeedTime(int allTime);
    std::string calculateTime(int allTime);//时间换算
private:
    
    
    //定时炸弹逻辑
    Label * m_pDinshiTime;
    
    ControlSlider * m_DinshiTime;
    
    
    int m_iDinshiVaule;
    
    //定时炸弹
    EventListenerTouchOneByOne* m_listener;
    Sprite* m_comBg;
    LayerColor* m_pLayer;
    int m_buildingId;
    int m_msgType;
    int m_productNum;//用于熔炼成功后现实的数量
    std::string m_content;
    pvpBuildInfo* m_buildInfo;
    pvpBuildInfo* m_pNextBuildInfo;//下一级建筑信息
    
    cocos2d::Ref * m_pTarget;    //确认的回调
    cocos2d::SEL_MenuHandler m_pHandler;
    
    cocos2d::Ref * m_pCancelTarget;   //取消的回调
    cocos2d::SEL_MenuHandler m_pCancelHandler;
    
    //关于熔炼
    int m_userBuildingId;
    int m_sliderValue;//滑块的当前值
    Label* m_pCost;//消耗的矿石数量
    Label* m_pGet;//获取的魔法石数量
    Sprite* m_arrow;//滑块上的箭头
    Node* m_node;
    ProgressTimer* m_progress;
    Label* m_pSliderValue;
    Sprite* m_pStone;
    Sprite* m_pKuangshi;
    Menu* m_menu;
    int m_needTime;//熔炼所需时间
    Label* m_pNeedTime;//熔炼所需时间
    Sprite* m_pClock;//闹钟
    Label* m_pTime;//显示剩余时间
    int m_remainTime;//熔连结束剩余时间
    Label* m_pNum;//当前矿石数量
    Label* m_pLimit;//最大熔炼个数
    float m_time;//
    bool m_flag;
    Menu* m_productMenu;//熔炼按钮
    Vec2 m_guidePos;
};


#endif /* defined(__WarOfGod__pvpCommonInfoLayer__) */
