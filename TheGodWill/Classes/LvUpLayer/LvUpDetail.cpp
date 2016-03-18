//
//  LvUpDetail.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/11/26.
//
//

#include "LvUpDetail.h"
#include "SkillUpLayer.h"
#include "LYPersonalData.h"
#include "DataManager.h"
#include "DBMgr.h"
#include "CSKillLVUpQueen.h"
#include "HeroSkillUpLayer.h"
#include "sysTime.h"
#include "CHitDialog.h"
#include "guiderManagerLayer.h"
#include "SkillLvLayerQue.h"
#include "SkillUpMenuLayer.h"
#include "LYFightManager.h"

#define  SKILLMINUS 0
#define  SKILLSEC   10

LvUpDetail::LvUpDetail(int index,bool bHero)
{
    m_iIndex = index;
    m_winSize = Director::getInstance()->getVisibleSize();
    m_bMenuSkill = false;
    m_bMenuSoldier = false;
    m_bMenuClose = false;
    m_pArmlayer = NULL;
    m_pMenuSkill = NULL;
    m_pLvNum = NULL;
    m_iToLv = 1;
    m_pLabelLvUpTime = NULL;
    m_pLabelLvUpTime2 = NULL;
    m_pLabelLvUpTime3 = NULL;
 
    m_bFinshCancle = false;
    m_iHour = 0;
    m_iSec = 0;
    m_iHour2 = 0;
    m_iSec2 = 0;
    m_iHour3 = 0;
    m_iSec3 =0;
    m_iTimeNum= 0;
    m_iSkillInex = 0;
  
    m_pProgress1 = NULL;
    m_pProgress2 = NULL;
    m_pProgress3 = NULL;

    m_bMenuFinishCancle2 = false;
    m_bMenuFinishCancle3 = false;
    m_pMenuFinishCancle = NULL;
    m_pMenuFinishCancle2 = NULL;
    m_pMenuFinishCancle3 = NULL;
    m_iAllSec1 = 0;
    m_iAllSec2 = 0;
    m_iAllSec3 = 0;
    m_bHero = bHero;
    m_pSoldierBg = NULL;
    m_pSkillMenuLayer = NULL;
    m_pSkillUpLayer = NULL;
    
}

LvUpDetail::~LvUpDetail()
{
    m_pSkillUpLayer = NULL;
    
}

void LvUpDetail::onExit()
{
    _eventDispatcher->removeEventListener(m_pListner);
    CBaseLayer::onExit();
    NotificationCenter::getInstance()->removeObserver(this, "upgradeGuide");
    NotificationCenter::getInstance()->removeObserver(this,"upgradeCloseGuide");
}

void LvUpDetail::CreateActionStart(Ref * pTarget,cocos2d::SEL_MenuHandler pHandler)
{
    m_pTarget = pTarget;
    m_pHandler = pHandler;
}

void  LvUpDetail::OnReturnBtn(Ref *pSender)
{
    removeFromParent();
}


void  LvUpDetail::addReturnBtn()
{
    
}

void LvUpDetail::OnSoldierLvup(Ref *pSender)
{
    
}

void LvUpDetail::addDetail()
{
    
    Size tBg = m_pSoldierBg->getContentSize();
    
    Sprite *pLvUp = Sprite::createWithSpriteFrameName("lvqueen.png");
    pLvUp->setScale(0.5);
    pLvUp->setPosition(tBg.width*0.88,tBg.height*0.8);
    m_pSoldierBg->addChild(pLvUp);
    
    Label *pLvUpQueue = Label::createWithSystemFont("升级队列","Arial",30);
    Size tLv = pLvUpQueue->getContentSize();
    pLvUpQueue->setPosition(Vec2(tLv.width*1.2,tLv.height*0.6));
    pLvUp->addChild(pLvUpQueue);
    
    m_iYanjiuNum = 0;
    
    //兵种名字拉动框
    Sprite *pSoldierName = Sprite::createWithSpriteFrameName("select_kuang.png");
    pSoldierName->setPosition(Vec2(m_winSize.width*0.5,m_winSize.height*0.15));
    pSoldierName->setScale(1.5);
    addChild(pSoldierName,1);

    Size tSoldierName = pSoldierName->getContentSize();
    monsterInfo* pMonsterInfo  = CDataManager::getMonsterInfoByMonsterId(m_pSoldierVec[m_iIndex]->monsterId);
    m_pUserName = Label::createWithSystemFont(pMonsterInfo->monsterName,"Arial",30);
    m_pUserName->setScale(0.6);
    m_pUserName->setPosition(Vec2(tSoldierName.width*0.5,tSoldierName.height*0.5));
    pSoldierName->addChild(m_pUserName);
    
    //左箭头
    Sprite *pLeftArrow = Sprite::createWithSpriteFrameName("arrow.png");
    pLeftArrow->setFlippedX(true);
    
    Sprite *pLeftArrowDisabale = Sprite::createWithSpriteFrameName("arrow.png");
    pLeftArrowDisabale->setFlippedX(true);
    pLeftArrowDisabale->setColor(Color3B::GRAY);
    
    
    m_pLeftItem = LYMenuItemSprite::create(pLeftArrow,NULL,pLeftArrowDisabale,CC_CALLBACK_1(LvUpDetail::OnLeftMenu, this), "music/back.mp3");
    m_pMenuUpNext = Menu::create(m_pLeftItem,NULL);
    

    m_pMenuUpNext->setPosition(Vec2(m_winSize.width*0.32,m_winSize.height*0.15));
    addChild(m_pMenuUpNext);
    
    //右箭头
    Sprite *pRightArrow = Sprite::createWithSpriteFrameName("arrow.png");
    
    Sprite *pRightArrowDisable  = Sprite::createWithSpriteFrameName("arrow.png");
    pRightArrowDisable ->setColor(Color3B::GRAY);
    
    m_pRightItem = LYMenuItemSprite::create(pRightArrow,NULL,pRightArrowDisable,CC_CALLBACK_1(LvUpDetail::OnRightMenu, this), "music/next.mp3");
    m_pNext = Menu::create(m_pRightItem,NULL);
    m_pNext->setPosition(Vec2(m_winSize.width*0.68,m_winSize.height*0.15));
    
    addChild(m_pNext);

    if((m_iIndex-1)<0)
    {
        m_pLeftItem->setEnabled(false);
    }
    
    if((m_iIndex+1)>=m_pSoldierVec.size())
    {
        m_pRightItem->setEnabled(false);
    }

    
    m_pMenuSoldier = Menu::create(NULL);
    m_pMenuSoldier->setPosition(m_winSize.width*0.8,m_winSize.height*0.15);
    addChild(m_pMenuSoldier);
    
    addReturnBtn();
    addMiddleLayer();
}



void LvUpDetail::OnLeftMenu(Ref *pSender)
{
    log("left Menu");
    m_pRightItem->setEnabled(true);
    char str[100]  ={0};

    LYMenuItemSprite *p =  (LYMenuItemSprite*) pSender;
    
    if( m_iIndex>0)
    {
        m_iIndex--;
        CSKillLvLayerQue *pSkillLayer =  (CSKillLvLayerQue*)getChildByTag(9000);
        if(pSkillLayer)
        {
            pSkillLayer->SetSoldierIndex(m_iIndex);
        }
        sprintf(str,"%d",m_pSoldierVec[m_iIndex]->level);
        m_pLvNum->setString(str);
        monsterInfo* pMonsterInfo  = CDataManager::getMonsterInfoByMonsterId(m_pSoldierVec[m_iIndex]->monsterId);
        m_pUserName->setString(pMonsterInfo->monsterName.c_str());
        cocos2d::__String   sModelName =   m_pSoldierVec[m_iIndex]->theModelInfo->modelName;
        
        m_pSprite->removeFromParent();
        
        m_pSprite = Sprite3D::create(sModelName.getCString());
        m_pSprite->setGlobalZOrder(100);
        auto s = Director::getInstance()->getWinSize();
        m_pSprite ->setPosition(Vec2(s.width * 2.5f / 5.f, s.height / 3.f));
        addChild(m_pSprite,1);
        
        m_pSprite->setScale(30);
    
        if(m_pSoldierVec[m_iIndex]->monsterId == 1004)
        {
            m_pSprite->setScale(50);
        }
        else if(m_pSoldierVec[m_iIndex]->monsterId == 10 ||m_pSoldierVec[m_iIndex]->monsterId == 6||m_pSoldierVec[m_iIndex]->monsterId == 9)
        {
            m_pSprite->setScale(15);
        }

        else if(m_pSoldierVec[m_iIndex]->monsterId == 16)
        {
            m_pSprite->setScale(0.8);
        }
        auto animation = Animation3D::create(sModelName.getCString());
        if (animation)
        {
            auto animate = Animate3D::create(animation,  m_pSoldierVec[m_iIndex]->theModelInfo->idleFrame.start/60,m_pSoldierVec[m_iIndex]->theModelInfo->idleFrame.duration/60);
            cocos2d::Action* _swim = RepeatForever::create(animate);
            m_pSprite->runAction( animate);
        }
        
        sprintf(str,"%d",m_pSoldierVec[m_iIndex]->maxHP);
        m_pHpNum->setString(str);
        
        sprintf(str,"%d",m_pSoldierVec[m_iIndex]->attk);
        m_pAtkNum->setString(str);
        p->setEnabled(true);
        
        //技能切换
        getChildByTag(1001)->removeFromParent();
        addSkillItem();
    }
    
    if((m_iIndex-1)<0)
    {
        m_pLeftItem->setEnabled(false);
    }
    
    if((m_iIndex+1)>=m_pSoldierVec.size())
    {
        m_pRightItem->setEnabled(false);
    }
}


void LvUpDetail::OnRightMenu(Ref *pSender)
{
    log("Right Menu");

    LYMenuItemSprite *p =  (LYMenuItemSprite*) pSender;
     m_pLeftItem->setEnabled(true);
    char str[100]  ={0};
    if( m_iIndex+1<m_pSoldierVec.size())
    {
        m_iIndex++;
        
        CSKillLvLayerQue *pSkillLayer =  (CSKillLvLayerQue*)getChildByTag(9000);
        pSkillLayer->SetSoldierIndex(m_iIndex);
        
        sprintf(str,"%d",m_pSoldierVec[m_iIndex]->level);
        m_pLvNum->setString(str);
        monsterInfo* pMonsterInfo  = CDataManager::getMonsterInfoByMonsterId(m_pSoldierVec[m_iIndex]-> monsterId);
        m_pUserName->setString(pMonsterInfo->monsterName.c_str());
        cocos2d::__String   sModelName =   m_pSoldierVec[m_iIndex]->theModelInfo->modelName;
        
        m_pSprite->removeFromParent();
        
        m_pSprite = Sprite3D::create(sModelName.getCString());
        m_pSprite->setGlobalZOrder(100);
        auto s = Director::getInstance()->getWinSize();
        m_pSprite ->setPosition(Vec2(s.width * 2.5f / 5.f, s.height / 3.f));
        addChild(m_pSprite,1);
        
        m_pSprite->setScale(30);
        
      
        if(m_pSoldierVec[m_iIndex]->monsterId == 1004)
        {
            m_pSprite->setScale(50);
        }
        else if(m_pSoldierVec[m_iIndex]->monsterId == 10 ||m_pSoldierVec[m_iIndex]->monsterId == 6||m_pSoldierVec[m_iIndex]->monsterId == 9)
        {
            m_pSprite->setScale(15);
        }

        else if(m_pSoldierVec[m_iIndex]->monsterId == 16)
        {
             m_pSprite->setScale(0.8);
        }
        
        auto animation = Animation3D::create(sModelName.getCString());
        if (animation)
        {
            auto animate = Animate3D::create(animation,m_pSoldierVec[m_iIndex]->theModelInfo->idleFrame.start/60, m_pSoldierVec[m_iIndex]->theModelInfo->idleFrame.duration/60);
            cocos2d::Action* _swim = RepeatForever::create(animate);
            m_pSprite->runAction(_swim);
        }
      
        sprintf(str,"%d",m_pSoldierVec[m_iIndex]->maxHP);
        m_pHpNum->setString(str);
        
        sprintf(str,"%d",m_pSoldierVec[m_iIndex]->attk);
        m_pAtkNum->setString(str);
        p->setEnabled(true);
        
        //技能切换
        getChildByTag(1001)->removeFromParent();
        addSkillItem();
    }
   
    if((m_iIndex-1)<0)
    {
        m_pLeftItem->setEnabled(false);
    }

    if((m_iIndex+1)>=m_pSoldierVec.size())
    {
        m_pRightItem->setEnabled(false);
    }
    
}

void LvUpDetail::RemoveChildTouch(Ref *pSender)
{
    
}

void LvUpDetail::OnSkillOneSelect(Ref *pSender)
{
    LYMenuItemSprite *p = (LYMenuItemSprite*)pSender;
    
    int iTag = p->getTag();

    vector<CDBMgr::MonsterSkillTime_s> pSize = CDBMgr::getInstance()->GetSkillUpgradeInfoVec();
    
    int iRare = m_pSoldierVec[m_iIndex]->rare;
    
    int iMonsterId = m_pSoldierVec[m_iIndex]->monsterId;
    

    if(iTag == 0)
    {
        
    }
    else if(iTag == 1)
    {
        if(iMonsterId<1000)
        {
        if(iRare<=iTag)
        {

            Sprite * thetarget = CFightManager::getTheFlyWord("2星开放此技能!","fonts/cuti.ttf");
            thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height*0.7));
            addChild(thetarget, 100);

            return ;
        }
        }
            
    }
    else if(iTag == 2)
    {
        if(iMonsterId<1000)
        {
            if(iRare<=iTag)
            {
                Sprite * thetarget = CFightManager::getTheFlyWord("3星开放此技能!","fonts/cuti.ttf");
                thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height*0.7));
                addChild(thetarget, 100);
                return ;
            }
        }
    }
   
   
    else if(iTag == 3)
    {
        if(iMonsterId<1000)
        {
            
            
            if(iRare<=iTag)
            {
                
                Sprite * thetarget = CFightManager::getTheFlyWord("4星开放此技能!","fonts/cuti.ttf");
                thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height*0.7));
                addChild(thetarget, 100);
                return ;
            }
        }
    }
    

    
    log("iIndex = %d\n",m_iIndex);
    int iSize = pSize.size();
    
    m_pSprite->setVisible(false);
    
    m_pSkillUpLayer = CSkillUpLayer::create(m_iIndex,iTag,iSize,m_bHero);
    addChild(m_pSkillUpLayer,1000);
}



void LvUpDetail::addMiddleLayer()
{
    int iHeight = m_winSize.height*0.75;
    
    //等级
    char str[100]  ={0};
    sprintf(str,"%d",m_pSoldierVec[m_iIndex]->level);
    
    Label *pLv =  Label::createWithTTF("等级:","fonts/cuti.ttf",30);
    pLv->setPosition(m_winSize.width*0.16,iHeight+50);
    addChild(pLv,2);
    pLv->setColor(Color3B(223,196,17));
    
    m_pLvNum =   Label::createWithTTF(str,"fonts/cuti.ttf",30);
    m_pLvNum->setPosition(pLv->getPositionX()+pLv->getContentSize().width,iHeight+50);
    
    addChild(m_pLvNum,2);
    
    //生命值
    Label *pHp =   Label::createWithTTF("生命值:","fonts/cuti.ttf",30);
    pHp->setPosition(m_winSize.width*0.17,iHeight);
    pHp->setColor(Color3B(116,205,51));
    addChild(pHp,2,100);
    
    sprintf(str,"%d",m_pSoldierVec[m_iIndex]->maxHP);
    
    m_pHpNum = Label::createWithTTF(str,"fonts/cuti.ttf",30);
  
    m_pHpNum->setPosition(pHp->getPositionX()+pHp->getContentSize().width,iHeight);
    addChild(m_pHpNum,2);
    
    //攻击力
    Label *pAtk = Label::createWithTTF("攻击力:","fonts/cuti.ttf",30);
    pAtk->setPosition(m_winSize.width*0.17,iHeight-50);
    pAtk->setColor(Color3B(226,98,13));
    addChild(pAtk,2);
    
    sprintf(str,"%d",m_pSoldierVec[m_iIndex]->attk);
    
    m_pAtkNum = Label::createWithTTF(str,"fonts/cuti.ttf",30);
    m_pAtkNum->setPosition(pAtk->getPositionX()+pAtk->getContentSize().width,iHeight-50);

    addChild(m_pAtkNum,2);
    
    addSkillItem();
}

void LvUpDetail::addSkillItem()
{
    Menu * p = (Menu *)getChildByTag(1001);
    
    if(p)
    {
        p->removeFromParent();
    }
    
    int iMonsterid = m_pSoldierVec[m_iIndex]->monsterId;
    int iRare = m_pSoldierVec[m_iIndex]->rare;

    std::vector<skillInfo*>  m_pskill = CDataManager::getAvailableSkillInfo(iMonsterid,5);
    
    LYMenuItemSprite *pMenuSkillOneItem = NULL;
    LYMenuItemSprite *pMenuSkillTwoItem = NULL;
    LYMenuItemSprite *pMenuSkillThreeItem = NULL;
    LYMenuItemSprite *pMenuSkillFourItem = NULL;
   
    bool bHero = false;
    
    if(iMonsterid>1000)
    {
        bHero = true;
    }
    
    if(bHero)
    {
        vector<heroSkill*> pVec = CDBMgr::getInstance()->getHeroSkillByMonsterId(iMonsterid);
        
        
        
        vector<heroSkill*> pHeroSkillInfo;
        
        for(int i=0;i<pVec.size();i++)
        {
            
            if(pVec[i]->heroSkillId !=1)
            {
                pHeroSkillInfo.push_back(pVec[i]);
                
            }
        }
        
        
        
        for(int i=0;i<pHeroSkillInfo.size();i++)
        {
           
            char strIcoPath[50] = {0};
            sprintf(strIcoPath,"%s",pHeroSkillInfo[i]->skillIcon.c_str());
            
            Sprite *pSkill = Sprite::createWithSpriteFrameName(strIcoPath);
            Sprite *pSkillSelect = Sprite::createWithSpriteFrameName(strIcoPath);
            pSkillSelect->setColor(Color3B::GRAY);
            
            Sprite *pSkillDisable = Sprite::createWithSpriteFrameName(strIcoPath);
            pSkillDisable->setColor(Color3B::GRAY);
            
            if(i==0)
            {
                pMenuSkillOneItem = LYMenuItemSprite::create(pSkill,pSkillSelect,pSkillDisable,CC_CALLBACK_1(LvUpDetail::OnSkillOneSelect, this));
                pMenuSkillOneItem->setTag(0);
                pMenuSkillOneItem->setPosition(m_winSize.width*0.35,m_winSize.height*0.6-m_winSize.height*0.3*i);
                pMenuSkillOneItem->setScale(3);
                
                int iSkillLv = CDBMgr::getInstance()->GetHeroSkillLvById(pHeroSkillInfo[i]->heroSkillId);
            
                bool  bGreenPlus = CDBMgr::getInstance()->IsSkillCanLvUp(iMonsterid,pHeroSkillInfo[i]->heroSkillId,true,iSkillLv);
                if(bGreenPlus)
                {
                    Size tSkillIco = pMenuSkillOneItem->getContentSize();
                    Sprite *pGreenPlus = Sprite::createWithSpriteFrameName("plus.png");
                    pGreenPlus->setScale(0.25);
                    pGreenPlus->setPosition(tSkillIco.width*0.25,tSkillIco.height*0.8);
                    pMenuSkillOneItem->addChild(pGreenPlus);
                }
            }
            else if(i==1)
            {
                pMenuSkillTwoItem = LYMenuItemSprite::create(pSkill,pSkillSelect,pSkillDisable,CC_CALLBACK_1(LvUpDetail::OnSkillOneSelect, this));
                pMenuSkillTwoItem->setTag(1);
                pMenuSkillTwoItem->setPosition(m_winSize.width*0.35,m_winSize.height*0.6-m_winSize.height*0.3*i);
                pMenuSkillTwoItem->setScale(3);
                
                int iSkillLv = CDBMgr::getInstance()->GetHeroSkillLvById(pHeroSkillInfo[i]->heroSkillId);
        
                {
                bool  bGreenPlus = CDBMgr::getInstance()->IsSkillCanLvUp(iMonsterid,pHeroSkillInfo[i]->heroSkillId,true,iSkillLv);
                
                
                if(bGreenPlus)
                {
                    Size tSkillIco = pMenuSkillOneItem->getContentSize();
                    Sprite *pGreenPlus = Sprite::createWithSpriteFrameName("plus.png");
                    pGreenPlus->setScale(0.25);
                    pGreenPlus->setPosition(tSkillIco.width*0.25,tSkillIco.height*0.8);
                    pMenuSkillTwoItem->addChild(pGreenPlus);
                }
                }
                
            }
            else if(i==2)
            {
                
                pMenuSkillThreeItem = LYMenuItemSprite::create(pSkill,pSkillSelect,pSkillDisable,CC_CALLBACK_1(LvUpDetail::OnSkillOneSelect, this));
                pMenuSkillThreeItem->setTag(2);
                pMenuSkillThreeItem->setPosition(m_winSize.width*0.65,m_winSize.height*0.6-m_winSize.height*0.3*(i-2));
                pMenuSkillThreeItem->setScale(3);
                int iSkillLv = CDBMgr::getInstance()->GetHeroSkillLvById(pHeroSkillInfo[i]->heroSkillId);
                
               
                {
                bool bGreenPlus = CDBMgr::getInstance()->IsSkillCanLvUp(iMonsterid,pHeroSkillInfo[i]->heroSkillId,true,iSkillLv);
                if(bGreenPlus)
                {
                    Size tSkillIco = pMenuSkillOneItem->getContentSize();
                    Sprite *pGreenPlus = Sprite::createWithSpriteFrameName("plus.png");
                    pGreenPlus->setScale(0.25);
                    pGreenPlus->setPosition(tSkillIco.width*0.25,tSkillIco.height*0.8);
                    pMenuSkillThreeItem->addChild(pGreenPlus);
                }
                }
                
            }
            else if(i==3)
            {
                pMenuSkillFourItem = LYMenuItemSprite::create(pSkill,pSkillSelect,pSkillDisable,CC_CALLBACK_1(LvUpDetail::OnSkillOneSelect, this));
                pMenuSkillFourItem->setTag(3);
                pMenuSkillFourItem->setPosition(m_winSize.width*0.65,m_winSize.height*0.6-m_winSize.height*0.3*(i-2));
                pMenuSkillFourItem->setScale(3);
                int iSkillLv = CDBMgr::getInstance()->GetHeroSkillLvById(pHeroSkillInfo[i]->heroSkillId);
                
               
                {
                bool  bGreenPlus = CDBMgr::getInstance()->IsSkillCanLvUp(iMonsterid,pHeroSkillInfo[i]->heroSkillId,true,iSkillLv);
                if(bGreenPlus)
                {
                    Size tSkillIco = pMenuSkillOneItem->getContentSize();
                    Sprite *pGreenPlus = Sprite::createWithSpriteFrameName("plus.png");
                    pGreenPlus->setScale(0.25);
                    pGreenPlus->setPosition(tSkillIco.width*0.25,tSkillIco.height*0.8);
                    pMenuSkillFourItem->addChild(pGreenPlus);
                }
                }
                
            }
            
            Size tSkill = pSkill->getContentSize();
            
            Sprite *pSkillBg = Sprite::createWithSpriteFrameName("minzk.png");
            pSkillBg->setScale(0.3);
            pSkillBg->setPosition(tSkill.width*0.5,-tSkill.height*0.19);
            pSkill->addChild(pSkillBg);
    
            Size tBg = pSkillBg->getContentSize();
            std::string strSkillName = pHeroSkillInfo[i]->heroSkillName;
            Label *pSkill1Name = Label::createWithSystemFont(strSkillName,"Aril",30);
            pSkill1Name->setPosition(tBg.width*0.5,tBg.height*0.5);
            pSkill1Name->setColor(Color3B(255,220,137));
            pSkillBg->addChild(pSkill1Name);
        }

    }
    else
    {
        for(int i=0;i<m_pskill.size();i++)
        {
            if(m_pSoldierVec[m_iIndex]->theFighterSkillInfo.size()>0)
            {
                Sprite *pSkill1 = CDataManager::getSkillIcoWithId(m_pskill[i]);
                Sprite *pSkillSelect = CDataManager::getSkillIcoWithId(m_pskill[i],1,true);
                pSkillSelect->setColor(Color3B::GRAY);
    
                Sprite *pSkillDisable = CDataManager::getSkillIcoWithId(m_pskill[i],1,true);
                pSkillDisable->setColor(Color3B::GRAY);
                
                int iSkillLv = m_pSoldierVec[m_iIndex]->theFighterSkillInfo[i].skillLevel;
                
                if(i==0)
                {
                    pMenuSkillOneItem = LYMenuItemSprite::create(pSkill1,pSkillSelect,pSkillDisable,CC_CALLBACK_1(LvUpDetail::OnSkillOneSelect, this));
                    pMenuSkillOneItem->setTag(0);
                    
                    
                    if(i<iRare)
                    {
                        
                    bool  bGreenPlus = CDBMgr::getInstance()->IsSkillCanLvUp(iMonsterid,m_pskill[i]->skillId,false,iSkillLv);
                    if(bGreenPlus)
                    {
                        Size tSkillIco = pMenuSkillOneItem->getContentSize();
                        Sprite *pGreenPlus = Sprite::createWithSpriteFrameName("plus.png");
                        pGreenPlus->setScale(0.25);
                        pGreenPlus->setPosition(tSkillIco.width*0.25,tSkillIco.height*0.8);
                        pMenuSkillOneItem->addChild(pGreenPlus);
                    }
                    }
                    else
                    {
                        pMenuSkillOneItem->setNormalImage(pSkillDisable);
                        //pMenuSkillOneItem->setEnabled(false);
                    }
        
                pMenuSkillOneItem->setPosition(m_winSize.width*0.35,m_winSize.height*0.6-m_winSize.height*0.3*i);
                pMenuSkillOneItem->setScale(3);
                    
            }
            else if(i==1)
            {
                pMenuSkillTwoItem = LYMenuItemSprite::create(pSkill1,pSkillSelect,CC_CALLBACK_1(LvUpDetail::OnSkillOneSelect, this));
                
                if(i<iRare)
                {
                    
                   // pMenuSkillTwoItem->setEnabled(false);
                    
                if(CDBMgr::getInstance()->IsSkillCanLvUp(iMonsterid,m_pskill[i]->skillId,false,iSkillLv))
                {
                    Size tSkillIco = pMenuSkillTwoItem->getContentSize();
                    Sprite *pGreenPlus = Sprite::createWithSpriteFrameName("plus.png");
                    pGreenPlus->setScale(0.25);
                    pGreenPlus->setPosition(tSkillIco.width*0.25,tSkillIco.height*0.8);
                    pMenuSkillTwoItem->addChild(pGreenPlus);
                }
                 }
                else
                {
                    pMenuSkillTwoItem->setNormalImage(pSkillDisable);
                }
            
                pMenuSkillTwoItem->setTag(1);
                pMenuSkillTwoItem->setScale(3);
                pMenuSkillTwoItem->setPosition(m_winSize.width*0.35,m_winSize.height*0.6-m_winSize.height*0.3*i);
            }
            else if(i==2)
            {
                pMenuSkillThreeItem = LYMenuItemSprite::create(pSkill1,pSkillSelect,CC_CALLBACK_1(LvUpDetail::OnSkillOneSelect, this));
                pMenuSkillThreeItem->setTag(2);
                
                 if(i<iRare)
                 {
                      /// pMenuSkillThreeItem->setEnabled(false);
                if(CDBMgr::getInstance()->IsSkillCanLvUp(iMonsterid,m_pskill[i]->skillId,false,iSkillLv))
                {
                    Size tSkillIco = pMenuSkillThreeItem->getContentSize();
                    Sprite *pGreenPlus = Sprite::createWithSpriteFrameName("plus.png");
                    pGreenPlus->setScale(0.25);
                    pGreenPlus->setPosition(tSkillIco.width*0.25,tSkillIco.height*0.8);
                    pMenuSkillThreeItem->addChild(pGreenPlus);
                }
                 }
                 else
                 {
                     pMenuSkillThreeItem->setNormalImage(pSkillDisable);
                 }
                
                
                
                pMenuSkillThreeItem->setPosition(m_winSize.width*0.65,m_winSize.height*0.6-m_winSize.height*0.3*(i-2));
                pMenuSkillThreeItem->setScale(3);
            }
            else if(i==3)
            {
            
                pMenuSkillFourItem = LYMenuItemSprite::create(pSkill1,pSkillSelect,CC_CALLBACK_1(LvUpDetail::OnSkillOneSelect, this));
                pMenuSkillFourItem->setTag(3);
                pMenuSkillFourItem->setPosition(m_winSize.width*0.65,m_winSize.height*0.6-m_winSize.height*0.3*(i-2));
                pMenuSkillFourItem->setScale(3);
                
                 if(i<iRare)
                 {
                    // pMenuSkillFourItem->setEnabled(false);
                     
                if(CDBMgr::getInstance()->IsSkillCanLvUp(iMonsterid,m_pskill[i]->skillId,false,iSkillLv))
                {
                    Size tSkillIco = pMenuSkillFourItem->getContentSize();
                    Sprite *pGreenPlus = Sprite::createWithSpriteFrameName("plus.png");
                    pGreenPlus->setScale(0.25);
                    pGreenPlus->setPosition(tSkillIco.width*0.25,tSkillIco.height*0.8);
                    pMenuSkillFourItem->addChild(pGreenPlus);
                    pMenuSkillFourItem->setScale(3);
                }
                 }
                else
                {
                   pMenuSkillFourItem->setNormalImage(pSkillDisable);
                
                }
            }
            
                Size tSkill;
                Sprite *pSkillBg = NULL;
                if(i==0)
                {
                    tSkill = pMenuSkillOneItem->getContentSize();
                    pSkillBg = Sprite::createWithSpriteFrameName("minzk.png");
                    pSkillBg->setScale(0.3);
                    pSkillBg->setPosition(tSkill.width*0.5,-tSkill.height*0.19);
                    pMenuSkillOneItem->addChild(pSkillBg);
                }
                else if(i==1)
                {
                    tSkill = pMenuSkillTwoItem->getContentSize();
                    pSkillBg = Sprite::createWithSpriteFrameName("minzk.png");
                    pSkillBg->setScale(0.3);
                    pSkillBg->setPosition(tSkill.width*0.5,-tSkill.height*0.19);
                    pMenuSkillTwoItem->addChild(pSkillBg);

                }
                else if(i==2)
                {
                    
                    tSkill = pMenuSkillThreeItem->getContentSize();
                    pSkillBg = Sprite::createWithSpriteFrameName("minzk.png");
                    pSkillBg->setScale(0.3);
                    pSkillBg->setPosition(tSkill.width*0.5,-tSkill.height*0.19);
                    pMenuSkillThreeItem->addChild(pSkillBg);
                }
                else if(i==3)
                {
                    tSkill = pMenuSkillFourItem ->getContentSize();
                    pSkillBg = Sprite::createWithSpriteFrameName("minzk.png");
                    pSkillBg->setScale(0.3);
                    pSkillBg->setPosition(tSkill.width*0.5,-tSkill.height*0.19);
                    pMenuSkillFourItem ->addChild(pSkillBg);
                }
            //技能名字
            
         
                
                Size tBg = pSkillBg->getContentSize();
                std::string strSkillName = m_pskill[i]->skillName;
                Label *pSkill1Name = Label::createWithSystemFont(strSkillName,"Aril",30);
                pSkill1Name->setPosition(tBg.width*0.5,tBg.height*0.5);
                pSkill1Name->setColor(Color3B(255,220,137));
                pSkillBg->addChild(pSkill1Name);
            }
        }
    }
    
    m_pMenuSkill=  Menu::create(pMenuSkillOneItem,pMenuSkillTwoItem,pMenuSkillThreeItem,pMenuSkillFourItem,NULL);
    m_pMenuSkill->setPosition(0,m_winSize.height*0.1);
    m_pMenuSkill->setAnchorPoint(Vec2(0,0));
    addChild(m_pMenuSkill,0,1001);
}


void LvUpDetail::OnMenuLvUp(Ref *pSender)
{
    CCLOG("Lvup ");
    
    if (m_pTarget && m_pHandler)
    {
        (m_pTarget->*m_pHandler)(pSender);
    }
    
    removeFromParent();
}

void LvUpDetail::OnTurnLeft(Ref *pSender)
{
    Vec3   rotation3D  =  m_pSprite->getRotation3D();
    rotation3D.y-= 30;
    m_pSprite->setRotation3D(rotation3D);
}

void LvUpDetail::OnTurnRight(Ref *pSender)
{
    Vec3   rotation3D  =  m_pSprite->getRotation3D();
    rotation3D.y+= 30;
    m_pSprite->setRotation3D(rotation3D);
}

void LvUpDetail::addCloseGuide(Ref *pSender)
{
    NotificationCenter::getInstance()->removeObserver(this,"upgradeCloseGuide");
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 10))
    {
        Vec2 pos = m_pMenuClose->convertToWorldSpace(Vec2::ZERO);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
        m_pSprite->setVisible(true);
    }
}

void LvUpDetail::addGuide(Ref *pSender)
{
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 8))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this);
        m_pSprite->setVisible(false);
    }
}


bool SortByRareLv(const FighterInfo *v1,const FighterInfo *v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致
{
    
    if(v1->rare == v2->rare)
    {
        return v1->level>v2->level;
        
    }
    return v1->rare>v2->rare;
}



bool LvUpDetail::init()
{
    CBaseLayer::init();
    
    std::vector<FighterInfo*>  pAll;
    
    pAll = CPersonalData::getInstance()->getAllSoldierInfo();
    
    for(int i=0;i<pAll.size();i++)
    {
        //1 英雄  0怪物
        
        if(m_bHero)
        {
            if(pAll[i]->theType == 1)
            {
                m_pSoldierVec.push_back(pAll[i]);
            }
        }
        else
        {
            if(pAll[i]->rare>0)
            {
                m_pSoldierVec.push_back(pAll[i]);
            }
        }
    }
    
     std::sort(m_pSoldierVec.begin(),m_pSoldierVec.end(),SortByRareLv);
    
    
    userInfoInDB* userInfoDB = CDataManager::getTheUSerInfDB();
    //std::vector<IdAndNum> heroSkills = userInfoDB->heroSkills;
    
    std::vector<IdAndNum> HeroSkillVec = userInfoDB->heroSkills;
    
    m_pListner = EventListenerTouchOneByOne::create();//创建一个触摸监听
    m_pListner->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false
    
    m_pListner->onTouchBegan = CC_CALLBACK_2(LvUpDetail::onTouchBegan, this);
    m_pListner->onTouchMoved = CC_CALLBACK_2(LvUpDetail::onTouchMoved, this);
    m_pListner->onTouchEnded = CC_CALLBACK_2(LvUpDetail::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner, this);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Sprite *pBg = Sprite::createWithSpriteFrameName("shjh.png");
    pBg->setScaleY(1.5);
    pBg->setPosition(visibleSize.width*0.2,visibleSize.height*0.75);
    addChild(pBg,1);
    
    Size  tshBg =  pBg->getContentSize();
    Sprite *ph1 =  Sprite::createWithSpriteFrameName("divider.png");
    ph1->cocos2d::Node::setScale(1.2);
    ph1->setFlippedX(true);
    ph1->setPosition(tshBg.width*0.6,tshBg.height*0.7);
    pBg->addChild(ph1);
    
    Sprite *ph2 =  Sprite::createWithSpriteFrameName("divider.png");
    ph2->cocos2d::Node::setScale(1.2);
    ph2->setFlippedX(true);
    ph2->setPosition(tshBg.width*0.6,tshBg.height*0.4);
    pBg->addChild(ph2);
    
    Sprite *ph3 =  Sprite::createWithSpriteFrameName("divider.png");
    ph3->cocos2d::Node::setScale(1.2);
    ph3->setFlippedX(true);
    ph3->setPosition(tshBg.width*0.6,tshBg.height*0.05);
    pBg->addChild(ph3);
    
    m_pSoldierBg = Sprite::createWithSpriteFrameName("soldierUp_bg.png");
    m_pSoldierBg->setScale(2);
    m_pSoldierBg->setPosition(Vec2(visibleSize.width*0.5, visibleSize.height*0.5));
    addChild(m_pSoldierBg);

    Size tTureBg = m_pSoldierBg->getContentSize();
    Label *pSKillLvTitle = Label::createWithSystemFont("技能升级","Aril",30);
    pSKillLvTitle->setColor(Color3B::YELLOW);
    pSKillLvTitle->setScale(0.5);
    pSKillLvTitle->setPosition(tTureBg.width*0.5,tTureBg.height*0.93);
    m_pSoldierBg->addChild(pSKillLvTitle);
    
    
    cocos2d::__String   sModelName = m_pSoldierVec[m_iIndex]->theModelInfo->modelName;
    
    
    m_pSprite =  Sprite3D::create(sModelName.getCString());
    
    
    Size modeSize = m_pSprite->getBoundingBox().size;

    
    m_pSprite->setGlobalZOrder(100);
    auto s = Director::getInstance()->getWinSize();
    m_pSprite ->setPosition(Vec2(s.width * 2.5f / 5.f, s.height / 3.f));
    addChild(m_pSprite,1,101);
    m_pSprite->setScale(30);
    
    if(m_pSoldierVec[m_iIndex]->monsterId == 1004)
    {
        m_pSprite->setScale(50);
    }
    else if(m_pSoldierVec[m_iIndex]->monsterId == 10 ||m_pSoldierVec[m_iIndex]->monsterId == 6||m_pSoldierVec[m_iIndex]->monsterId == 9)
    {
        m_pSprite->setScale(15);
    }
    else if(m_pSoldierVec[m_iIndex]->monsterId == 16)
    {
        m_pSprite->setScale(0.8);
        
    }
    
    auto animation = Animation3D::create(sModelName.getCString());
    if (animation)
    {
        auto animate = Animate3D::create(animation,m_pSoldierVec[m_iIndex]->theModelInfo->idleFrame.start/60, m_pSoldierVec[m_iIndex]->theModelInfo->idleFrame.duration/60);
        
        cocos2d::Action* _swim = RepeatForever::create(animate);
        m_pSprite->runAction(_swim);
    }
    
    if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE)
    {
        m_pSprite->setVisible(false);
    }
    
    Sprite *pCloseBtn = Sprite::createWithSpriteFrameName("closeBtn.png");
    
    Sprite *pCloseBtnSelect = Sprite::createWithSpriteFrameName("closeBtn.png");
    pCloseBtnSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pCloseItem = LYMenuItemSprite::create(pCloseBtn,pCloseBtnSelect,CC_CALLBACK_1(LvUpDetail::OnCloseMenu,this));
    pCloseItem->setScale(0.5);
    
    Size tBg = m_pSoldierBg->getContentSize();
    
    m_pMenuClose = Menu::create(pCloseItem,NULL);
    m_pMenuClose->setPosition(tBg.width*0.95,tBg.height*0.934);
    
    m_pSoldierBg->addChild(m_pMenuClose);
    
    addDetail();
    
    CSKillLvLayerQue *pSkillLayer = CSKillLvLayerQue::create(m_iIndex,m_pSoldierBg);
    addChild(pSkillLayer,100,9000);
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 5))
    {
        
        NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LvUpDetail::addGuide), "upgradeGuide", NULL);
        NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(LvUpDetail::addCloseGuide), "upgradeCloseGuide", NULL);
        Vec2 pos = Vec2(m_winSize.width * 0.35f, m_winSize.height * 0.7f);
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
    }
    
    return true;
}

void LvUpDetail::showSkillInfo()
{
    int iMonsterid = m_pSoldierVec[m_iIndex]->monsterId;
    int iTag = 0;
    if(CPersonalData::getInstance()->m_jumpInfo.type == 0)
    {
        std::vector<skillInfo*>  m_pskill = CDataManager::getAvailableSkillInfo(iMonsterid,5);
        for(int i=0;i<m_pskill.size();i++)
        {
            if(m_pskill[i]->skillId == CPersonalData::getInstance()->m_jumpInfo.skillId)
            {
                iTag = i;
                break;
            }
        }
    }
    else
    {
        vector<heroSkill*> pHeroSkillInfo = CDBMgr::getInstance()->getHeroSkillByMonsterId(iMonsterid);
        
        for(int i=0;i<pHeroSkillInfo.size();i++)
        {
            if(pHeroSkillInfo[i]->heroSkillId == CPersonalData::getInstance()->m_jumpInfo.skillId)
            {
                iTag = i;
                break;
            }
        }
    }
    CPersonalData::getInstance()->setJumpFromSkill(false);
    CPersonalData::getInstance()->setJumpInfoNull();
    
    vector<CDBMgr::MonsterSkillTime_s> pSize = CDBMgr::getInstance()->GetSkillUpgradeInfoVec();
    int iSize = pSize.size();
    m_pSprite->setVisible(false);
    m_pSkillUpLayer = CSkillUpLayer::create(m_iIndex,iTag,iSize,m_bHero);
    addChild(m_pSkillUpLayer,1000);
}

void  LvUpDetail::OnCloseMenu(Ref *pSender)
{
    if(m_bHero)
    {
        if(m_pSkillMenuLayer)
        {
            m_pSkillMenuLayer->StartHeroLayer();
        }
    }
    else
    {
        if(m_pSkillMenuLayer)
        {
             m_pSkillMenuLayer->StartSoldierLayer();
        }
       
    }
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 10))
    {
        //显示指引关卡
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->removeGuiderLayer();
        NotificationCenter::getInstance()->postNotification("CloseSkillGuide");
    }
    removeFromParent();
}


bool LvUpDetail::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    m_bMenuSoldier = m_pMenuSoldier->onTouchBegan(touch,event);

    if(m_pMenuSkill)
    {
        m_bMenuSkill = m_pMenuSkill->onTouchBegan(touch,event);
    }
    
    m_bMenuUpNext = m_pMenuUpNext->onTouchBegan(touch,event);
    
    if(m_pMenuFinishCancle)
    {
        m_bFinshCancle = m_pMenuFinishCancle->onTouchBegan(touch,event);
    }
    
    if(m_pMenuFinishCancle2)
    {
        m_bMenuFinishCancle2 = m_pMenuFinishCancle2->onTouchBegan(touch,event);
    }
    
    if(m_pMenuFinishCancle3)
    {
        m_bMenuFinishCancle3 = m_pMenuFinishCancle3->onTouchBegan(touch,event);
    }
    
    m_bMenuNext = m_pNext->onTouchBegan(touch,event);
    
    m_bMenuClose = m_pMenuClose->onTouchBegan(touch,event);

    return true;
}

void LvUpDetail::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(m_bMenuSoldier)
    {
        m_pMenuSoldier->onTouchMoved(touch,event);
    }
    else if(m_bMenuSkill)
    {
        m_pMenuSkill->onTouchMoved(touch,event);
    }
    else if(m_bMenuUpNext)
    {
        m_pMenuUpNext->onTouchMoved(touch,event);
    }
    else if(m_bMenuNext)
    {
        m_pNext->onTouchMoved(touch,event);
    }
    else if(m_bMenuClose)
    {
        m_pMenuClose->onTouchMoved(touch,event);
    }
    else if(m_bFinshCancle)
    {
      m_pMenuFinishCancle->onTouchMoved(touch,event);
    }
    
    else if(m_bMenuFinishCancle2)
    {
        m_pMenuFinishCancle2->onTouchMoved(touch,event);
    }
    
    else if(m_bMenuFinishCancle3)
    {
        m_pMenuFinishCancle3->onTouchMoved(touch,event);
    }
    
    Vec2 t  =  touch->getPreviousLocation();
    
    Vec2 pCurTouch = touch->getLocation();
    
    if(pCurTouch.x<t.x)
    {
        Vec3   rotation3D  =  m_pSprite->getRotation3D();
        rotation3D.y-= 20;
        m_pSprite->setRotation3D(rotation3D);
    }
    else
    {
        Vec3   rotation3D  =  m_pSprite->getRotation3D();
        rotation3D.y+= 20;
        m_pSprite->setRotation3D(rotation3D);
    }

}

void LvUpDetail::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(m_bMenuSoldier)
    {
        m_pMenuSoldier->onTouchEnded(touch,event);
        m_bMenuSoldier = false;
    }
    else if(m_bMenuSkill)
    {
        m_pMenuSkill->onTouchEnded(touch,event);
        m_bMenuSkill = false;
    }
    else if(m_bMenuUpNext)
    {
        m_pMenuUpNext->onTouchEnded(touch,event);
        m_bMenuUpNext = false;
    }
    else if(m_bMenuNext)
    {
        m_pNext->onTouchEnded(touch,event);
        m_bMenuNext = false;
    }
    else if(m_bMenuClose)
    {
        m_pMenuClose->onTouchEnded(touch,event);
        m_bMenuClose = false;
    }
    else if(m_bFinshCancle)
    {
        m_pMenuFinishCancle->onTouchEnded(touch,event);
    }
  
    else if(m_bMenuFinishCancle2)
    {
        m_pMenuFinishCancle2->onTouchEnded(touch,event);
        m_bMenuFinishCancle2 = false;
    }
    
    else if(m_bMenuFinishCancle3)
    {
        m_pMenuFinishCancle3->onTouchEnded(touch,event);
        m_bMenuFinishCancle3 = false;
    }
}


void LvUpDetail::onTouchCancelled(Touch *touch, Event *event)
{
    if(m_bMenuSoldier)
    {
        m_pMenuSoldier->onTouchCancelled(touch,event);
        m_bMenuSoldier = false;
    }
    else if(m_bMenuSkill)
    {
        m_pMenuSkill->onTouchCancelled(touch,event);
        m_bMenuSkill = false;
    }
    else if(m_bMenuUpNext)
    {
        m_pMenuUpNext->onTouchCancelled(touch,event);
        m_bMenuUpNext = false;
    }
    else if(m_bMenuClose)
    {
        m_pMenuClose->onTouchCancelled(touch,event);
        m_bMenuClose = false;
    }
    else if(m_bFinshCancle)
    {
        m_pMenuFinishCancle->onTouchCancelled(touch,event);
    }
    else if(m_bMenuFinishCancle2)
    {
        m_pMenuFinishCancle2->onTouchCancelled(touch,event);
        m_bMenuFinishCancle2 = false;
    }
    
    else if(m_bMenuFinishCancle3)
    {
        m_pMenuFinishCancle3->onTouchCancelled(touch,event);
        m_bMenuFinishCancle3 = false;
    }
    

}
