//
//  chargeInfoLayer.cpp
//  WarOfGod
//
//  Created by user on 15/5/4.
//
//
#include "zzbNetControlModule.h"
#include "chargeInfoLayer.h"
#include "LYPersonalData.h"
#include "DataManager.h"
#include "zzbNewNetStruct.h"
#include "DBMgr.h"
#include "HttpEncrypt.h"
#include "TDCCVirtualCurrency.h"
#include "CSharedPlist.h"
#include "Common.h"


#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include <jni.h>
    #include "platform/android/jni/JniHelper.h"
#else
    #import "CBiOSStoreManager.h"
#endif

#define STONE_INFO_WIDTH 275
#define STONE_INGO_HEIGH 480

#define MY_PERSION CPersonalData::getInstance()

std::string productName[6] = {"购买100魔法石","购买300魔法石","购买1031魔法石","购买2200魔法石","购买3858魔法石","购买8100魔法石"};

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSArray *productID = [NSArray arrayWithObjects:@"ALittleStone", @"halfStone", @"BagOfStones",  @"PreciousStones", @"CartfulGemstone",
                        @"LargeNumberGemstone", nil];
#endif

int m_stoneNum = 0;
chargeInfoLayer::chargeInfoLayer()
{
    listener = NULL;
    m_pBgLayer = NULL;
    m_scrollView = NULL;
    m_stoneMenu = NULL;
    m_pExitMenu = NULL;
    m_bScrollView = false;
    m_btnMenu = false;
    m_bStoneMenu = false;
    m_chargeInfo.clear();
    m_chargeNum.clear();
    m_tempValue.clear();
    m_pBg = NULL;
    m_isMoved = false;
    m_start = Vec2::ZERO;
    m_pInfo = NULL;
    CSharedPlist::getInstance()->pushPlist("plist/shop.plist");
}

chargeInfoLayer::~chargeInfoLayer()
{
    _eventDispatcher->removeEventListener(listener);
    CSharedPlist::getInstance()->erasePlist("plist/shop.plist");
}

void chargeInfoLayer::onEnter()
{
    CBaseLayer::onEnter();
}

void chargeInfoLayer::onExit()
{
    NotificationCenter::getInstance()->removeObserver(this, "payCallBack");
    CBaseLayer::onExit();
}

std::string chargeInfoLayer::getDigitFromBackStr(const char* str)
{
    const char* p = str;
    string ret;
    while (*p != '=')
    {
        p++;
    }
    p++;
    while(*p != '\0')
    {
        ret += *p;
        p++;
    }
    return ret;
}

std::string chargeInfoLayer::getStrFromBackStr(const char* str)
{
    const char* p = str;
    string ret;
    while(*p != '=')
    {
        ret += *p;
        p++;
    }
    return ret;
}

void chargeInfoLayer::getChargeInfoBack(Ref* pSender)
{
    _netRecvEventGetChargeInfo* pInfo = (_netRecvEventGetChargeInfo*)pSender;
    chargeInfo spInfo;
    int info = 1,state = 1;
    for(std::vector<std::string>::iterator it = pInfo->m_paramsStr.begin(); it!= pInfo->m_paramsStr.end(); it++)
    {
        string str = *it;
        string name = getStrFromBackStr(str.c_str());
        
        if(name == "info")
        {
            info = atoi(getDigitFromBackStr(it->c_str()).c_str());
        }
        
        else if (name == "state")
        {
            state = atoi(getDigitFromBackStr(it->c_str()).c_str());
            if(state != 0)
            {
                log("获取充值信息失败");
                return;
            }
        }
        else if (name == "goodsInfo")
        {
            std::string tempValue = getDigitFromBackStr(it->c_str());
            //tempValue = "1,1;2,1";
            std::vector<__String> parameters = CDBMgr::getInstance()->separateInfoWithMark(tempValue, ";");
            for (int i = 0;i < parameters.size(); i++)
            {
                std::vector<__String> tempParam = CDBMgr::getInstance()->separateInfoWithMark(parameters[i], ",");
                IdAndNum sp ;
                sp.theId = tempParam[0].intValue();
                sp.theNumber = tempParam[1].intValue();
                m_chargeNum.push_back(sp);
                m_tempValue.push_back(sp);
            }
        }
    }
    addInfo();
}

bool chargeInfoLayer::onTouchBegan(Touch* pTouch, Event* pEvent)
{
    m_start = pTouch->getLocation();
    if(m_scrollView)
    {
        m_bScrollView = m_scrollView->onTouchBegan(pTouch, pEvent);
        if(m_stoneMenu)
        {
            m_bStoneMenu = m_stoneMenu->onTouchBegan(pTouch, pEvent);
        }
    }
    if(m_pExitMenu)
    {
        m_btnMenu = m_pExitMenu->onTouchBegan(pTouch, pEvent);
    }
    return true;
}

void chargeInfoLayer::onTouchMoved(Touch* pTouch, Event* pEvent)
{
    if(m_bScrollView)
    {
        if(m_bStoneMenu)
        {
            if (fabsf(pTouch->getLocation().x - m_start.x) > 8) {
                m_isMoved = true;
            }
        }
        m_scrollView->onTouchMoved(pTouch, pEvent);
    }
    else if(m_btnMenu)
    {
        m_pExitMenu->onTouchMoved(pTouch, pEvent);
    }
}

void chargeInfoLayer::onTouchEnded(Touch* pTouch, Event* pEvent)
{
    if(m_bScrollView)
    {
        if(m_bStoneMenu)
        {
            if(m_isMoved)
            {
                m_stoneMenu->onTouchCancelled(pTouch, pEvent);
            }
            else{
                m_stoneMenu->onTouchEnded(pTouch, pEvent);
            }
        }
        m_scrollView->onTouchEnded(pTouch, pEvent);
    }
    else if(m_btnMenu)
    {
        m_pExitMenu->onTouchEnded(pTouch, pEvent);
    }
    m_bScrollView = false;
    m_bStoneMenu = false;
    m_btnMenu = false;
    m_isMoved = false;
}

void chargeInfoLayer::onTouchCancelled(Touch* pTouch, Event* pEvent)
{
    if(m_scrollView)
    {
        m_scrollView->onTouchCancelled(pTouch, pEvent);
    }
    if(m_stoneMenu)
    {
        m_stoneMenu->onTouchCancelled(pTouch, pEvent);
    }
    if(m_pExitMenu)
    {
        m_pExitMenu->onTouchCancelled(pTouch, pEvent);
    }
}

bool chargeInfoLayer::init()
{
    if(!CBaseLayer::initWithColor(Color4B(0, 0, 0, 200)))
    {
        return false;
    }
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(chargeInfoLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(chargeInfoLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(chargeInfoLayer::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(chargeInfoLayer::onTouchCancelled, this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _eventDispatcher->addEventListenerWithFixedPriority(listener, -200);
    m_winSize = Director::getInstance()->getWinSize();
    CNetControlModule::sharedNetControlModule()->getChargeInfo(this, menu_selector(chargeInfoLayer::getChargeInfoBack));
    return true;
}

void chargeInfoLayer::addInfo()
{
    Sprite* light = Sprite::createWithSpriteFrameName("backlight3.png");
    addChild(light);
    light->setScale(4.5f);
    light->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.8f));
    
    m_pBg = Sprite::createWithSpriteFrameName("chongzhidi.png");
    m_pBg->setScale(2.0f);
    addChild(m_pBg);
    m_pBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.49f));
    cocos2d::Size bgSize = m_pBg->getBoundingBox().size;
    
    m_pBgLayer = LayerColor::create(Color4B(255, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(m_pBgLayer);
    m_pBgLayer->setPosition(Vec2(m_pBg->getPositionX() - bgSize.width / 2, m_pBg->getPositionY() - bgSize.height / 2));
    
    Sprite* title = Sprite::createWithSpriteFrameName("shop_title.png");
    m_pBgLayer->addChild(title);
    title->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.965f));
    
    Sprite* word = Sprite::createWithSpriteFrameName("charge_title.png");
    m_pBgLayer->addChild(word);
    word->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.943f));
    //已经达到购买次数则不再显示
    
    showGoodsInfo();
    addBtnMenu();
}

void chargeInfoLayer::showGoodsInfo()
{
    m_chargeInfo.clear();
    
    m_chargeInfo = CDataManager::getChageInfo();
    std::vector<chargeInfo> chargeInfo;
    std::vector<int> goodId;//要显示的商品ID
    for(int i= 0; i < m_chargeInfo.size(); i++)
    {
        bool bValid = true;
        if((m_chargeInfo[i].type == 1) || (m_chargeInfo[i].type == 3))
        {
            for( int j = 0; j < m_chargeNum.size(); j++)
            {
                if((m_chargeNum[j].theId == m_chargeInfo[i].chargeId) && (m_chargeNum[j].theNumber > 0))
                {
                    bValid = false;
                    break;
                }
            }
        }
        if(bValid)
        {
            chargeInfo.push_back(m_chargeInfo[i]);
            goodId.push_back(m_chargeInfo[i].chargeId);
        }
        
    }
    m_chargeInfo = chargeInfo;
    int allSize = m_chargeInfo.size();
    
    cocos2d::Size bgSize = m_pBg->getBoundingBox().size;
    if(m_scrollView)
    {
        m_scrollView->removeFromParent();
        m_scrollView = NULL;
    }
    int width = STONE_INFO_WIDTH * allSize;
    auto scroll_layer = LayerColor::create(Color4B(255,0,0,0), width, STONE_INGO_HEIGH);
    m_stoneMenu = Menu::create();
    scroll_layer->addChild(m_stoneMenu);
    m_stoneMenu->setPosition(Vec2::ZERO);
    m_scrollView = ScrollView::create(cocos2d::Size(bgSize.width * 0.93f, bgSize.height * 0.75f), scroll_layer);
    m_pBgLayer->addChild(m_scrollView);
    m_scrollView->setPosition(Vec2(35, bgSize.height * 0.1f));
    m_scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
    //m_scrollView->setBounceable(false);
    int xpos = STONE_INFO_WIDTH * 6 * 0.085f;//记录第一项的初始位置
    for(int i = 0; i < allSize; i++)
    {
        Vec2 pos = Vec2(xpos + i * 270, STONE_INGO_HEIGH*0.515f);
        addStoneInfo("chongzhismalldi.png", "chongzhismalldi.png", m_stoneMenu, pos, goodId[i]);
    }
}

void chargeInfoLayer::addStoneInfo(const char* nor, const char* sel, Menu* parent, Vec2 pos, int tag)
{
    Sprite* normal = Sprite::createWithSpriteFrameName(nor);
    Sprite* select = Sprite::createWithSpriteFrameName(sel);
    LYMenuItemSprite* pItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(chargeInfoLayer::btnCallBack, this));
    
    pItem->setScale(0.9);
    
    parent->addChild(pItem);
    pItem->setPosition(pos);
    pItem->setTag(tag);

    chargeInfo* pInfo = CDataManager::getChargeInfoById(tag);
    int index = 0;
    for(int i = 0 ; i < m_chargeNum.size(); i++)
    {
        if(tag == m_chargeNum[i].theId)
        {
            index = i;
            break;
        }
    }
    if(pInfo)
    {
    	Label* pLabel = Label::createWithSystemFont(pInfo->desc.c_str(), "Arial", 30);
    	pLabel->setColor(Color3B(70, 131, 29));
    	pItem->addChild(pLabel);
    	pLabel->setPosition(Vec2(normal->getContentSize().width * 0.5f, normal->getContentSize().height * 0.92f));

    	char buf[256];
    	Sprite* stone = Sprite::createWithSpriteFrameName("stone.png");
    	pItem->addChild(stone);
    	stone->setScale(0.7f);
    	stone->setPosition(Vec2(normal->getContentSize().width * 0.32f, normal->getContentSize().height * 0.81f));
    	sprintf(buf, "%d", pInfo->num);
    	log("tag = %d, id = %d, icon = %s, desc = %s, 魔法石数量 = %d, price = %d, 折扣 = %d", tag, pInfo->chargeId,
    			pInfo->icon.c_str(), pInfo->desc.c_str(), pInfo->num, pInfo->price, pInfo->disCount);
        
        
    	//Label* pStonLabel =  Label::createWithTTF(buf,"fonts/cuti.ttf",30);
        
        LabelAtlas *pStonLabel =  LabelAtlas::create(buf,"fonts/numAtlas.png",24,37,'0');
        
    	pItem->addChild(pStonLabel);
        pStonLabel->setAnchorPoint(Vec2(0, 0.5f));
    	//pStonLabel->enableOutline(Color4B(0, 0, 0, 250), 3);
    	pStonLabel->setPosition(Vec2(stone->getPositionX() + stone->getBoundingBox().size.width * 0.65f, stone->getPositionY() + 2));

    	//光圈
    	Sprite* light = Sprite::createWithSpriteFrameName("backlight3.png");
    	pItem->addChild(light);
    	light->setScale(2.1f);
    	light->setPosition(Vec2(normal->getContentSize().width * 0.5f, normal->getContentSize().height * 0.52f));

    	Sprite* page = Sprite::createWithSpriteFrameName(pInfo->icon.c_str());
    	pItem->addChild(page);
    	page->setPosition(Vec2(normal->getContentSize().width * 0.5f, normal->getContentSize().height * 0.48f));

        Sprite* star = Sprite::createWithSpriteFrameName("chargeStar.png");
        pItem->addChild(star);
        star->setPosition(Vec2(normal->getContentSize().width * 0.5f, normal->getContentSize().height * 0.5f));
        
        
    	sprintf(buf, "￥%d元", pInfo->price);
        Label* priceLabel = Label::createWithTTF(buf,"fonts/cuti.ttf",30);
        priceLabel->enableOutline(Color4B(0,0,0,255),3);
        //(buf, "Arial", 34);
    	pItem->addChild(priceLabel,2);
    	priceLabel->setPosition(Vec2(normal->getContentSize().width * 0.5f, normal->getContentSize().height * 0.0975f));

    	Sprite* limitBuy = Sprite::createWithSpriteFrameName("limitBuy.png");
    	pItem->addChild(limitBuy);
    	limitBuy->setPosition(Vec2(normal->getContentSize().width * 0.5f, normal->getContentSize().height * 0.23f));
    	limitBuy->setVisible(false);

        
        Sprite *pbg = Sprite::createWithSpriteFrameName("chongzhibtn.png");
        pbg->cocos2d::Node::setPosition(pItem->getContentSize().width*0.5,pItem->getContentSize().height*0.1);
        pItem->addChild(pbg);
    
        
    	Sprite* sellBest = Sprite::createWithSpriteFrameName("rexiao.png");
    	pItem->addChild(sellBest);
    	sellBest->setScale(0.9f);
    	sellBest->setPosition(Vec2(normal->getContentSize().width * 0.17f, normal->getContentSize().height * 0.68f));
    	sellBest->setVisible(false);

    	//折扣
    	Sprite* disCountBg = Sprite::createWithSpriteFrameName("dz.png");
    	pItem->addChild(disCountBg);
    	disCountBg->setScale(1.25f);
    	disCountBg->setPosition(Vec2(normal->getContentSize().width * 0.68f, normal->getContentSize().height * 0.53f));
    	disCountBg->setVisible(false);

    	float num = pInfo->disCount/10.0;
    	sprintf(buf, "%.1f", num);
    	if(num == pInfo->disCount/10)
    	{
    	    num = pInfo->disCount/10;
    	    sprintf(buf, "%.f", num);
    	}
    	float xpos = disCountBg->getContentSize().width * 0.5f;
    	std::string temp = buf;
    	std::string::size_type startPos = temp.find(".");
    	if(startPos != std::string::npos)
    	{
    	    temp.replace(startPos, 1, ":");
    	    xpos = disCountBg->getContentSize().width * 0.57f;
    	}
    	Label* pDiscountNum = Label::createWithCharMap("fonts/zheNumber.png", 14, 25, '0');
    	pDiscountNum->setString(temp);
    	pDiscountNum->setScale(0.8f);
    	disCountBg->addChild(pDiscountNum);
    	pDiscountNum->setPosition(Vec2(disCountBg->getContentSize().width * 0.35f, disCountBg->getContentSize().height * 0.5f));

    	Sprite* disCountWord = Sprite::createWithSpriteFrameName("zhezi.png");
    	disCountBg->addChild(disCountWord);
    	disCountWord->setScale(0.8f);
    	disCountWord->setAnchorPoint(Vec2(0, 0.5));
    	disCountWord->setPosition(Vec2(xpos, disCountBg->getContentSize().height * 0.5f));
        if(m_chargeNum.size() == 0)
        {
            if(pInfo->type == 1)
            {
                limitBuy->setVisible(true);
            }
            else if(pInfo->type == 2){
                sellBest->setVisible(true);
            }
            else if(pInfo->type == 3)
            {
                limitBuy->setVisible(true);
                sellBest->setVisible(true);
            }
        }
        else
        {
            if(pInfo->type == 3)
            {
                if(m_chargeNum[index].theNumber == 0)
                {
                    limitBuy->setVisible(true);
                }
                sellBest->setVisible(true);
            }
            else if((pInfo->type == 1) && (m_chargeNum[index].theNumber == 0))
            {
                limitBuy->setVisible(true);
            }
            else if(pInfo->type == 2)
            {
                sellBest->setVisible(true);
            }
        }
        if(pInfo->disCount < 100)
        {
            disCountBg->setVisible(true);
        }
    }
    
}

void chargeInfoLayer::addBtnMenu()
{
    cocos2d::Size bgSize = m_pBgLayer->getContentSize();
    Sprite* normal = Sprite::createWithSpriteFrameName("closeBtn.png");
    Sprite* select = Sprite::createWithSpriteFrameName("closeBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* exitItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(chargeInfoLayer::btnCallBack, this));
    CommonData::enlargeItemSprite(exitItem, 2);
    exitItem->setTag(CHARGE_EXIT);
    m_pExitMenu = Menu::create(exitItem, NULL);
    m_pExitMenu->setPosition(Vec2(bgSize.width * 0.97f, bgSize.height * 0.95f));
    m_pBgLayer->addChild(m_pExitMenu);
    
    
    char buf[128];
    
    //体力
//    Sprite* pStaminaBg = Sprite::createWithSpriteFrameName("information_bg.png");
//    m_pBgLayer->addChild(pStaminaBg);
//    pStaminaBg->setScale(0.85f);
//    pStaminaBg->setPosition(Vec2(bgSize.width * 0.25f, bgSize.height * 0.08f));
//    Sprite* staminaIcon = Sprite::createWithSpriteFrameName("stamina.png");
//    pStaminaBg->addChild(staminaIcon);
//    staminaIcon->setScale(0.79f);
//    staminaIcon->setPosition(Vec2(pStaminaBg->getBoundingBox().size.width, pStaminaBg->getContentSize().height * 0.5));
//    
//    //当前体力
//    sprintf(buf, "%d", CPersonalData::getInstance()->GetPhysicalNum());
//    Label* pStaminaLabel = Label::createWithSystemFont(buf, "Arial", 28);
//    pStaminaLabel->setAnchorPoint(Vec2(0, 0.5));
//    pStaminaBg->addChild(pStaminaLabel);
//    pStaminaLabel->setPosition(Vec2(pStaminaBg->getBoundingBox().size.width * 0.2, pStaminaBg->getContentSize().height * 0.5f));
//    
//    //显示金币
//    Sprite* coinBg = Sprite::createWithSpriteFrameName("information_bg.png");
//    m_pBgLayer->addChild(coinBg);
//    coinBg->setScale(0.85f);
//    coinBg->setPosition(bgSize.width * 0.5, pStaminaBg->getPositionY());
//    
//    Sprite* coinIcon = Sprite::createWithSpriteFrameName("single_coin.png");
//    coinIcon->setScale(0.75f);
//    coinBg->addChild(coinIcon);
//    coinIcon->setPosition(Vec2(coinBg->getBoundingBox().size.width, coinBg->getContentSize().height * 0.45));
//    //金币充值
//    sprintf(buf, "%d", CPersonalData::getInstance()->getUserCoin());
//    Label* pCoin = Label::createWithSystemFont(buf, "Arial", 30);
//    pCoin->setAnchorPoint(Vec2(0, 0.5));
//    coinBg->addChild(pCoin);
//    pCoin->setPosition(Vec2(coinBg->getBoundingBox().size.width * 0.2, coinBg->getContentSize().height * 0.5f));
    
    
    //魔法石
    Sprite* stoneBg = Sprite::createWithSpriteFrameName("information_bg.png");
    m_pBgLayer->addChild(stoneBg);
    stoneBg->setScale(0.85f);
    stoneBg->setPosition(Vec2(bgSize.width * 0.5, bgSize.height * 0.1f));
    Sprite* stoneIcon = Sprite::createWithSpriteFrameName("stone.png");
    stoneIcon->setRotation(20);
    stoneBg->addChild(stoneIcon);
    stoneIcon->setScale(0.76f);
    stoneIcon->setPosition(Vec2(stoneBg->getBoundingBox().size.width * 0.15f, stoneBg->getBoundingBox().size.height * 0.5f));
    //魔法石充值
    sprintf(buf, "%d", CPersonalData::getInstance()->getUserStone());
    Label* pStone = Label::createWithSystemFont(buf, "Arial", 30);
    pStone->setAnchorPoint(Vec2(0, 0.5));
    stoneBg->addChild(pStone);
    pStone->setPosition(Vec2(stoneBg->getBoundingBox().size.width * 0.52f, stoneBg->getContentSize().height * 0.5f));

}
void chargeInfoLayer::paymentClick(int userId,int mfscount,float price,float oPrice,int count,string productId,string productName,string payDescription,string orderId, std::string goodsInfo)
{
    CCLOG("%d,%d,%f,%f,%s,%s,%s,%s",userId,mfscount,price,oPrice,productId.c_str(),productName.c_str(),payDescription.c_str(),orderId.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android
    JniMethodInfo minfo;
    JniHelper::getStaticMethodInfo(minfo,"cn.xinpin.view.activity/GameActivity", "getInstance","()Ljava/lang/Object;");
    jobject obj= minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    
    JniHelper::getMethodInfo(minfo,"cn.xinpin.view.activity/GameActivity","paymentClick","(IIFFILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    jstring jProductId = minfo.env->NewStringUTF(productId.c_str());
    jstring jProductName = minfo.env->NewStringUTF(productName.c_str());
    jstring jPayDescription = minfo.env->NewStringUTF(payDescription.c_str());
    jstring jOrderId = minfo.env->NewStringUTF(orderId.c_str());
    jstring jGoodsInfo = minfo.env->NewStringUTF(goodsInfo.c_str());
    
    minfo.env->CallVoidMethod(obj,minfo.methodID,userId,mfscount,price ,oPrice ,count , jProductId ,jProductName , jPayDescription, jOrderId, jGoodsInfo);
    minfo.env->DeleteLocalRef(jProductId);
    minfo.env->DeleteLocalRef(jProductName);
    minfo.env->DeleteLocalRef(jPayDescription);
    minfo.env->DeleteLocalRef(jOrderId);
    minfo.env->DeleteLocalRef(jGoodsInfo);
#endif
}

char *random_uuid( char buf[37] )
{
    const char *c = "89ab";
    char *p = buf;
    int n;
    for( n = 0; n < 16; ++n )
    {
        int b = rand()%255;
        switch( n )
        {
            case 6:
                sprintf(p, "4%x", b%15 );
                break;
            case 8:
                sprintf(p, "%c%x", c[rand()%strlen(c)], b%15 );
                break;
            default:
                sprintf(p, "%02x", b);
                break;
        }
        
        p += 2;
        switch( n )
        {
            case 3:
            case 5:
            case 7:
            case 9:
                *p++ = '-';
                break;
        }
    }
    *p = 0;
    return buf;
}

void chargeInfoLayer::btnCallBack(Ref *pSender)
{
    int tag = ((Node*)pSender)->getTag();
    if(tag == CHARGE_EXIT)
    {
        CPersonalData::getInstance()->setTouchRmvFlag(true);
        removeFromParent();
    }
    else
    {
        m_pInfo = CDataManager::getChargeInfoById(tag);
        log("充值%d", tag);
        
        CPersonalData::getInstance()->m_pChargeInfo = this;
        char buf[128];
        m_order = getOrderNumber();
        
        sprintf(buf, "%d", m_pInfo->chargeId);
        std::string produtcId = buf;
        if(m_chargeNum.size() == 0)
        {
            m_goodsInfo += buf;
            m_goodsInfo += ",1";
            IdAndNum info;
            info.theId = m_pInfo->chargeId;
            info.theNumber = 1;
            m_tempValue.push_back(info);
        }
        else
        {
            int num = 0;
            for(int i = 0; i < m_tempValue.size(); i++)
            {
                num = 0;
                sprintf(buf, "%d", m_chargeNum[i].theId);
                m_goodsInfo += buf;
                m_goodsInfo += ",";
                if(m_chargeNum[i].theId == tag)
                {
                    m_tempValue[i].theNumber++;
                    num = m_chargeNum[i].theNumber + 1;
                }
                else{
                    num = m_chargeNum[i].theNumber;
                }
                sprintf(buf, "%d", num);
                
                m_goodsInfo += buf;
                if(i < m_tempValue.size() - 1)
                {
                    m_goodsInfo += ";";
                }
            }
            bool bEisted = false;
            for(int i = 0; i < m_chargeNum.size(); i++)
            {
                if(m_chargeNum[i].theId == tag)
                {
                    bEisted = true;
                    break;
                }
            }
            if(!bEisted)
            {
                m_goodsInfo += ";";
                sprintf(buf, "%d", tag);
                m_goodsInfo += buf;
                m_goodsInfo += ",1";
            }
            
        }
        m_stoneNum = m_pInfo->num;
        //talkingData数据统计
        TDCCVirtualCurrency::onChargeRequest(m_order.c_str(), m_pInfo->desc.c_str(), m_pInfo->price, "CNY", m_pInfo->num, GAME_VERSION);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        NSLog(@"%d", tag);
        NSLog(@"%@", productID);
        NSLog(@"%@", [productID objectAtIndex: tag - 1]);
        [[CBiOSStoreManager sharedInstance] initialStore: this];
        [[CBiOSStoreManager sharedInstance] buy:[productID objectAtIndex: tag - 1]];
        NSLog(@"%@", [productID objectAtIndex: tag - 1]);
#else
        log("商品信息%s", m_goodsInfo.c_str());
        if(GAME_VERSION == "wandoujia")
        {
            sprintf(buf, "%d", m_pInfo->num);
            m_order += "-";
            m_order += buf;
        }
        paymentClick(MY_PERSION->getUserInfo()->m_iUserId,  m_pInfo->num,  m_pInfo->price,  m_pInfo->price,
                     1 , produtcId, productName[tag - 1],  m_pInfo->desc, m_order, m_goodsInfo);
#endif
    }
    
}

void chargeInfoLayer::iapSendMessage()
{
    //开始发送请求更新服务器数据
    __String* buf = __String::createWithFormat("userId=%d&productId=%d&stone=%d&price=%d&orderId=%s&info=%s", CPersonalData::getInstance()->getUserInfo()->m_iUserId,m_pInfo->chargeId, m_pInfo->num, m_pInfo->price, m_order.c_str(), m_goodsInfo.c_str());
    CDataManager::insertMessageInfo(buf->getCString());
    CNetControlModule::sharedNetControlModule()->sendChargeInfo(this, menu_selector(chargeInfoLayer::getMessgageBack), buf->getCString());
}

void chargeInfoLayer::getMessgageBack(Ref* pSender)
{
    _netRecvEventGetBuyInfo* pInfo = (_netRecvEventGetBuyInfo*)pSender;
    int info = 1;
    int userStone = 0;
    for(std::vector<std::string>::iterator it = pInfo->m_paramsStr.begin(); it!= pInfo->m_paramsStr.end(); it++)
    {
        string str = *it;
        string name = getStrFromBackStr(str.c_str());
        
        if(name == "info")
        {
            info = atoi(getDigitFromBackStr(it->c_str()).c_str());
        }
        else if(name == "stone")
        {
            userStone = atoi(getDigitFromBackStr(it->c_str()).c_str());
        }
    }
    if(info == 0)
    {
        CPersonalData::getInstance()->setUserStone(CPersonalData::getInstance()->getUserStone() + m_stoneNum);
        __String* tempStr1 = __String::createWithFormat("%d", m_stoneNum);
        EventParamMap paramMap;
        
        paramMap.insert(EventParamPair("购买魔法石",tempStr1->getCString()));
        
        TDCCTalkingDataGA::onEvent("充值界面", &paramMap);
        m_chargeNum = m_tempValue;
        showGoodsInfo();
        CDataManager::delMeeageInfo(m_goodsInfo);
        //removeFromParent();
    }
    else{
        log("充值更新服务器数据失败!");
    }
}


#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C" {
    void Java_cn_xinpin_view_activity_GameActivity_paymentCallBack(JNIEnv *env, jobject o, jint diamondCount, jstring serial,jstring payDescription)
    {
        CCLog("paymentCallBack");
        char* rtn = NULL;
        jclass clsstring = env->FindClass("java/lang/String");
        jstring strencode = env->NewStringUTF("utf-8");
        jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
        jbyteArray barr= (jbyteArray)env->CallObjectMethod(serial, mid, strencode);
        jsize alen = env->GetArrayLength(barr);
        jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
        if (alen > 0)
        {
            rtn = (char*)malloc(alen + 1);
            memcpy(rtn, ba, alen);
            rtn[alen] = 0;
        }
        env->ReleaseByteArrayElements(barr, ba, 0);
        
        const char *_payDescription = env->GetStringUTFChars(payDescription, 0);
        std::string str = _payDescription;
        CCLOG("花费%d购买%s", diamondCount, _payDescription);
        m_stoneNum = diamondCount;
        CPersonalData::getInstance()->setChargeSucess(true);
        CPersonalData::getInstance()->m_pChargeInfo->payCallBack();
        const char* javavolume = env->GetStringUTFChars(serial, 0);
        TDCCVirtualCurrency::onChargeSuccess(javavolume);
    }
    void Java_cn_xinpin_view_activity_GameActivity_paymentFailCallBack(JNIEnv *env, jobject o, jstring errCode) {
        CCLog("paymentFailCallBack购买失败^.^");
        CPersonalData::getInstance()->setChargeSucess(false);
        CPersonalData::getInstance()->m_pChargeInfo->payCallBack();
    }
}
#endif


void chargeInfoLayer::payCallBack()
{
    if(CPersonalData::getInstance()->getChargeSucess())
    {
    	log("支付回调,保存魔法石");
        CPersonalData::getInstance()->setChargeSucess(false);
        //保存魔法师数量到本地服务器
        log("保存数据库%d", m_stoneNum);
        CPersonalData::getInstance()->setUserStone(CPersonalData::getInstance()->getUserStone() + m_stoneNum);
        char buf[256];
        sprintf(buf, "%d", m_stoneNum);
        std::string tempStr = "购买成功！\n";
        tempStr += "获得";
        tempStr += buf;
        tempStr += "宝石";
        /*Label* pLabel = Label::createWithSystemFont(tempStr.c_str(), "Arial", 35);
        addChild(pLabel, 100);
        pLabel->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.5f));
        pLabel->setScaleX(0);
        pLabel->runAction(Sequence::create(ScaleTo::create(0.5, 1, 1), DelayTime::create(1.0f), ScaleTo::create(0.5, 1, 0),
                                           RemoveSelf::create(), NULL));*/
        Sprite * thetarget = CFightManager::getTheFlyWord(tempStr,"fonts/cuti.ttf");
        thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
        addChild(thetarget, 100);
        
        m_chargeNum = m_tempValue;
        log("支付回调talkData");
        __String* tempStr1 = __String::createWithFormat("%d", m_stoneNum);
        EventParamMap paramMap;
        
        paramMap.insert(EventParamPair("购买魔法石",tempStr1->getCString()));
        
        TDCCTalkingDataGA::onEvent("充值界面", &paramMap);
        //showGoodsInfo();
        removeFromParent();
    }
    else{
        
        /*Label* pLabel = Label::createWithSystemFont("购买失败咯!", "Arial", 35);
        addChild(pLabel, 100);
        pLabel->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.5f));
        pLabel->setScaleX(0);
        pLabel->runAction(Sequence::create(ScaleTo::create(0.5, 1, 1), DelayTime::create(1.0f), ScaleTo::create(0.3, 1, 0),
                                           RemoveSelf::create(), NULL));*/
    
        Sprite * thetarget = CFightManager::getTheFlyWord("购买失败哟!","fonts/cuti.ttf");
        thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
        addChild(thetarget, 100);
        
    }
}
