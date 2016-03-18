//
//  Chest.cpp
//  WarOfGod
//
//  Created by prefer_shan on 15/5/5.
//
//

#include "Chest.h"
#include "DBMgr.h"
#include "Shop.h"
#include "BuyProp.h"

std::string ChestLayer::getShopIcoBg(int iPropLevel)
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

void ChestLayer::OnCloseMenu(Ref *psender)
{
    removeFromParent();
    
}


ChestLayer::ChestLayer()
{
    m_pListener = NULL;
    m_pCoinLabel = NULL;
    m_pStoneLabel = NULL;
    
    CSharedPlist::getInstance()->pushPlist("plist/battleResult.plist");
    CSharedPlist::getInstance()->pushPlist("plist/shop.plist");
    CSharedPlist::getInstance()->pushPlist("plist/propIcon_1.plist");
}

ChestLayer::~ChestLayer()
{
    CSharedPlist::getInstance()->erasePlist("plist/battleResult.plist");
    CSharedPlist::getInstance()->erasePlist("plist/shop.plist");
    CSharedPlist::getInstance()->erasePlist("plist/propIcon_1.plist");
}


bool ChestLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    Vec2 location =  pTouch->getLocation();
    
    for(int i=0;i<m_pSpriteVec.size();i++)
    {
        cocos2d::Rect rect  =  m_pSpriteVec[i]->boundingBox();
        
        if(rect.containsPoint(location))
        {
            log("rect = %d",i);
            break;
        }
        
    }
    
    return true;
}


void ChestLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    
}

void ChestLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    
}

void ChestLayer::onTouchCancelled(Touch *touch, Event *pEvent)
{
    
}

void ChestLayer::OnWupin(Ref *pSender)
{
    
    getParent()->addChild(CShopLayer::create(),9999);
    removeFromParent();
    
}

void ChestLayer::OnChest(Ref *pSender)
{

}


void ChestLayer::UpdateMoney(Ref *pSender)
{

    int iCoin = CPersonalData::getInstance()->getUserCoin();
    char buf[50] = {0};
    sprintf(buf,"%d",iCoin);
    
    m_pCoinLabel->setString(buf);
    
    
    int iStone = CPersonalData::getInstance()->getUserStone();
    sprintf(buf,"%d",iStone);
    m_pStoneLabel->setString(buf);
}

void  ChestLayer::OnGreenTouch(Ref *pSender)
{
    MenuItemSprite* p = (MenuItemSprite*)pSender;
    int iTag = p->getTag();
    
    log("%d\n",iTag);
    
    cocos2d::extension::CBuyProp *pProp = cocos2d::extension::CBuyProp::create(iTag,true);
    pProp->BuProp(this,menu_selector(ChestLayer::UpdateMoney));
    addChild(pProp,111);
}

void ChestLayer::OnLeftItem(Ref *pSender)
{

}

void ChestLayer::OnRightItem(Ref *pSender)
{

    
}

bool ChestLayer::init()
{
    
    CBaseLayer::initWithColor(Color4B(26,11,24, 255*0.8));
    
    m_pListener = EventListenerTouchOneByOne::create();//创建一个触摸监听
    m_pListener->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false
    m_pListener->onTouchBegan = CC_CALLBACK_2(ChestLayer::onTouchBegan, this);
    m_pListener->onTouchMoved = CC_CALLBACK_2(ChestLayer::onTouchMoved, this);
    m_pListener->onTouchEnded = CC_CALLBACK_2(ChestLayer::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener, this);
    
    Sprite *pLight = Sprite::createWithSpriteFrameName("backlight3.png");
    pLight->setScale(3);
    pLight->setPosition(m_winSize.width*0.5,m_winSize.height*0.9);
    addChild(pLight);
    
    Sprite *pBg = Sprite::createWithSpriteFrameName("shop_bg.png");
    pBg->setScale(2);
    
    pBg->setPosition(m_winSize.width*0.5,m_winSize.height*0.45);
    addChild(pBg,1);
    
    Size tBG = pBg->getContentSize();
    
    Sprite *pCloseSprite = Sprite::createWithSpriteFrameName("closeBtn.png");
    Sprite *pCloseSpriteSelc  = Sprite::createWithSpriteFrameName("closeBtn1.png");
    
    LYMenuItemSprite *pCloseItem = LYMenuItemSprite::create(pCloseSprite,pCloseSpriteSelc,CC_CALLBACK_1(ChestLayer::OnCloseMenu, this));
    pCloseItem->setScale(0.5);
    
    Menu * m_pMenuClose = NULL;
    
    m_pMenuClose =  Menu::create(pCloseItem,NULL);
    
    m_pMenuClose->setPosition(tBG.width*0.98,tBG.height*0.98);
    pBg->addChild(m_pMenuClose,1);
    
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
    
    MenuItemSprite *pWupinItem = MenuItemSprite::create(pWupinNormal,pWupinSelect,pWupinDisbale,CC_CALLBACK_1(ChestLayer::OnWupin, this));
    pWupinItem->setScale(0.5);
    pWupinItem->setPosition(tBG.width*0.15,tBG.height*0.89);
    
    Size tBgItem  = pWupinItem->getContentSize();
    
    Label *pLabelWupin = Label::createWithSystemFont("物品","Aril",30);
    pLabelWupin->setColor(Color3B(233,188,143));
    
    pLabelWupin->setPosition(tBgItem.width*0.5,tBgItem.height*0.5);
    pWupinItem->addChild(pLabelWupin);
    
    
    //宝箱
    Sprite *pBoxNoraml = Sprite::createWithSpriteFrameName("shop_btNormal.png");
    Sprite *pBoxSelect = Sprite::createWithSpriteFrameName("shop_btPress.png");
   
    
    Sprite *pBoxDisable = Sprite::createWithSpriteFrameName("shop_btPress.png");
    pBoxDisable->setColor(Color3B::GRAY);
     pBoxSelect->setColor(Color3B::GRAY);
    MenuItemSprite *pBoxItem = MenuItemSprite::create(pBoxNoraml,pBoxSelect,pBoxDisable,CC_CALLBACK_1(ChestLayer::OnChest,this));
    pBoxItem->setEnabled(false);
    pBoxItem->setScale(0.5);
    
    Label *pLabelBox = Label::createWithSystemFont("宝箱","Aril",30);
    pLabelBox->setColor(Color3B(233,188,143));
    pLabelBox->setPosition(tBgItem.width*0.5,tBgItem.height*0.5);
    pBoxItem->addChild(pLabelBox);
    
    pBoxItem->setPosition(tBG.width*0.35,tBG.height*0.89);
    
    Menu *pMenu = Menu::create(pWupinItem,pBoxItem,NULL);
    pMenu->setPosition(Vec2(0,0));
    pBg->addChild(pMenu);
    
    m_pSpriteVec.clear();
    
    
    
    
    std::vector<ShopItemInfo*> pShopInfo =  CDBMgr::getInstance()->GetUserShopInfo();
    
    std::vector<ShopItemInfo*> pPropInfo;
    
    int j = 0;
    
    std::map<int,std::vector<ShopItemInfo*>> m_pPropMap;
    
    
    
    for(int i=0;i<pShopInfo.size();i++)
    {
        if(pShopInfo[i]->m_iItemId>=36&&pShopInfo[i]->m_iItemId<=41)
        {
           
           // pPropInfo.clear();
 
              //pPropInfo.clear();
    

            pPropInfo.push_back(pShopInfo[i]);
        }
        else
        {
            
        }
    }
    
    

    //道具 物品
   for(int i=0;i<pPropInfo.size();i++)
    {
        std::string str;
        
        propInfo *p = CDBMgr::getInstance()->getPropInfoById(pPropInfo[i]->m_iItemId);
        
        str = getShopIcoBg(p->m_iItemLevel);
        
        Sprite *pPropbg = Sprite::createWithSpriteFrameName(str);
        pPropbg->cocos2d::Node::setScale(0.5);
        pPropbg->setPosition(tBG.width*0.145+i%4*tBG.width*0.225,tBG.height*0.665-i/4*tBG.height*0.365);
        pBg->addChild(pPropbg);
        
        Size tPropsize = pPropbg->getContentSize();
        
        
        Sprite *pPropIco = Sprite::createWithSpriteFrameName(p->m_sItemIcon);
        pPropIco->cocos2d::Node::setScale(2);
        pPropIco->setPosition(tPropsize.width*0.5,tPropsize.height*0.65);
        pPropbg->addChild(pPropIco);
        
        char strPrize[50] = {0};
        
        sprintf(strPrize,"%d",pPropInfo[i]->m_iPrize);
        
        Label *pNum = Label::createWithSystemFont(strPrize,"Aril",25);
        pNum->setColor(Color3B::BLACK);
        pNum->setPosition(tPropsize.width*0.3,tPropsize.height*0.15);
        pPropbg->addChild(pNum);
        
        //金币或者魔法石
        
        std::string strType = "coin.png";
        if(pShopInfo[i]->m_iCostType==1)
        {
            strType = "stone.png";
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
        
        MenuItemSprite *pGreenItem = MenuItemSprite::create(pGreen,pGreenSelect,CC_CALLBACK_1(ChestLayer::OnGreenTouch,this));
        pGreenItem->setTag(i);
        pGreenItem->setPosition(tPropsize.width*0.85,tPropsize.height*0.15);
        
        Menu *pGreenMenu = Menu::create(pGreenItem,NULL);
        pGreenMenu->setPosition(Vec2(0,0));
        pPropbg->addChild(pGreenMenu);
        
        m_pSpriteVec.push_back(pPropbg);
        
        
    
        
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
            pRexiao->setPosition(pPropbg->getContentSize().width*0.2,pPropbg->getContentSize().height*0.5);
            pPropbg->addChild(pRexiao);

            
        }
        else if(pPropInfo[i]->m_saleType == "3")
        {
            Sprite *pRexiao = Sprite::createWithSpriteFrameName("newTower.png");
            pRexiao->setPosition(pPropbg->getContentSize().width*0.5,pPropbg->getContentSize().height*0.85);
            pPropbg->addChild(pRexiao);
        }
        
        {
            if(pPropInfo[i]->m_iRebate!=100)
            {
                Sprite *disCountBg = Sprite::createWithSpriteFrameName("dz.png");
                disCountBg->setScale(1.5f);
                disCountBg->setPosition(pPropbg->getContentSize().width,pPropbg->getContentSize().height*0.5);
                pPropbg->addChild(disCountBg,11);
                
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

            
        }

        
    }
    
     //233 ,188,143


    
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
    Sprite *pGreenPlus = Sprite::createWithSpriteFrameName("greenplus.png");
    pGreenPlus->cocos2d::Node::setScale(0.8);
    pGreenPlus->setPosition(tpCoin.width*0.8,tpCoin.height*0.4);
    pCoinIcon->addChild(pGreenPlus);
    
    
    
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
    
    stoneBg->addChild(stoneIcon);
    stoneIcon->setScale(0.8f);
    stoneIcon->setPosition(Vec2(40, stoneBg->getContentSize().height * 0.45));
    
    tpCoin = stoneIcon->getContentSize();
    
    Sprite *pGreenPlus1 = Sprite::createWithSpriteFrameName("greenplus.png");
    pGreenPlus1->cocos2d::Node::setScale(0.8);
    pGreenPlus1->setPosition(tpCoin.width*0.8,tpCoin.height*0.4);
    stoneIcon->addChild(pGreenPlus1);
    
    sprintf(strBuf,"%d",CPersonalData::getInstance()->getUserStone());
    m_pStoneLabel = Label::createWithTTF(strBuf,"fonts/cuti.ttf",30);
    m_pStoneLabel->setPosition(stoneIcon->getPositionX()+stoneIcon->getContentSize().width*0.5+m_pStoneLabel->getContentSize().width*2
                               ,stoneBg->getContentSize().height * 0.45);
    stoneBg->addChild(m_pStoneLabel);

    return true;
}