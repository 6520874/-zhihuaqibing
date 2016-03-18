//
//  DrawCardSucess.cpp
//  WarOfGod
//
//  Created by prefer_shan on 15/6/8.
//
//

#include "DrawCardSucess.h"
#include "LYFightManager.h"
#include "DataManager.h"
#include "CSharedPlist.h"
#include "DrawCard.h"
#include "DBMgr.h"
#include "guiderManagerLayer.h"



DrawCardSucess::DrawCardSucess(int  iMagic,CDrawCard * Parent)
{
    m_iMagic = iMagic;
    m_pListener = NULL;
    m_pMenuClose = NULL;
    m_bMenuClose = false;
    
    
    m_Parent  =Parent;
    
  
    CSharedPlist::getInstance()->pushPlist("plist/propIcon_1.plist");
    CSharedPlist::getInstance()->pushPlist("plist/propIcon_2.plist");
    CSharedPlist::getInstance()->pushPlist("plist/drawCardEffect.plist");
    CSharedPlist::getInstance()->pushPlist("plist/login.plist");
    
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
    
}

DrawCardSucess::~DrawCardSucess()
{
        CSharedPlist::getInstance()->erasePlist("plist/propIcon_1.plist");
    CSharedPlist::getInstance()->erasePlist("plist/propIcon_2.plist");
    CSharedPlist::getInstance()->erasePlist("plist/drawCardEffect.plist");
    CSharedPlist::getInstance()->erasePlist("plist/login.plist");


    SimpleAudioEngine::getInstance()->stopAllEffects();
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
}




void DrawCardSucess::onExit()
{
    CBaseLayer::onExit();
    _eventDispatcher->removeEventListener(m_pListener);
}

bool DrawCardSucess::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    
    //m_bMenuClose =  m_pMenuClose->onTouchBegan(pTouch,pEvent);
    
    return true;
}

void DrawCardSucess::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    if(m_bMenuClose)
    {
        m_pMenuClose->onTouchMoved(pTouch,pEvent);
        
    }
}

void DrawCardSucess::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if(m_bMenuClose)
    {
        m_pMenuClose->onTouchEnded(pTouch,pEvent);
        m_bMenuClose = false;
    }
}

void DrawCardSucess::onTouchCancelled(Touch *Touch, Event *pEvent)
{

    if(m_bMenuClose)
    {
        m_pMenuClose->onTouchCancelled(Touch,pEvent);
        m_bMenuClose = false;
    }

}


void DrawCardSucess::OnAction(Node *pSender)
{
    CDrawCard *pLayer = (CDrawCard*)getParent();

    if(pLayer)
    {
        pLayer->InitUI();
    }

}





void DrawCardSucess::OnCloseMenu(Ref *pSender)
{
    if(((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_COMMON_CARD) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
       || ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_COMMON_CARD) && (guiderManagerLayer::getInstance()->getStepNum() == 3)))
    {
        return;
    }
    CDrawCard *pLayer = (CDrawCard*)getParent();
    
     SimpleAudioEngine::getInstance()->playEffect("music/click_btn.mp3");
//    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_COMMON_CARD)
//        ||((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SENIOR_CARD)))
//    {
//        guiderManagerLayer::getInstance()->removeGuiderLayer();
//    }
    removeFromParent();
    if(pLayer)
    {
        pLayer->StartInfo();
    }
    
}

void DrawCardSucess::AddCloseMenu()
{
    
    Sprite *pCloseNormal = Sprite::createWithSpriteFrameName("closeBtn.png");
    Sprite *pCloseSelect = Sprite::createWithSpriteFrameName("closeBtn.png");
    pCloseSelect->setColor(Color3B::GRAY);
    
    MenuItemSprite *pMenuItemSprite = MenuItemSprite::create(pCloseNormal,pCloseSelect,CC_CALLBACK_1(DrawCardSucess::OnCloseMenu, this));
    pMenuItemSprite->setPosition(m_winSize.width*0.1,m_winSize.height*0.9);
    
    
    m_pMenuClose = Menu::create(pMenuItemSprite,NULL);
    m_pMenuClose->setPosition(Vec2(0,0));
   // m_pMenuClose->setVisible(false);
    addChild(m_pMenuClose);
    
}




bool DrawCardSucess::init()
{
    CBaseLayer::initWithColor(Color4B(0,0,0, 255*0.8));

    m_pListener = EventListenerTouchOneByOne::create();//创建一个触摸监听
    m_pListener->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false
    m_pListener->onTouchBegan = CC_CALLBACK_2(DrawCardSucess::onTouchBegan, this);
    m_pListener->onTouchMoved = CC_CALLBACK_2(DrawCardSucess::onTouchMoved, this);
    m_pListener->onTouchEnded = CC_CALLBACK_2(DrawCardSucess::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener, this);

    AddCloseMenu();
    
    m_CardInfos = CFightData::getInstance()->getDrawCardInfo();
    float ft =m_winSize.height*0.65f;
    
    Sprite *pDrawText ;
    
    Sprite *pBg = NULL;
    
    if(m_iMagic>=2)
    {
        pDrawText = Sprite::createWithSpriteFrameName("gaojidrawcard.png");
        
        pBg  = Sprite::createWithSpriteFrameName("drawsucess.png");
        pBg->setPosition(m_winSize.width*0.5,ft);
    }
    else
    {
        pDrawText = Sprite::createWithSpriteFrameName("ptdrawcard.png");
        pBg = Sprite::createWithSpriteFrameName("drawcoin.png");
        pBg->setPosition(m_winSize.width*0.5,ft-0.1f);
    }
    
    
    pDrawText->setPosition(m_winSize.width*0.5,ft+m_winSize.height*0.25);
    addChild(pDrawText);
    
    
    addChild(pBg,2);

    
    Sprite *pEffect = Sprite::create();
    
    Size tBg = pBg->getBoundingBox().size;

    Animate* theAnimate = CFightManager::getAnimateWithPath("chou_",1,5,".png",0.2f);
    
    if(m_iMagic>=2)
    {
        pEffect->setPosition(tBg.width*0.5,tBg.height*0.625);
    }
    else
    {
        pEffect->setPosition(tBg.width*0.5,tBg.height*0.655);
    }
    pBg->addChild(pEffect);
   
    
    Sprite *pWing = Sprite::createWithSpriteFrameName("chibang.png");
    pWing->setPosition(m_winSize.width*0.5,ft+0.1f);
    pWing->setScale(1.5);
    addChild(pWing,1);
    
    Sprite *pBeijin = Sprite::createWithSpriteFrameName("xingguang.png");
    pBeijin ->setOpacity(100);
    pBeijin->setPosition(Point(m_winSize.width/2,ft));
    addChild(pBeijin,3);
    
    
    Sprite *pLight = Sprite::createWithSpriteFrameName("backlight3.png");
    pLight ->setOpacity(100);
    pLight->cocos2d::Node::setScale(4);
    pLight->runAction(RepeatForever::create(RotateBy::create(1.0, 90)));
    pLight->setPosition(Vec2(m_winSize.width/2,ft));
    addChild(pLight);
    
    Sprite *pMidLan = Sprite::createWithSpriteFrameName("goodsdikuang.png");
    pMidLan->setPosition(m_winSize.width*0.5,m_winSize.height*0.3);
    pMidLan->setScaleX(10);
    pMidLan->setScaleY(7);
    
    pMidLan->setOpacity(255);
    addChild(pMidLan,1);
    
    
    std::vector<DrawCardResultInfo*> pInfo  = CDBMgr::getInstance()->GetDrawCardResultInfo();
    
    std::vector<DrawCardResultInfo*> pCoinInfo;
    std::vector<DrawCardResultInfo*> pStoneInfo;
    
    
    std::string iconBg[5]={"whiteBg.png","greenBg.png","blueBg.png","purpleBg.png", "orangeBg.png"};
    
    for(int i=0;i<pInfo.size();i++)
    {
        if(pInfo[i]->m_bCoin)
        {
            pCoinInfo.push_back(pInfo[i]);
        }
        else
        {
            pStoneInfo.push_back(pInfo[i]);
        }
    }
    
    //CallFuncN* guide = CallFuncN::create(CC_CALLBACK_1(DrawCardSucess::OnAction, this));
    
    if(m_iMagic==1)  //十连抽金币
    {
        Sprite *pMidLan2 = Sprite::createWithSpriteFrameName("goodsdikuang.png");
        pMidLan2->setPosition(m_winSize.width*0.5,m_winSize.height*0.1);
        pMidLan2->setScaleX(10);
        pMidLan2->setScaleY(7);
        pMidLan2->setOpacity(255);
        addChild(pMidLan2,1);
        pMidLan->setPosition(m_winSize.width*0.5,m_winSize.height*0.3);
        
        
        pEffect->stopAction(theAnimate);
        pEffect->runAction(RepeatForever::create(Sequence::create(theAnimate,NULL)));

        bool bHaveGoldenKey = false;
        
        for(int i=0;i<10;i++)
        {
            float fRandom = random(0.01,0.9);
            
            float fNowTime = 0.0f;
            
            DrawCardResultInfo* pDrawInfo;
            
            for(int i=0;i<pCoinInfo.size();i++)
            {
                float f = atof(pCoinInfo[i]->m_sRadio.c_str());
                fNowTime+= f;
                
                if(fNowTime>=fRandom)
                {
                    pDrawInfo = pCoinInfo[i];
                    break;
                }
            }
    
            Sprite *pDebrisIcon = NULL;
            
            float fScale = 4.0f;
            
           
            if(i==9&&!bHaveGoldenKey)
            {
                
                pDrawInfo->m_iType = 1;
                pDrawInfo->m_iGotItem = 41;
                pDrawInfo->m_iGotNum = 1;
            }
    
            if(pDrawInfo->m_iType ==0) //碎片
            {
                int id = random(1,13);
                
                pDebrisIcon = CDataManager::getMonsterInfodebris(id);
                
                 //pDebrisIcon->cocos2d::Node::setScale(1);
                
                CDBMgr::getInstance()->saveUserDebrisToDB(id,1);
                
            }
            else if(pDrawInfo->m_iType ==1) //道具
            {
                fScale = 2.0;
                
                propInfo*  info = CDataManager::getPropInfoById(pDrawInfo->m_iGotItem);
                
                char buf[50] = {0};
                
                sprintf(buf, "%s", iconBg[info->m_iItemLevel - 1].c_str());
                pDebrisIcon = Sprite::createWithSpriteFrameName(buf);
                
                sprintf(buf, "%s",info->m_sItemIcon.c_str());
                
                Sprite *pProp = Sprite::createWithSpriteFrameName(buf);
                
             
                Size tDBg = pDebrisIcon->getContentSize();
                
                pProp->setPosition(tDBg.width*0.5,tDBg.height*0.5);
                

                pDebrisIcon->addChild(pProp);
                
                pDebrisIcon->cocos2d::Node::setScale(fScale/2);
                
                pDebrisIcon->setOpacity(255*0.5);
                pDebrisIcon->setCascadeOpacityEnabled(true);
                
                fScale = 2.0f;
                
                 int iNum = CDBMgr::getInstance()->HavePropNumById(pDrawInfo->m_iGotItem);
                
                 CDataManager::SaveUserPropToDB(pDrawInfo->m_iGotItem,iNum+1);
            }

            
            if(pDrawInfo->m_iGotItem==41)
            {
                bHaveGoldenKey = true;
            }
            
            pDebrisIcon->setPosition(m_winSize.width*0.5,m_winSize.height*0.75);
            pDebrisIcon->runAction(Sequence::create(DelayTime::create(0.6f*i),CallFunc::create([=]{
              
                if(i==0)
                {
                    
                    SimpleAudioEngine::getInstance()->playEffect("music/DrawoffCardAppear.mp3");
                }
            }),Spawn::create(MoveTo::create(0.25f,Vec2(m_winSize.width*0.2+i%5*m_winSize.width*0.15,m_winSize.height*0.3-i/5*m_winSize.height*0.2)),RotateBy::create(0.25f,360),FadeIn::create(0.25),NULL),ScaleTo::create(0.2f,fScale*2),ScaleTo::create(0.2f,fScale),CallFunc::create([=]{
            
                if(i==9)
                {
                    
                    if(pEffect)
                    {
                        pEffect->removeFromParent();
                    }
                        SimpleAudioEngine::getInstance()->playEffect("music/lvlSucess.mp3",1);
                }
                
            }),NULL));
            addChild(pDebrisIcon,1);
            
            m_pMenuClose->setVisible(true);
        }
    
    }
    
    else if(m_iMagic==0) //一连抽金币
    {
    
        DrawCardResultInfo* pDrawInfo =  m_Parent->m_pDrawInfoCoin;
        
        Sprite *pDebrisIcon = NULL;
        
        float fScale = 4.0f;
        
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_COMMON_CARD) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
        {
             pDebrisIcon = CDataManager::getMonsterInfodebris(9);
             //CDBMgr::getInstance()->saveUserDebrisToDB(9,1);
        }
        else
        {
            
             if(m_CardInfos.m_bfirstDrawCoin)
             {
                 pDrawInfo->m_iType = 0;
             }
            
            if(pDrawInfo->m_iType ==0) //碎片
            {
                int id = random(1,13);
                
                pDebrisIcon = CDataManager::getMonsterInfodebris(id);
                //CDBMgr::getInstance()->saveUserDebrisToDB(id,1);
                
            }
            else if(pDrawInfo->m_iType ==1) //道具
            {
                propInfo*  info = CDataManager::getPropInfoById(pDrawInfo->m_iGotItem);
                
                char buf[50] = {0};
                
                sprintf(buf, "%s", iconBg[info->m_iItemLevel - 1].c_str());
                pDebrisIcon = Sprite::createWithSpriteFrameName(buf);
                
                sprintf(buf, "%s",info->m_sItemIcon.c_str());
                
                Sprite *pProp = Sprite::createWithSpriteFrameName(buf);
                
                Size tDBg = pDebrisIcon->getContentSize();
                
                pProp->setPosition(tDBg.width*0.5,tDBg.height*0.5);
                
                pDebrisIcon->addChild(pProp);
                fScale = 2.0f;
                
                //int iNum = CDBMgr::getInstance()->HavePropNumById(pDrawInfo->m_iGotItem);
                
                //CDataManager::SaveUserPropToDB(pDrawInfo->m_iGotItem,iNum+1);
                
                Label *pPropName = Label::createWithTTF(info->m_sItemName.c_str(),"fonts/cuti.ttf",15);
                pPropName->setColor(Color3B(255,184,12));
                pPropName->setPosition(tDBg.width*0.5,-tDBg.height*0.15);
                pDebrisIcon->addChild(pPropName);
                
            }
            else
            {
                pDebrisIcon  = Sprite::create();
            }
        }
        
        pDebrisIcon->setScale(0);
        pDebrisIcon->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
        addChild(pDebrisIcon,3);
        pDebrisIcon->runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([=]{
        
            SimpleAudioEngine::getInstance()->playEffect("music/DropCard.mp3");
        }),MoveTo::create(0.3f,Vec2(m_winSize.width*0.5,m_winSize.height*0.3)),RotateTo::create(0.1f,360),ScaleTo::create(0.1f,fScale),
                                                CallFunc::create([=]{
            
            SimpleAudioEngine::getInstance()->playEffect("music/lvlSucess.mp3",1);
            if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_COMMON_CARD) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
            {
                CDataManager::saveGuideInfoToDB(GUIDE_COMMON_CARD);
                guiderManagerLayer::getInstance()->createLayer(this);
            }
        
        }),
                                                NULL));
        
        pEffect->runAction(Sequence::create(theAnimate,DelayTime::create(0.1f),RemoveSelf::create(),NULL));
    
        m_pMenuClose->setVisible(true);
        //runAction(Sequence::create(DelayTime::create(4.0f),guide,NULL));
    
    }
    else if(m_iMagic == 2) //一连抽魔法石
    {
        DrawCardResultInfo* pDrawInfo;
    
        {
            int iRandom = random(1,100);
            
            if(iRandom<=40)
            {
                pDrawInfo = pStoneInfo[0];
            }
            else if (iRandom>40&&iRandom<=70)
            {
                pDrawInfo = pStoneInfo[1];
            }
            else if(iRandom>70&&iRandom<=90)
            {
                pDrawInfo = pStoneInfo[2];
            }
            else if(iRandom>90&&iRandom<=95)
            {
                pDrawInfo = pStoneInfo[3];
            }
            else if(iRandom>95&&iRandom<=98)
            {
                pDrawInfo = pStoneInfo[4];
            }
            else if(iRandom==99)
            {
                pDrawInfo = pStoneInfo[5];
            }
            else if(iRandom == 100)
            {
                pDrawInfo = pStoneInfo[6];
            }
            else
            {
                pDrawInfo = pStoneInfo[0];
            }
        }

        int iNum = pDrawInfo->m_iGotNum;
        
    
        int id = random(1,13);
        
        if(m_CardInfos.m_bfirstDrawStone)
        {
            iNum = 10;
        }
      
        
        //获取怪物的个数
      
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SENIOR_CARD) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
        {
            id = 12;
            iNum = 10;
        }
        

        Sprite *pDebrisIcon = CDataManager::getMonsterInfodebris(id);
        pDebrisIcon->setScale(0);
        pDebrisIcon->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
        addChild(pDebrisIcon,3);
        pDebrisIcon->runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([=]{
            
            SimpleAudioEngine::getInstance()->playEffect("music/GotHelpCard.mp3");
        }),MoveTo::create(0.3f,Vec2(m_winSize.width*0.5,m_winSize.height*0.3)),ScaleTo::create(0.1f,6),RotateTo::create(0.1f,360),ScaleTo::create(0.1f,4),CallFunc::create([=]{
            
            if(iNum>=2)
            {
                RunStarAnimate(iNum,pDebrisIcon);
            }
            SimpleAudioEngine::getInstance()->playEffect("music/lvlSucess.mp3",1);
        }),NULL));
        pEffect->runAction(Sequence::create(theAnimate,DelayTime::create(0.1f),RemoveSelf::create(), CallFunc::create([=]{
            if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SENIOR_CARD) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
            {
                guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
                Vec2 pos = Vec2(m_winSize.width*0.1,m_winSize.height*0.9);
                guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
            }

        }), NULL));

         m_pMenuClose->setVisible(true);
        
        //CDBMgr::getInstance()->saveUserDebrisToDB(id,iNum);
    }
    else if(m_iMagic == 3) //十连抽魔法石
    {
        Sprite *pMidLan2 = Sprite::createWithSpriteFrameName("goodsdikuang.png");
        pMidLan2->setPosition(m_winSize.width*0.5,m_winSize.height*0.1);
        pMidLan2->setScaleX(10);
        pMidLan2->setScaleY(7);
        pMidLan2->setOpacity(255);
        addChild(pMidLan2,1);
        pMidLan->setPosition(m_winSize.width*0.5,m_winSize.height*0.3);
        
        pEffect->stopAction(theAnimate);
        pEffect->runAction(RepeatForever::create(Sequence::create(theAnimate,NULL)));
  
        
        bool bHaveTen = false;
        
        for(int i=0;i<10;i++)
        {
            int iRandom = random(1,100);
            
            DrawCardResultInfo* pDrawInfo;
            
            if(iRandom<=40)
            {
                pDrawInfo = pStoneInfo[0];
            }
            else if (iRandom>40&&iRandom<=70)
            {
                pDrawInfo = pStoneInfo[1];
            }
            else if(iRandom>70&&iRandom<=90)
            {
                pDrawInfo = pStoneInfo[2];
            }
            else if(iRandom>90&&iRandom<=95)
            {
                pDrawInfo = pStoneInfo[3];
            }
            else if(iRandom>95&&iRandom<=98)
            {
                pDrawInfo = pStoneInfo[4];
            }
            else if(iRandom==99)
            {
                pDrawInfo = pStoneInfo[5];
            }
            else if(iRandom == 100)
            {
                pDrawInfo = pStoneInfo[6];
            }
            
            int iNum = pDrawInfo->m_iGotNum;
            int id = random(1,13);
        
            
            if(i==9&&!bHaveTen)
            {
                iNum  = 10;
            }
       
            Sprite *pDebrisIcon =  CDataManager::getMonsterInfodebris(id);
            pDebrisIcon->setPosition(m_winSize.width*0.5,m_winSize.height*0.75);
            pDebrisIcon->setScale(2);
            
            pDebrisIcon->setOpacity(255*0.5);
            pDebrisIcon->setCascadeOpacityEnabled(true);
            
            Size  tBgIco = pDebrisIcon->getContentSize();
            
            if(iNum == 10)
            {
                bHaveTen = true;
            }
            pDebrisIcon->runAction(Sequence::create(DelayTime::create(0.8f*i),CallFunc::create([=]{
                if(i==0)
                {
                    SimpleAudioEngine::getInstance()->playEffect("music/DrawoffCardAppear.mp3");
                }
            }),Spawn::create(MoveTo::create(0.25f,Vec2(m_winSize.width*0.2+i%5*m_winSize.width*0.15,m_winSize.height*0.3-i/5*m_winSize.height*0.2)),RotateBy::create(0.25f,360),FadeIn::create(0.25),NULL),ScaleTo::create(0.2f,8),ScaleTo::create(0.2f,4),CallFunc::create([=]{
             
                if(iNum>=2)
                {
                    RunStarAnimate(iNum,pDebrisIcon);
                }
                
                if(i==9)
                {
                    if(pEffect)
                    {
                        pEffect->removeFromParent();
                    }
                     SimpleAudioEngine::getInstance()->playEffect("music/lvlSucess.mp3",1);
                }
                
            }),NULL));
            
            addChild(pDebrisIcon,1,101);
            CDBMgr::getInstance()->saveUserDebrisToDB(id,iNum);
        }
       
    }
    if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SENIOR_CARD)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 6)
        {
             CDataManager::saveGuideInfoToDB(GUIDE_SENIOR_CARD);
        }
    }
    return true;
    
}

void  DrawCardSucess::RunStarAnimate(int iNum,Sprite *pDebrisIcon)
{
 
    
    Sprite *pEffectAnimate = Sprite::create();
    
    Size tBgIco  = pDebrisIcon->getContentSize();
    
    pEffectAnimate->setPosition(tBgIco.width*0.5,tBgIco.height*0.5);
    
    ClippingNode* clippingNode;
    
    Sprite *pbg =NULL;
    
    
    if(iNum <=3)
    {
        Animate* theAnimate = CFightManager::getAnimateWithPath("bluecircle_",1,4,".png",0.1f);
        
        pEffectAnimate->runAction(RepeatForever::create(theAnimate));
        pDebrisIcon->addChild(pEffectAnimate,-1);
        
    }
    else if(iNum==4||iNum==5)
    {
        Animate* theAnimate = CFightManager::getAnimateWithPath("blue_",1,10,".png",0.1f);
        pEffectAnimate->runAction(RepeatForever::create(theAnimate));
        pDebrisIcon->addChild(pEffectAnimate,1);
        
    }
    else if(iNum == 8 ||iNum == 10)
    {
        Animate* theAnimate = CFightManager::getAnimateWithPath("yellowfire_",1,15,".png",0.1f);
        pEffectAnimate->runAction(RepeatForever::create(theAnimate));
        pDebrisIcon->addChild(pEffectAnimate,1);
        
        Sprite *pGuang= Sprite::createWithSpriteFrameName("lg_dg.png");
        pGuang->setScaleY(0.6);
        pGuang->setScaleX(0.2);
        pGuang->setOpacity(255*0.75);
        pGuang->setFlippedY(true);

        
       pbg = Sprite::createWithSpriteFrameName("debrisBg.png");
        clippingNode = ClippingNode::create();
        clippingNode->setPosition(tBgIco.width*0.5,tBgIco.height*0.5);
        pDebrisIcon->addChild(clippingNode,999);
    
        cocos2d::Size  clipSize = pDebrisIcon->getContentSize();
       // clippingNode->setAlphaThreshold(0.01f); //设置alpha闸值
        clippingNode->setContentSize(clipSize); //设置尺寸大小
        clippingNode->setStencil(pbg);   //设置模板stencil
    
        clippingNode->addChild(pbg,98);  //先添加标题,会完全显示出来,因为跟模板一样大小
        clippingNode->addChild(pGuang,99);       //会被裁减*/
        
        pGuang->setRotation(15);
        
        pGuang->runAction(RepeatForever::create(
                         Sequence::create(ToggleVisibility::create(),MoveTo::create(0.1,Vec2(-10,0)),ToggleVisibility::create(),
                                           MoveTo::create(0.75,Vec2(tBgIco.width,-50)),
                                           NULL
                                           )));
    }
    
    pEffectAnimate->setScale(0.45);
    
    ParticleSystemQuad* pEffect2 = ParticleSystemQuad::create("particleSystem/star2.plist");
    pEffect2->setAutoRemoveOnFinish(true);
    pEffect2->setScale(0.2);
    pEffect2->setPosition(tBgIco.width*0.5,tBgIco.height*0.5);
    pDebrisIcon->addChild(pEffect2);
    
    
    char strNum[10] = {0};
    sprintf(strNum,"x%d",iNum);

    
    Sprite *pHeibg = Sprite::createWithSpriteFrameName("drawtenbg.png");
    pHeibg->setScale(0.2);
    pHeibg->setPosition(tBgIco.width*0.5,tBgIco.height*0.1);
    
    
    if(iNum == 8 ||iNum == 10)
    {
        pbg ->addChild(pHeibg,10);
    }
    else
    {
       pDebrisIcon->addChild(pHeibg);
    }
   
    
    if(iNum<=1)
    {
        pHeibg->setVisible(false);
    }
    
    Size tHeiBg = pHeibg->getContentSize();
    
    Sprite *pX = Sprite::createWithSpriteFrameName("x.png");
    pX->setPosition(tHeiBg.width*0.35,tHeiBg.height*0.5);
    pHeibg->addChild(pX);
    pX->setScale(4);
    pX->runAction(Sequence::create(ScaleTo::create(1,1),NULL));
    
    
    Label *theHurt = Label::createWithCharMap("fonts/Damage_num.png", 24, 36,'0');
    theHurt->setScale(4);
    theHurt->setString(strNum);
    theHurt->setPosition(tHeiBg.width*0.65,tHeiBg.height*0.5);
    pHeibg->addChild(theHurt);
    theHurt->runAction(Sequence::create(ScaleTo::create(1,1),NULL));
    
    
}
