//
//  FightWinLayer.h
//  TheGodWill
//
//  Created by prefer_shan on 14/12/18.
//
//

#ifndef __TheGodWill__FightWinLayer__
#define __TheGodWill__FightWinLayer__

#include <stdio.h>

#include "cocos2d.h"
#include "BaseLayer.h"
#include "LYBeatNumbers.h"
USING_NS_CC;


class smallAwardInfo;

enum{
    RESULT_STAR_ONE = 10,
    RESULT_STAR_TWO,
    RESULT_STAR_THREE,
};

class CFightWinLayer:public CBaseLayer
{
    
public:
    CREATE_FUNC(CFightWinLayer);
    
    virtual void initData();
    void initTowerData();
    virtual void initUI();
    void  AddFightWinNumberAction();
    
    void addStarEffect();
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(Touch *touch, Event *event);
    
    void onExit();
    void onEnterTransitionDidFinish();
    void btnCallBack(Ref *pSender);
    
    void addExp();
    void GoSecond();
    void GoCoinRun();
    
    int judgeIfGetNewHero();//判断是否获得新英雄
    void showNewHero();//显示解锁的新英雄
    void judgeIfUpgrade(int exp);
    void addShowGuide();//添加下一步引导
    void backPvpMainLayer(Ref* pSender);
private:
    void addBetaStar(float ft);
    void addBetaStar2(float ft);
    void addBetastar3(float ft);
    void UpdateRobStone(float ft);
    
    LYMenuItemSprite *m_pBackItem;
    LYMenuItemSprite * m_pAgainItem;
    LYMenuItemSprite* m_pNextItem;
    void OnPvpOK(Ref *pSender);
    
    void addParticleEffect(float ft);
    void AddSucessDonghua();
    void AddWupinDonghua();
    
    void OnRemoveNotice();
    void AddBaozhaStar();
    
    void OnRemoveSucessLayer();
    void OnRemoveWupin();
    void OnUpdateStoneAndCoin(Ref *pSender);
    
    Menu *m_pMenu;
    bool bValid;
    Sprite *m_pSucess;
    
    bool m_bWupin;
    bool m_bJiesuan;
    int  before;
    int  after;
    int m_monsterId;
    cocos2d::Size m_szBg;//背景图片大小
    

    
    
protected:
    
    CC_SYNTHESIZE(int, m_iGotGold, GotGold);
    CC_SYNTHESIZE(int, m_iGotExp, GotExp);
    CC_SYNTHESIZE(int, m_iGotRare, GotRare);
    CC_SYNTHESIZE(std::vector<smallAwardInfo> , m_VecGotItems, GotItems);
    
private:
    CFightWinLayer();
    ~CFightWinLayer();
    bool init();
    void OnCallBack(Ref *pSender);
    void OnFightAgain(Ref *pSender);
    Vec2 m_StarEffectVec2;
    
    bool m_bOnce;
    CBeatNumbers * m_pNumber;
    CBeatNumbers * m_pExpBeatNum;
    CBeatNumbers * m_pKillBetaCoin;
    cocos2d::Size m_winSize;
    EventListenerTouchOneByOne* listen;

    int m_iStartNum;
    int m_iTime;
    int m_iTime2;
    int m_iTime3;
    bool bFlag;
    int m_reSmallLevel;//记录胜利之前的关卡进度
    Vec2 m_guidePos;//pvp引导记录按钮位置
};
#endif /* defined(__TheGodWill__FightWinLayer__) */
