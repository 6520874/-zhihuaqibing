//
//  SkillLvCellLayer.cpp
//  WarOfGod
//
//  Created by prefer_shan on 15/9/1.
//
//

#include "SkillLvCellLayer.h"
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
#include "LYMenuItemSprite.h"
#include "SkillLvLayerQue.h"
#include  "SkillUpLayer.h"


CSkillLvCelLayer::CSkillLvCelLayer(int iTime,int iMonsterId,int iSkillId,int iNeedTime)
{
    m_fAllTime = 0;
    m_iCnt = 1;
    m_iCostMagic = 0;
    m_iMin = 0;
    m_iHour = 0;
    m_iSec = 0;
    
}


CSkillLvCelLayer::~CSkillLvCelLayer()
{


}


void CSkillLvCelLayer::RemoveCell( )
{
    unscheduleUpdate();
    
    CSKillLvLayerQue* Parent  = (CSKillLvLayerQue*)getParent();
    
    
    Parent->RemoveOnecell(this);
    
    Parent->SortCell();
    
    removeFromParent();

}


void  CSkillLvCelLayer::FinishAtOnce(int index)
{
    
    {
        CDBMgr::getInstance()->DelSkillUpgradeToStr(m_strMonsSkill1Id);
        
        
        // RemoveFromTime(20);
        //SetPostionLvUpDetail(21);
        
        AddSkillFinish(m_strMonsSkill1Id);
        m_pLabelLvUpTime =NULL;
    }
       //talkingData
    __String* tempStr = __String::createWithFormat("%d", m_iCostMagic);
    EventParamMap paramMap;
    
    paramMap.insert(EventParamPair("技能等级升级花费金币",tempStr->getCString()));
    
    TDCCTalkingDataGA::onEvent("技能等级升级", &paramMap);
    
    CallFunc* guide = CallFunc::create(CC_CALLBACK_0(CSkillLvCelLayer::RemoveCell, this));
    runAction(Sequence::create(DelayTime::create(0.5),guide,NULL));
    
    
}



void CSkillLvCelLayer::OnUpdateStone(Ref *pSender)
{

    CDataManager::saveTaskInfoToDB(4);
    CDataManager::saveTaskInfoToDB(18, 1);
}


void CSkillLvCelLayer::OnGetYes(Ref *pSender)
{
 
    log("花费100元魔法石\n");
    //删除定时器
    LvUpDetail *pLayer = (LvUpDetail*)getParent()->getParent();
    
    if(pLayer)
    {
        Sprite *p = (Sprite *)pLayer->getChildByTag(101);
        
        if(p)
        {
            p->setVisible(true);
        }
    }

    
    std::string strMonstSkillStr;
    
    if(CPersonalData::getInstance()->getUserStone() >= m_iCostMagic )
    {
        int iSkillId = 0;
        int iMonsterid =0;
        CDBMgr::getInstance()->FengeStr(strMonstSkillStr,iMonsterid,iSkillId);
        
        CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(CSkillLvCelLayer::OnUpdateStone), 0,-m_iCostMagic ,0);

        
        TDCCItem::onPurchase("技能升级", 1, m_iCostMagic);
    }
    else
    {
        Sprite * thetarget = CFightManager::getTheFlyWord("魔法石不够耶!","fonts/cuti.ttf");
        thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height*0.7));
        addChild(thetarget, 1000);
        return ;
    }
    
    FinishAtOnce(m_iSkillInex);

}



void CSkillLvCelLayer::CancleAtOnce(int iTimer)
{
    LvUpDetail *pLayer = (LvUpDetail*)getParent()->getParent();
    
    if(pLayer)
    {
        Sprite *p = (Sprite *)pLayer->getChildByTag(101);
        
        if(p)
        {
            p->setVisible(false);
        }
    }
    
    
    std::map<int,int>::iterator it =  CommonData::GetSingletonPtr()->m_PropMap.begin();
    
    for(int i=0;it!=CommonData::GetSingletonPtr()->m_PropMap.end();it++,i++)
    {
        int iNum  = CDBMgr::getInstance()->HavePropNumById(it->first);
        CDBMgr::getInstance()->SaveUserPropDb(it->first,iNum+it->second);
        
        propInfo* pInfo =  CDBMgr::getInstance()->getPropInfoById(it->first);
        char strText[100] = {0};
        
        sprintf(strText,"获得%s*%d",pInfo->m_sItemName.c_str(),it->second);
        Sprite *pText =  CFightManager::getRewardWord(strText,i*1.0f);
        pText->setPosition(m_winSize.width*0.5,m_winSize.height*0.7);
        addChild(pText,99999);
    }
    

    {
 
        
        //返还道具
        
        CDBMgr::getInstance()->DelSkillUpgradeToStr(m_strMonsSkill1Id);
    
        log("%s","时间1到了");

        m_pLabelLvUpTime =NULL;
        
        //增加等级
    }

    

   Sprite *pBg=  (Sprite*)getChildByTag(20);
    pBg->setVisible(false);
    
    
    
    CallFunc* guide = CallFunc::create(CC_CALLBACK_0(CSkillLvCelLayer::RemoveCell, this));
    runAction(Sequence::create(DelayTime::create(2),guide,NULL));

    
}




void CSkillLvCelLayer::OnGeSkillCancleYes(Ref *pSender)
{

    CancleAtOnce(m_iSkillInex);
    LvUpDetail *pLayer = (LvUpDetail*)getParent()->getParent();
    
    
    if(pLayer)
    {
        if(pLayer->m_pSkillUpLayer)
        {
            //CSkillUpLayer* pSkillUpLayer =  (CSkillUpLayer*)(pLayer->m_pSkillUpLayer);
            
            //pSkillUpLayer->RefreshUI();
        }
        
        else
        {
            Sprite *p = (Sprite *)pLayer->getChildByTag(101);
            if(p)
            {
                p->setVisible(true);
            }
        }
        
    }
    


}


void CSkillLvCelLayer::OnGetCancleNo(Ref *pSender)
{
    LvUpDetail *pLayer = (LvUpDetail*)getParent()->getParent();
    
    if(pLayer)
    {
        //判断技能升级界面是否存在，不在的话屏蔽掉
        if(pLayer->m_pSkillUpLayer)
        {
            
        }
        else
        {
            Sprite *p = (Sprite *)pLayer->getChildByTag(101);
            if(p)
            {
                p->setVisible(true);
            }
        }
    }
    

    

    
}


void CSkillLvCelLayer::OnFinishAtOnce(Ref *pSender)
{
    LYMenuItemSprite * p = (LYMenuItemSprite*)pSender;
    long idx =  (long)p->getUserData();
    m_iSkillInex = idx;
    
    LvUpDetail *pLayer = (LvUpDetail*)getParent()->getParent();
    
    if(pLayer)
    {
        Sprite *p = (Sprite *)pLayer->getChildByTag(101);
        
        if(p)
        {
            p->setVisible(false);
        }
    }
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 8))
    {
        //CDataManager::saveGuideInfoToDB(GUIDE_SKILL_UPGRADE);
        guiderManagerLayer::getInstance()->removeGuiderLayer();
        FinishAtOnce(m_iSkillInex);
    }
    else
    {
        CHitDialog * pDlg = CHitDialog::create();
        int iCostMagic = 0;
        float fTime = 100.0;
        float i  =  fTime/60;
        
        if(idx == 1)
        {
            float ft =  m_iMin;
            iCostMagic = ft;
        }
        
        if(iCostMagic<1)
        {
            iCostMagic = 1;
            
        }
        
        char strText[50] = {0};
        
        m_iCostMagic = iCostMagic;
        
        sprintf(strText, "是否花费%d魔法石来升级完成",m_iCostMagic);
        
        pDlg->createYesNO(this ,menu_selector(CSkillLvCelLayer::OnGetYes),strText,menu_selector(CSkillLvCelLayer::OnGetCancleNo));
        addChild(pDlg,1000,201);
    }
    
}


void CSkillLvCelLayer::OnCancleLvUp(Ref *pSender)
{
    log("onCancleLvup");
    
    LvUpDetail *pLayer = (LvUpDetail*)getParent()->getParent();
    
    if(pLayer)
    {
        Sprite *p = (Sprite *)pLayer->getChildByTag(101);
        
        if(p)
        {
            p->setVisible(false);
        }
    }
    

    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 8))
    {
        return;
    }
    
    LYMenuItemSprite * p = (LYMenuItemSprite*)pSender;
    long idx =  (long)p->getUserData();
    
    m_iSkillInex = idx;
    
    CHitDialog * pDlg = CHitDialog::create();
    
    //\n(取消升级将返还所有升级材料)
    
    pDlg->createYesNO(this ,menu_selector(CSkillLvCelLayer::OnGeSkillCancleYes),"是否取消该技能升级\n(取消升级将返还所有升级材料)",menu_selector(CSkillLvCelLayer::OnGetCancleNo));
    
    addChild(pDlg,1000);

}


bool CSkillLvCelLayer::init(int iTime,int iMonsterId,int iSkillId,int iNeedTime)
{

    CBaseLayer::init();
    
    
    int iPreTime = iTime;

    
    bool bHero = false;
    
    if(iMonsterId>1000)
    {
        bHero = true;
    }
    
    char strMonsterSkillId[50] = {0};
    sprintf(strMonsterSkillId,"%d_%d",iMonsterId,iSkillId);
    
    m_strMonsSkill1Id = strMonsterSkillId;
    m_iSkillInex = CDBMgr::getInstance()->GetHeroSkillIndex(iSkillId);
    
    Sprite *pSpriteSoldier = CDataManager::getMonsterIconAndBg(iMonsterId,true);
    pSpriteSoldier->Node::setAnchorPoint(Vec2::ZERO);
    pSpriteSoldier->setScale(2);

    
    pSpriteSoldier->setPosition(m_winSize.width*0.78,m_winSize.height*0.65);
    addChild(pSpriteSoldier,0,20);
    
    Size tSoldier = pSpriteSoldier->getContentSize();
    
    //立即完成
    Sprite *pFinishAtOnce = Sprite::createWithSpriteFrameName("finishatonce.png");
    Sprite *pFinishAtOnceSelect = Sprite::createWithSpriteFrameName("finishatonce.png");
    pFinishAtOnceSelect->setColor(Color3B::GRAY);
    
    
    m_pFinishItem = LYMenuItemSprite::create(pFinishAtOnce,pFinishAtOnceSelect,NULL,CC_CALLBACK_1(CSkillLvCelLayer::OnFinishAtOnce,this));

    
    m_pFinishItem->setTag(1);
    m_pFinishItem->setScale(0.4);
    m_pFinishItem->setUserData((void*)1);
    m_pFinishItem->setPosition(Vec2(-tSoldier.width*0.8,tSoldier.height*0.7));
    

    //取消完成
    Sprite *pCancle = Sprite::createWithSpriteFrameName("canclelvup.png");
    Sprite *pCancleSelect = Sprite::createWithSpriteFrameName("canclelvup.png");
    pCancleSelect->setColor(Color3B::GRAY);
    m_pCancleLvUpItem = LYMenuItemSprite::create(pCancle,pCancleSelect,NULL,CC_CALLBACK_1(CSkillLvCelLayer::OnCancleLvUp,this));
    m_pCancleLvUpItem->setTag(2);
    m_pCancleLvUpItem->setScale(0.4);
    m_pCancleLvUpItem->setUserData((void*)1);
    m_pCancleLvUpItem->setPosition(Vec2(-tSoldier.width*0.8,tSoldier.height*0.1));
    
    m_pMenuFinishCancle = Menu::create(m_pFinishItem,m_pCancleLvUpItem,NULL);
    m_pMenuFinishCancle->setPosition(Vec2(0,0));
    pSpriteSoldier->addChild(m_pMenuFinishCancle,0,30);
    
    skillInfo* pSkillInfo = CDataManager::getTheSkillInfoWithId(iSkillId);
    Sprite *pSkillIco = NULL;
    
    if(bHero)
    {
        char strIcoPath[50] = {0};
        std::vector<heroSkill*> pSkillInfo = CDBMgr::getInstance()->GetHeroSkillInfo();
        sprintf(strIcoPath,"%s",pSkillInfo[m_iSkillInex]->skillIcon.c_str());
        pSkillIco = Sprite::createWithSpriteFrameName(strIcoPath);
        pSkillIco->setScale(0.7);
    }
    else
    {
        pSkillInfo = CDataManager::getTheSkillInfoWithId(iSkillId);
        pSkillIco =  CDataManager::getSkillIcoWithId(pSkillInfo,0.5);
        pSkillIco->setScale(0.8);
    }
    
    pSkillIco->setPosition(tSoldier.width*1.35,tSoldier.height*0.5);
    pSpriteSoldier->addChild(pSkillIco,0,20);
    
    int iNowTime = CGameTime::getNowTimeInSecond();
    int iTimeRemain = iNowTime -iPreTime;
    
    //获取技能升级需要时间

    
    heroSkillUpgradeInfo* pHeroSkillUpgradeInfo = NULL;
    
    
    if(bHero)
    {
        int iLv = CDBMgr::getInstance()->GetHeroSkillLvById(iSkillId);
        pHeroSkillUpgradeInfo = CDBMgr::getInstance()->GetHeroSkillWithId(iSkillId,iLv);
        iNeedTime = pHeroSkillUpgradeInfo->m_iNeedTime;
    }
    else
    {
        iNeedTime;
    }
    
    int iSec = iNeedTime - iTimeRemain;
    
    if(iSec<0)
    {
        iSec = 0;
    }
    
    if(iSec >= 0)
    {
        m_iHour  = iSec/3600;
        m_iMin = iSec%3600/60;
        m_iSec = iSec%60;
    }

    
    m_iAllSec = iSec;
    
    char strTime[50] = {0};
    sprintf(strTime,"(%.2d:%.2d:%.2d)",m_iHour,m_iMin,m_iSec);
    
    if(m_iHour<=0)
    {
         sprintf(strTime,"(%.2d:%.2d)",m_iMin,m_iSec);
    }
    Size tSpr = pSpriteSoldier->getContentSize();
    
    m_pLabelLvUpTime = Label::createWithSystemFont(strTime,"Arial",25);
    m_pLabelLvUpTime->setScale(0.4);
    m_pLabelLvUpTime->setPosition(tSpr.width*0.5,tSpr.height*-0.125);
    
    pSpriteSoldier->addChild(m_pLabelLvUpTime);
    
    Sprite *progressbgSprite = Sprite::createWithSpriteFrameName("progressbg.png");
    progressbgSprite->setPosition(Point(tSpr.width*0.5,tSpr.height*0.1));
    progressbgSprite->setScaleX(0.3);
    progressbgSprite->setScaleY(0.5);
    pSpriteSoldier->addChild(progressbgSprite, 1);
    
    Sprite *progressSprite= Sprite::createWithSpriteFrameName("progressblue.png");
    
    float fTime = m_iHour*3600+m_iMin*60+m_iSec;
    float fHave = fTime/iNeedTime*100;
    float iPer = 100-fHave;
    
    m_pProgress1 = ProgressTimer::create(progressSprite);
    m_pProgress1->setType( ProgressTimer::Type::BAR);
    m_pProgress1->setPosition(Vec2(tSpr.width*0.5,tSpr.height*0.1));
    
    //进度动画运动方向，可以多试几个值，看看效果
    m_pProgress1->setMidpoint(Point(0, 0));
    
    //进度条宽高变化
    m_pProgress1->setBarChangeRate(Point(1, 0));
    m_pProgress1->setPercentage(iPer);
    m_pProgress1->setScaleX(0.3);
    m_pProgress1->setScaleY(0.25);
    
    pSpriteSoldier->addChild(m_pProgress1,1);
    

    scheduleUpdate();
    
    return true;
}


void CSkillLvCelLayer::AddSkillFinish(std::string strMonsSkillId)
{
    log("%s",strMonsSkillId.c_str());
    
    size_t index = strMonsSkillId.find("_");
    int iMonsterId  = 0;
    
    std::string str = strMonsSkillId.substr(index+1,strMonsSkillId.length());
    
    std::string strMonster = strMonsSkillId.substr(0,index);
    
    iMonsterId = atoi(strMonster.c_str());
    
    bool bHero = false;
    
    if(iMonsterId>1000)
    {
        bHero = true;
    }
    
    int  iSkillId = atoi(str.c_str());
    
    std::string strSKillName;
    std::vector<heroSkill*> pHeroSkillInfo = CDBMgr::getInstance()->GetHeroSkillInfo();
    
    
    std::string  strImg;
    
    if(bHero)
    {
        int iHeroSkillIndex = CDBMgr::getInstance()->GetHeroSkillIndex(iSkillId);
        strSKillName =  pHeroSkillInfo[iHeroSkillIndex]->heroSkillName;
        
        userInfoInDB* userInfoDB = CDataManager::getTheUSerInfDB();
        
        strImg = pHeroSkillInfo[iHeroSkillIndex]->skillIcon;
        
        for(int i=0;i<CPersonalData::getInstance()->getHeroSkillVec().size();i++)
        {
            if(CPersonalData::getInstance()->getHeroSkillVec()[i].theSkill->heroSkillId ==  iSkillId)
            {
                std::vector<heroSkillInfo>  t =   CPersonalData::getInstance()->getHeroSkillVec();
                
                t[i].level ++;
                userInfoDB->heroSkills[i].theNumber++;
                
                CPersonalData::getInstance()->setHeroSkillVec(t);
                
                break;
            }
            
        }
        std::vector<IdAndNum> pIdNumVec = userInfoDB->heroSkills;
        
        char strSkillPath[100] = {0};
        
        for(int i=0;i<pIdNumVec.size();i++)
        {
            sprintf(strSkillPath+strlen(strSkillPath),"%d,%d;",pIdNumVec[i].theId
                    ,pIdNumVec[i].theNumber);
        }
        
        CDBMgr::getInstance()->SaveHeroSkillInfoToDb(strSkillPath);
        
    }
    else
    {
        int iIndex = CDBMgr::getInstance()->GetMonsterSkillIndex(iMonsterId,iSkillId);
        
        //monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(iMonsterId);
        
        myMonster * pMyMonster = CDataManager::getuserMonsterById(iMonsterId);
        
        if(iIndex == 0)
        {
            pMyMonster->skillLevel1++;
        }
        else if(iIndex == 1)
        {
            pMyMonster->skillLevel2++;
        }
        else if(iIndex == 2)
        {
            pMyMonster->skillLevel3++;
        }
        else if(iIndex == 3)
        {
            pMyMonster->skillLevel4++;
        }
        
        FighterInfo* pFightInfo = CPersonalData::getInstance()->refreshSoldierInfoWithInfo(pMyMonster);
        skillInfo* pInfo = CDataManager::getTheSkillInfoWithId(iSkillId);
        strImg = pInfo->skillIcon;
        strSKillName = pInfo->skillName;
        
        //保存到数据库
        CDBMgr::getInstance()->SaveUserMonsterSkillToDB(iMonsterId,iIndex+1,pFightInfo->theFighterSkillInfo[iIndex].skillLevel);
    }
    
   LvUpDetail *pLayer1 = (LvUpDetail*)getParent()->getParent();
    
    if(pLayer1)
    {
        Sprite *p = (Sprite *)pLayer1->getChildByTag(101);
        
        if(p)
        {
            p->setVisible(false);
        }
    }
    

    ((CSKillLvLayerQue*)getParent())->AddSucess(strSKillName,strImg,bHero);
    
    CDataManager::saveTaskInfoToDB(4);
    CDataManager::saveTaskInfoToDB(18, 1);
    
}



void CSkillLvCelLayer::OnStartLvUp(std::string strMonsterSkill,monsterInfo* pInfo,std::string pSkillInfo, int iNeedTime)
{
    /*m_strMonsSkill1Id = strMonsterSkill;
    
    Sprite *pSpriteSoldier = CDataManager::getMonsterIconAndBg(pInfo->monsterId,true);
    
    pSpriteSoldier->Node::setAnchorPoint(Vec2::ZERO);
    pSpriteSoldier->setPosition(m_winSize.width*0.8,m_winSize.height*0.65);
    addChild(pSpriteSoldier,0,20);
    
    Size tSoldier = pSpriteSoldier->getContentSize();
    //立即完成
    Sprite *pFinishAtOnce = Sprite::createWithSpriteFrameName("finishatonce.png");
    Sprite *pFinishAtOnceSelect = Sprite::createWithSpriteFrameName("finishatonce.png");
    pFinishAtOnceSelect->setColor(Color3B::GRAY);
    
    m_pFinishItem = LYMenuItemSprite::create(pFinishAtOnce,pFinishAtOnceSelect,NULL,CC_CALLBACK_1(CSkillLvCelLayer::OnFinishAtOnce,this));
    m_pFinishItem->setTag(1);
    m_pFinishItem->setScale(0.4);
    
    m_pFinishItem->setUserData((void*)1);
    m_pFinishItem->setPosition(Vec2(-tSoldier.width*0.8,tSoldier.height*0.7));
    
    //取消完成
    Sprite *pCancle = Sprite::createWithSpriteFrameName("canclelvup.png");
    Sprite *pCancleSelect = Sprite::createWithSpriteFrameName("canclelvup.png");
    pCancleSelect->setColor(Color3B::GRAY);
    m_pCancleLvUpItem = LYMenuItemSprite::create(pCancle,pCancleSelect,NULL,CC_CALLBACK_1(CSkillLvCelLayer::OnCancleLvUp,this));
    m_pCancleLvUpItem->setTag(2);
    m_pCancleLvUpItem->setScale(0.4);
    m_pCancleLvUpItem->setUserData((void*)1);
    m_pCancleLvUpItem->setPosition(Vec2(-tSoldier.width*0.8,tSoldier.height*0.1));
    
    m_pMenuFinishCancle = Menu::create(m_pFinishItem,m_pCancleLvUpItem,NULL);
    m_pMenuFinishCancle->setPosition(Vec2(0,0));
    pSpriteSoldier->addChild(m_pMenuFinishCancle,0,30);
    
    Sprite *pSkillIco =  CDataManager::getSkillIcoWithId(pSkillInfo,0.5);
    pSkillIco->setScale(0.8);
    pSkillIco->setPosition(tSoldier.width*1.35,tSoldier.height*0.5);
    pSpriteSoldier->addChild(pSkillIco,0,10);
    
    if(guiderManagerLayer::getInstance()->getStepNum() == 7)
    {
       // log("烂苹果");
    }
    
    if(!((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 7)))
    {
        int iTime = iNeedTime;
        
        m_iHour = iTime/60;
        m_iSec = iTime%60;
        
        int iSec =  iTime;
        
        if(iSec!= 0)
        {
            m_iHour = iSec/60;
            m_iSec = iSec%60;
        }
        else
        {
            iSec = iTime;
        }
        
        m_iAllSec = iSec;
        char strTime[50] = {0};
        sprintf(strTime,"(%.2d:%.2d)",m_iHour,m_iSec);
        Size tSpr = pSpriteSoldier->getContentSize();
        
        //时间数值
        m_pLabelLvUpTime = Label::createWithSystemFont(strTime,"Arial",25);
        m_pLabelLvUpTime->cocos2d::Node::setScale(0.4);
        m_pLabelLvUpTime->setPosition(tSpr.width*0.5,-tSpr.height*0.125);
        pSpriteSoldier->addChild(m_pLabelLvUpTime);
        
        Sprite *progressbgSprite = Sprite::createWithSpriteFrameName("progressbg.png");
        progressbgSprite->setPosition(Point(tSpr.width*0.5,tSpr.height*0.1));
        progressbgSprite->setScaleX(0.3);
        progressbgSprite->setScaleY(0.5);
        pSpriteSoldier->addChild(progressbgSprite,1);
        
        Sprite *progressSprite= Sprite::createWithSpriteFrameName("progressblue.png");
        
        m_pProgress1 = ProgressTimer::create(progressSprite);
        m_pProgress1->setType(ProgressTimer::Type::BAR);
        m_pProgress1->setPosition(Vec2(tSpr.width*0.5,tSpr.height*0.1));
        
        //进度动画运动方向，可以多试几个值，看看效果
        m_pProgress1->setMidpoint(Point(0, 0));
        
        //进度条宽高变化
        m_pProgress1->setBarChangeRate(Point(1, 0));
        m_pProgress1->setPercentage(0);
        m_pProgress1->setScaleX(0.3);
        m_pProgress1->setScaleY(0.25);
        
        pSpriteSoldier->addChild(m_pProgress1,1);
    }
    */
}

void CSkillLvCelLayer::update(float time)
{
   // && (guiderManagerLayer::getInstance()->getStepNum() == 7)
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 7))
    {
        int id =0;
        int iSkillId = 0;
        CDBMgr::getInstance()->FengeStr(m_strMonsSkill1Id,id,iSkillId);

        if(id==1&&iSkillId==1)
        {
            return;
        }
        
    }
    
    
    m_fAllTime +=time;
    
    if(m_fAllTime/1>m_iCnt)
    {
        m_iCnt++;
        
        char strTime[50] = {0};
        
        if(m_iSec>0)
        {
            m_iSec -=1;
        }
        else if(m_iMin>0)
        {
            m_iMin -=1;
            m_iSec = 59;
        }
        else if(m_iHour>0)
        {
            m_iHour -=1;
            m_iMin = 59;
        }
        
        int id =0;
        int iSkillId = 0;
        CDBMgr::getInstance()->FengeStr(m_strMonsSkill1Id,id,iSkillId);
        
        bool bHero = false;
        
        if(id>1000)
        {
            bHero = true;
        }
        sprintf(strTime,"(%.2d:%.2d:%.2d)",m_iHour,m_iMin,m_iSec);
        if(m_iHour<=0)
        {
            sprintf(strTime,"(%.2d:%.2d)",m_iMin,m_iSec);
        }
        
        if(m_pLabelLvUpTime)
        {
            m_pLabelLvUpTime->setString(strTime);
            
            SkillUpgradeInfo* pInfo = NULL;
            heroSkillUpgradeInfo* pHeroSkillUpgradeInfo = NULL;
            
            int iAlltime = 0;
            
            if(bHero)
            {
                int iLv = CDBMgr::getInstance()->GetHeroSkillLvById(iSkillId);
                pHeroSkillUpgradeInfo = CDBMgr::getInstance()->GetHeroSkillWithId(iSkillId,iLv);
                iAlltime = pHeroSkillUpgradeInfo->m_iNeedTime;
            }
            else
            {
                int iLv = CDBMgr::getInstance()->GetMonsterSKillLv(id,iSkillId);
                monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(id);
                pInfo = CDataManager::getSkillUpgradeInfowithId(pMonsterInfo->monsterType,iLv);
                iAlltime = pInfo->m_iNeedTime;
            }
            
            float iTime =  m_iHour*3600+m_iMin*60+m_iSec;
            float fHave = iTime/iAlltime*100;
            float iPer = 100-fHave;
            
            m_pProgress1->setPercentage(iPer);
            
            if(iPer>=100)
            {
                char strIdType[10] = {0};
                sprintf(strIdType,"%d_%d",1,0);
                
                char strTime[10] = {0};
                sprintf(strTime,"%d_%d",1,0);
                
                AddSkillFinish(m_strMonsSkill1Id);
            
                
               // m_pSkillUpLayer
        
                
                
                if(bHero)
                {
                    std::vector<heroSkillInfo> pHeroSkillInfo  = CPersonalData::getInstance()->getHeroSkillVec();
                    
                    for(int i=0;i<pHeroSkillInfo.size();i++)
                    {
                        
                        if(pHeroSkillInfo[i].theSkill->heroSkillId == iSkillId)
                        {
                            pHeroSkillInfo[i].level++;
                            break;
                        }
                    }
                }
                
                CDBMgr::getInstance()->DelSkillUpgradeToStr(m_strMonsSkill1Id);
                
                
                
                m_pLabelLvUpTime = NULL;
                RemoveCell();
                
            }
            
        }
        

    }

}
