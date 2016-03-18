//
//  SkillLvCellLayer.h
//  WarOfGod
//
//  Created by prefer_shan on 15/9/1.
//
//

#ifndef __WarOfGod__SkillLvCellLayer__
#define __WarOfGod__SkillLvCellLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "AllDefine.h"

//技能升级队列



class  CSkillLvCelLayer:public CBaseLayer
{
    CSkillLvCelLayer(int iTime,int iMonsterId,int iSkillId,int iNeedTime);
    ~CSkillLvCelLayer();

public:
    
    static CSkillLvCelLayer* create(int iTime,int iMonsterId,int iSkillId,int iNeedTime) \
    { \
        CSkillLvCelLayer*pRet = new  CSkillLvCelLayer(iTime,iMonsterId,iSkillId,iNeedTime); \
        if (pRet && pRet->init(iTime,iMonsterId,iSkillId,iNeedTime)) \
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
    
    
    void OnStartLvUp(std::string strMonsterSkill,monsterInfo* pInfo,std::string pSkillInfo, int iNeedTime);
    
    LYMenuItemSprite *m_pFinishItem;
private:
    void RemoveCell();
    
    void OnGetYes(Ref *pSender);
    
    
    void OnUpdateStone(Ref *pSender);
    
    
    void OnGetCancleNo(Ref *pSender);
    
    void OnGeSkillCancleYes(Ref *pSender);
    
    
    void CancleAtOnce(int iTimer);
    
    bool init(int iTime,int iMonsterId,int iSkillId,int iNeedTime);
    
    void update(float time);
    
    void AddSkillFinish(std::string strMonsSkillId);
    
    void OnFinishAtOnce(Ref *pSender);

    void OnCancleLvUp(Ref *pSender);
    
    void  FinishAtOnce(int index);
    
  
    
    
    float  m_fAllTime;
    
    int m_iCnt;// 时间计数
    
    int m_iIndex;
    int m_iHour;
    int m_iMin;
    int m_iSec;
    int m_iAllSec;
    
    int m_iCostMagic;
    
    ProgressTimer * m_pProgress1;

    Label * m_pLabelLvUpTime;
 
    LYMenuItemSprite *m_pCancleLvUpItem;
    Menu * m_pMenuFinishCancle;
    
    std::string m_strMonsSkill1Id;
    int m_iSkillInex;
    
};

#endif /* defined(__WarOfGod__SkillLvCellLayer__) */
