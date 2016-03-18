//
//  LvUpDetail.h
//  TheGodWill
//
//  Created by prefer_shan on 14/11/26.
//
//

#ifndef __TheGodWill__LvUpDetail__
#define __TheGodWill__LvUpDetail__

#include <stdio.h>
#include "cocos2d.h"
#include "AllDefine.h"
#include "SoldierSkillUpLayer.h"
#include "BaseLayer.h"
#include "SkillUpMenuLayer.h"

USING_NS_CC;

class CArmLvUpLayer;
class CSkillUpLayer;

class LvUpDetail:public  CBaseLayer
{
    LvUpDetail(int index,bool bHero=false);
    ~LvUpDetail();
public:
    
    static LvUpDetail* create(CSKillMenuLayer* parent, int index,bool bHero=false) \
    { \
        LvUpDetail *pRet = new LvUpDetail(index,bHero); \
        pRet->m_pSkillMenuLayer = parent;
        
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
    
    void OnShowSkillLV(skillInfo *pSkillInfo,int iNeedTime);
    void OnShowHeroSkillLv(heroSkill *pSkillInfo,int iNeedTime);
    void addSkillItem();
    void showSkillInfo();
    void CreateActionStart(cocos2d::Ref * pTarget,cocos2d::SEL_MenuHandler pHandler);
    cocos2d::Sprite3D *m_pSprite;
    CSkillUpLayer *m_pSkillUpLayer;
    
    
private:
    bool init();
    void RemoveFromTime(int index);
    void addReturnBtn();
    void addDetail();
    void OnTurnLeft(Ref *pSender);
    void OnTurnRight(Ref *pSender);
    void OnMenuLvUp(Ref *pSender);
    void OnReturnBtn(Ref *pSender);
    void OnSoldierLvup(Ref *pSender);
    void OnLeftMenu(Ref *pSender);
    void OnRightMenu(Ref *pSender);
    void OnSkillOneSelect(Ref *pSender);
   
    void onExit();
    void addMiddleLayer();
    void RemoveChildTouch(Ref *pSender);
    void OnCloseMenu(Ref *pSender);
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) ;
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchCancelled(Touch *touch, Event *event);

    void addGuide(Ref *pSender);//添加引导
    void addCloseGuide(Ref *pSender);//添加关闭引导
    
    Sprite * m_pSoldierBg;
    
    cocos2d::Size  m_winSize;
    cocos2d::Ref * m_pTarget;
    Menu * m_pMenuSoldier;
    Menu * m_pMenuSkill;
    Menu * m_pMenuUpNext;
    Menu *m_pNext;
    Menu *m_pMenuClose;
    Menu *m_pMenuFinishCancle;
    Menu *m_pMenuFinishCancle2;
    Menu *m_pMenuFinishCancle3;
    
    bool m_bMenuSoldier;
    bool m_bMenuSkill;
    bool m_bMenuUpNext;
    bool m_bMenuNext;
    bool m_bMenuClose;
    bool m_bFinshCancle;
    bool m_bMenuFinishCancle2;
    bool m_bMenuFinishCancle3;
    
    
    bool m_bHero;
    int m_iTimeNum;
    Label * m_pLvNum;
    Label * m_pUpLvNum;
    Label * m_pHpNum;
    Label * m_pAtkNum;
    Label * m_pUserName;
    
    int m_iToLv;
  
    std::vector<FighterInfo*> m_pSoldierVec;
    std::string m_strMonsSkill1Id;
    std::string m_strMonsSkill2Id;
    std::string m_strMonsSkill3Id;
    
    int m_iIndex;
    int m_iHour;
    int m_iSec;
    int m_iHour2;
    int m_iSec2;
    int m_iHour3;
    int m_iSec3;
    int m_iYanjiuNum;
    int m_iSkillInex;
    
    CC_SYNTHESIZE_RETAIN(CSKillMenuLayer*, m_pSkillMenuLayer, SkillMenuLayer);
    
    int m_iAllSec1;
    int m_iAllSec2;
    int m_iAllSec3;
    
    Label *m_pYanjiuNum;
    Label *m_pLabelLvUpTime;
    Label *m_pLabelLvUpTime2;
    Label *m_pLabelLvUpTime3;
    
    ProgressTimer* m_pProgress1;
    ProgressTimer* m_pProgress2;
    ProgressTimer* m_pProgress3;
    
    LYMenuItemSprite * m_pLeftItem;
    LYMenuItemSprite * m_pRightItem;
  
    EventListenerTouchOneByOne* m_pListner;
    cocos2d::SEL_MenuHandler m_pHandler;
   
    CArmLvUpLayer *m_pArmlayer;
};
#endif /* defined(__TheGodWill__LvUpDetail__) */
