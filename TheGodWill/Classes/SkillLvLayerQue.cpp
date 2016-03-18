//
//  SkillLvLayerQue.cpp
//  TheGodWill
//
//  Created by prefer_shan on 15/3/4.
//
//

#include "SkillLvLayerQue.h"
#include "DBMgr.h"
#include "sysTime.h"
#include "guiderManagerLayer.h"
#include "CHitDialog.h"
#include "LvUpDetail.h"
#include "SkillUpSucessLayer.h"
#include "CSharedPlist.h"
#include "LYFightManager.h"
#include "Common.h"
#include "NetWorkDataSingle.h"
#include "SkillLvCellLayer.h"


CSKillLvLayerQue::CSKillLvLayerQue(int index,Sprite *pBg)
{
    m_iIndex = index;
    m_pBg = pBg;
    m_iSkillInex = 0;
    m_pYanjiuNum = NULL;
 
    m_pLeftItem = NULL;
    m_pRightItem = NULL;

    m_iYanjiuNum = 0;
  
    m_grayBg = NULL;
 
    
    CSharedPlist::getInstance()->pushPlist("plist/skill.plist");
}

CSKillLvLayerQue::~CSKillLvLayerQue()
{
    
    CSharedPlist::getInstance()->erasePlist("plist/skill.plist");
}

CSKillLvLayerQue* CSKillLvLayerQue::create(int index,Sprite *pBg)
{ \
    CSKillLvLayerQue *pRet = new CSKillLvLayerQue(index,pBg); \
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


void CSKillLvLayerQue::SetSoldierIndex(int index)
{
    m_iIndex = index;
}

bool CSKillLvLayerQue::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
    return true;
}

void CSKillLvLayerQue::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}

void CSKillLvLayerQue::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}

void CSKillLvLayerQue::onTouchCancelled(Touch *touch, Event *event)
{

}

void CSKillLvLayerQue::onExit()
{
    
    CBaseLayer::onExit();
    NotificationCenter::getInstance()->removeObserver(this,"clickUpgrade");
}

void CSKillLvLayerQue::clickUpgradeBtn(Ref *pSender)
{
    NotificationCenter::getInstance()->removeObserver(this,"clickUpgrade");
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 7))
    {

       
        CSkillLvCelLayer* pChild = (CSkillLvCelLayer*)getChildByTag(101);
        
      
        Vec2 pos = pChild->m_pFinishItem->convertToWorldSpaceAR(Vec2::ZERO);
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(getParent(), GUIDE_SHOW_BOTH, pos);
    }
}


void CSKillLvLayerQue::RemoveOnecell(CSkillLvCelLayer *Point)
{
    Vector<CSkillLvCelLayer *>::iterator it = m_pCellAll.begin();
    
    for(;it!= m_pCellAll.end();it++)
    {
        if(*it == Point)
        {
            m_pCellAll.erase(it);
            return;
            
        }
    }
    
}

void CSKillLvLayerQue::SortCell()
{
 
    for(int i=0;i<m_pCellAll.size();i++)
    {
        Sprite *pCell = (Sprite*)m_pCellAll.at(i)->getChildByTag(20);
        
        if(i==0)
        {
            pCell->setPosition(m_winSize.width*0.78,m_winSize.height*0.65);
        }
        else if(i==1)
        {
            pCell->setPosition(m_winSize.width*0.78,m_winSize.height*0.5);
        }
         else if(i==2)
         {
             pCell->setPosition(m_winSize.width*0.78,m_winSize.height*0.35);
         }
 
    }
    
    m_iYanjiuNum = m_pCellAll.size();
    
    char strYanjiuNum[50] = {0};
    sprintf(strYanjiuNum,"研究: %d",m_iYanjiuNum);
    if(m_pYanjiuNum)
    {
        m_pYanjiuNum->setString(strYanjiuNum);
    }
    
}


void CSKillLvLayerQue::OnShowHeroSkillLv(heroSkill *pSkillInfo,int iNeedTime)
{
   
    
    std::vector<CDBMgr::MonsterSkillTime_s> pInfoSkill = CDBMgr::getInstance()->GetSkillUpgradeInfoVec();
    
    myMonster * pMyMonster = CDataManager::getuserMonsterById(m_pHeroVec[m_iIndex]->monsterId);
    
    monsterInfo* pInfo = CDataManager::getMonsterInfoByMonsterId(m_pHeroVec[m_iIndex]->monsterId);
    
    CDataManager::SaveMonsterInfoById(pMyMonster);
  
    
    //+号刷新

    
    char buf[100] = {0};
    sprintf(buf,"%s", pInfo->monsterIcon.c_str());
    
    char strMonsterSkill[50] = {0};
    sprintf(strMonsterSkill,"%d_%d",pInfo->monsterId,pSkillInfo->heroSkillId);
    
    
    CSkillLvCelLayer *pCellLayer = CSkillLvCelLayer::create(pInfoSkill[m_iYanjiuNum].m_iTime,pInfoSkill[m_iYanjiuNum].m_iMonsterId,pInfoSkill[ m_iYanjiuNum].m_iSkillId,pInfoSkill[m_iYanjiuNum].m_iNeedTime);
    addChild(pCellLayer,0,101);
    
    m_pCellAll.pushBack(pCellLayer);
    
    SortCell();

}







void CSKillLvLayerQue::AddSucess(std::string strSKillName,std::string  strImg,bool bHero)
{
    CSKillUpSucessLayer *pLayer = CSKillUpSucessLayer::create(strSKillName,strImg,bHero);
    
    getParent()->
     addChild(pLayer,9900);
    CDataManager::saveTaskInfoToDB(4);
    CDataManager::saveTaskInfoToDB(18, 1);
    
}

int CSKillLvLayerQue::GetSkillIdFromStr(std::string strMonsSkillId)
{
    int index = strMonsSkillId.find("_");
    
    std::string str = strMonsSkillId.substr(index+1,strMonsSkillId.length());
    
    int iSkillId = atoi(str.c_str());
    
    return iSkillId;
}





void CSKillLvLayerQue::OnUpdateStone(Ref *pSender)
{
    //CDataManager::saveUserStoneAndCoinToDB(m_iCostMagic, 1);
    
    
    //CNetWorkDataSingle::
    CDataManager::saveTaskInfoToDB(4);
    CDataManager::saveTaskInfoToDB(18, 1);
}




void CSKillLvLayerQue::OnShowSkillLV(skillInfo *pSkillInfo,int iNeedTime)
{
    
    vector<CDBMgr::MonsterSkillTime_s> pInfoSkill = CDBMgr::getInstance()->GetSkillUpgradeInfoVec();
    
    myMonster * pMyMonster = CDataManager::getuserMonsterById(m_pSoldierVec[m_iIndex]->monsterId);
    monsterInfo* pInfo = CDataManager::getMonsterInfoByMonsterId(m_pSoldierVec[m_iIndex]->monsterId);
    
    
    CDataManager::SaveMonsterInfoById(pMyMonster);
    
   
    
    char buf[100] = {0};
    sprintf(buf,"%s", pInfo->monsterIcon.c_str());
    
    char strMonsterSkill[50] = {0};
    sprintf(strMonsterSkill,"%d_%d",pInfo->monsterId,pSkillInfo->skillId);
    
    //开始训练
    Sprite * thetarget = CFightManager::getTheFlyWord("开始升级","fonts/cuti.ttf");
    thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.7);
    addChild(thetarget,10);

        
    CSkillLvCelLayer *pCellLayer = CSkillLvCelLayer::create(pInfoSkill[m_iYanjiuNum].m_iTime,pInfoSkill[m_iYanjiuNum].m_iMonsterId,pInfoSkill[ m_iYanjiuNum].m_iSkillId,pInfoSkill[m_iYanjiuNum].m_iNeedTime);
    addChild(pCellLayer,0,101);
    
        m_pCellAll.pushBack(pCellLayer);
        
        SortCell();
 

   }

int CSKillLvLayerQue::GetPassTime(int iSkillIndex)
{
    
    vector<CDBMgr::MonsterSkillTime_s> pTime = CDBMgr::getInstance()->GetSkillUpgradeInfoVec();
    
    return 0;
}


void CSKillLvLayerQue::InitYanjiuNum()
{
    vector<CDBMgr::MonsterSkillTime_s> pInfo = CDBMgr::getInstance()->GetSkillUpgradeInfoVec();
    
    Sprite *pFenge = Sprite::createWithSpriteFrameName("divider.png");
    pFenge->setPosition(m_winSize.width*0.8,m_winSize.height*0.2);
    addChild(pFenge);
    

    
    char strYanjiuNum[50] = {0};
    sprintf(strYanjiuNum,"研究: %d",m_iYanjiuNum);
    
    m_pYanjiuNum =  Label::createWithSystemFont(strYanjiuNum,"Arail",20);
    m_pYanjiuNum->setAnchorPoint(Vec2(0,0.5));
    m_pYanjiuNum->setScale(0.5);
    m_pYanjiuNum->setPosition(m_tBg.width*0.88,m_tBg.height*0.25);
    m_pBg->addChild(m_pYanjiuNum);
    
    Label *pYanjiuAllNum = Label::createWithSystemFont("/3","Aril",20);
    pYanjiuAllNum->setScale(0.5);
    pYanjiuAllNum->setAnchorPoint(Vec2(0,0.5));
    pYanjiuAllNum->setPosition(m_pYanjiuNum->getPositionX()+m_pYanjiuNum->getBoundingBox().size.width,m_pYanjiuNum->getPositionY());
    m_pBg->addChild(pYanjiuAllNum);
    
    
    size_t iSize = pInfo.size();

    m_pCellAll.clear();
    
    for(int i=0;i<iSize;i++)
    {
        CSkillLvCelLayer *pCellLayer = CSkillLvCelLayer::create(pInfo[i].m_iTime,pInfo[i].m_iMonsterId,pInfo[i].m_iSkillId,
                                                                
        pInfo[i].m_iNeedTime);
        
        addChild(pCellLayer);

        m_pCellAll.pushBack(pCellLayer);
       
    }
   
    SortCell();
    
}

bool SortByRare(const FighterInfo *v1,const FighterInfo *v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致
{
    
    if(v1->rare == v2->rare)
    {
        return v1->level>v2->level;
        
    }
    return v1->rare>v2->rare;
}



bool CSKillLvLayerQue::init()
{
    CBaseLayer::init();
    
    m_plistener = EventListenerTouchOneByOne::create();
    m_plistener->setSwallowTouches(true);
    
    m_tBg = m_pBg->getContentSize();
    
    std::vector<FighterInfo*>  pAll;
    
    pAll = CPersonalData::getInstance()->getAllSoldierInfo();
    
    for(int i=0;i<pAll.size();i++)
    {
        //1 英雄  0怪物
        {
            if(pAll[i]->theType == 1)
            {
                m_pHeroVec.push_back(pAll[i]);
                
            }
        }
        
        {
            if(pAll[i]->rare>0)
            {
                m_pSoldierVec.push_back(pAll[i]);
            }
        }
    }

    
    std::sort(m_pSoldierVec.begin(),m_pSoldierVec.end(),SortByRare);
    
    InitYanjiuNum();
    //scheduleUpdate();
    

    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 5))
    {
        NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CSKillLvLayerQue::clickUpgradeBtn), "clickUpgrade", NULL);
    }
    return true;
}
