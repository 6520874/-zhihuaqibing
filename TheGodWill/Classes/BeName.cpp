//
//  BeName.cpp
//  WarOfGod
//
//  Created by prefer_shan on 15/4/14.
//
//

#include "BeName.h"
#include "LYMenuItemSprite.h"
#include "DBMgr.h"
#include "LYFightData.h"
#include "LYPersonalData.h"
#include "CSharedPlist.h"
#include "NetWorkDataSingle.h"

CBeNameLayer::CBeNameLayer(int index)
{
    m_index = index;
    m_pListener = NULL;
    m_pInput = NULL;
    m_pTalkText = NULL;
    m_pBg=  NULL;
    CSharedPlist::getInstance()->pushPlist("plist/skill.plist");
    
}

CBeNameLayer::~CBeNameLayer()
{
    CSharedPlist::getInstance()->erasePlist("plist/skill.plist");
}


bool CBeNameLayer::onTouchBegan(Touch *touch,Event *event)
{

    log("beName\n");
    
   // Sprite *pBg
    
    Vec2 location = touch->getLocation();
    
    cocos2d::Rect rect = m_pBg->boundingBox();
    
    if(!rect.containsPoint(location))
    {
    
         if(m_pTalkText)
         {
             m_pTalkText->detachWithIME();
         }
    }

    
    return true;
}

void CBeNameLayer::onTouchMoved(Touch *touch,Event *event)
{

}


void CBeNameLayer::onTouchCancelled(Touch *touch, Event *event)
{

    
}

void CBeNameLayer::onTouchEnded(Touch *touch,Event *event)
{

}


void CBeNameLayer::OnOpenInput(Ref *pSender)
{
    
    m_pTalkText->attachWithIME();
    m_pTalkText->setDelegate(this);
    
}

void CBeNameLayer::OnYaoShaizi(Ref *pSender)
{
    //log("onYaoShaizi\n");

    int iRandAdj = rand()%m_AdjVec.size();
    int iNameVec = rand()%m_NameVec.size();
    
    
    std::string str11 = m_AdjVec[iRandAdj].getCString();
    std::string str21  = m_NameVec[iNameVec].getCString();

    std::string strAll = str11+str21;
    
    m_pTalkText->setString(strAll);
    
}


void CBeNameLayer::OnUpdateSure(Ref *pSender)
{
    if(m_index==1)
    {
        CPersonalData::getInstance()->setUpdateUserInfo(true);
        removeFromParent();
        
        return ;
    }
    int iLevelId = 1;
    
    CFightData::getInstance()->newLevelStart(iLevelId);
    CFightData::getInstance()->setNowLevelId(iLevelId);
    CFightData::getInstance()->setNowMidLevelId(1);
    
    
    
    //保存用户表
    
    CLayerManager::GetSingletonPtr()->createLayerWithLoading(eLayer_Fight,NULL,false);
}

void CBeNameLayer::OnCancle(Ref *pSender)
{
    removeFromParent();
    
}



void CBeNameLayer::OnSure(Ref *pSender)
{
    //log("onSure\n");
    m_pTalkText->detachWithIME();
    
    UserInfo* pInfo = CPersonalData::getInstance()->getUserInfo();
    pInfo->userName = m_pTalkText->getString();
    
    //保存用户表
    
    CDBMgr::getInstance()->saveUserAllInfo();


    CNetWorkDataSingle::GetSingletonPtr()->updateUserName(this,menu_selector(CBeNameLayer::OnUpdateSure),pInfo->userName);


    
    
    
    
}

void CBeNameLayer::onExit()
{
    _eventDispatcher->removeEventListener(m_pListener);
    CBaseLayer::onExit();
}

bool CBeNameLayer::onTextFieldAttachWithIME(TextFieldTTF * sender)
{
    if(m_pTalkText!=NULL)
    {
        //m_pTalkText->setString("");
    }
    
    return false;
}


bool CBeNameLayer::onTextFieldDetachWithIME(TextFieldTTF * sender)
{
    m_sTalkContent = sender->getString();
    
    return false;
}

bool CBeNameLayer::onTextFieldInsertText(TextFieldTTF * sender, const char * text, int nLen)
{
    std::string str = sender->getString();
    m_sTalkContent= str+text;
    
    if (sender->getCharCount() >=18)
    {
        m_pTalkText->detachWithIME();
        
        return true;
    }
    return false;
}

bool CBeNameLayer::onTextFieldDeleteBackward(TextFieldTTF * sender, const char * delText, int nLen)
{
    
    return false;
}

bool CBeNameLayer::init()
{
    //CBaseLayer::init();
    CBaseLayer::initWithColor(Color4B(0, 0, 0,255*0.8));
    
    m_pListener = EventListenerTouchOneByOne::create();
    m_pListener->setSwallowTouches(true);
    m_pListener->onTouchBegan = CC_CALLBACK_2(CBeNameLayer::onTouchBegan, this);
    m_pListener->onTouchMoved = CC_CALLBACK_2(CBeNameLayer::onTouchMoved, this);
    m_pListener->onTouchEnded = CC_CALLBACK_2(CBeNameLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener,this);
    
    
    std::vector<UserName> UserName = CDBMgr::getInstance()->GetUserNameInfo();
    std::string strName1 =  UserName[0].m_sUserName1;
    std::string strName2 =  UserName[0].m_sUserName2;
    
    m_AdjVec = CDBMgr::getInstance()->separateInfoWithMark(strName1,";");
    m_NameVec = CDBMgr::getInstance()->separateInfoWithMark(strName2,";");
    
    int iRandAdj = rand()%m_AdjVec.size();
    int iNameVec = rand()%m_NameVec.size();

    std::string str11 = m_AdjVec[iRandAdj].getCString();
    std::string str21  = m_NameVec[iNameVec].getCString();
    std::string strAll = str11+str21;
    

    Sprite *pSoldier = Sprite::createWithSpriteFrameName("fengshen.png");

    pSoldier->setPosition(pSoldier->getBoundingBox().size.width*0.5,m_winSize.height*0.5);
    
    addChild(pSoldier);
    
    //
    
    m_pBg = Sprite::createWithSpriteFrameName("name_bg2.png");
    m_pBg->setPosition(m_winSize.width-m_pBg->getBoundingBox().size.width*0.5,m_winSize.height*0.725);
    addChild(m_pBg,1);
    
    float  fJiange = m_pBg->getPositionX()-pSoldier->getPositionX();
    
    float fMaxJiange = pSoldier->getBoundingBox().size.width*0.5+m_pBg->getBoundingBox().size.width*0.5;
    
    
    if(fJiange>fMaxJiange)
    {
        m_pBg->setPosition(pSoldier->getBoundingBox().size.width+m_pBg->getBoundingBox().size.width*0.5,m_winSize.height*0.725);
        
    }
    
    Size tBg = m_pBg->getBoundingBox().size;
    Sprite *pInputNormal = Sprite::createWithSpriteFrameName("name_input.png");
    Sprite *pInputSelect = Sprite::createWithSpriteFrameName("name_input.png");
    pInputSelect->setColor(Color3B::GRAY);
    
    MenuItemSprite *pInputItem = MenuItemSprite::create(pInputNormal,pInputSelect,CC_CALLBACK_1(CBeNameLayer::OnOpenInput, this));
    Menu *pMenuInput = Menu::create(pInputItem,NULL);
    pMenuInput->setPosition(tBg.width*0.45,tBg.height*0.6);
    m_pBg->addChild(pMenuInput);
    
    
    Size tInputBg = pInputItem->getBoundingBox().size;
    m_pTalkText = TextFieldTTF::textFieldWithPlaceHolder("",Size(tInputBg.width*0.95,0), TextHAlignment::LEFT,"Arial",30);
    
    m_pTalkText->setPosition(tInputBg.width*0.5,tInputBg.height*0.5);
    pInputItem->addChild(m_pTalkText);
    
    
    m_pTalkText->setString(strAll);
    
    Label *pName = Label::createWithTTF("您的大名","fonts/cuti.ttf", 30);
    pName->setColor(Color3B(239,198,40));
    pName->setPosition(tBg.width*0.5,tBg.height*0.9);
    m_pBg->addChild(pName);
    
    Sprite *pShaizi =  Sprite::createWithSpriteFrameName("name_shaizi.png");
    Sprite *pShaiziSelcet = Sprite::createWithSpriteFrameName("name_shaizi.png");
    pShaiziSelcet->setColor(Color3B::GRAY);
    
    MenuItemSprite *pShaiziItem = MenuItemSprite::create(pShaizi,pShaiziSelcet,CC_CALLBACK_1(CBeNameLayer::OnYaoShaizi, this));
    
    pShaiziItem->setScale(0.8);
    Menu *pMenShaizi = Menu::create(pShaiziItem,NULL);
    pMenShaizi->setPosition(tBg.width*0.91,tBg.height*0.6);
    m_pBg->addChild(pMenShaizi);
    
    Sprite *pSure1 = Sprite::createWithSpriteFrameName("fight_blueBtn.png");
    Sprite *pSure2 =  Sprite::createWithSpriteFrameName("fight_blueBtn.png");
    Sprite *pSure3 =  Sprite::createWithSpriteFrameName("fight_blueBtn.png");
    pSure3->setColor(Color3B::GRAY);
    pSure2->setColor(Color3B::GRAY);

    
    LYMenuItemSprite *pSureItem = LYMenuItemSprite::create(pSure1,pSure2,pSure3,CC_CALLBACK_1(CBeNameLayer::OnSure,this));
    pSureItem->setPosition(tBg.width*0.5,tBg.height*0.2);
    
    Label * pLabelReturn = Label::createWithTTF("确定","fonts/cuti.ttf",30);
    pLabelReturn->enableOutline(Color4B(0,0,0,255),3);
    Size tBlueSize = pSureItem->getBoundingBox().size;
    pLabelReturn->setPosition(tBlueSize.width*0.5,tBlueSize.height*0.52);
    pSureItem->addChild(pLabelReturn);

    
    
    LYMenuItemSprite *pCancleItem = NULL;
    
    if(m_index==1)
    {
        Sprite * pCancle1 = Sprite::createWithSpriteFrameName("fight_redBtn.png");
         Sprite * pCancle2 = Sprite::createWithSpriteFrameName("fight_redBtn.png");
         Sprite * pCancle3 = Sprite::createWithSpriteFrameName("fight_redBtn.png");
        
        pCancle2->setColor(Color3B::GRAY);
       pCancle3->setColor(Color3B::GRAY);
        pCancleItem = LYMenuItemSprite::create(pCancle1,pCancle2,pCancle3,CC_CALLBACK_1(CBeNameLayer::OnCancle,this));
        
        Label * pLabelReturn = Label::createWithTTF("取消","fonts/cuti.ttf",30);
         pLabelReturn->enableOutline(Color4B(0,0,0,255),3);
        Size tBlueSize = pCancleItem->getBoundingBox().size;
        pLabelReturn->setPosition(tBlueSize.width*0.5,tBlueSize.height*0.52);
        pCancleItem->addChild(pLabelReturn);
        
        
        pCancleItem->setPosition(tBg.width*0.3,tBg.height*0.2);
         pSureItem->setPosition(tBg.width*0.7,tBg.height*0.2);
        
    }
    Menu *pMenuSure = Menu::create(pSureItem,pCancleItem,NULL);

    pMenuSure->setPosition(Vec2(0,0));
    
    m_pBg->addChild(pMenuSure);
    
    
    
    return true;
}

