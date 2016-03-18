//
//  LvIntroduce.cpp
//  WarOfGod
//
//  Created by prefer_shan on 15/4/10.
//
//

#include "LvIntroduce.h"
#include "LYMenuItemSprite.h"
#include "LYPersonalData.h"

CLvIntroduce::CLvIntroduce()
{
    m_winSize = Director::getInstance()->getWinSize();
    m_pListener = NULL;
}

CLvIntroduce::~CLvIntroduce()
{
    
}



void CLvIntroduce::OnCloseMenu(Ref *pSender)
{

    log("oncloseMenu\n");
    removeFromParent();

}


void CLvIntroduce::onExit()
{
    _eventDispatcher->removeEventListener(m_pListener);
    CBaseLayer::onExit();
    
}

bool CLvIntroduce::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{

    return true;
}


void CLvIntroduce::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{

}

void CLvIntroduce::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{

}

void CLvIntroduce::onTouchCancelled(Touch *touch, Event *event)
{

}



bool CLvIntroduce::init()
{
    
    

    CBaseLayer::init();
    CBaseLayer::initWithColor(Color4B(0, 0, 0, 125));
    
    
    m_pListener = EventListenerTouchOneByOne::create();
    m_pListener->setSwallowTouches(true);
    m_pListener->onTouchBegan = CC_CALLBACK_2(CLvIntroduce::onTouchBegan, this);
    m_pListener->onTouchMoved = CC_CALLBACK_2(CLvIntroduce::onTouchMoved, this);
    m_pListener->onTouchEnded = CC_CALLBACK_2(CLvIntroduce::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener,this);
    
    
    
    Sprite *pMainlLayer =  Sprite::createWithSpriteFrameName("common_bg.png");
    pMainlLayer->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(pMainlLayer,0,100);
    
    Size tMain = pMainlLayer->getBoundingBox().size;
    
    Label *pLv = Label::createWithSystemFont("等级介绍","fonts/cuti.tt",30);
    pLv->setColor(Color3B(249,212,63));
    pLv->setPosition(tMain.width*0.5,tMain.height*0.9);
    pMainlLayer->addChild(pLv);
    
    Sprite* progressBg = Sprite::createWithSpriteFrameName("level.png");
    progressBg->setPosition(tMain.width*0.7,tMain.height*0.6);
    progressBg->setScale(0.8);
    pMainlLayer->addChild(progressBg);
    
    //level_light.png
    
    Sprite *pLight = Sprite::createWithSpriteFrameName("level_light.png");
    pLight->setPosition(tMain.width*0.7,tMain.height*0.6);
    pLight->setScale(0.8);
    pMainlLayer->addChild(pLight,1);
    
    Sprite* pLevelBg = Sprite::createWithSpriteFrameName("level_progress.png");
    pLevelBg->setScale(0.8);
    pLevelBg->setPosition(tMain.width*0.7,tMain.height*0.6);
    pMainlLayer->addChild(pLevelBg);
    
    Size tLevelSize = pLevelBg->getBoundingBox().size;
    int  iLv =  CPersonalData::getInstance()->getUserInfo()->m_iUserLevel;
    char strUserLv[10] = {0};
    sprintf(strUserLv,"%d",iLv);

    
    Label *pLvNumber = Label::createWithSystemFont(strUserLv,"Aril",30);
    pLvNumber->setPosition(tLevelSize.width*0.6,tLevelSize.height*0.55);
    pLevelBg->addChild(pLvNumber);
    
    char strLv[50] = {0};
    sprintf(strLv,"当前士兵可升级的最大等级为");
    char soldierLv2[100] = {"士兵等级最大不得超过玩家等级        的2倍"};

    
    Label *pSoldier1 = Label::createWithTTF(strLv,"fonts/cuti.ttf", 30);
    pSoldier1->setAlignment(TextHAlignment::LEFT);
    pSoldier1->setPosition(tMain.width*0.4,tMain.height*0.7);
    pMainlLayer->addChild(pSoldier1);

    sprintf(strUserLv,"%d",iLv*2);
    
    Label *pUserLv = Label::createWithTTF(strUserLv,"fonts/cuti.ttf",30);
    pUserLv->setColor(Color3B(255,112,16));
    pUserLv->setPosition(pSoldier1->getPositionX()+pSoldier1->getContentSize().width*0.53,tMain.height*0.7);
    pMainlLayer->addChild(pUserLv);

    
    Label *pSlRemain = Label::createWithTTF("级","fonts/cuti.ttf",30);
    pSlRemain->setPosition(pUserLv->getPositionX()+pUserLv->getContentSize().width+pSlRemain->getContentSize().width*0.5,tMain.height*0.7);
    pMainlLayer->cocos2d::Node::addChild(pSlRemain);

    Label *pSoldier2 = Label::createWithTTF(soldierLv2,"fonts/cuti.ttf",30);
    pSoldier2->setAlignment(TextHAlignment::LEFT);
    pSoldier2->setPosition(tMain.width*0.5,tMain.height*0.6);
    pMainlLayer->addChild(pSoldier2);

    Label *pSoldier3  = Label::createWithTTF("想要提高士兵最大等级赶快升级玩家等级吧!","fonts/cuti.ttf",30);
    pSoldier3->setColor(Color3B(131,231,101));
    pSoldier3->setPosition(tMain.width*0.5,tMain.height*0.4);
    pMainlLayer->addChild(pSoldier3);
    
    Sprite *pCloseBtn = Sprite::createWithSpriteFrameName("closeBtn.png");
    Sprite *pCloseBtnSelect = Sprite::createWithSpriteFrameName("closeBtn.png");
    pCloseBtnSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pCloseItem = LYMenuItemSprite::create(pCloseBtn,pCloseBtnSelect,CC_CALLBACK_1(CLvIntroduce::OnCloseMenu,this));
    
    Menu *pMenuClose = Menu::create(pCloseItem,NULL);
    pMenuClose->setPosition(tMain.width*0.95,tMain.height*0.85);
    pMainlLayer->addChild(pMenuClose);

    
    return true;
}




