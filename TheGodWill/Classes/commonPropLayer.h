//
//  CPropCommonLayer.h
//  TheGodWill
//
//  Created by user on 14/12/25.
//
//

#ifndef __TheGodWill__CPropCommonLayer__
#define __TheGodWill__CPropCommonLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "BaseLayer.h"
#include "AllDefine.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum{
    DEBRIS_BUY_TYPE = 100,//购买碎片
    HERO_ADVANCED_TYPE,//进阶
    LEVEL_UPGRADE_TYPE,//等级提升
    USER_CHEST_TYPE,
    SALE_PROP_TYPE,
    GAME_PAUSE_TYPE,
    LEVEL_STAR_TYPE,
    GAME_SKILL_BLOW,
    GAME_SKILL_CUT,
    TOWER_MODE_INTRODUCE,//塔防模式关卡介绍
    STONE_EXCHANGE_COIN,//魔法石兑换金币
    STONE_EXCHANGE_STAMINA,//魔法石兑换体力
    GAME_RMV_SOLIDER,//删除士兵
};

enum
{
    BUY_SURE_TAG = 0,
    BUY_CANCEL_TAG,
    BUY_NUM_ADD_TAG,
    BUY_NUM_MINUS_TAG,
    ADVANCE_SURE_TAG,
    UPGRADE_SUER_TAG,
    USERPROP_SURE_TAG,
    GAME_EXIT_TAG,
    GAME_CONTINUE_TAG,
    GAME_RESTART_TAG,
    CLICK_EXIT_TAG,
    EXCHANGE_COIN_TAG,
    EXCHANGE_STAMINA_TAG,
    EXCHANGE_CANCEL_TAG,
    EXCHANGE_SHOP_CANCLE_TAG,
    EXCHANGE_SHOP_OK_TAG,
};

enum
{
    GONXI_TAG = 100,
    BackLight_TAG,
    BgKuang_TAG,
    SmallBgKauang_TAG,
};
class CommonPropLayer:public CBaseLayer
{
public:
    CommonPropLayer();
    ~CommonPropLayer();
    CREATE_FUNC(CommonPropLayer);
    static CommonPropLayer* create(int type, bool isLevelEnd = false);
    static CommonPropLayer* create(int type,int monsterId,bool bProp,int iNum=1);//如果是卖出道具则w为卖出数量
    static CommonPropLayer* create(int type,std::string strText);
    static CommonPropLayer* create(int type,int levDown, int levUp);
    void onExit();
    bool init();
    void initUI();
    void initBuyDebris();
    void initHeroAdvance();
    void setScucessCallBack(cocos2d::Ref * pTarget,cocos2d::SEL_MenuHandler pHandler);  //设置成功回调
    void setFailCallBack(cocos2d::Ref * pTarget,cocos2d::SEL_MenuHandler pHandler);    //设置失败回调
    
    void salePropDialog(cocos2d::Ref * pTarget,cocos2d::SEL_MenuHandler pHandler,std::string sContent, int num = 1);
    void rmvSolider(cocos2d::Ref * pTarget,cocos2d::SEL_MenuHandler pHandler,int num);//移除士兵回调
    void pauseGame();
    void initLevelStarInfo();
    void showTowerLevelInfo();//显示塔防模式的介绍
    void showExchangeInfo();//显示魔法石兑换金币
    void showExchangeStamina();//显示魔法石兑换体力
    void exchangeCallBack(Ref* pSender);//兑换回调
    
    void addSkillEffect();
    void postMessage();
    
    void upgradeLevel();
    void btnCallBack(Ref* pSender);
    void soliderCallBack(Ref * sender, Control::EventType controlEvent);
    void addBtn();
    void checkBtnEnabled(ControlSlider* slider);
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    
    void gotoShop(Ref* pSender);//进入商店重置魔法石
    void initUserChestUI();
    
    
    void postGuideInfo();
    
    void OnSure(Ref *pSender);
    void OnCancel(Ref *pSender);
    
    void lightEffect(Node *pTf);
    void startShowParticle();
    void randTime(float dt);
    void remove();
    
    void addParticleEffect(float dt);//添加升级例子特效

private:
    cocos2d::Size m_winSize;
    //ProgressTimer* m_progress;
    Label* m_pSliderValue;
    Label* m_buyNum;//购买数量
    Label* m_costNum;//价格
    Sprite* m_arrow;
    Sprite* m_pBg;
    LayerColor* m_pLayer;//购买碎片透明背景
    Sprite* m_bg;//进阶背景特殊处理
    Menu* m_menu;
    int m_iPropNum;
    int m_type;
    int monsterId;//怪物id、道具id
    int m_rare;//星级
    int m_availNum;//现有材料数量
    std::string m_strText;//显示的内容
    MonsterEvolutionInfo* m_monster;
    
    bool m_isLevelEnd;//当前关卡是否结束
    int m_beforeLevel;//当前等级
    int m_afterUpgrade;//升级后的等级
    int m_sliderValue;//滑块原有的值
    int m_stone;//兑换所需魔法石
    int m_addCoin;//兑换的金币
    int m_addStamina;
    
    cocos2d::Ref * m_pTarget;    //确认的回调
    cocos2d::SEL_MenuHandler m_pHandler;
    EventListenerTouchOneByOne* listen;
    
    cocos2d::Ref * m_pCancelTarget;   //取消的回调
    cocos2d::SEL_MenuHandler m_pCancelHandler;
    
    //ParticleSystemQuad* m_effect;
};

#endif /* defined(__TheGodWill__CPropCommonLayer__) */
