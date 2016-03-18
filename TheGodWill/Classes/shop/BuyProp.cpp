//
//  BuyProp.cpp
//  WarOfGod
//
//  Created by prefer_shan on 15/5/4.
//
//

#include "BuyProp.h"
#include "DBMgr.h"
#include "LYFightManager.h"
#include "NetWorkDataSingle.h"
NS_CC_EXT_BEGIN

bool CBuyProp::onTouchBegan(Touch *pTouch, Event *pEvent)
{
   m_bContinerMenu  = m_solider->onTouchBegan(pTouch,pEvent);
    m_bSureMenu  =  m_pSureMenu->onTouchBegan(pTouch,pEvent);
    m_bCancleMenu = m_pCancleMenu->onTouchBegan(pTouch,pEvent);
    m_bMenuMinus = m_pMenuMinus->onTouchBegan(pTouch,pEvent);
    m_bMenuPlus = m_pMenuPlus->onTouchBegan(pTouch,pEvent);
    return true;
}


void CBuyProp::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    if(m_bSureMenu)
    {
        m_pSureMenu->onTouchMoved(pTouch,pEvent);
    }
    
    if(m_bCancleMenu)
    {
        m_pCancleMenu->onTouchMoved(pTouch,pEvent);
    }

    if(m_bContinerMenu)
    {
        m_solider->onTouchMoved(pTouch,pEvent);
    }
    
    if(m_bMenuPlus)
    {
        m_pMenuPlus->onTouchMoved(pTouch,pEvent);
    }
    
    if(m_bMenuMinus)
    {
        m_pMenuMinus->onTouchMoved(pTouch,pEvent);
        
    }
}

void CBuyProp::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if(m_bSureMenu)
    {
        m_pSureMenu->onTouchEnded(pTouch,pEvent);
        m_bSureMenu  = false;
        return;
    }
    
    if(m_bCancleMenu)
    {
        m_pCancleMenu->onTouchEnded(pTouch,pEvent);
        m_bCancleMenu  = false;
        return;
    }
    
    if(m_bContinerMenu)
    {
        m_solider->Layer::onTouchEnded(pTouch,pEvent);
        m_bContinerMenu = false;
        return;
    }
    
    if(m_bMenuPlus)
    {
        m_pMenuPlus->onTouchEnded(pTouch,pEvent);
        m_bMenuPlus = false;
        return;
    }
    
    if(m_bMenuMinus)
    {
        m_pMenuMinus->onTouchEnded(pTouch,pEvent);
        m_bMenuMinus = false;
        return;
    }
    
}

void CBuyProp::onTouchCancelled(Touch *touch, Event *pEvent)
{
    if(m_bSureMenu)
    {
        m_pSureMenu->onTouchCancelled(touch,pEvent);
        m_bSureMenu  = false;
    }
    
    if(m_bCancleMenu)
    {
        m_pCancleMenu->onTouchCancelled(touch,pEvent);
        m_bCancleMenu  = false;
    }
    
    if(m_bContinerMenu)
    {
        m_solider->Layer::onTouchCancelled(touch,pEvent);
        m_bContinerMenu = false;
        
    }
    if(m_bMenuPlus)
    {
        m_pMenuPlus->onTouchCancelled(touch,pEvent);
        m_bMenuPlus = false;
    }
    
    if(m_bMenuMinus)
    {
        m_pMenuMinus->onTouchCancelled(touch,pEvent);
        m_bMenuMinus = false;
    }
}


void CBuyProp::soliderCallBack(Ref * sender, cocos2d::extension::Control::EventType controlEvent)
{
    
    auto slide_control = (cocos2d::extension::ControlSlider*)sender;//通过回调参数sender 获得ControlSlider‘
    
    int iCurrent_value = slide_control->getValue();//获取slide当前的值
    
    
    if(m_arrow)
    {
        m_arrow->setPosition(Vec2(slide_control->getContentSize().width * slide_control->getValue()/slide_control->getMaximumValue() - 5, m_arrow->getPositionY()));
        char strBuf[20] = {0};
        sprintf(strBuf,"%d",iCurrent_value);

        m_pSliderValue->setString(strBuf);
    }

    
    int iValue = iCurrent_value - m_iPropNum;
    
    m_iPropNum = iCurrent_value;
    
    int iPreValue = m_solider->getValue();
    
    
     //iCurrent_value - iPreValue;
    
       log("iCurrent_value = %d\n",iValue);
        char strNum[50]= {0};
    sprintf(strNum,"%d",m_iPropNum);
    
    if(m_PropNum)
    {
        m_PropNum->setString(strNum);
        sprintf(strNum,"%d",m_iPropNum*m_iPropPrize);
        m_pCostNum->setString(strNum);
        
    }
    
    
    //m_solider->setValue(iCurrent_value);
    
}


void CBuyProp::UpdateCoinStone(Ref *pSedner)
{

    
}



bool CBuyProp::init()
{
    CBaseLayer::initWithColor(Color4B(0,0,0, 255*0.6));

    m_pListener = EventListenerTouchOneByOne::create();//创建一个触摸监听
    m_pListener->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false
    m_pListener->onTouchBegan = CC_CALLBACK_2(CBuyProp::onTouchBegan, this);
    m_pListener->onTouchMoved = CC_CALLBACK_2(CBuyProp::onTouchMoved, this);
    m_pListener->onTouchEnded = CC_CALLBACK_2(CBuyProp::onTouchEnded, this);
   // _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener, this);
    _eventDispatcher->addEventListenerWithFixedPriority(m_pListener, -200);
    
    Sprite *pBg = Sprite::createWithSpriteFrameName("common_bg.png");
    pBg->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(pBg);
    
    pBg->cocos2d::Node::setScale(0.9);
    
    Size tBg = pBg->getContentSize();
    
    Label *pTitle = Label::createWithTTF("批量购买","fonts/cuti.ttf",30);
    pTitle->setPosition(tBg.width*0.5,tBg.height*0.89);
    pBg->addChild(pTitle);
    
    
    Sprite* backgroundSprite = Sprite::createWithSpriteFrameName("jindu.png");
    Sprite* pogressSprite = Sprite::createWithSpriteFrameName("processyellow.png");
    Sprite* thumbSprite = Sprite::createWithSpriteFrameName("ladong.png");
    
    m_solider =  cocos2d::extension::ControlSlider::create(backgroundSprite,pogressSprite,thumbSprite);
    m_solider->setPosition(Vec2(tBg.width*0.5,tBg.height*0.4));
    m_solider->addTargetWithActionForControlEvents(this, cccontrol_selector(CBuyProp::soliderCallBack), cocos2d::extension::Control::EventType::VALUE_CHANGED);
    
    m_solider->setMaximumValue(100);
    m_solider->setMinimumValue(1);
    
    m_solider->setValue(1);
    
    pBg->addChild(m_solider);
    

    m_arrow = Sprite::createWithSpriteFrameName("numPoint.png");
    m_solider->addChild(m_arrow);
    m_arrow->setPosition(Vec2(m_solider->getContentSize().width * m_solider->getValue()/m_solider->getMaximumValue() - 5, m_solider->getContentSize().height * 1.42f));
    
    char buf[40] = {0};
    
    sprintf(buf, "%d", int(m_solider->getValue()));
    m_pSliderValue = Label::createWithSystemFont(buf, "Arial", 15);
    m_pSliderValue->setPosition(m_arrow->getContentSize().width*0.5,m_arrow->getContentSize().height*0.7);
    
    m_arrow->addChild(m_pSliderValue);
    
    
    Sprite *pInputNormal = Sprite::createWithSpriteFrameName("name_input.png");
    pInputNormal->cocos2d::Node::setScale(0.8);
    pInputNormal->setPosition(tBg.width*0.5,tBg.height*0.4);
    pBg->addChild(pInputNormal);
    
    pInputNormal->setVisible(false);
    Size tInput = pInputNormal->getBoundingBox().size;
    
    m_PropNum = Label::createWithTTF("1","fonts/cuti.ttf",40);
    m_PropNum->setPosition(tInput.width*0.5,tInput.height*0.5);
    pInputNormal->addChild(m_PropNum);
    
    
    Sprite *pMinus = Sprite::createWithSpriteFrameName("minus.png");
    Sprite *pMinusSelect = Sprite::createWithSpriteFrameName("minus.png");
    pMinusSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pMinusItem = LYMenuItemSprite::create(pMinus,pMinusSelect,CC_CALLBACK_1(CBuyProp::OnMinus, this));
    m_pMenuMinus = Menu::create(pMinusItem,NULL);
    
    m_pMenuMinus->setPosition(tBg.width*0.15,tBg.height*0.4);
    pBg->addChild(m_pMenuMinus);
    
    
    //+号
    Sprite *pPlus = Sprite::createWithSpriteFrameName("plus.png");
    Sprite *pPlusSeclect = Sprite::createWithSpriteFrameName("plus.png");
    pPlusSeclect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pPlusItem = LYMenuItemSprite::create(pPlus,pPlusSeclect,CC_CALLBACK_1(CBuyProp::OnPlus, this));
    
    m_pMenuPlus = Menu::create(pPlusItem,NULL);
    m_pMenuPlus->setPosition(tBg.width*0.85,tBg.height*0.4);
    pBg->addChild(m_pMenuPlus);
    
    //花费2000元
    Label *pCostLabel = Label::createWithTTF("花费:","fonts/cuti.ttf",30);
    pCostLabel->setPosition(tBg.width*0.25,tBg.height*0.25);
    pBg->addChild(pCostLabel);
    

    std::vector<ShopItemInfo*> pAllIno;
    
    pAllIno  = CDBMgr::getInstance()->GetUserShopInfo();
    
    
    for(int i=0;i<pAllIno.size();i++)
    {
        if(m_bBox)
        {
            if(pAllIno[i]->m_iItemId>=36&&pAllIno[i]->m_iItemId<=41)
            {
                m_pShopInfo.push_back(pAllIno[i]);
                
            }
        }
        else
        {
            //if(pAllIno[i]->m_iItemId<36)
            {
                m_pShopInfo.push_back(pAllIno[i]);
                
            }
            
        }
    }

    m_iPropPrize = m_pShopInfo[m_index]->m_iPrize;
    
    std::string strPic = "coin.png";
    
    if(m_pShopInfo[m_index]->m_iCostType == 1)
    {
        strPic = "stone.png";
        m_bMagic = true;
    }
    
    propInfo* pInfo = CDataManager::getPropInfoById(m_pShopInfo[m_index]->m_iItemId);

    std::string iconBg[5]={"whiteBg.png","greenBg.png","blueBg.png","purpleBg.png","orangeBg.png"};
    sprintf(buf, "%s", iconBg[pInfo->m_iItemLevel - 1].c_str());


    Sprite *pPropBg =  Sprite::createWithSpriteFrameName(buf);
    pPropBg->setScale(2.5);
    pPropBg->setPosition(tBg.width*0.15,tBg.height*0.725);
    pBg->addChild(pPropBg);
    
    Size  tPropBg =  pPropBg->getContentSize();

    Sprite *pPropIco = Sprite::createWithSpriteFrameName(pInfo->m_sItemIcon);
    pPropIco->setPosition(tPropBg.width*0.5,tPropBg.height*0.5);
    pPropBg->addChild(pPropIco);
    
    sprintf(buf,"%s",pInfo->m_sItemName.c_str());
    
    Label *pPropName = Label::createWithTTF(buf,"fonts/cuti.ttf",30);
    pPropName->setPosition(tBg.width*0.325,tBg.height*0.75);
    pPropName->setColor(Color3B::YELLOW);
    pBg->addChild(pPropName);
    
    
    int iNum = CDBMgr::getInstance()->HavePropNumById(pInfo->m_itemId);
    sprintf(buf, "已拥有%d件",iNum);
    Label *pHaveNum = Label::createWithTTF(buf,"fonts/cuti.ttf",30);
    pHaveNum->setPosition(tBg.width*0.325,tBg.height*0.65);
    pBg->addChild(pHaveNum);
    
    
    Sprite *pCoin = Sprite::createWithSpriteFrameName(strPic);
    pCoin->setPosition(pCostLabel->getBoundingBox().size.width*0.5+pCostLabel->getPositionX()+pCoin->getBoundingBox().size.width,tBg.height*0.25);
    pBg->addChild(pCoin);
    
    char strNum[50] = {0};
    sprintf(strNum,"%d",m_iPropPrize);
    
    m_pCostNum = Label::createWithTTF(strNum,"fonts/cuti.ttf",30);
    m_pCostNum->setPosition(pCoin->getPositionX()+pCoin->getBoundingBox().size.width*0.5+m_pCostNum->getBoundingBox().size.width,tBg.height*0.25);
    pBg->addChild(m_pCostNum);
    m_pCostNum->setColor(Color3B::YELLOW);
    
    
    Sprite *pYes = Sprite::createWithSpriteFrameName("greenbtn.png");
    Sprite *pYesSelect = Sprite::createWithSpriteFrameName("greenbtn.png");
    pYesSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pYesSelectItem  = LYMenuItemSprite::create(pYes,pYesSelect,CC_CALLBACK_1(CBuyProp::OnSure, this));
    pYesSelectItem->setScale(2.5);
    
    
    m_pSureMenu = Menu::create(pYesSelectItem, NULL);
    m_pSureMenu->setPosition(tBg.width*0.7,tBg.height*0.14);
    pBg->addChild(m_pSureMenu);
    
    Size tYesItem  = pYesSelectItem->getContentSize();
    
    Label *pLabelYes = Label::createWithTTF("确定","fonts/cuti.ttf",30);
    pLabelYes->setScale(0.5);
    pLabelYes->setPosition(tYesItem.width*0.5,tYesItem.height*0.5);
    pYesSelectItem->addChild(pLabelYes);
    
    Sprite *pNo  = Sprite::createWithSpriteFrameName("redbtn.png");
    Sprite *pNoSelect = Sprite::createWithSpriteFrameName("redbtn.png");
    pNoSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pNoItemSprite = LYMenuItemSprite::create(pNo,pNoSelect,CC_CALLBACK_1(CBuyProp::OnCancel, this));
    pNoItemSprite->setScale(2.5);
    Size tNoItem  = pNoItemSprite->getContentSize();

    
    m_pCancleMenu = Menu::create(pNoItemSprite, NULL);
    m_pCancleMenu->setPosition(tBg.width*0.3,tBg.height*0.14);
    pBg->addChild(m_pCancleMenu);
    
    Label *pLabelNo = Label::createWithTTF("取消","fonts/cuti.ttf",30);
    pLabelNo->setScale(0.5);
    pLabelNo->setPosition(tNoItem.width*0.5,tNoItem.height*0.5);
    pNoItemSprite->addChild(pLabelNo);
    
    //输入框
    return true;
}

void CBuyProp::BuProp(cocos2d::Ref *target, cocos2d::SEL_CallFuncO handler)
{
    m_pHandlerBuyProp = handler;
    m_pTarget  = target;
}

void CBuyProp::OnUserMcf(Ref *pSender)
{
    int iPropId = m_pShopInfo[m_index]->m_iItemId;
    int iNum = CDBMgr::getInstance()->HavePropNumById(iPropId);
    
    CDataManager::SaveUserPropToDB(iPropId,iNum+m_iPropNum);
    
    Sprite * thetarget = CFightManager::getRewardWord("购买成功!");
    thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
 

    getParent()->
    addChild(thetarget,1000);
    if(m_pTarget&&m_pHandlerBuyProp)
    {
        (m_pTarget->*m_pHandlerBuyProp)(this);
        m_pTarget = NULL;
        m_pHandlerBuyProp= NULL;
        
    }
    int m_stoneNum = m_iPropNum*m_iPropPrize;

    __String* tempStr1 = __String::createWithFormat("%d", m_stoneNum);
    EventParamMap paramMap;
    
    paramMap.insert(EventParamPair("购买道具",tempStr1->getCString()));
    
    TDCCTalkingDataGA::onEvent("商店界面", &paramMap);
    
    //runAction(Sequence::create(DelayTime::create(1.0f),RemoveSelf::create(), NULL));
    removeFromParent();

}

void CBuyProp::OnSure(Ref *pSender)
{
    int iCostPrize = m_iPropNum*m_iPropPrize;
    
    if(m_bMagic)
    {
        int iStone = CPersonalData::getInstance()->getUserStone();
        if(iStone<iCostPrize)
        {
            Sprite * thetarget = CFightManager::getTheFlyWord("魔法石不足","fonts/cuti.ttf");
            thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
            addChild(thetarget, 10);
            return ;
        }
        else
        {

            CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(CBuyProp::OnUserMcf),0,-iCostPrize,0);
            TDCCItem::onPurchase("购买道具", m_iPropNum, m_iPropPrize);
            //CDataManager::saveUserStoneAndCoinToDB(iCostPrize,1,0);
        }
    }
    else
    {
        int iCoin = CPersonalData::getInstance()->getUserCoin();
        if(iCoin<iCostPrize)
        {
            Sprite * thetarget = CFightManager::getTheFlyWord("金币不足","fonts/cuti.ttf");
            thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
            addChild(thetarget,10);
            return ;
        }

        CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(CBuyProp::OnUserMcf),-iCostPrize,0,0);
        //CDataManager::saveUserStoneAndCoinToDB(iCostPrize,0,0);
    }
    
    //增加道具
    
    log("on sure");
}

void CBuyProp::OnCancel(Ref *pSender)
{
    removeFromParent();
}


void CBuyProp::OnMinus(Ref *pSender)
{

    if(m_iPropNum>1)
    {
        m_iPropNum--;
            m_solider->setValue( m_iPropNum);
        char strNum[50]= {0};
        sprintf(strNum,"%d",m_iPropNum);
        m_PropNum->setString(strNum);
        sprintf(strNum,"%d",m_iPropNum*m_iPropPrize);
        
        m_pCostNum->setString(strNum);
    }
}

void CBuyProp::OnPlus(Ref *pSender)
{
    m_iPropNum++;

    
    
    m_solider->setValue( m_iPropNum);
   
    
    
    char strNum[50]= {0};
    sprintf(strNum,"%d",m_iPropNum);
    m_PropNum->setString(strNum);
    sprintf(strNum,"%d",m_iPropNum*m_iPropPrize);
    
    m_pCostNum->setString(strNum);
}

void CBuyProp::onExit()
{
    CBaseLayer::onExit();
    _eventDispatcher->removeEventListener(m_pListener);
    
}

CBuyProp::CBuyProp(int index,bool bBox)
{
    m_pListener = NULL;
    m_bBox = bBox;
    m_PropNum = NULL;
    m_pCostNum = NULL;
    std::vector<ShopItemInfo*> pShopInfo =CDBMgr::getInstance()->GetUserShopInfo();
    m_solider = NULL;
    m_iCurValue = 1;
    m_arrow = NULL;
    m_iPropNum = pShopInfo[index]->m_iNum;
    m_bMagic = false;
    m_index = index;
    m_pSliderValue = NULL;
    m_pSureMenu = NULL;
    m_pCancleMenu = NULL;
    m_bSureMenu = false;
    m_bCancleMenu = false;
    m_pMenuMinus = NULL;
    m_pMenuPlus = NULL;
    m_bMenuMinus = false;
    m_bMenuPlus = false;
    
}

CBuyProp::~CBuyProp()
{

    
}
NS_CC_EXT_END
