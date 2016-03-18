//
//  CHitDialog.cpp
//  TheGodWill
//
//  Created by prefer_shan on 15/1/4.
//
//

#include "CHitDialog.h"
#include "LYPersonalData.h"

CHitDialog::CHitDialog()
{
    m_winSize = Director::getInstance()->getWinSize();
    m_bMenuNo = false;
    m_bMenuYes = false;
    m_bMenuHitDialog = false;
    m_pMenuHitDialog = NULL;
    m_pMenuYes  = NULL;
    m_pMenuNo = NULL;
    m_pTarget = NULL;
    m_pHandler= NULL;
    m_eDType= e_Common;
    
}

CHitDialog::~CHitDialog()
{


}


bool CHitDialog::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if(m_pMenuHitDialog)
    m_bMenuHitDialog = m_pMenuHitDialog->onTouchBegan(pTouch,pEvent);
    
    if(m_pMenuYes)
    {
    m_bMenuYes = m_pMenuYes->onTouchBegan(pTouch,pEvent);
    }
    if(m_pMenuNo)
    {
    m_bMenuNo = m_pMenuNo->onTouchBegan(pTouch,pEvent);
    }
    
    return  true;
}

void CHitDialog::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    if(m_bMenuHitDialog)
    {
        m_pMenuHitDialog->onTouchMoved(pTouch,pEvent);
    }
    else if(m_bMenuYes)
    {
        m_pMenuYes->onTouchMoved(pTouch,pEvent);
    }
    else if(m_bMenuNo)
    {
        m_pMenuNo->onTouchMoved(pTouch,pEvent);
    }
}

void CHitDialog::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if(m_bMenuHitDialog)
    {
        m_pMenuHitDialog->onTouchEnded(pTouch,pEvent);
        m_bMenuHitDialog = false;
    }
    else if(m_bMenuYes)
    {
        m_pMenuYes->onTouchEnded(pTouch,pEvent);
        m_bMenuYes = false;
    }
    else if(m_bMenuNo)
    {
        m_pMenuNo->onTouchEnded(pTouch,pEvent);
        m_bMenuNo = false;
    }
}

void CHitDialog::onTouchCancelled(Touch *touch, Event *pEvent)
{
    if(m_bMenuHitDialog)
    {
        m_pMenuHitDialog->onTouchCancelled(touch,pEvent);
        m_bMenuHitDialog = false;
    }
    else if(m_bMenuYes)
    {
        m_pMenuYes->onTouchCancelled(touch,pEvent);
        m_bMenuYes =  false;
    }
    else if(m_bMenuNo)
    {
        m_pMenuNo->onTouchCancelled(touch,pEvent);
        m_bMenuNo = false;
    }
    
}


void CHitDialog::onExit()
{
    _eventDispatcher->removeEventListener(m_pListner);
    
    CBaseLayer::onExit();
 
}



void CHitDialog::CreateFightExitDialog(cocos2d::Ref * pTarget,cocos2d::SEL_MenuHandler pHandlerYes,cocos2d::SEL_MenuHandler pHandlerNo)
{
    m_pTarget = pTarget;
    m_pHandler = pHandlerYes;
    
    m_pHandler2 = pHandlerNo;
 
    
    Sprite *pkuang = Sprite::createWithSpriteFrameName("pvp_smallmap.png");
    
    pkuang->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    
    addChild(pkuang,99,1997);
    
    Sprite *pbg = Sprite::createWithSpriteFrameName("pvp_infoBg.png");
    
    pbg->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    
    addChild(pbg,99,1998);
    
    
    
    cocos2d::Size tBg = pbg->getContentSize();
    
    
    std::string str ="是否退出战斗?";
    
    
    if(CPersonalData::getInstance()->getEnemyType() == FIGHT_BACK_VIDEO)
    {
        str = "是否退出回放?";
    }
    Label *pLabelFight = Label::createWithTTF(str, "fonts/cuti.ttf", 30);
    
    pLabelFight->enableOutline(Color4B(0,0,0,255),3);
    pLabelFight->setPosition(tBg.width*0.5,tBg.height*0.7);
    
    pbg->addChild(pLabelFight);
    
    
    Sprite *pRedBtn = Sprite::createWithSpriteFrameName("redbtn.png");
    
    Sprite *pRedBtnSelct = Sprite::createWithSpriteFrameName("redbtn.png");
    
    pRedBtnSelct->setColor(Color3B::GRAY);
    
    
    Sprite *pGreenBtn = Sprite::createWithSpriteFrameName("greenbtn.png");
    
    Sprite *pGreenBtnSelct = Sprite::createWithSpriteFrameName("greenbtn.png");
    
    pRedBtnSelct->setColor(Color3B::GRAY);
    
    MenuItemSprite *pCancleItem = MenuItemSprite::create(pRedBtn, pRedBtnSelct,CC_CALLBACK_1(CHitDialog::OnNo, this));
    pCancleItem->setScale(2);
    pCancleItem->setPosition(tBg.width*0.3,tBg.height*0.3);
    
    
    Label *pCancleLable = Label::createWithTTF("取消", "fonts/cuti.ttf", 30);
    
    pCancleLable->cocos2d::Node::setScale(0.5);
    
    pCancleLable->enableOutline(Color4B(0,0,0,255),3);
    
    pCancleLable->setPosition(pCancleItem->getContentSize().width*0.5,pCancleItem->getContentSize().height*0.5);
    
    pCancleItem->addChild(pCancleLable);
    
    MenuItemSprite *pOkItem = MenuItemSprite::create(pGreenBtn, pGreenBtnSelct,CC_CALLBACK_1(CHitDialog::OnYes, this));
    pOkItem->setPosition(tBg.width*0.7,tBg.height*0.3);
    
    pOkItem->setScale(2);
    
    Label *pOkLable = Label::createWithTTF("确定", "fonts/cuti.ttf", 30);
    
    pOkLable->cocos2d::Node::setScale(0.5);
    pOkLable->enableOutline(Color4B(0,0,0,255),3);
    pOkLable->setPosition(pOkItem->getContentSize().width*0.5,pOkItem->getContentSize().height*0.5);
    pOkItem->addChild(pOkLable);
    
    
    //m_pMenuNo = Menu::create(pAgaginItem,NULL);

    
    m_pMenuNo = Menu::create(pCancleItem,pOkItem,NULL);
    
    m_pMenuNo->setPosition(Vec2(0,0));
    pbg->addChild(m_pMenuNo,99);
    Director::getInstance()->pause();
}


void CHitDialog::CreateHuifanDialog(cocos2d::Ref * pTarget,cocos2d::SEL_MenuHandler pHandlerYes,cocos2d::SEL_MenuHandler pHandlerNo)
{

    m_pTarget = pTarget;
    m_pHandler = pHandlerYes;
    
    m_pHandler2 = pHandlerNo;
   
    
    LayerColor *pColor  = LayerColor::create();
    
    pColor->setColor(Color3B::BLACK);
    
    pColor->setOpacity(255*0.8);
    
    pColor->setContentSize(cocos2d::Size(m_winSize.width,m_winSize.height));
    
    addChild(pColor,9999);
    

    
    Sprite *pBackDi = Sprite::createWithSpriteFrameName("pvp_fight.png");
    
    Sprite *pBackDiSelect = Sprite::createWithSpriteFrameName("pvp_fight.png");
    
    MenuItemSprite * pBackItem =  MenuItemSprite::create(pBackDi,pBackDiSelect,CC_CALLBACK_1(CHitDialog::OnNo, this));
    
    m_pMenuYes = Menu::create(pBackItem,NULL);
    m_pMenuYes->setPosition(m_winSize.width*0.1,m_winSize.height*0.1);
    
    pColor->addChild(m_pMenuYes);
    
    Label *pBack = Label::createWithTTF("返回营地", "fonts/cuti.ttf", 30);
    
    pBack->enableOutline(Color4B(0,0,0,255),3);
    
    pBack->setPosition(pBackItem->getContentSize().width*0.5,pBackItem->getContentSize().height*0.5);
    
    pBackItem->addChild(pBack);
    
    
    Sprite *pZhongfa = Sprite::createWithSpriteFrameName("rebo.png");
    Sprite *pZhongfaSelect  = Sprite::createWithSpriteFrameName("rebo.png");
    
    MenuItemSprite *pAgaginItem = MenuItemSprite::create(pZhongfa,pZhongfaSelect,CC_CALLBACK_1(CHitDialog::OnYes,this));
    
    
    m_pMenuNo = Menu::create(pAgaginItem,NULL);
    
    m_pMenuNo->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    
    pColor->addChild(m_pMenuNo);
    
    
    Label *pRebo = Label::createWithTTF("重放", "fonts/cuti.ttf", 30);
    
    pRebo->enableOutline(Color4B(0,0,0,255),3);
    
    pRebo->setPosition(pAgaginItem ->getContentSize().width*0.5,-pAgaginItem ->getContentSize().height*0.25);
    pAgaginItem->addChild(pRebo);

    
}

void CHitDialog::CreateUrlDialog(std::string sContent,std::string strUrl,cocos2d::Ref * pTarget ,cocos2d::SEL_MenuHandler pHandlerYes,cocos2d::SEL_MenuHandler pHandlerNo,e_DialogType eType)
{

    m_eDType = eType;
    
    m_strUrl = strUrl;
    
    m_pTarget = pTarget;
    m_pHandler = pHandlerYes;
    
    m_pHandler2 = pHandlerNo;
    
    Sprite *pBg = Sprite::createWithSpriteFrameName("name_bg2.png");
    pBg->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(pBg);
    

    Size tBg = pBg->getContentSize();

    Label *pBiaoti =  Label::createWithSystemFont("错误","Arial",30);
    
    pBiaoti->setPosition(tBg.width*0.5,tBg.height*0.9);
    
    pBg->addChild(pBiaoti);

    
    
    Label *pText = Label::createWithSystemFont(sContent,"Aril",30);
    pText->setAlignment(TextHAlignment::CENTER);
    pText->setPosition(tBg.width*0.5,tBg.height*0.6);
    pBg->addChild(pText);
    
    //yes按钮
    Sprite *pYes = Sprite::createWithSpriteFrameName("greenan.png");
    
    Sprite *pYesSelect = Sprite::createWithSpriteFrameName("greenan.png");
    pYesSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pYesSelectItem  = LYMenuItemSprite::create(pYes,pYesSelect,CC_CALLBACK_1(CHitDialog::OnYes, this));
    pYesSelectItem->setScale(1.5);
    
    m_pMenuYes = Menu::create(pYesSelectItem,NULL);
    m_pMenuYes->setPosition(tBg.width*0.3,tBg.height*0.3);
    pBg->addChild(m_pMenuYes);
    
    Size tYesItem  = pYesSelectItem->getContentSize();
    
    Label *pLabelYes = Label::createWithSystemFont("是","Aril",30);
    pLabelYes->setScale(0.5);
    pLabelYes->setPosition(tYesItem.width*0.5,tYesItem.height*0.5);
    pYesSelectItem->addChild(pLabelYes);
    
    Sprite *pNo  = Sprite::createWithSpriteFrameName("redan.png");
    Sprite *pNoSelect = Sprite::createWithSpriteFrameName("redan.png");
    pNoSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pNoItemSprite = LYMenuItemSprite::create(pNo,pNoSelect,CC_CALLBACK_1(CHitDialog::OnNo, this));
    pNoItemSprite->setScale(1.5);
    Size tNoItem  = pNoItemSprite->getContentSize();
    
    m_pMenuNo = Menu::create(pNoItemSprite,NULL);
    m_pMenuNo->setPosition(tBg.width*0.7,tBg.height*0.3);
    pBg->addChild(m_pMenuNo);
    
    Label *pLabelNo = Label::createWithSystemFont("否","Aril",30);
    pLabelNo->setScale(0.5);
    pLabelNo->setPosition(tNoItem.width*0.5,tNoItem.height*0.5);
    pNoItemSprite->addChild(pLabelNo);

   

}

void CHitDialog::createYesNO(cocos2d::Ref * pTarget,cocos2d::SEL_MenuHandler pHandler,std::string sContent,cocos2d::SEL_MenuHandler pHandlerNo)
{
   /* if (sContent.size()>0)
    {
        m_pLabelContent->setDimensions(m_showSize);
        m_pLabelContent->setString(sContent.c_str());
        m_pLabelContent->setVisible(true);
    }*/
    

    m_pTarget = pTarget;
    m_pHandler = pHandler;

    m_pHandler2 = pHandlerNo;
    
    Sprite *pBg = Sprite::createWithSpriteFrameName("name_bg2.png");
    pBg->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(pBg);
    
    Size tBg = pBg->getContentSize();
    
    Label *pText = Label::createWithSystemFont(sContent,"Aril",30);
    pText->setAlignment(TextHAlignment::CENTER);
    pText->setPosition(tBg.width*0.5,tBg.height*0.6);
    pBg->addChild(pText);
    
    //yes按钮
    Sprite *pYes = Sprite::createWithSpriteFrameName("greenan.png");
    
    Sprite *pYesSelect = Sprite::createWithSpriteFrameName("greenan.png");
    pYesSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pYesSelectItem  = LYMenuItemSprite::create(pYes,pYesSelect,CC_CALLBACK_1(CHitDialog::OnYes, this));
    pYesSelectItem->setScale(1.5);
    
    m_pMenuYes = Menu::create(pYesSelectItem,NULL);
    m_pMenuYes->setPosition(tBg.width*0.7,tBg.height*0.2);
    pBg->addChild(m_pMenuYes);
    
    Size tYesItem  = pYesSelectItem->getContentSize();

    Label *pLabelYes = Label::createWithSystemFont("是","Aril",30);
    pLabelYes->setScale(0.5);
    pLabelYes->setPosition(tYesItem.width*0.5,tYesItem.height*0.5);
    pYesSelectItem->addChild(pLabelYes);
    
    Sprite *pNo  = Sprite::createWithSpriteFrameName("redan.png");
    Sprite *pNoSelect = Sprite::createWithSpriteFrameName("redan.png");
    pNoSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pNoItemSprite = LYMenuItemSprite::create(pNo,pNoSelect,CC_CALLBACK_1(CHitDialog::OnNo, this));
    pNoItemSprite->setScale(1.5);
    Size tNoItem  = pNoItemSprite->getContentSize();
    
    m_pMenuNo = Menu::create(pNoItemSprite,NULL);
    m_pMenuNo->setPosition(tBg.width*0.3,tBg.height*0.2);
    pBg->addChild(m_pMenuNo);
    
    Label *pLabelNo = Label::createWithSystemFont("否","Aril",30);
    pLabelNo->setScale(0.5);
    pLabelNo->setPosition(tNoItem.width*0.5,tNoItem.height*0.5);
    pNoItemSprite->addChild(pLabelNo);
    
}

void CHitDialog::OnYes(Ref *pSender)
{
    log("yes");
    
    if (m_pTarget && m_pHandler)
    {
        (m_pTarget->*m_pHandler)(pSender);
    }
    
 if( m_eDType == e_OpenUrl)
 {
     Application::getInstance()->openURL(m_strUrl);
     
 }

    removeFromParent();
}


void CHitDialog::OnNo(Ref *pSender)
{
     log("no");
    if (m_pTarget && m_pHandler2)
    {
        (m_pTarget->*m_pHandler2)(pSender);
    }
    
    if(CPersonalData::getInstance()->getPvpMainLayer())
    {
        CPersonalData::getInstance()->setTouchRmvFlag(true);
    }
    removeFromParent();
    
}

void CHitDialog::CreateDialog(std::string strText ,Ref * pTarget,cocos2d::SEL_MenuHandler pHandler)
{
    m_pTarget = pTarget;
    m_pHandler = pHandler;
    
    
    
    Sprite *pBg = Sprite::createWithSpriteFrameName("name_bg2.png");
    pBg->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(pBg);
    
    Size tBg = pBg->getBoundingBox().size;
    
    Label *pBiaoti =  Label::createWithSystemFont("错误","Arial",30);

    pBiaoti->setPosition(tBg.width*0.5,tBg.height*0.9);
    
    pBg->addChild(pBiaoti);
    
    Label *pText = Label::createWithSystemFont(strText,"Arial",30);
    pText->setAlignment(TextHAlignment::CENTER);
    pText->setPosition(tBg.width*0.5,tBg.height*0.6);
    pBg->addChild(pText);
    
    
    Sprite *pOk = Sprite::createWithSpriteFrameName("greenbtn.png");
    Sprite *pOkSelect = Sprite::createWithSpriteFrameName("greenbtn.png");

 
    LYMenuItemSprite *pOkItem = LYMenuItemSprite::create(pOk,pOkSelect,CC_CALLBACK_1(CHitDialog::OnExitLayer, this));
    pOkItem->setPosition(tBg.width*0.5,tBg.height*0.225);
    
    pOkItem->setScale(3);

    m_pMenuHitDialog = Menu::create(pOkItem,NULL);
    m_pMenuHitDialog->setAnchorPoint(Point::ZERO);
    m_pMenuHitDialog->setPosition(Point::ZERO);
    pBg->addChild(m_pMenuHitDialog);
    
    
    Size tOk = pOk->getBoundingBox().size;
    
    Label *pOkName =  Label::createWithTTF("确定","fonts/cuti.ttf",20);

    
    pOkName->setScale(0.66);
    pOkName->setPosition(tOk.width*0.5,tOk.height*0.5);
    pOk->addChild(pOkName);
    
}

void CHitDialog::OnExitLayer(Ref *pSender)
{
    
    if (m_pTarget && m_pHandler)
    {
        (m_pTarget->*m_pHandler)(pSender);
    }
    
    removeFromParent();
}

bool CHitDialog::init()
{
    CBaseLayer::initWithColor(Color4B(0, 0, 0, 125));
    
    m_pListner = EventListenerTouchOneByOne::create();//创建一个触摸监听
    m_pListner->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false
    
    m_pListner->onTouchBegan = CC_CALLBACK_2(CHitDialog::onTouchBegan, this);
    m_pListner->onTouchMoved = CC_CALLBACK_2(CHitDialog::onTouchMoved, this);
    m_pListner->onTouchEnded = CC_CALLBACK_2(CHitDialog::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithFixedPriority(m_pListner,-400);
    
    return true;
}