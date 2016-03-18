//
//  SkillLvLayerQue.h
//  TheGodWill
//
//  Created by prefer_shan on 15/3/4.
//
//

#ifndef __TheGodWill__SkillLvLayerQue__
#define __TheGodWill__SkillLvLayerQue__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "AllDefine.h"

//技能升级队列

enum{
   
     SKILL_OPEN_TAG = 100,
     SKILL_SUCESS_TAG =99000,
    
};

class  CSkillLvCelLayer;

class  CSKillLvLayerQue:public CBaseLayer
{
public:
    static CSKillLvLayerQue* create(int index,Sprite *pBg=NULL);
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) ;
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
    
    void SetSoldierIndex(int index);
    bool init();
    void InitYanjiuNum();

    void OnShowSkillLV(skillInfo *pSkillInfo,int iNeedTime);
    void OnShowHeroSkillLv(heroSkill *pSkillInfo,int iNeedTime);
     void SortCell();
    void RemoveOnecell(CSkillLvCelLayer *Point);
    void AddSucess(std::string strSKillName,std::string  strImg,bool bHero);
    
private:
    void StartTouch();
    void EndTouch();
 

    int  GetSkillIdFromStr(std::string strMonsSkillId);
    void onExit();
    void OnUpdateStone(Ref *pSender);
    
    
    //获取第几个技能的剩余时间
    int GetPassTime(int iSkillIndex);
    
    
    void OnRemoveNotice();
    void OnRemoveAll();

    void clickUpgradeBtn(Ref *pSender);//点击升级按钮之后的响应
    
    CSKillLvLayerQue(int index,Sprite *pBg);
    ~CSKillLvLayerQue();
    

    
    int m_iTimeNum;
    Label * m_pLvNum;
    Label * m_pUpLvNum;
    Label * m_pHpNum;
    Label * m_pAtkNum;
    Label * m_pUserName;
    int m_iToLv;
    
    Sprite *m_grayBg;
    
    Sprite *m_pBg;
    
    Size m_tBg;
    
    std::vector<FighterInfo*> m_pSoldierVec;
    
    std::vector<FighterInfo*>m_pHeroVec;
    
    int m_iIndex;
    int m_iYanjiuNum;
    int m_iSkillInex;
    

    
    Label *m_pYanjiuNum;

    

    LYMenuItemSprite * m_pLeftItem;
    LYMenuItemSprite * m_pRightItem;


    EventListenerTouchOneByOne * m_plistener;
    Vector<CSkillLvCelLayer *>  m_pCellAll;
    
};
#endif /* defined(__TheGodWill__SkillLvLayerQue__) */
