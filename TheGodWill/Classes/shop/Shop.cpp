//
//  Shop.cpp
//  WarOfGod
//
//  Created by prefer_shan on 15/5/4.
//
//

#include "Shop.h"
#include "DBMgr.h"
#include "BuyProp.h"
#include "Chest.h"
#include "commonPropLayer.h"
#include "Common.h"



std::string CShopLayer::getShopIcoBg(int iPropLevel)
{
    if(iPropLevel==1)
    {
        return "shop_white.png";
    }
    else if(iPropLevel==2)
    {
            return "shop_green.png";
    }
    else if(iPropLevel==3)
    {
        return "shop_blue.png";
        
    }
    else if(iPropLevel==4)
    {
        return "shop_purple.png";
    }
    else if(iPropLevel==5)
    {
        return "orangle.png";
    }
    
    return "shop_white.png";
}

void CShopLayer::OnCloseMenu(Ref *psender)
{
     SimpleAudioEngine::getInstance()->playEffect("music/click_btn.mp3");
    
    removeFromParent();
}



void CShopLayer::OnMenuPropIcoTouch(Ref *pSender)
{
    log("propico touch");
    SimpleAudioEngine::getInstance()->playEffect("music/click_btn.mp3");
    MenuItemSprite* p = (MenuItemSprite*)pSender;
    int iTag = p->getTag();
    
    log("%d\n",iTag);
    
    cocos2d::extension::CBuyProp *pProp = cocos2d::extension::CBuyProp::create(iTag,!m_bProp);
    pProp->BuProp(this,menu_selector(CShopLayer::UpdateMoney));
    addChild(pProp,111);
}



void  CShopLayer::ReloadData()
{
    removeAllChildren();
    
    m_pPropMap.clear();
    
    Sprite *pLight = Sprite::createWithSpriteFrameName("backlight3.png");
    pLight->setScale(3);
    pLight->setPosition(m_winSize.width*0.5,m_winSize.height*0.9);
    addChild(pLight);
    
    Sprite *pBg = Sprite::createWithSpriteFrameName("shop_bg.png");
    pBg->setScale(2);
    pBg->setPosition(m_winSize.width*0.5,m_winSize.height*0.45);
    addChild(pBg,1);
    
    LayerColor *pLayer = LayerColor::create();
    
    pLayer->setScale(2);
    
    m_scrollView = ScrollView::create(Size(pBg->getBoundingBox().size.width*0.95,pBg->getBoundingBox().size.height),pLayer);
    m_scrollView->setPosition(Vec2(pBg->getPositionX()-pBg->getBoundingBox().size.width*0.475 ,m_winSize.height * 0.1f));
    m_scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
    m_scrollView->setTouchEnabled(true);
    m_scrollView->setBounceable(true);
    addChild(m_scrollView,11,100);
    
    Size tBG = pBg->getContentSize();
    
    Sprite *pCloseSprite = Sprite::createWithSpriteFrameName("closeBtn.png");
    Sprite *pCloseSpriteSelc  = Sprite::createWithSpriteFrameName("closeBtn.png");
    pCloseSpriteSelc->setColor(Color3B::GRAY);
    LYMenuItemSprite *pCloseItem = LYMenuItemSprite::create(pCloseSprite,pCloseSpriteSelc,CC_CALLBACK_1(CShopLayer::OnCloseMenu, this));
    CommonData::enlargeItemSprite(pCloseItem, 2);
    pCloseItem->setScale(0.5);
    
    m_pCloseItem =  Menu::create(pCloseItem,NULL);
    m_pCloseItem->setPosition(tBG.width*0.98,tBG.height*0.98);
    pBg->addChild(m_pCloseItem,1);
    
    
    Sprite *pTitle = Sprite::createWithSpriteFrameName("shop_title.png");
    pTitle->setScale(0.5);
    pTitle->setPosition(tBG.width*0.5,tBG.height*1.04);
    pBg->addChild(pTitle);
    
    Size tTitle = pTitle->getContentSize();
    
    Sprite *pShopTx = Sprite::createWithSpriteFrameName("shop_Text.png");
    pShopTx->setPosition(tTitle.width*0.5,tTitle.height*0.4);
    pTitle->addChild(pShopTx);
    
    //物品
    Sprite *pWupinNormal = Sprite::createWithSpriteFrameName("shop_btNormal.png");
    Sprite *pWupinSelect = Sprite::createWithSpriteFrameName("shop_btPress.png");
    
    Sprite *pWupinDisbale = Sprite::createWithSpriteFrameName("shop_btPress.png");
    pWupinDisbale->setColor(Color3B::GRAY);
    
    m_pWupinItem = MenuItemSprite::create(pWupinNormal,pWupinSelect,pWupinDisbale,CC_CALLBACK_1(CShopLayer::OnWupin, this));
    m_pWupinItem->setScale(0.5);
    m_pWupinItem->setPosition(tBG.width*0.15,tBG.height*0.89);
    m_pWupinItem->setEnabled(false);
    
    Size tBgItem  = m_pWupinItem->getContentSize();
    
    Label *pLabelWupin = Label::createWithSystemFont("物品","Aril",30);
    pLabelWupin->setColor(Color3B(233,188,143));
    
    pLabelWupin->setPosition(tBgItem.width*0.5,tBgItem.height*0.5);
    m_pWupinItem->addChild(pLabelWupin);
    
    
    //左箭头
    //shop_arrow.png
    Sprite *pLeftArrow = Sprite::createWithSpriteFrameName("shop_arrow.png");
    pLeftArrow->cocos2d::Node::setScale(0.5);
    pLeftArrow->setFlippedX(true);
    
    Sprite *pLeftArrowSelect = Sprite::createWithSpriteFrameName("shop_arrow.png");
    pLeftArrowSelect->setColor(Color3B::GRAY);
    pLeftArrowSelect->cocos2d::Node::setScale(0.5);
    pLeftArrowSelect->setFlippedX(true);
    

    //宝箱
    Sprite *pBoxNoraml = Sprite::createWithSpriteFrameName("shop_btNormal.png");
    Sprite *pBoxSelect = Sprite::createWithSpriteFrameName("shop_btPress.png");
    Sprite *pBoxDisable = Sprite::createWithSpriteFrameName("shop_btPress.png");
    pBoxDisable->setColor(Color3B::GRAY);
    
    m_pBoxItem = MenuItemSprite::create(pBoxNoraml,pBoxSelect,pBoxDisable,CC_CALLBACK_1(CShopLayer::OnChest,this));
    m_pBoxItem->setScale(0.5);
    
    Label *pLabelBox = Label::createWithSystemFont("宝箱","Aril",30);
    pLabelBox->setColor(Color3B(233,188,143));
    pLabelBox->setPosition(tBgItem.width*0.5,tBgItem.height*0.5);
    m_pBoxItem->addChild(pLabelBox);
    
    m_pBoxItem->setPosition(tBG.width*0.36,tBG.height*0.89);
    
    m_pMenuItem = Menu::create(m_pWupinItem,m_pBoxItem,NULL);
    m_pMenuItem->setPosition(Vec2(0,0));
    pBg->addChild(m_pMenuItem);
    
    m_pSpriteVec.clear();
    
    std::vector<ShopItemInfo*> pShopInfo=  CDBMgr::getInstance()->GetUserShopInfo();
    
    std::vector<ShopItemInfo*> pPropInfo;
    
    if(m_bProp)
    {
        pLayer->setContentSize(Size(pBg->getContentSize().width*2,pBg->getContentSize().height));
    }
    else
    {
        pLayer->setContentSize(Size(pBg->getContentSize().width,pBg->getContentSize().height));
    }
    
    
    for(int i=0;i<pShopInfo.size();i++)
    {
        if(!m_bProp)
        {
            if(pShopInfo[i]->m_iItemId>=36&&pShopInfo[i]->m_iItemId<=41)
            {
                pPropInfo.push_back(pShopInfo[i]);
            }
        
        }
        else
        {
            if(pShopInfo[i]->m_iItemId<36)
            {
                pPropInfo.push_back(pShopInfo[i]);
                
            }
        }
    }
    
    m_pPropMenu = Menu::create();
    m_pPropMenu->setPosition(Vec2(0,0));
    pLayer->addChild(m_pPropMenu,1);

    m_pGreenMenu = Menu::create();
    m_pGreenMenu->setPosition(Vec2(0,0));
    pLayer->addChild(m_pGreenMenu,1);
    
    //道具 物品
    for(int i=0;i<pPropInfo.size();i++)
    {
        std::string str;
        
        propInfo *p = CDBMgr::getInstance()->getPropInfoById(pPropInfo[i]->m_iItemId);
        
        str = getShopIcoBg(p->m_iItemLevel);
        
        Sprite *pPropbg = Sprite::createWithSpriteFrameName(str);
        pPropbg->cocos2d::Node::setScale(0.5);
        
        if(m_bProp)
        {
            pPropbg->setPosition(tBG.width*0.625+i%8*tBG.width*0.225,tBG.height*0.825-i/8*tBG.height*0.365);
        }
        else
        {
            pPropbg->setPosition(tBG.width*0.375+i%4*tBG.width*0.225,tBG.height*0.825-i/4*tBG.height*0.365);
        }
        pLayer->addChild(pPropbg);
    
        Size tPropsize = pPropbg->getContentSize();
        
        Sprite *pPropIco = Sprite::createWithSpriteFrameName(p->m_sItemIcon);
        Sprite *pProIcoSelect = Sprite::createWithSpriteFrameName(p->m_sItemIcon);
    
        MenuItemSprite *pProItem = MenuItemSprite::create(pPropIco,pProIcoSelect,CC_CALLBACK_1(CShopLayer::OnMenuPropIcoTouch, this));
        pProItem->setTag(i);
        pProItem->setPosition(pPropbg->getPositionX(),pPropbg->getPositionY()+m_winSize.height/75);
        m_pPropMenu->addChild(pProItem);
    
        
        Size tPropItemSize = pProItem->getContentSize();
        
        
        char strPrize[50] = {0};
        sprintf(strPrize,"%d",pPropInfo[i]->m_iPrize);
        
        Label *pNum = Label::createWithSystemFont(strPrize,"Aril",25);
        pNum->setColor(Color3B::BLACK);
        pNum->setPosition(tPropsize.width*0.3,tPropsize.height*0.15);
        pPropbg->addChild(pNum);
        
        //金币或者魔法石、
        
        std::string strType = "coin.png";
        if(pShopInfo[i]->m_iCostType==1)
        {
            strType = "stone.png";
        }
        
        /*0：无
        1：限购
        2：热销
        3：new*/

        
        if(pPropInfo[i]->m_saleType=="1")
        {
            //数量只有1个
        }
        else if(pPropInfo[i]->m_saleType == "2")
        {
            Sprite *pRexiao = Sprite::createWithSpriteFrameName("shop_hotSale.png");
            pRexiao->setScale(0.6);
            pRexiao->setPosition(tPropItemSize.width*0.2,tPropItemSize.height*0.1);
            pProItem->addChild(pRexiao);
            
        }
        else if(pPropInfo[i]->m_saleType == "3")
        {
            Sprite *pRexiao = Sprite::createWithSpriteFrameName("newTower.png");
            pRexiao->setScale(0.6);
            pRexiao->setPosition(tPropItemSize.width*0.5,tPropItemSize.height*0.85);
            pProItem->addChild(pRexiao);
        }
        
        if(pPropInfo[i]->m_iRebate!=100)
        {
            Sprite *disCountBg = Sprite::createWithSpriteFrameName("dz.png");
            disCountBg->setScale(0.7f);
            disCountBg->setPosition(tPropItemSize.width*1.2,tPropItemSize.height*0.8);
            pProItem->addChild(disCountBg,11);
            
            int  iRebate = pPropInfo[i]->m_iRebate;
            
            int iNum  = iRebate/10;
            int iNumTwo = iRebate%10;
        
            char strbuf[10] = {0};
            if(iNumTwo==0)
            {
                sprintf(strbuf,"%d",iNum);
            }
            else
            {
                sprintf(strbuf,"%d:%d",iNum,iNumTwo);
            }
    
            Label* pDiscountNum = Label::createWithCharMap("fonts/zheNumber.png", 14, 25, '0');
            pDiscountNum->setScale(0.66);
            pDiscountNum->setString(strbuf);
            disCountBg->addChild(pDiscountNum);
            pDiscountNum->setPosition(Vec2(disCountBg->getContentSize().width * 0.37f, disCountBg->getContentSize().height * 0.5f));
            
            Sprite *pzhe  = Sprite::createWithSpriteFrameName("zhezi.png");
            pzhe->setScale(0.66);
            pzhe->setPosition(pDiscountNum->getPositionX()+pDiscountNum->getContentSize().width*0.5+pzhe->getContentSize().width*0.25
                              ,disCountBg->getContentSize().height * 0.5f);
            disCountBg->addChild(pzhe);
        }
        
        Sprite *pMoney =  Sprite::createWithSpriteFrameName(strType);
        pMoney->setPosition(tPropsize.width*0.58,tPropsize.height*0.15);
        pMoney->cocos2d::Node::setScale(0.5);
        pPropbg->addChild(pMoney);
        
        Label *pPropName = Label::createWithSystemFont(p->m_sItemName,"Aril",30);
        pPropName->setPosition(tPropsize.width*0.5,-tPropsize.height*0.1);
        pPropbg->addChild(pPropName);
        
        Sprite *pGreen = Sprite::createWithSpriteFrameName("prop_plus.png");
        
        Sprite *pGreenSelect = Sprite::createWithSpriteFrameName("prop_plus.png");
        pGreenSelect->setColor(Color3B::GRAY);
        
        MenuItemSprite *pGreenItem = MenuItemSprite::create(pGreen,pGreenSelect,CC_CALLBACK_1(CShopLayer::OnGreenTouch,this));
        pGreenItem->setTag(i);
        pGreenItem->setScale(0.5);
        
        pGreenItem->setPosition(pPropbg->getPositionX()+m_winSize.height/25,pPropbg->getPositionY()-m_winSize.height/21.42);
        
        m_pGreenMenu->addChild(pGreenItem,1);
        
        m_pSpriteVec.push_back(pPropbg);
    }

    
    //当前金币和魔法石个数
    Sprite* coinBg = Sprite::createWithSpriteFrameName("information_bg.png");
    pBg->addChild(coinBg);
    coinBg->setScale(0.5f);
    coinBg->setPosition(pBg->getContentSize().width * 0.61, pBg->getContentSize().height*0.88);
    
    Sprite* pCoinIcon = Sprite::createWithSpriteFrameName("single_coin.png");
   
    pCoinIcon->setScale(0.8f);
    coinBg->addChild(pCoinIcon);
    pCoinIcon->setPosition(Vec2(40, coinBg->getContentSize().height * 0.45));

    Size tpCoin = pCoinIcon->getContentSize();
    Sprite *pGreenPlus = Sprite::createWithSpriteFrameName("plus.png");
    Sprite *pGreenSelect = Sprite::createWithSpriteFrameName("plus.png");
    pGreenSelect->setColor(Color3B::GRAY);
    
    MenuItemSprite *pGreenItem = MenuItemSprite::create(pGreenPlus,pGreenSelect,CC_CALLBACK_1(CShopLayer::OnCoinPlus, this));
    pGreenItem->setScale(0.8);
    CommonData::enlargeItemSprite(pGreenItem,2);

    m_pCoinItem = Menu::create(pGreenItem,NULL);
    m_pCoinItem->setPosition(tpCoin.width*0.8,tpCoin.height*0.4);
    pCoinIcon->addChild(m_pCoinItem);
    
    char strBuf[50] = {0};
    sprintf(strBuf,"%d",CPersonalData::getInstance()->getUserCoin());
    m_pCoinLabel = Label::createWithTTF(strBuf,"fonts/cuti.ttf",30);
    m_pCoinLabel->setPosition(pCoinIcon->getPositionX()+pCoinIcon->getContentSize().width*0.5+m_pCoinLabel->getContentSize().width*0.5
    +m_pCoinLabel->getContentSize().width*0.5,
                              coinBg->getContentSize().height*0.45);
    coinBg->addChild(m_pCoinLabel);
    
    Sprite* stoneBg = Sprite::createWithSpriteFrameName("information_bg.png");
    pBg->addChild(stoneBg);
    stoneBg->setScale(0.5f);
    stoneBg->setPosition(Vec2(pBg->getContentSize().width * 0.83, pBg->getContentSize().height*0.88));

    
    Sprite* stoneIcon = Sprite::createWithSpriteFrameName("stone.png");
   stoneIcon->setScale(0.8f);
    stoneIcon->setPosition(tpCoin.width*0.8,tpCoin.height*0.4);
    stoneBg->addChild(stoneIcon);
    
    tpCoin = stoneIcon->getContentSize();
    
    Sprite *pGreenPlus1 = Sprite::createWithSpriteFrameName("plus.png");
    
    Sprite *pGreenPlusSelect = Sprite::createWithSpriteFrameName("plus.png");
    pGreenPlusSelect->setColor(Color3B::GRAY);
    
    MenuItemSprite *pGreenStone = MenuItemSprite::create(pGreenPlus1,pGreenPlusSelect,CC_CALLBACK_1(CShopLayer::OnStonePlus, this));
    pGreenStone->cocos2d::Node::setScale(0.8);
    CommonData::enlargeItemSprite(pGreenStone,2);

    
    m_pStoneItem = Menu::create(pGreenStone,NULL);
    m_pStoneItem->setPosition(tpCoin.width*0.8,tpCoin.height*0.4);
    stoneIcon->addChild(m_pStoneItem);
    
    sprintf(strBuf,"%d",CPersonalData::getInstance()->getUserStone());
    m_pStoneLabel = Label::createWithTTF(strBuf,"fonts/cuti.ttf",30);
    m_pStoneLabel->setPosition(stoneIcon->getPositionX()+stoneIcon->getContentSize().width*0.5+m_pStoneLabel->getContentSize().width
                               ,stoneBg->getContentSize().height * 0.45);
    stoneBg->addChild(m_pStoneLabel);
    
}


bool CShopLayer::init()
{
    CBaseLayer::initWithColor(Color4B(0,0,0,255*0.8));

    m_pListener = EventListenerTouchOneByOne::create();//创建一个触摸监听
    m_pListener->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false
    m_pListener->onTouchBegan = CC_CALLBACK_2(CShopLayer::onTouchBegan, this);
    m_pListener->onTouchMoved = CC_CALLBACK_2(CShopLayer::onTouchMoved, this);
    m_pListener->onTouchEnded = CC_CALLBACK_2(CShopLayer::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithFixedPriority(m_pListener, -100);
    ReloadData();
    
    return true;
}


void CShopLayer::UpdateMoney(Ref *pSender)
{
 
    int iCoin = CPersonalData::getInstance()->getUserCoin();
    char buf[50] = {0};
    sprintf(buf,"%d",iCoin);
    
    m_pCoinLabel->setString(buf);
    
    
    int iStone = CPersonalData::getInstance()->getUserStone();
    sprintf(buf,"%d",iStone);
    m_pStoneLabel->setString(buf);
    
}

void CShopLayer::OnGreenTouch(Ref *pSender)
{
    MenuItemSprite* p = (MenuItemSprite*)pSender;
    int iTag = p->getTag();

    log("%d\n",iTag);
    SimpleAudioEngine::getInstance()->playEffect("music/click_btn.mp3");
    
    cocos2d::extension::CBuyProp *pProp = cocos2d::extension::CBuyProp::create(iTag,!m_bProp);
    pProp->BuProp(this,menu_selector(CShopLayer::UpdateMoney));
    addChild(pProp,111);
    
}

bool CShopLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    m_start=  pTouch->getLocation();
    
    
    if(m_bTouch)
    {
        return false;
    }
    m_isMoved=false;

    m_bScrollView = m_scrollView->onTouchBegan(pTouch,pEvent);

    m_bPropMenu =  m_pPropMenu->onTouchBegan(pTouch,pEvent);
    
    m_bGreenMenu = m_pGreenMenu->onTouchBegan(pTouch,pEvent);
    
    m_bItemMenu =  m_pMenuItem->onTouchBegan(pTouch,pEvent);

    m_bCloseMenu = m_pCloseItem->onTouchBegan(pTouch,pEvent);

    
    m_bCoinMenu = m_pCoinItem->onTouchBegan(pTouch,pEvent);
    
    m_bStoneMenu = m_pStoneItem->onTouchBegan(pTouch,pEvent);

    return true;
}


void CShopLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    if(m_bScrollView)
    {
        if(fabsf(pTouch->getLocation().x - m_start.x) > 8)
        {
            m_isMoved = true;
        }
        m_scrollView->onTouchMoved(pTouch,pEvent);
    }
    else if(m_bPropMenu)
    {
        m_pPropMenu->onTouchMoved(pTouch,pEvent);
    }
    else if(m_bGreenMenu)
    {
        m_pGreenMenu->onTouchMoved(pTouch,pEvent);
    }
    
    else  if(m_bItemMenu)
    {
        m_pMenuItem->onTouchMoved(pTouch,pEvent);
    }

    if(m_bCloseMenu)
    {
        m_pCloseItem->onTouchMoved(pTouch,pEvent);
    }
    
    if(m_bCoinMenu)
    {
        m_pCoinItem->onTouchMoved(pTouch,pEvent);
    }
    
    if(m_bStoneMenu)
    {
        m_pStoneItem->onTouchMoved(pTouch,pEvent);
    }
    
 }

void CShopLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if(m_bScrollView)
    {
        m_scrollView->onTouchEnded(pTouch,pEvent);
        m_bScrollView = false;
    }
    
    if(m_isMoved)
    {
        if(m_bPropMenu)
        {
            m_pPropMenu->onTouchCancelled(pTouch, pEvent);
        }
        
        if(m_bGreenMenu)
        {
            m_pGreenMenu->onTouchCancelled(pTouch, pEvent);
        }
        
    }
    else
    {
        if(m_bPropMenu)
        {
            m_pPropMenu->onTouchEnded(pTouch, pEvent);
        }
        
        if(m_bGreenMenu)
        {
            m_pGreenMenu->onTouchEnded(pTouch, pEvent);
        }
    }
    
    if(m_bItemMenu)
    {
        m_pMenuItem->onTouchEnded(pTouch,pEvent);
        m_bItemMenu = false;
    }
    
    if(m_bCloseMenu)
    {
        m_pCloseItem->onTouchEnded(pTouch,pEvent);
        m_bCloseMenu = false;
    }

    if(m_bCoinMenu)
    {
        m_pCoinItem->onTouchEnded(pTouch,pEvent);
        m_bCoinMenu = false;
    }
    
    if(m_bStoneMenu)
    {
        m_pStoneItem->onTouchEnded(pTouch,pEvent);
        m_bStoneMenu = false;
    }
  }

void CShopLayer::onTouchCancelled(Touch *touch, Event *pEvent)
{
    if(m_bScrollView)
    {
        m_scrollView->onTouchCancelled(touch,pEvent);
        m_bScrollView = false;
    }
    
    if(m_bPropMenu)
    {
        m_pPropMenu->onTouchCancelled(touch,pEvent);
        m_bPropMenu = false;
    }
    
    if(m_bGreenMenu)
    {
        m_pGreenMenu->onTouchCancelled(touch,pEvent);
        m_bGreenMenu = false;
    }
    
    if(m_bItemMenu)
    {
        m_pMenuItem->onTouchCancelled(touch,pEvent);
        m_bItemMenu = false;
    }

    if(m_bCloseMenu)
    {
        m_pCloseItem->onTouchCancelled(touch,pEvent);
        m_bCloseMenu = false;
    }
    
    if(m_bCoinMenu)
    {
        m_pCoinItem->onTouchCancelled(touch,pEvent);
        m_bCoinMenu = false;
    }
    
    if(m_bStoneMenu)
    {
        m_pStoneItem->onTouchCancelled(touch,pEvent);
        m_bStoneMenu = false;
    }
    
}


void CShopLayer::onExit()
{
    _eventDispatcher->removeEventListener(m_pListener);
    CBaseLayer::onExit();
}


void CShopLayer::OnWupin(Ref *pSender)
{
     SimpleAudioEngine::getInstance()->playEffect("music/click_btn.mp3");
    
    //log("onWupin\n");
    m_bProp = true;
    ReloadData();
    
    m_pWupinItem->setEnabled(false);
    m_pBoxItem->setEnabled(true);
}


void CShopLayer::OnSetVisible(Ref *pSender)
{
    m_bTouch = false;
    
}

void CShopLayer::OnCoinPlus(Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect("music/click_btn.mp3");
    
    CommonPropLayer* pLayer = CommonPropLayer::create(STONE_EXCHANGE_COIN);
    pLayer->setScucessCallBack(this,menu_selector(CShopLayer::OnSetVisible));
    pLayer->setFailCallBack(this,menu_selector(CShopLayer::OnSetVisible));
    addChild(pLayer, 999);
    m_bTouch = true;
}


void CShopLayer::OnStonePlus(Ref *pSender)
{
     SimpleAudioEngine::getInstance()->playEffect("music/click_btn.mp3");
    
      CLayerManager::GetSingletonPtr()->createLayer(eLayer_Charge,this,false);
}

void CShopLayer::OnChest(Ref *pSender)
{
     SimpleAudioEngine::getInstance()->playEffect("music/click_btn.mp3");
    log("onChest\n");

    m_bProp = false;
    
    ReloadData();

    m_pWupinItem->setEnabled(true);
    m_pBoxItem->setEnabled(false);
}

CShopLayer::CShopLayer()
{
    m_pListener = NULL;
    m_pCoinLabel = NULL;
    m_pStoneLabel = NULL;
    m_scrollView = NULL;
    m_bScrollView= false;
    m_pPropMenu = NULL;
    m_pGreenMenu = NULL;
    m_pMenuItem = NULL;
    m_pCoinItem = NULL;
    m_pStoneItem = NULL;
    m_isMoved = false;
    m_bPropMenu = false;
    m_bItemMenu = false;
    m_bCoinMenu = false;
    m_bStoneMenu = false;
    m_bTouch = false;
    
    m_pMinusItem = NULL;
    m_pPlusItem = NULL;
    m_bProp = true;
    m_index = 0;
    m_start = Vec2::ZERO;
    
    CSharedPlist::getInstance()->pushPlist("plist/battleResult.plist");
    CSharedPlist::getInstance()->pushPlist("plist/shop.plist");
    CSharedPlist::getInstance()->pushPlist("plist/propIcon_1.plist");
}


CShopLayer::~CShopLayer()
{
    CSharedPlist::getInstance()->erasePlist("plist/battleResult.plist");
    CSharedPlist::getInstance()->erasePlist("plist/shop.plist");
    CSharedPlist::getInstance()->erasePlist("plist/propIcon_1.plist");
}