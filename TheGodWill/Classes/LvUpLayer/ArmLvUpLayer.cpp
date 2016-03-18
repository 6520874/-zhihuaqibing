//
//  ArmLvUpLayer.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/12/17.
//
//

#include "ArmLvUpLayer.h"
#include "LYPersonalData.h"
#include "DataManager.h"
#include "guiderManagerLayer.h"
#include "DBMgr.h"
#include "LYFightManager.h"
#include "NetWorkDataSingle.h"

CArmLvUpLayer::CArmLvUpLayer(FighterInfo *pInfo,int index)
{
    m_winSize = Director::getInstance()->getVisibleSize();
    m_pBg = NULL;
    m_pJindu = NULL;
    m_pMenuClose = NULL;
    m_pMenuMinus = NULL;
    m_pMenuPlus = NULL;
    m_bMenuCloSe = false;
    m_bMenuMinus = false;
    m_bMenuPlus = false;
    m_iToLV = 2;
    m_pFighterInfo = pInfo;
    m_pCostMoneyLabel = NULL;
    m_bPlus = false;
    m_bMinus = false;
    m_iAddHp = 0;
    m_iAddAtk = 0;
    m_pAddAtk = NULL;
    m_pAddHp = NULL;
    m_index = index;
    m_grayBg = NULL;
    m_bFlag = false;
}

CArmLvUpLayer::~CArmLvUpLayer()
{
    
    
}

bool CArmLvUpLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    m_bMenuCloSe = m_pMenuClose->onTouchBegan(touch,event);
    m_bMenuMinus = m_pMenuMinus->onTouchBegan(touch,event);
    m_bMenuPlus = m_pMenuPlus->onTouchBegan(touch,event);
    m_bMenuLvUp = m_pMenuLvUp->onTouchBegan(touch,event);
    return true;
    
}

void CArmLvUpLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(m_bMenuCloSe)
    {
        m_pMenuClose->onTouchMoved(touch,event);
    }
    else if(m_bMenuMinus)
    {
        m_pMenuMinus->onTouchMoved(touch,event);
    }
    else if(m_bMenuPlus)
    {
        m_pMenuPlus->onTouchMoved(touch,event);
    }
    else if(m_bMenuLvUp)
    {
        m_pMenuLvUp->onTouchMoved(touch,event);
    }
}

void CArmLvUpLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    CCLOG("滑块1");
    if(m_bMenuCloSe)
    {
        m_pMenuClose->onTouchEnded(touch,event);
        m_bMenuCloSe = false;
    }
    else if(m_bMenuMinus)
    {
        m_pMenuMinus->onTouchEnded(touch,event);
        m_bMenuMinus = false;
    }
    else if(m_bMenuPlus)
    {
        m_pMenuPlus->onTouchEnded(touch,event);
        m_bMenuPlus = false;
    }
    else if(m_bMenuLvUp)
    {
        m_pMenuLvUp->onTouchEnded(touch,event);
        m_bMenuLvUp = false;
    }
    if(m_grayBg && (m_grayBg->getBoundingBox().containsPoint(touch->getLocation())))
    {
        removeFromParent();
    }
}

void CArmLvUpLayer::onTouchCancelled(Touch *touch, Event *event)
{
    if(m_bMenuCloSe)
    {
        m_pMenuClose->onTouchCancelled(touch,event);
        m_bMenuCloSe = false;
    }
    else if(m_bMenuMinus)
    {
        m_pMenuMinus->onTouchCancelled(touch,event);
        m_bMenuMinus = false;
    }
    else if(m_bMenuPlus)
    {
        m_pMenuPlus->onTouchCancelled(touch,event);
        m_bMenuPlus = false;
    }
    else if(m_bMenuLvUp)
    {
        m_pMenuLvUp->onTouchCancelled(touch,event);
        m_bMenuLvUp = false;
    }
}


bool CArmLvUpLayer::init()
{
    CBaseLayer::init();
    
    bool bRet = false;
    do {
        
        CC_BREAK_IF(!CCLayer::init());
        
        LayerColor* layerColor = LayerColor::create();
        
        layerColor->setColor(Color3B(0, 0, 0));
        
        layerColor->setOpacity(100);
        layerColor->setContentSize(Size(m_winSize.width,m_winSize.height));
        
        this->addChild(layerColor);
        bRet = true;
        
    } while (0);
    
    
    m_pSoldierVec = CPersonalData::getInstance()->getAllSoldierInfo();
    m_pListener = EventListenerTouchOneByOne::create();
    m_pListener->setSwallowTouches(true);
    
    m_pListener->onTouchBegan = CC_CALLBACK_2(CArmLvUpLayer::onTouchBegan, this);
    m_pListener->onTouchMoved = CC_CALLBACK_2(CArmLvUpLayer::onTouchMoved, this);
    m_pListener->onTouchEnded = CC_CALLBACK_2(CArmLvUpLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener,this);
    
    //common_bg.png
    m_pBg = Sprite::createWithSpriteFrameName("common_bg.png");
    m_pBg->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(m_pBg);
    
    addChildCont();
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
    {
        Vec2 pos = m_solider->convertToWorldSpaceAR(Vec2::ZERO);
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
    }
    
    return true;
}

void CArmLvUpLayer::onExit()
{
    
    cocos2d::extension::CSoldierLvUpLayer *pLayer =  (cocos2d::extension::CSoldierLvUpLayer*)(getParent()->getChildByTag(100));
    
    cocos2d::extension::CHeroLvUpLayer *pLayer1 = (cocos2d::extension::CHeroLvUpLayer*)(getParent()->getChildByTag(102));
    
    if(pLayer1)
    {
        pLayer1->m_tableView->updateCellAtIndex(m_index);
        _eventDispatcher->removeEventListener(m_pListener);
        
    }
    if(pLayer)
    {
        pLayer->m_tableView->updateCellAtIndex(m_index);
        _eventDispatcher->removeEventListener(m_pListener);
    }
    
    unscheduleUpdate();
    CBaseLayer::onExit();
    
}


void CArmLvUpLayer::OnRemoveNotice()
{
    Label *  p =  (Label *)getChildByTag(1000);
    
    if(p)
        p->removeFromParent();
}


void CArmLvUpLayer::OnUpdateCoin(Ref *pSender)
{
    log("onUpdateCoin\n");

    m_pBg->setVisible(false);

    
    float fPre = 1.5;
    
    Sprite *pMidLan = Sprite::createWithSpriteFrameName("goodsdikuang.png");
    pMidLan->setPosition(m_winSize.width*0.5,m_winSize.height*0.6);
    pMidLan->setScale(100);
    pMidLan->setOpacity(0);
    addChild(pMidLan,1);
  

    Sprite *pBeijin = Sprite::createWithSpriteFrameName("xingguang.png");
     pBeijin ->setOpacity(100);
    pBeijin->setScale(fPre);
    pBeijin->setPosition(Point(m_winSize.width/2,m_winSize.height*0.6));
    addChild(pBeijin,305);
    
    
    Sprite *pLight = Sprite::createWithSpriteFrameName("backlight3.png");
   pLight ->setOpacity(100);
    pLight->setScale(fPre*4);
    pLight->runAction(RepeatForever::create(RotateBy::create(1.0, 90)));
    pLight->setPosition(Vec2(m_winSize.width/2,m_winSize.height*0.6));
    addChild(pLight,301);
    
    
    Sprite *pLvUpSucessLayer = Sprite::createWithSpriteFrameName("lvsucess.png");
    pLvUpSucessLayer->setOpacity(100);
    pLvUpSucessLayer->setPosition(Point(m_winSize.width / 2, m_winSize.height*0.25));
    pLvUpSucessLayer->setScale(fPre);
    addChild(pLvUpSucessLayer,302);
    
    Sprite *pWing = Sprite::createWithSpriteFrameName("chibang.png");
    pWing->setOpacity(100);
    pWing->setScale(fPre);
    pWing->setPosition(Vec2(m_winSize.width/2,m_winSize.height*0.6));
    addChild(pWing,304);
    
    Size tWing = pWing->getContentSize();
    Sprite *pHero = Sprite::createWithSpriteFrameName("upgrade_di.png");
    pHero->setOpacity(100);
    pHero->setScale(fPre);
    pHero->setPosition(Vec2(tWing.width / 2,tWing.height * 0.2));
    pWing->addChild(pHero,303);
    
    float ft = 1;
    
    pBeijin->runAction(ScaleTo::create(0.1,ft));
    pBeijin->runAction(FadeTo::create(0.1, 255));
    pLight->runAction(ScaleTo::create(0.1,ft*4));
    
    pLight->runAction(FadeTo::create(0.1, 255));
    pLvUpSucessLayer->runAction(ScaleTo::create(0.1,ft*2.5));
     pLvUpSucessLayer->runAction(FadeTo::create(0.1, 255));
    pWing->runAction(ScaleTo::create(0.1,ft));
    pWing->runAction(FadeTo::create(0.1, 255));
    pHero->runAction(ScaleTo::create(0.1,ft));
    pHero->runAction(FadeTo::create(0.1, 255));
    pMidLan->runAction(ScaleTo::create(0.1,10 ));
     pMidLan->runAction(FadeTo::create(0.1, 255));
    
    CallFunc* guide = CallFunc::create(CC_CALLBACK_0(CArmLvUpLayer::addGuide, this));
    runAction(Sequence::create(DelayTime::create(1.0f), RemoveSelf::create(), guide, NULL));
    m_pFighterInfo->level = m_iToLV;
    
    myMonster * pMyMonster = CDataManager::getuserMonsterById(m_pFighterInfo->monsterId);
    int curLevel = pMyMonster->monsterLevel;
    pMyMonster->monsterLevel = m_pFighterInfo->level;
    
    m_pFighterInfo = CPersonalData::getInstance()->refreshSoldierInfoWithInfo(pMyMonster);
    
    char  strHeroPath[50] = {0};
    
    if(m_pFighterInfo->monsterId>1000)
    {
        int iLv2 = 0;
    
        {
            {
                iLv2 = CDBMgr::getInstance()->GetHeroLvWithId(1005);
                if(m_pFighterInfo->monsterId== 1005)
                {
                    iLv2 = m_iToLV;
                }
                
                sprintf(strHeroPath+strlen(strHeroPath),"1005,%d;",iLv2);
            }
        }
    
        userInfoInDB* userInfoDB = CDataManager::getTheUSerInfDB();
        
        userInfoDB->m_strHeroLv = strHeroPath;
        
        CDBMgr::getInstance()->SaveHeroLv(strHeroPath);
    }
    else
    {
        CDataManager::SaveMonsterInfoById(pMyMonster);
    }
    
    CDataManager::saveTaskInfoToDB(3);
    int num = curLevel + m_solider->getValue();
    CDataManager::saveTaskInfoToDB(19, num, true);
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER) && (guiderManagerLayer::getInstance()->getStepNum() == 5))
    {
        CDataManager::saveGuideInfoToDB(GUIDE_UPGRADE_SOLIDER);
        guiderManagerLayer::getInstance()->removeGuiderLayer();
    }

}

void CArmLvUpLayer::OnLvUp(Ref *pSender)
{
    log("lvup");
    int iCoin = CPersonalData::getInstance()->getUserCoin();
    if(m_iCurValue==0)
    {
        return;
    }
    
    if(iCoin <m_iCostMoney)
    {
        SimpleAudioEngine::getInstance()->playEffect("music/noBtn.mp3");
        
        Sprite * thetarget = CFightManager::getTheFlyWord("金币不够耶!","fonts/cuti.ttf");
        thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
        addChild(thetarget, 100);
        
        return;
    }
    
   
    
    LayerColor *pBlackColor = LayerColor::create();
    pBlackColor->setColor(Color3B::BLACK);
    pBlackColor->setAnchorPoint(Vec2(0,0));
    pBlackColor->setPosition(Vec2(0, 0));
    pBlackColor->setOpacity(150);
    addChild(pBlackColor);
    
    //扣钱
    iCoin -= m_iCostMoney;
    
    //if(m_iCostMoney!=0)
    {
         SimpleAudioEngine::getInstance()->playEffect("music/lvlSucess.mp3");
    }
    //talkingData
    __String* tempStr = __String::createWithFormat("%f", m_iCostMoney);
    EventParamMap paramMap;
    
    paramMap.insert(EventParamPair("等级升级花费金币",tempStr->getCString()));
    
    TDCCTalkingDataGA::onEvent("等级升级", &paramMap);
    
    CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(CArmLvUpLayer::OnUpdateCoin),-m_iCostMoney,0,0);
    
}

void CArmLvUpLayer::addGuide()
{
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER) && (guiderManagerLayer::getInstance()->getStepNum() == 5))
    {
        NotificationCenter::getInstance()->postNotification("soliderGuide");
    }
}

void CArmLvUpLayer::OnMinus(Ref *pSender)
{
    log("minus");
    int iValue = m_solider->getValue();
    
    if(iValue>=1)
    {
        m_bMinus = true;
        m_solider->setValue(iValue-1);
        m_iCurValue = iValue-1;
        
        m_iToLV -=1;
        
        
        GetCostCoin(m_iToLV);
        
        log("minus = %d\n",m_iToLV);
        //((m_iToLV-1)*(m_iToLV-1)+8*(m_iToLV-1))*0.8;
        
        char strCostMoney[100]  ={0};
        sprintf(strCostMoney,"%.0f",m_iCostMoney);
        m_pCostMoneyLabel->setString(strCostMoney);
        
        char strToLv[100] =  {0};
        sprintf(strToLv,"LV%d",m_iToLV);
        m_pUpLvNum->setString(strToLv);
        
        m_pUpLvNum->setPositionX(m_pLvArrow->getPositionX()+m_pLvArrow->getBoundingBox().size.width*0.5+m_pUpLvNum->getBoundingBox().size.width*0.5);

        
        char strAddHp[100] =  {0};
        sprintf(strAddHp,"%d",(m_iToLV-m_pFighterInfo->level)*m_iAddHp);
        
        char strAddAtk[100]= {0};
        sprintf(strAddAtk,"%d",(m_iToLV-m_pFighterInfo->level)*m_iAddAtk);
        
        int iCoin = CPersonalData::getInstance()->getUserCoin();
        if(iCoin<(int)m_iCostMoney)
        {
            m_pCostMoneyLabel->setColor(Color3B::RED);
        }
        else
        {
            m_pCostMoneyLabel->setColor(Color3B::YELLOW);
        }
        
        m_pAddHp->setString(strAddHp);
        m_pAddAtk->setString(strAddAtk);
    }
}


void CArmLvUpLayer::OnPlus(Ref *pSender)
{
    log("plus");
    int iValue = m_solider->getValue();
    
    int iMaxValue = m_solider->getMaximumValue();
    
    log("iValue = %d\n",iValue);
    
    if(iValue<iMaxValue)
    {
        m_bPlus = true;
        m_solider->setValue(iValue+1);
        m_iCurValue  = iValue+1;
        
        m_iToLV +=1;
        
        log("plus = %d\n",m_iToLV);
        GetCostCoin(m_iToLV);
        
        char strCostMoney[100]  ={0};
        sprintf(strCostMoney,"%.0f",m_iCostMoney);
        m_pCostMoneyLabel->setString(strCostMoney);
        
        char strToLv[100] =  {0};
        sprintf(strToLv,"LV%d", m_iToLV);
        m_pUpLvNum->setString(strToLv);
        m_pUpLvNum->setPositionX(m_pLvArrow->getPositionX()+m_pLvArrow->getBoundingBox().size.width*0.5+m_pUpLvNum->getBoundingBox().size.width*0.5);

        
        
        char strAddHp[100] =  {0};
        sprintf(strAddHp,"%d",(m_iToLV-m_pFighterInfo->level)*m_iAddHp);
        
        char strAddAtk[100]= {0};
        sprintf(strAddAtk,"%d",(m_iToLV-m_pFighterInfo->level)*m_iAddAtk);
        
        int iCoin = CPersonalData::getInstance()->getUserCoin();
        if(iCoin<(int)m_iCostMoney)
        {
            m_pCostMoneyLabel->setColor(Color3B::RED);
        }
        else
        {
            m_pCostMoneyLabel->setColor(Color3B::YELLOW);
        }
        
        
        m_pAddHp->setString(strAddHp);
        m_pAddAtk->setString(strAddAtk);
    }
}



int CArmLvUpLayer::GetCostCoin(int ToLv)
{
    int tmp = 0;
    for(int i=m_pFighterInfo->level;i<ToLv;i++)
    {
        tmp += (i*i+8*i)*0.8;
    }
    m_iCostMoney = tmp;
    return m_iCostMoney;
}


void CArmLvUpLayer::soliderCallBack(Ref * sender, cocos2d::extension::Control::EventType controlEvent)
{
    

 
    auto slide_control = (cocos2d::extension::ControlSlider*)sender;//通过回调参数sender 获得ControlSlider‘
    
    int iCurrent_value = slide_control->getValue();//获取slide当前的值
    
    log("Value = %d",iCurrent_value);
    
    if(!m_bPlus &&!m_bMinus)
    {
        int iValue = iCurrent_value - m_iCurValue;
        
        m_iCurValue = iCurrent_value;
        m_iToLV += iValue;
        
        log("m_iToLV %d iValue %d",m_iToLV,iValue);
        
          if(iValue!=0)
          {
           SimpleAudioEngine::getInstance()->playEffect("music/slider.mp3");
          }
        if(iCurrent_value == 0)
        {
            GetCostCoin(m_pFighterInfo->level);
        }
        else
        { //（等级的2次幂+8*等级）*0.8
            GetCostCoin(m_iToLV);
        }
        
        char strCostMoney[100]  ={0};
        sprintf(strCostMoney,"%.0f",m_iCostMoney);
        m_pCostMoneyLabel->setString(strCostMoney);
        
        char strToLv[100] =  {0};
        char strAddHp[100] = {0};
        
        
        sprintf(strAddHp,"%d", (m_iToLV-m_pFighterInfo->level)*m_iAddHp);
        char strAddAtk[100]= {0};
        sprintf(strAddAtk,"%d", (m_iToLV-m_pFighterInfo->level)*m_iAddAtk);
        
        m_pAddHp->setString(strAddHp);
        m_pAddAtk->setString(strAddAtk);
        
        sprintf(strToLv,"LV%d", m_iToLV);
        m_pUpLvNum->setString(strToLv);
        m_pUpLvNum->setPositionX(m_pLvArrow->getPositionX()+m_pLvArrow->getBoundingBox().size.width*0.5+m_pUpLvNum->getBoundingBox().size.width*0.5);

        
    }
    
    int iCoin = CPersonalData::getInstance()->getUserCoin();
    if(iCoin<(int)m_iCostMoney)
    {
        m_pCostMoneyLabel->setColor(Color3B::RED);
    }
    else
    {
        m_pCostMoneyLabel->setColor(Color3B::YELLOW);
    }
    
    
    
    m_bMinus  = false;
    m_bPlus = false;
    if(m_solider->getValue() == m_solider->getMaximumValue() && !m_bFlag)
    {
        m_bFlag = true;
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
        {
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            Vec2 pos = m_pMenuLvUp->convertToWorldSpace(Vec2::ZERO);
            guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
        }
    }
    if((m_solider->getValue() < m_solider->getMaximumValue()) && m_bFlag && ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER)
                   && (guiderManagerLayer::getInstance()->getStepNum() == 5)))
    {
        m_bFlag = false;
        Vec2 pos = m_solider->convertToWorldSpaceAR(Vec2::ZERO);
        guiderManagerLayer::getInstance()->setStepNum(4);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
    }
}

void CArmLvUpLayer::addChildCont()
{
    Size  SzBg = m_pBg->getContentSize();
    
    Label *pLvUpLable = Label::createWithSystemFont("等级升级","Aril",30);
    pLvUpLable->setPosition(SzBg.width*0.5,SzBg.height*0.9);
    pLvUpLable->setColor(Color3B(254,229,74));
    m_pBg->addChild(pLvUpLable);
    
    m_pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(m_pFighterInfo->monsterId);
    
    //兵种图标
    Sprite *pHeadPic = CDataManager::getMonsterIconAndBg(m_pMonsterInfo->monsterId);
    pHeadPic->setScale(4);
    pHeadPic->setPosition(SzBg.width*0.2,SzBg.height*0.75);
    m_pBg->addChild(pHeadPic);
    
    Size tSoldierSize = pHeadPic->getContentSize();
    
    Sprite * pType = CDBMgr::getInstance()->GetShareSpriteByFigtherType(m_pMonsterInfo->monsterType);
    pType->setScale(0.25);
    pType->setPosition(tSoldierSize.width*0.25,tSoldierSize.height*0.2);
    pHeadPic->addChild(pType);

    
    //兵种名字
    
    CDataManager::getTheAddAtrWith(m_pMonsterInfo,&m_iAddHp,&m_iAddAtk);
    
    std::string strSkillName = "空";
    
    if(m_pFighterInfo->theFighterSkillInfo.size()>0)
    {
        strSkillName = m_pFighterInfo->theFighterSkillInfo[0].theSkillInfo->skillName;
    }
    
    Label *pSoldierName = Label::createWithSystemFont(m_pMonsterInfo->monsterName,"Aril",30);
    pSoldierName->setPosition(pHeadPic->getPositionX()+pHeadPic->boundingBox().size.width,pHeadPic->getPositionY()*1.05);
    m_pBg->addChild(pSoldierName);
    
    //等级
    char strLv[100] = {0};
    sprintf(strLv,"LV:%d", m_pFighterInfo->level);
    
    Label *pLvUp  = Label::createWithSystemFont(strLv,"Aril'",30);
    pLvUp->setPosition(pHeadPic->getPositionX()+pHeadPic->boundingBox().size.width,pHeadPic->getPositionY()*0.9);
    m_pBg->addChild(pLvUp);
    
    m_pLvArrow = Sprite::createWithSpriteFrameName("LvArrow.png");
    m_pLvArrow->setPosition(pLvUp->getPositionX()+pLvUp->getContentSize().width*0.5+m_pLvArrow->getContentSize().width*0.5,SzBg.height*0.675);
    m_pBg->addChild(m_pLvArrow);
    
    //目标等级
    char strToLv[100] =  {0};
    m_iToLV = m_pFighterInfo->level+1;
    sprintf(strToLv,"LV:%d",m_iToLV);
    
    m_pUpLvNum = Label::createWithSystemFont(strToLv,"Aril",30);
    m_pUpLvNum->setPosition(m_pLvArrow->getPositionX()+m_pLvArrow->getBoundingBox().size.width*0.5+m_pUpLvNum->getBoundingBox().size.width*0.5,pHeadPic->getPositionY()*0.9);
    m_pBg->addChild(m_pUpLvNum);
    
    //血药水
    Sprite *pRed =  Sprite::createWithSpriteFrameName("hppic.png");
    pRed->setPosition(SzBg.width*0.3,SzBg.height*0.55);
    m_pBg->addChild(pRed);
    
    char strHp[100] = {0};
    sprintf(strHp,"%d", m_pFighterInfo->maxHP);
    
    Label *pHp = Label::createWithSystemFont(strHp,"Aril",30);
    pHp->setPosition(SzBg.width*0.3,SzBg.height*0.45);
    m_pBg->addChild(pHp);
    
    Label *pHpText = Label::createWithSystemFont("血:","Aril",25);
    pHpText->setPosition(SzBg.width*0.225,SzBg.height*0.4);
    pHpText->setColor(Color3B(166,53,9));
    m_pBg->addChild(pHpText);
    
    //箭头1
    Sprite *pHpArrow = Sprite::createWithSpriteFrameName("upArrow.png");
    pHpArrow->setPosition(SzBg.width*0.255,SzBg.height*0.4);
    m_pBg->addChild(pHpArrow);
    
    char strAddHp[100] = {0};
    sprintf(strAddHp,"%d",m_iAddHp);
    
    m_pAddHp = Label::createWithSystemFont(strAddHp,"Aril",25);
    m_pAddHp->setPosition(SzBg.width*0.3,SzBg.height*0.4);
    m_pAddHp->setColor(Color3B::GREEN);
    m_pBg->addChild(m_pAddHp);
    
    //攻击药水
    Sprite *pYellow =  Sprite::createWithSpriteFrameName("attk.png");
    pYellow->setPosition(SzBg.width*0.5,SzBg.height*0.55);
    m_pBg->addChild(pYellow);
    
    char strAtk[100] = {0};
    sprintf(strAtk,"%d", m_pFighterInfo->attk);
    
    Label *pAtk = Label::createWithSystemFont(strAtk,"Aril",30);
    pAtk->setPosition(SzBg.width*0.5,SzBg.height*0.45);
    m_pBg->addChild(pAtk);
    
    Label *pAddAtkText = Label::createWithSystemFont("攻:","Aril",25);
    pAddAtkText->setPosition(SzBg.width*0.425,SzBg.height*0.4);
    pAddAtkText->setColor(Color3B(254,184,35));
    m_pBg->addChild(pAddAtkText);
    
    Sprite *pAtkArrow = Sprite::createWithSpriteFrameName("upArrow.png");
    pAtkArrow->setPosition(SzBg.width*0.45,SzBg.height*0.4);
    m_pBg->addChild(pAtkArrow);
    
    char strAddAtk[100] = {0};
    sprintf(strAddAtk,"%d",m_iAddAtk);
    
    m_pAddAtk = Label::createWithSystemFont(strAddAtk,"Aril",25);
    m_pAddAtk->setPosition(SzBg.width*0.5,SzBg.height*0.4);
    m_pAddAtk->setColor(Color3B::GREEN);
    m_pBg->addChild(m_pAddAtk);
    
    //青色
    Sprite *pQing =  Sprite::createWithSpriteFrameName("defpic.png");
    pQing->setPosition(SzBg.width*0.7,SzBg.height*0.55);
    m_pBg->addChild(pQing);
    
    Label *pDef = Label::createWithSystemFont("125","Aril",30);
    pDef->setPosition(SzBg.width*0.7,SzBg.height*0.45);
    m_pBg->addChild(pDef);
    
    Label *pDefText = Label::createWithSystemFont("防:","Aril",25);
    pDefText->setPosition(SzBg.width*0.625,SzBg.height*0.4);
    pDefText->setColor(Color3B(49,202,225));
    m_pBg->addChild(pDefText);
    
    Sprite *pDefArrow = Sprite::createWithSpriteFrameName("upArrow.png");
    pDefArrow->setPosition(SzBg.width*0.65,SzBg.height*0.4);
    m_pBg->addChild(pDefArrow);
    
    
    Label *pAddDef = Label::createWithSystemFont("0","Aril",25);
    pAddDef->setPosition(SzBg.width*0.7,SzBg.height*0.4);
    pAddDef->setColor(Color3B::GREEN);
    m_pBg->addChild(pAddDef);
    
    //减号
    Sprite *pMinus = Sprite::createWithSpriteFrameName("minus.png");
    Sprite *pMinusSelect = Sprite::createWithSpriteFrameName("minus.png");
    pMinusSelect->setColor(Color3B::GRAY);
    
    int iaddHp = 0;
    int iaddAtk = 0;
    
    log("hp = %d  atk = %d",iaddHp,iaddAtk);
    
    LYMenuItemSprite *pMinusItem = LYMenuItemSprite::create(pMinus,pMinusSelect,CC_CALLBACK_1(CArmLvUpLayer::OnMinus, this));
    m_pMenuMinus = Menu::create(pMinusItem,NULL);
    
    m_pMenuMinus->setPosition(SzBg.width*0.15,SzBg.height*0.3);
    m_pBg->addChild(m_pMenuMinus);
    
    Sprite *pPlus = Sprite::createWithSpriteFrameName("plus.png");
    Sprite *pPlusSeclect = Sprite::createWithSpriteFrameName("plus.png");
    pPlusSeclect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pPlusItem = LYMenuItemSprite::create(pPlus,pPlusSeclect,CC_CALLBACK_1(CArmLvUpLayer::OnPlus, this));
    m_pMenuPlus = Menu::create(pPlusItem,NULL);
    m_pMenuPlus->setPosition(SzBg.width*0.85,SzBg.height*0.3);
    m_pBg->addChild(m_pMenuPlus);
    
    Sprite* backgroundSprite = Sprite::createWithSpriteFrameName("jindu.png");
    Sprite* pogressSprite = Sprite::createWithSpriteFrameName("processyellow.png");
    Sprite* thumbSprite = Sprite::createWithSpriteFrameName("ladong.png");
    
    m_solider = cocos2d::extension::ControlSlider::create(backgroundSprite,pogressSprite,thumbSprite);
    m_solider->setPosition(Vec2(SzBg.width / 2,SzBg.height * 0.3f));
    
    m_solider->setScale(1);
    //获取当前等级的2倍
    
    int iUserLv = CPersonalData::getInstance()->getUserInfo()->m_iUserLevel;
    
    log("%d",iUserLv);
    
    m_solider->setMaximumValue(iUserLv*2-m_pFighterInfo->level);
    
    m_solider->setMinimumValue(0);
    
    m_iCurValue = 1;
    
    m_solider->setValue(m_iCurValue);
    
    m_pBg->addChild(m_solider, 0, 200);
    
    m_solider->addTargetWithActionForControlEvents(this, cccontrol_selector(CArmLvUpLayer::soliderCallBack), cocos2d::extension::Control::EventType::VALUE_CHANGED);
    
    //花费金币
    Label *pCostLabel = Label::createWithSystemFont("花费:","Aril",30);
    pCostLabel->setPosition(SzBg.width*0.16,SzBg.height*0.12);
    m_pBg->addChild(pCostLabel);
    
    Sprite *pMoneySprite = Sprite::createWithSpriteFrameName("coin.png");
    pMoneySprite->setPosition(SzBg.width*0.26,SzBg.height*0.12);
    m_pBg->addChild(pMoneySprite);
    
    //m_iCostMoney = (1*1+1*8*1)*0.8;
    
    GetCostCoin(m_pFighterInfo->level+1);
    
    char strCostMoney[50] = {0};
    sprintf(strCostMoney,"%.0f",m_iCostMoney);
    
    int iCoin = CPersonalData::getInstance()->getUserCoin();
    m_pCostMoneyLabel = Label::createWithSystemFont(strCostMoney,"Aril",30);
    m_pCostMoneyLabel->setPosition(SzBg.width*0.36,SzBg.height*0.12);
    if(iCoin<(int)m_iCostMoney)
    {
        m_pCostMoneyLabel->setColor(Color3B::RED);
    }
    else
    {
        m_pCostMoneyLabel->setColor(Color3B::YELLOW);
    }
    m_pBg->addChild(m_pCostMoneyLabel);
    
    
    //升级
    Sprite *pLvUpBtn =  Sprite::createWithSpriteFrameName("greenbtn.png");
    Sprite *pLvUpSelect =  Sprite::createWithSpriteFrameName("greenbtn.png");
    pLvUpSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pLvUpItem = LYMenuItemSprite::create(pLvUpBtn,pLvUpSelect,CC_CALLBACK_1(CArmLvUpLayer::OnLvUp, this));
    pLvUpItem->setScale(2.5);
    m_pMenuLvUp = Menu::create(pLvUpItem,NULL);
    m_pMenuLvUp->setPosition(SzBg.width*0.85,SzBg.height*0.12);
    m_pBg->addChild(m_pMenuLvUp);
    
    Label *pLvUpTextLable = Label::createWithSystemFont("升级","Aril",30);
    pLvUpTextLable->setScale(0.3);
    pLvUpTextLable->setPosition(pLvUpBtn->boundingBox().size.width*0.5,pLvUpBtn->boundingBox().size.height*0.5);
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
    {
        m_pMenuLvUp->setEnabled(false);
        scheduleUpdate();
    }
    
    pLvUpItem->addChild(pLvUpTextLable);
    Sprite *pCloseBtn = Sprite::createWithSpriteFrameName("closeBtn.png");
    
    Sprite *pCloseBtnSelect = Sprite::createWithSpriteFrameName("closeBtn.png");
    pCloseBtnSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pCloseItem = LYMenuItemSprite::create(pCloseBtn,pCloseBtnSelect,CC_CALLBACK_1(CArmLvUpLayer::OnCloseMenu,this));
    
    m_pMenuClose = Menu::create(pCloseItem,NULL);
    m_pMenuClose->setPosition(SzBg.width*0.95,SzBg.height*0.9);
    m_pBg->addChild(m_pMenuClose);
    
}

void CArmLvUpLayer::update(float dt)
{
    if((m_solider->getValue() > 0) && (m_solider->getValue() == m_solider->getMaximumValue()))
    {
        m_pMenuLvUp->setEnabled(true);
    }
    else{
        m_pMenuLvUp->setEnabled(false);
    }
}

void CArmLvUpLayer::OnCloseMenu(Ref *pSender)
{
    
    removeFromParent();
}
