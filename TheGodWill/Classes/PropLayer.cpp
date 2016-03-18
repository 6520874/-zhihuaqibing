//
//  PropLayer.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/12/24.
//
//

#include "PropLayer.h"
#include "DBMgr.h"
#include "CHitDialog.h"
#include "commonPropLayer.h"
#include "guiderManagerLayer.h"

#include "CSharedPlist.h"
#include <algorithm>
#include "LYFightManager.h"
#include "NetWorkDataSingle.h"
#include "Common.h"

std::string iconBg[5]={"whiteBg.png","greenBg.png","blueBg.png","purpleBg.png", "orangeBg.png"};

bool SortByProp( const MyPropInfo* v1, const MyPropInfo* v2)
{
    propInfo* pInfo1 =  CDataManager::getPropInfoById(v1->m_iPropId);
    
    propInfo* pInfo2 =  CDataManager::getPropInfoById(v2->m_iPropId);
    
    if(pInfo1->m_iItemLevel< pInfo2->m_iItemLevel)
    {
        return v1<v2;//升序排列
    }
    else
    {
        return v1>v2;
    }
}

CPropLayer::CPropLayer()
{
    m_winSize = Director::getInstance()->getWinSize();
    m_bMenuSale = false;
    m_bMenuSaleAll = false;
    m_bMenuUse = false;
    m_bMenuClose = false;
    m_bMenuNext = false;

    m_pMenuSale = NULL;
    m_pMenuClose = NULL;
    m_pMenuNext = NULL;
    m_pMenuUp = NULL;
    m_pMenuUse = NULL;
    m_pMenuSaleAll = NULL;
    m_iSelectIndex = 0;
    m_pSaleNum = NULL;
    m_pLabelHaveNum = NULL;
    m_pLabelNeedNum = NULL;
    m_pNeedSprite = NULL;
    m_pCurIndex = NULL;
    m_pAllIndex = NULL;
    CDBMgr::getInstance()->OpenDB();
    CDBMgr::getInstance()->loadUserPropDB();
    CDataBaseOper::closeDB();
    m_selected = 0;
    m_UserChest.clear();
    m_pSwitchMenu = NULL;
    m_pMainTaks = NULL;
    m_pCoin = NULL;
    m_pStone = NULL;
    m_boxIndex = 1;
    m_iboxSelected = 0;
    m_pKey = NULL;
    CSharedPlist::getInstance()->pushPlist("plist/propIcon_1.plist");
    CSharedPlist::getInstance()->pushPlist("plist/propIcon_2.plist");
    m_iHaveNum = 0;
    m_iNeedNum = 0;
    
}

CPropLayer::~CPropLayer()
{
    CSharedPlist::getInstance()->erasePlist("plist/propIcon_1.plist");
    CSharedPlist::getInstance()->erasePlist("plist/propIcon_2.plist");
}


bool CPropLayer::init()
{
    ///CBaseLayer::init();
    CBaseLayer::initWithColor(Color4B(0, 0, 0, 200));
    
    m_pListner = EventListenerTouchOneByOne::create();//创建一个触摸监听
    m_pListner->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false
    m_pListner->onTouchBegan = CC_CALLBACK_2(CPropLayer::onTouchBegan, this);
    m_pListner->onTouchMoved = CC_CALLBACK_2(CPropLayer::onTouchMoved, this);
    m_pListner->onTouchEnded = CC_CALLBACK_2(CPropLayer::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListner, this);
    
    m_pMyPropAllInfo = CDBMgr::getInstance()->GetUserProp();

    
    
    
    int j = 0;
    
    std::vector<MyPropInfo*>  pp;
    
    for(int i=0;i<m_pMyPropAllInfo.size();i++)
    {
        bool isChest = false;
        if(m_pMyPropAllInfo.at(i)->m_iPropId >= 36)//用物品id来区分
        {
            isChest = true;
            m_UserChest.push_back(m_pMyPropAllInfo.at(i));
        }
        else if(i!=0)
        {
            if((i)%12==0)
            {
                m_pPropMap[j] = pp;
                j++;
                pp.clear();
            }
        }
        if(!isChest)
        {
            pp.push_back(m_pMyPropAllInfo[i]);
        }
    }
    m_pPropMap[j++] = pp;
    
    m_iSelectIndex = 0;
    m_pMainTaks = Sprite::createWithSpriteFrameName("storageBg.png");
    m_pMainTaks->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    
    Vec2 tx = m_pMainTaks->getAnchorPoint();
    
    
    addChild(m_pMainTaks);
    Size bgSize = m_pMainTaks->getBoundingBox().size;
    

    Sprite *pCloseSprite = Sprite::createWithSpriteFrameName("closeBtn.png");
    Sprite *pCloseSpriteSelc  = Sprite::createWithSpriteFrameName("closeBtn.png");
    pCloseSpriteSelc->setColor(Color3B::GRAY);
    LYMenuItemSprite *pCloseItem = LYMenuItemSprite::create(pCloseSprite,pCloseSpriteSelc,CC_CALLBACK_1(CPropLayer::OnExitLvLayer, this));
    CommonData::enlargeItemSprite(pCloseItem, 2);
    pCloseItem->setTag(10);
    m_pMenuClose =  Menu::create(pCloseItem,NULL);
    
    m_pMenuClose->setPosition(m_pMainTaks->getBoundingBox().size.width*0.945, m_pMainTaks->getBoundingBox().size.height*0.94);
    m_pMainTaks->addChild(m_pMenuClose,1);
    
    addBtnMenu();
    
 
    char buf[128];
    
    //拥有数量
    m_pPropVecSprite.clear();
    
    m_pMyPropInfo = m_pPropMap[0];
    m_iMapIndex = 0;
    
    //15个一份
    
    std::sort(m_pMyPropInfo.begin(),m_pMyPropInfo.end(),SortByProp);
    
    for(int i=0;i<m_pMyPropInfo.size();i++)
    {
        propInfo* info = CDataManager::getPropInfoById(m_pMyPropInfo.at(i)->m_iPropId);
        //道具背景
        
        sprintf(buf, "%s", iconBg[info->m_iItemLevel - 1].c_str());
        
        Sprite* propBg = Sprite::createWithSpriteFrameName(buf);
        m_pMainTaks->addChild(propBg,0,100);
        
        propBg->setScale(2);
        
        propBg->setPosition(bgSize.width*0.1+i%4*bgSize.width*0.14, bgSize.height*0.78-i/4*bgSize.height*0.24);
        
        sprintf(buf, "%s",info->m_sItemIcon.c_str());
        
        Sprite *pProp = Sprite::createWithSpriteFrameName(buf);
        pProp->setScale(1);
        pProp->setPosition(Vec2(pProp->getBoundingBox().size.width / 2, pProp->getBoundingBox().size.height / 2));
        propBg->addChild(pProp);
        
        Size tProp = pProp->getBoundingBox().size;
        
        Sprite *pPropNum = Sprite::createWithSpriteFrameName("numbg.png");
        pPropNum->setScale(0.5);
        
        pPropNum->setPosition(tProp.width*0.71,tProp.height*0.2);
        pProp->addChild(pPropNum);
        
        Size tPropNum = pPropNum->getContentSize();
        char strPropHave[100] = {0};
        
        sprintf(strPropHave,"%d",m_pMyPropInfo[i]->m_iPropNum);
        Label *pLabelNum = Label::createWithSystemFont(strPropHave,"Aril",22);
        
        pLabelNum->setPosition(tPropNum.width*0.5,tPropNum.height*0.5);
        pPropNum->addChild(pLabelNum);
        
        //名字
        int iPropId=  m_pMyPropInfo[i]->m_iPropId;
        propInfo* pInfo = CDataManager::getPropInfoById(iPropId);
        
        //m_pMyPropInfo
        Label *pPropName = Label::createWithSystemFont(pInfo->m_sItemName,"Aril",24);
        pPropName->setScale(0.5);
        pPropName->setPosition(tProp.width*0.5,-tProp.height*0.1);
        pProp->addChild(pPropName);
        m_pPropVecSprite.push_back(propBg);
    }
    
    //MyPropInfo*
    Sprite *pSelectKuang = Sprite::createWithSpriteFrameName("selectkuang.png");
    //pSelectKuang->setScale(4);


    if(m_pPropVecSprite.size()>0)
    {
        Size tSProp = m_pPropVecSprite[m_iSelectIndex]->getBoundingBox().size;
        pSelectKuang->setScale(1.8);
        pSelectKuang->setPosition(tSProp.width*0.25,tSProp.height*0.25);
        m_pPropVecSprite[m_iSelectIndex]->addChild(pSelectKuang,0,101);
    }
    
    //左箭头
    Sprite *pLeftArrow = Sprite::createWithSpriteFrameName("arrow.png");
    pLeftArrow->setFlippedX(true);
    LYMenuItemSprite *pLeftItem = LYMenuItemSprite::create(pLeftArrow,NULL,CC_CALLBACK_1(CPropLayer::OnLeftMenu, this), "music/back.mp3");
    m_pMenuUp = Menu::create(pLeftItem,NULL);
    m_pMenuUp->setPosition(Vec2(bgSize.width*0.1,bgSize.height*0.13));
    m_pMainTaks->addChild(m_pMenuUp);
    
    //右箭头
    Sprite *pRightArrow = Sprite::createWithSpriteFrameName("arrow.png");
    LYMenuItemSprite *pRightItem = LYMenuItemSprite::create(pRightArrow,NULL,CC_CALLBACK_1(CPropLayer::OnRightMenu, this), "music/next.mp3");
    m_pMenuNext = Menu::create(pRightItem,NULL);
    m_pMenuNext->setPosition(Vec2(bgSize.width*0.5,bgSize.height*0.13));
    m_pMainTaks->addChild(m_pMenuNext);
    
    //数据中间
    Sprite *pMidLine = Sprite::createWithSpriteFrameName("select_kuang.png");
    pMidLine->setScale(2);
    
    pMidLine->setPosition(bgSize.width*0.3,bgSize.height*0.13);
    m_pMainTaks->addChild(pMidLine);
    
    m_pCurIndex = Label::createWithSystemFont("1","Aril",30);
    Size tMiddle = pMidLine->getContentSize();
    m_pCurIndex->setScale(0.5);
    m_pCurIndex->setPosition(tMiddle.width*0.46,tMiddle.height*0.5);
    pMidLine->addChild(m_pCurIndex);
    
    char strMapIndex[50] = {0};
    sprintf(strMapIndex,"/%lu",m_pPropMap.size());
    m_pAllIndex = Label::createWithSystemFont(strMapIndex,"Aril",30);
    m_pAllIndex->setScale(0.5);
    m_pAllIndex->setPosition(tMiddle.width*0.525,tMiddle.height*0.5);
    pMidLine->addChild(m_pAllIndex);
    
    Sprite *pLine = Sprite::createWithSpriteFrameName("split_line.png");
    pLine->setPosition(Vec2(bgSize.width*0.6,bgSize.height*0.48));
    m_pMainTaks->addChild(pLine);
    
    //onebei.png
    Sprite *pDesc = Sprite::createWithSpriteFrameName("propInfo_bg.png");
    pDesc->setPosition(Vec2(bgSize.width*0.795,bgSize.height*0.5));
    m_pMainTaks->addChild(pDesc);

    
    m_tDesc = pDesc->getContentSize();
    
    int iPropId = 1;
    
    
    if(m_pPropVecSprite.size()>0)
    {
        iPropId = m_pMyPropInfo[m_iSelectIndex]->m_iPropId;
    }
    
    propInfo* pInfo = CDBMgr::getInstance()->getPropInfoById(iPropId);
    
    m_pPropNameLabel = Label::createWithSystemFont(pInfo->m_sItemName,"Aril",30);
    m_pPropNameLabel->setPosition(m_tDesc.width*0.5,m_tDesc.height*0.925);
    pDesc->addChild( m_pPropNameLabel);
    
    sprintf(strMapIndex, "%s", pInfo->m_sItemIcon.c_str());

    m_pNeedSprite = Sprite::createWithSpriteFrameName(strMapIndex);
    m_pNeedSprite->cocos2d::Node::setScale(1.5);
    
    pDesc->addChild( m_pNeedSprite);
    m_pNeedSprite->setPosition(m_tDesc.width*0.5,m_tDesc.height*0.774);
    
    m_pPropDescLabel = Label::createWithSystemFont(pInfo->m_sItemDes,"Aril",20, Size(302,60));
    m_pPropDescLabel->setPosition(m_tDesc.width*0.5,m_tDesc.height*0.55);
    pDesc->addChild(m_pPropDescLabel);
    
    
    //卖出单价
    Label *pSalePrize = Label::createWithSystemFont("卖出单价:","Aril",20);
    pSalePrize->setColor(Color3B::YELLOW);
    pSalePrize->setPosition(m_tDesc.width*0.23,m_tDesc.height*0.38);
    pDesc->addChild(pSalePrize);
    
    Sprite *pSaleCoin = Sprite::createWithSpriteFrameName("coin.png");
    pSaleCoin->setPosition(m_tDesc.width*0.45,m_tDesc.height*0.38);
    pDesc->addChild(pSaleCoin);
    pSaleCoin->setScale(0.8f);
    
    char strSalePrize[100] = {0};
    sprintf(strSalePrize,"%d",pInfo->m_iSalePrize);
    
    m_pSaleNum = Label::createWithSystemFont(strSalePrize,"Aril",30);
    m_pSaleNum->setPosition(pSaleCoin->getPositionX()+pSaleCoin->getContentSize().width*1.1,m_tDesc.height*0.38);
    pDesc->addChild(m_pSaleNum);
    
    //卖出
    Sprite *pSaleSprite = Sprite::createWithSpriteFrameName("redbtn.png");
    Sprite *pSaleSpriteSelect = Sprite::createWithSpriteFrameName("redbtn.png");
    pSaleSpriteSelect->setColor(Color3B::GRAY);
    
    m_pSaleMenuItem = LYMenuItemSprite::create(pSaleSprite,pSaleSpriteSelect,CC_CALLBACK_1(CPropLayer::OnSaleMenu, this));
    m_pSaleMenuItem->setScale(2);
    
    m_pMenuSale = Menu::create(m_pSaleMenuItem,NULL);
    //m_pMenuSale->setPosition(m_tDesc.width*0.01,m_tDesc.height*0.24);
    pDesc->addChild(m_pMenuSale);
    
    Size tSale = m_pSaleMenuItem->getContentSize();
    m_pLabelSale = Label::createWithSystemFont("卖出","Aril",30);
    m_pLabelSale->setPosition(tSale.width*0.5,tSale.height*0.5);
    m_pLabelSale->setScale(0.5);
    m_pSaleMenuItem->addChild(m_pLabelSale);
    
    //可以使用  不可以使用
    
    bool bUseful = pInfo->m_bUse;
    
    Sprite *pUseSprite = Sprite::createWithSpriteFrameName("greenbtn.png");
    
    Sprite *pUseSpriteSelect = Sprite::createWithSpriteFrameName("greenbtn.png");
    pUseSpriteSelect->setColor(Color3B::GRAY);
    
    m_pUseMenuItem = LYMenuItemSprite::create(pUseSprite,pUseSpriteSelect,CC_CALLBACK_1(CPropLayer::OnUseMenu,this));
    m_pUseMenuItem->setScale(2);
    
    Size tUse = m_pUseMenuItem->getContentSize();
    Label *pLableUse = Label::createWithSystemFont("开启","Aril",30);
    pLableUse->cocos2d::Node::setScale(0.5);
    pLableUse->setPosition(tUse.width*0.5,tUse.height*0.5);
    m_pUseMenuItem->addChild(pLableUse);
    
    m_pMenuUse = Menu::create(m_pUseMenuItem,NULL);
    m_pMenuUse->setPosition(m_tDesc.width*0.5,m_tDesc.height*0.075);
    pDesc->addChild(m_pMenuUse);

    
    if(!bUseful)
    {
        m_pMenuUse->setVisible(false);
        m_pMenuSale->setPosition(m_tDesc.width*0.3,m_tDesc.height*0.24);
    }
    
    //全部卖出
    Sprite *pSaleAllSprite = Sprite::createWithSpriteFrameName("redbtn.png");
    Sprite *pSaleAllSelect = Sprite::createWithSpriteFrameName("redbtn.png");
    pSaleAllSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pMenuItemSaleall = LYMenuItemSprite::create(pSaleAllSprite,pSaleAllSelect,CC_CALLBACK_1(CPropLayer::OnSaleAllMenu, this));
    pMenuItemSaleall->setScale(2);
    
    m_pMenuSaleAll = Menu::create(pMenuItemSaleall,NULL);
    
    if(m_selected == 1)
    {
        m_pMenuSaleAll->setPosition(m_tDesc.width*0.5,m_tDesc.height*0.24);
    }
    else
    {
         m_pMenuSaleAll->setPosition(m_tDesc.width*0.7,m_tDesc.height*0.24);
    }
    pDesc->addChild(m_pMenuSaleAll);
    
    Label *pLableSaleAll = Label::createWithSystemFont("全部卖出","Aril",30);
    pLableSaleAll->setScale(0.5);
    Size tSallSprite = pSaleAllSprite->getContentSize();
    pLableSaleAll->setPosition(tSallSprite.width*0.5,tSallSprite.height*0.5);
    pSaleAllSprite->addChild(pLableSaleAll);

    
    //需要一个道具
    int iNeedPropId = pInfo->m_iNeedPropId;
    
    //道具个数
    int iNeedNum = pInfo->m_iNeedPropNum;
    
    int iHaveNum = 0;
    
    iHaveNum = CDBMgr::getInstance()->HavePropNumById(iNeedPropId);
    propInfo *p = CDBMgr::getInstance()->getPropInfoById(iNeedPropId);
    
    if(p==NULL)
    {
        p = CDBMgr::getInstance()->getPropInfoById(1);
    }
    
    char strNeedNum[100] = {0};
    sprintf(strNeedNum,"/%d",iNeedNum);
    
    char strHaveNum[100] = {0};
    sprintf(strHaveNum,"%d",iHaveNum);
    
    m_pKeyText = Label::createWithSystemFont("需要:","Aril",20);
    m_pKeyText->setColor(Color3B::YELLOW);
    m_pKeyText->setPosition(m_tDesc.width*0.3,m_tDesc.height*0.673);
    pDesc->addChild(m_pKeyText);
    
    sprintf(strMapIndex, "%s", p->m_sItemIcon.c_str());
    
    m_pKey = Sprite::createWithSpriteFrameName(strMapIndex);
    m_pKey->setScale(0.5);
    m_pKey->setPosition(m_tDesc.width*0.42,m_tDesc.height*0.673);
    pDesc->addChild(m_pKey);
    
    m_pLabelHaveNum = Label::createWithSystemFont(strHaveNum,"Aril",20);
    m_pLabelHaveNum->setPosition(m_tDesc.width*0.49,m_tDesc.height*0.673);
    pDesc->addChild(m_pLabelHaveNum);
    
    
    m_pLabelNeedNum = Label::createWithSystemFont(strNeedNum,"Aril",20);
    m_pLabelNeedNum->setColor(Color3B::GREEN);
    m_pLabelNeedNum->setPosition(m_pLabelHaveNum->getPositionX()+m_pLabelHaveNum->getContentSize().width*0.8+m_pLabelNeedNum->getContentSize().width*0.5
                               ,m_tDesc.height*0.673);
    pDesc->addChild(m_pLabelNeedNum);

 
    
    if(iNeedNum>iHaveNum)
    {
        m_pLabelHaveNum->setColor(Color3B::RED);
    }
    else
    {
        m_pLabelHaveNum->setColor(Color3B::GREEN);
    }
    
    if(iNeedNum<=0)
    {
        m_pKeyText->setVisible(false);
        m_pKey->setVisible(false);
        m_pLabelHaveNum->setVisible(false);
        m_pLabelNeedNum->setVisible(false);
    }
    
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ENTER_STORAGE) && (guiderManagerLayer::getInstance()->getStepNum() == 1))
    {
        LYMenuItemSprite* chestItem = (LYMenuItemSprite*)m_pSwitchMenu->getChildByTag(STORAGE_CHEST_TAG);
        Vec2 pos = chestItem->convertToWorldSpaceAR(Vec2::ZERO);
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
        NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CPropLayer::addCloseGuide), "storageGuide", NULL);
    }
    
    return true;
}

void CPropLayer::addBtnMenu()
{
    //切换按钮
    
    char buf[128];
    m_pSwitchMenu = Menu::create();
    m_pMainTaks->addChild(m_pSwitchMenu,1);
    m_pSwitchMenu->setPosition(Vec2::ZERO);
    
    Sprite* normal = Sprite::createWithSpriteFrameName("goodsBtn2.png");
    Sprite* select = Sprite::createWithSpriteFrameName("goodsBtn1.png");
    LYMenuItemSprite* goodItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CPropLayer::onBtnCallBack, this));
    goodItem->setTag(STORAGE_GOOD_TAG);
    goodItem->setEnabled(false);
    m_pSwitchMenu->addChild(goodItem);
    goodItem->setPosition(m_pMainTaks->getContentSize().width * 0.17, m_pMainTaks->getContentSize().height - normal->getContentSize().height * 0.77);
    Label* pGoodLabel = Label::createWithSystemFont("物品", "Arial", 30);
    goodItem->addChild(pGoodLabel, 5);
    pGoodLabel->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height / 2));
    
    normal = Sprite::createWithSpriteFrameName("goodsBtn1.png");
    select = Sprite::createWithSpriteFrameName("goodsBtn2.png");
    LYMenuItemSprite* chestItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CPropLayer::onBtnCallBack, this));
    chestItem->setTag(STORAGE_CHEST_TAG);
    m_pSwitchMenu->addChild(chestItem);
    chestItem->setPosition(m_pMainTaks->getContentSize().width * 0.39, m_pMainTaks->getContentSize().height - normal->getContentSize().height * 0.77);
    Label* pBoxLabel = Label::createWithSystemFont("宝箱", "Arial", 30);
    chestItem->addChild(pBoxLabel, 5);
    pBoxLabel->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height * 0.5f));
    
    //显示金币魔法石
    Sprite* coinBg = Sprite::createWithSpriteFrameName("information_bg.png");
    m_pMainTaks->addChild(coinBg);
    coinBg->setScale(0.72f);
    coinBg->setPosition(m_pMainTaks->getContentSize().width * 0.61, m_pMainTaks->getContentSize().height - normal->getContentSize().height * 0.768);
    
    Sprite* coinIcon = Sprite::createWithSpriteFrameName("single_coin.png");
    coinIcon->setAnchorPoint(Vec2(0, 0.5));
    coinIcon->setScale(0.8f);
    coinBg->addChild(coinIcon);
    coinIcon->setPosition(Vec2(12, coinBg->getContentSize().height * 0.45));
    //金币充值
    sprintf(buf, "%d", CPersonalData::getInstance()->getUserCoin());
    m_pCoin = Label::createWithSystemFont(buf, "Arial", 30);
    m_pCoin->setColor(Color3B::YELLOW);
    m_pCoin->setAnchorPoint(Vec2(1, 0.5));
    coinBg->addChild(m_pCoin);
    m_pCoin->setPosition(Vec2(coinBg->getContentSize().width * 0.9f, coinBg->getContentSize().height * 0.5f));
    normal = Sprite::createWithSpriteFrameName("plus.png");
    select = Sprite::createWithSpriteFrameName("plus.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* coinItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CPropLayer::onBtnCallBack, this));
    coinItem->setTag(STORAGE_COIN_CHARGE);
    coinItem->setScale(0.8f);
    Menu* pCoinMenu = Menu::create(coinItem, NULL);
    coinBg->addChild(pCoinMenu);
    pCoinMenu->setPosition(Vec2(coinBg->getContentSize().width * 0.23f, coinBg->getContentSize().height * 0.42f));
#ifdef NO_PAY
    coinItem->setVisible(false);
#endif
    
    
    //魔法石
    Sprite* stoneBg = Sprite::createWithSpriteFrameName("information_bg.png");
    m_pMainTaks->addChild(stoneBg);
    stoneBg->setScale(0.72f);
    stoneBg->setPosition(Vec2(m_pMainTaks->getContentSize().width * 0.81, coinBg->getPositionY()));
    Sprite* stoneIcon = Sprite::createWithSpriteFrameName("stone.png");
    stoneIcon->setAnchorPoint(Vec2(0, 0.5));
    stoneBg->addChild(stoneIcon);
    stoneIcon->setScale(0.8f);
    stoneIcon->setPosition(Vec2(12, stoneBg->getContentSize().height * 0.43));
    //魔法石充值
    sprintf(buf, "%d", CPersonalData::getInstance()->getUserStone());
    m_pStone = Label::createWithSystemFont(buf, "Arial", 30);
    m_pStone->setColor(Color3B::YELLOW);
    m_pStone->setAnchorPoint(Vec2(1, 0.5));
    stoneBg->addChild(m_pStone);
    m_pStone->setPosition(Vec2(stoneBg->getContentSize().width * 0.9f, stoneBg->getContentSize().height * 0.5f));

    
    normal = Sprite::createWithSpriteFrameName("plus.png");
    select = Sprite::createWithSpriteFrameName("plus.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* stoneItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CPropLayer::onBtnCallBack, this));
    stoneItem->setTag(STORAGE_STONE_CHARGE);
    stoneItem->setScale(0.8f);
    Menu* pStoneMenu = Menu::create(stoneItem, NULL);
    stoneBg->addChild(pStoneMenu);
    pStoneMenu->setPosition(Vec2(stoneBg->getContentSize().width * 0.23f, stoneBg->getContentSize().height * 0.42));
#ifdef NO_PAY
    stoneItem->setVisible(false);
#endif
    
}

void CPropLayer::updateCoinAndStone()
{
    char buf[128];
    sprintf(buf, "%d", CPersonalData::getInstance()->getUserCoin());
    m_pCoin->setString(buf);
    
    sprintf(buf, "%d", CPersonalData::getInstance()->getUserStone());
    m_pStone->setString(buf);
}

void CPropLayer::onBtnCallBack(Ref* pSender)
{
    m_pMyPropAllInfo = CDBMgr::getInstance()->GetUserProp();
    
    int tag = ((Node*)pSender)->getTag();
    if(tag == STORAGE_GOOD_TAG)
    {
        LYMenuItemSprite* tempSp = (LYMenuItemSprite*)(m_pSwitchMenu->getChildByTag(STORAGE_GOOD_TAG));
        if (tempSp)
        {
            tempSp->setEnabled(false);
            Sprite* sp = Sprite::createWithSpriteFrameName("goodsBtn2.png");
            tempSp->setNormalImage(sp);
        }
        tempSp = (LYMenuItemSprite*)(m_pSwitchMenu->getChildByTag(STORAGE_CHEST_TAG));
        if (tempSp)
        {
            tempSp->setEnabled(true);
            Sprite* sp = Sprite::createWithSpriteFrameName("goodsBtn1.png");
            tempSp->setNormalImage(sp);
        }
        
        m_selected = 0;
        char buf[128];
        sprintf(buf,"%d",m_iMapIndex+1);
        if(m_pCurIndex)
        {
            m_pCurIndex->setString(buf);
        }
        ReloadData();
    }
    else if(tag == STORAGE_CHEST_TAG)
    {
        LYMenuItemSprite* tempSp = (LYMenuItemSprite*)(m_pSwitchMenu->getChildByTag(STORAGE_GOOD_TAG));
        if (tempSp)
        {
            tempSp->setEnabled(true);
            Sprite* sp = Sprite::createWithSpriteFrameName("goodsBtn1.png");
            tempSp->setNormalImage(sp);
        }
        tempSp = (LYMenuItemSprite*)(m_pSwitchMenu->getChildByTag(STORAGE_CHEST_TAG));
        if (tempSp)
        {
            tempSp->setEnabled(false);
            Sprite* sp = Sprite::createWithSpriteFrameName("goodsBtn2.png");
            tempSp->setNormalImage(sp);
        }
        m_selected = 1;
        
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ENTER_STORAGE) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
        {
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            Vec2 pos = m_pPropVecSprite[0]->convertToWorldSpaceAR(Vec2::ZERO);
            guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
        }
              ReloadData();
    }
    else if(tag == STORAGE_STONE_CHARGE )
    {
        CLayerManager::GetSingletonPtr()->createLayer(eLayer_Charge,this,false);
    }
    else if(tag == STORAGE_COIN_CHARGE)
    {
        CommonPropLayer* pLayer = CommonPropLayer::create(STONE_EXCHANGE_COIN);
        addChild(pLayer, 999);
    }
}

void CPropLayer::ReloadData()
{
    std::vector<Sprite*>::iterator it = m_pPropVecSprite.begin();
    
    for(;it!=m_pPropVecSprite.end();it++)
    {
        (*it)->removeFromParent();
    }
    
    std::vector<MyPropInfo*>  pp;
    m_pPropMap.clear();
    m_UserChest.clear();
    
    int j = 0;

    m_pMyPropAllInfo = CDBMgr::getInstance()->GetUserProp();
    
    if(m_selected==0)
    {
        std::sort(m_pMyPropAllInfo.begin(),m_pMyPropAllInfo.end(),SortByProp);
    }
    
    
    log("MyPropAllInfo = %zi\n",m_pMyPropAllInfo.size());
    
    for(int i=0,k=0;i<m_pMyPropAllInfo.size();i++)
    {
        bool isChest = false;
        if(m_pMyPropAllInfo.at(i)->m_iPropId >= 36)//用物品id来区分
        {
            isChest = true;
            m_UserChest.push_back(m_pMyPropAllInfo.at(i));
        }
        else if(k!=0)
        {
            if((k)%12==0)
            {
                m_pPropMap[j] = pp;
                j++;
                pp.clear();
            }
        }
        if(!isChest){
            k++;
            pp.push_back(m_pMyPropAllInfo[i]);
        }
    }
    
    m_pPropMap[j++] = pp;
    m_pMyPropInfo = m_pPropMap[m_iMapIndex];
    
    m_pPropVecSprite.clear();
    
    if(m_selected == 1)
    {
        m_pMyPropInfo = m_UserChest;
    }
    
    std::vector<MyPropInfo*>::iterator itProp = m_pMyPropInfo.begin();
    
    char buf[128] = {0};
    Size bgSize = m_pMainTaks->getBoundingBox().size;
    
    for(int i=0;itProp!=m_pMyPropInfo.end();i++,itProp++)
    {
        propInfo* pInfo = CDataManager::getPropInfoById(m_pMyPropInfo.at(i)->m_iPropId);
        //道具背景
        sprintf(buf, "%s", iconBg[pInfo->m_iItemLevel - 1].c_str());
        Sprite* propBg = Sprite::createWithSpriteFrameName(buf);
        m_pMainTaks->addChild(propBg,0,100);
        propBg->setScale(2);
        
        propBg->setPosition(bgSize.width*0.1+i%4*bgSize.width*0.14, bgSize.height*0.775-i/4*bgSize.height*0.24);
        
        sprintf(buf, "%s", pInfo->m_sItemIcon.c_str());
        Sprite *pProp = Sprite::createWithSpriteFrameName(buf);
        pProp->setPosition(Vec2(pProp->getContentSize().width / 2, pProp->getContentSize().height / 2));
        propBg->addChild(pProp);
        Size tProp = propBg->getContentSize();
        
        if((m_selected == 0) && (i==m_iSelectIndex))
        {
            Sprite *pSelectKuang = Sprite::createWithSpriteFrameName("selectkuang.png");
            pSelectKuang->setScale(1.8);
            pSelectKuang->setPosition(tProp.width*0.5,tProp.height*0.5);
            propBg->addChild(pSelectKuang,0,101);
        }
        else if((m_selected == 1) && (i==m_iboxSelected))
        {
            Sprite *pSelectKuang = Sprite::createWithSpriteFrameName("selectkuang.png");
            pSelectKuang->setScale(1.8);
            pSelectKuang->setPosition(tProp.width*0.5,tProp.height*0.5);
            propBg->addChild(pSelectKuang,0,101);
        }
        Sprite *pPropNum = Sprite::createWithSpriteFrameName("numbg.png");
        pPropNum->setPosition(tProp.width*0.78,tProp.height*0.2);
        pPropNum->setScale(0.4);
        propBg->addChild(pPropNum);
        
        Size tPropNum = pPropNum->getContentSize();
        char strHaveNum[100] = {0};
        sprintf(strHaveNum,"%d",(*itProp)->m_iPropNum);
        
        Label *pLabelNum = Label::createWithSystemFont(strHaveNum,"Aril",22);
        pLabelNum->setPosition(tPropNum.width*0.5,tPropNum.height*0.5);
        pPropNum->addChild(pLabelNum);
        
        //名字
        Label *pPropName = Label::createWithSystemFont(pInfo->m_sItemName,"Aril",24);
        pPropName->setPosition(tProp.width*0.5,-tProp.height*0.1);
        pPropName->setScale(0.5);
        propBg->addChild(pPropName);
        m_pPropVecSprite.push_back(propBg);
    }
    
    if(m_pMyPropInfo.size()<=0)
    {
        return;
    }
    
    int iPropId=  m_pMyPropInfo[m_iSelectIndex]->m_iPropId;
    if(m_selected == 1)
    {
        iPropId=  m_pMyPropInfo[m_iboxSelected]->m_iPropId;
    }
    propInfo* pInfo = CDBMgr::getInstance()->getPropInfoById(iPropId);
    
    if(pInfo->m_bUse)
    {
        m_pMenuUse->setVisible(true);
        m_pMenuSale->setPosition(m_tDesc.width*0.3,m_tDesc.height*0.24);
    }
    else
    {
        m_pMenuUse->setVisible(false);
    }
    
    Size tSale = m_pSaleMenuItem->getContentSize();
    if(pInfo->m_bUse)
    {
        m_pMenuUse->setVisible(true);
        m_pLabelSale->setPosition(tSale.width*0.5,tSale.height*0.5);
    }
    else
    {
        m_pMenuUse->setVisible(false);
       m_pLabelSale->setPosition(tSale.width*0.5,tSale.height*0.5);
    }
    
    m_pPropNameLabel->setString(pInfo->m_sItemName.c_str());
    m_pPropDescLabel->setString(pInfo->m_sItemDes.c_str());
    
    char strSalePrize[100] = {0};
    sprintf(strSalePrize,"%d",pInfo->m_iSalePrize);
    m_pSaleNum->setString(strSalePrize);
    
    //道具个数
    int iNeedNum = pInfo->m_iNeedPropNum;
    
    //需要一个道具
    int iNeedPropId = pInfo->m_iNeedPropId;
    
    sprintf(strSalePrize, "%s", pInfo->m_sItemIcon.c_str());
    
    m_pNeedSprite->setSpriteFrame(strSalePrize);
    
    int iHaveNum = 0;
    
    iHaveNum = CDBMgr::getInstance()->HavePropNumById(iNeedPropId);
    
    char strNeedNum[100] = {0};
    sprintf(strNeedNum,"/%d",iNeedNum);
    
    char strHaveNum[100] = {0};
    sprintf(strHaveNum,"%d",iHaveNum);
    
    m_pLabelNeedNum->setPosition(m_pLabelHaveNum->getPositionX()+m_pLabelHaveNum->getContentSize().width+m_pLabelNeedNum->getContentSize().width*0.5,m_tDesc.height*0.673);
    
    
    m_pLabelNeedNum->setString(strNeedNum);
    m_pLabelNeedNum->setColor(Color3B::GREEN);
    m_pLabelHaveNum->setString(strHaveNum);
    
    if(iNeedNum>iHaveNum)
    {
        m_pLabelHaveNum->setColor(Color3B::RED);
    }
    else
    {
        m_pLabelHaveNum->setColor(Color3B::GREEN);
    }

    
    if(iNeedNum<=0)
    {
        m_pKeyText->setVisible(false);
        m_pKey->setVisible(false);
        m_pLabelHaveNum->setVisible(false);
        m_pLabelNeedNum->setVisible(false);
    }
    else
    {
        propInfo *p = CDBMgr::getInstance()->getPropInfoById(iNeedPropId);
        char strKey[50] = {0};
        sprintf(strKey,"%s",p->m_sItemIcon.c_str());
        m_pKey->setSpriteFrame(strKey);
        m_pKeyText->setVisible(true);
        m_pKey->setVisible(true);
        m_pLabelHaveNum->setVisible(true);
        m_pLabelNeedNum->setVisible(true);
    }
    
    //总个数
    char strMapIndex[50] = {0};
    if(m_selected == 1)
    {
        int allBoxNum = m_UserChest.size();
        int allPage = 0;
        if(allBoxNum % 12 == 0){
            allPage = allBoxNum/12;
            if(allPage == 0)
            {
                allPage = 1;
            }
        }
        else
        {
            allPage = allBoxNum / 12 + 1;
        }
        sprintf(strMapIndex,"/%d",allPage);
        m_pAllIndex->setString(strMapIndex);
        sprintf(strMapIndex, "%d", m_boxIndex);
        m_pCurIndex->setString(strMapIndex);
    }
    else{
        sprintf(strMapIndex,"/%lu",m_pPropMap.size());
        m_pAllIndex->setString(strMapIndex);
        
        if(m_pPropMap[m_iMapIndex].size()<=0)
        {
            
            OnLeftMenu(NULL);
        }
    }
    updateCoinAndStone();
}

bool CPropLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if(m_pMenuSale)
    {
        m_bMenuSale = m_pMenuSale->onTouchBegan(pTouch,pEvent);
    }
    
    if(m_pMenuUse)
    {
        m_bMenuUse =  m_pMenuUse->onTouchBegan(pTouch,pEvent);
    }
    
    if(m_pMenuSaleAll)
    {
        m_bMenuSaleAll = m_pMenuSaleAll->onTouchBegan(pTouch,pEvent);
    }
    
    if(m_pMenuClose)
    {
        m_bMenuClose = m_pMenuClose->onTouchBegan(pTouch,pEvent);
    }
    
    if(m_pMenuUp)
    {
        m_bMenuUp = m_pMenuUp->onTouchBegan(pTouch,pEvent);
    }
    
    if(m_pMenuNext)
    {
        m_bMenuNext = m_pMenuNext->onTouchBegan(pTouch,pEvent);
    }
    return true;
}


void CPropLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    if(m_bMenuSale)
    {
        m_pMenuSale->onTouchMoved(pTouch,pEvent);
    }
    else if(m_bMenuUse)
    {
        m_pMenuUse->onTouchMoved(pTouch,pEvent);
    }
    else if(m_bMenuSaleAll)
    {
        m_pMenuSaleAll->onTouchMoved(pTouch,pEvent);
    }
    else if(m_bMenuClose)
    {
        m_pMenuClose->onTouchMoved(pTouch,pEvent);
    }
    else if(m_bMenuUp)
    {
        m_pMenuUp->onTouchBegan(pTouch,pEvent);
    }
    
    else if(m_bMenuNext)
    {
        
        m_pMenuNext->onTouchBegan(pTouch,pEvent);
    }
}

void CPropLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if(m_bMenuSale)
    {
        m_pMenuSale->onTouchEnded(pTouch,pEvent);
        return;
    }
    else if(m_bMenuUse)
    {
        m_pMenuUse->onTouchEnded(pTouch,pEvent);
        return;
    }
    else if(m_bMenuSaleAll)
    {
        m_pMenuSaleAll->onTouchEnded(pTouch,pEvent);
        return;
    }
    else if(m_bMenuClose)
    {
        m_pMenuClose->onTouchEnded(pTouch,pEvent);
        return;
    }
    else if(m_bMenuUp)
    {
        m_pMenuUp->onTouchEnded(pTouch,pEvent);
        return;
    }
    
    else if(m_bMenuNext)
    {
        m_pMenuNext->onTouchEnded(pTouch,pEvent);
        return;
    }

    
    if(!m_bMenuSale&&!m_bMenuUse&&!m_bMenuSaleAll)
    {
        cocos2d::Vec2 tTouch = pTouch->getLocation();
    
        log("touchx = %f, touchy= %f\n",tTouch.x,tTouch.y);
        
        
        for(int i=0;i<m_pPropVecSprite.size();i++)
        {
            Rect  pVec = m_pPropVecSprite[i]->boundingBox();
            
            //Vec2(-m_pPropVecSprite[i]->getBoundingBox().size.width*0.5,m_pPropVecSprite[i]->getBoundingBox().size.height*0.5);
            
            pVec.origin = m_pPropVecSprite[i]->convertToWorldSpace(Vec2::ZERO);
            
          
        
            if(pVec.containsPoint(tTouch))
            {
                for(int i=0;i<m_pPropVecSprite.size();i++)
                {
                    Sprite *pProp  = (Sprite *)m_pPropVecSprite[i]->getChildByTag(101);
                    
                    if(pProp)
                    {
                        pProp->removeFromParent();
                    }
                }
                
                SimpleAudioEngine::getInstance()->playEffect("music/click_btn.mp3");
            
                Size tSProp = m_pPropVecSprite[i]->getBoundingBox().size;
                Sprite *pSelectKuang = Sprite::createWithSpriteFrameName("selectkuang.png");
                //pSelectKuang->setAnchorPoint(Vec2(0.5,0.5));
                pSelectKuang->setScale(1.8);
                pSelectKuang->setPosition(tSProp.width*0.25,tSProp.height*0.25);
                m_pPropVecSprite[i]->addChild(pSelectKuang,0,101);
                
                if(m_selected == 1)
                {
                    m_iboxSelected = i;
                }
                else
                {
                    m_iSelectIndex= i;
                }
                if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ENTER_STORAGE) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
                {
                    Vec2 pos = m_pUseMenuItem->convertToWorldSpaceAR(Vec2::ZERO);
                    guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
                    guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
                }
                break;
            }
        }
    }
    
    if(m_pMyPropInfo.size()<=0)
    {
        return;
    }
    int iPropId=  m_pMyPropInfo[m_iSelectIndex]->m_iPropId;
    
    if(m_selected == 1)
    {
        iPropId=  m_pMyPropInfo[m_iboxSelected]->m_iPropId;
    }
    
    //log("iPropId = %d",iPropId);
    
    propInfo* pInfo = CDBMgr::getInstance()->getPropInfoById(iPropId);
    int iNeedPropId = 1;
    
    if(pInfo)
    {
        iNeedPropId = pInfo->m_iNeedPropId;
    }
    
    //道具个数
    m_iNeedNum = pInfo->m_iNeedPropNum;
    
    m_iHaveNum = CDBMgr::getInstance()->HavePropNumById(iNeedPropId);
    
    
    char strHaveNum[100] = {0};
    sprintf(strHaveNum,"%d",m_iHaveNum);
    
    char strNeedNum[100] = {0};
    sprintf(strNeedNum,"/%d",m_iNeedNum);
    
    if(m_iNeedNum<=0)
    {
        m_pKeyText->setVisible(false);
        m_pKey->setVisible(false);
        m_pLabelHaveNum->setVisible(false);
        m_pLabelNeedNum->setVisible(false);
    }
    else
    {
        propInfo *p = CDBMgr::getInstance()->getPropInfoById(iNeedPropId);
        char strKey[50] = {0};
        sprintf(strKey,"%s",p->m_sItemIcon.c_str());
        
        m_pKey->setSpriteFrame(strKey);
        m_pKeyText->setVisible(true);
        m_pKey->setVisible(true);
        
        m_pLabelHaveNum->setVisible(true);
        m_pLabelNeedNum->setVisible(true);
    }
    
    m_pLabelNeedNum->setString(strNeedNum);
    m_pLabelHaveNum->setString(strHaveNum);
    
    if(m_iNeedNum>m_iHaveNum)
    {
        m_pLabelHaveNum->setColor(Color3B::RED);
    }
    else
    {
        m_pLabelHaveNum->setColor(Color3B::GREEN);
    }
    
    Size tSale = m_pSaleMenuItem->getContentSize();
    
    char strSalePrize[100] = {0};
    sprintf(strSalePrize,"%d",pInfo->m_iSalePrize);
    m_pSaleNum->setString(strSalePrize);
    
    if(pInfo->m_bUse)
    {
        m_pMenuUse->setVisible(true);
        //m_pMenuSale->setPosition(m_tDesc.width*0.5,m_tDesc.height*0.24);
        m_pLabelSale->setPosition(tSale.width*0.5,tSale.height*0.5);
    }
    else
    {
        m_pMenuUse->setVisible(false);
        m_pLabelSale->setPosition(tSale.width*0.5,tSale.height*0.5);
        //m_pMenuSale->setPosition(m_tDesc.width*0.3,m_tDesc.height*0.24);
    }
    
    m_pPropDescLabel->setString(pInfo->m_sItemDes);
    m_pPropNameLabel->setString(pInfo->m_sItemName);
    sprintf(strSalePrize, "%s", pInfo->m_sItemIcon.c_str());
    m_pNeedSprite->setSpriteFrame(strSalePrize);
}


void CPropLayer::onTouchCancelled(Touch *touch, Event *pEvent)
{
    if(m_bMenuSale)
    {
        m_pMenuSale->onTouchCancelled(touch,pEvent);
    }
    else if(m_bMenuUse)
    {
        m_pMenuUse->onTouchCancelled(touch,pEvent);
    }
    else if(m_bMenuSaleAll)
    {
        m_pMenuSaleAll->onTouchCancelled(touch,pEvent);
    }
    else if(m_bMenuClose)
    {
        m_pMenuClose->onTouchCancelled(touch,pEvent);
    }
}


void CPropLayer::onExit()
{
    //道具保存
    _eventDispatcher->removeEventListener(m_pListner);
    CBaseLayer::onExit();
    NotificationCenter::getInstance()->removeObserver(this, "storageGuide");
}

void CPropLayer::OnSaleAllProp(Ref *pSender)
{
    OnEraseIt(m_iSaleAllPropId,0);
    ReloadData();
    
}


void CPropLayer::OnSaleProp(Ref *pSender)
{
     m_pMyPropInfo[m_iSaleIndexId]->m_iPropNum--;
    
     if(m_pMyPropInfo[m_iSaleIndexId]->m_iPropNum <=0)
     {
         OnEraseIt(m_iSalePropId);
     }
    else
    {
        CDataManager::SaveUserPropToDB(m_iSalePropId, m_pMyPropInfo[m_iSaleIndexId]->m_iPropNum);
    }
    
    


    ReloadData();

}


void CPropLayer::OnUserProp(Ref *pSender)
{
    
    ReloadData();
    
    m_pMenuSale->setEnabled(true);
    m_pMenuSaleAll->setEnabled(true);
    m_pMenuUse->setEnabled(true);
    
}



void CPropLayer::OnSaleAllCallBack(Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect("music/sale.mp3");
    //增加金钱
    std::vector<Sprite*>::iterator it = m_pPropVecSprite.begin();
    std::vector<MyPropInfo*>::iterator itPro = m_pMyPropInfo.begin();
    
    for(int i=0;it!=m_pPropVecSprite.end();it++,i++,itPro++)
    {
        if(i== m_iSelectIndex)
        {
            int iPropId=  m_pMyPropInfo[i]->m_iPropId;
            propInfo* pInfo = CDBMgr::getInstance()->getPropInfoById(iPropId);
            int iSalePrize = pInfo->m_iSalePrize;
            
            //增加钱
            //userInfoInDB* userInfo = CDataManager::getTheUSerInfDB();
            
            
            m_iSaleAllPropId = iPropId;
            
            CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(CPropLayer::OnSaleAllProp),iSalePrize*m_pMyPropInfo[i]->m_iPropNum,0,0);
            
            //CDataManager::saveUserStoneAndCoinToDB(iSalePrize*m_pMyPropInfo[i]->m_iPropNum, 0,true);
            
            {
                //m_pMyPropInfo.erase(itPro);
               // OnEraseIt(iPropId,0);
            }

            break;
        }
    }
    //ReloadData();
}


void CPropLayer::OnEraseIt(int iPropId,int iNum)
{
    std::vector<MyPropInfo*>::iterator it = m_pMyPropAllInfo.begin();
    
    for(;it!=m_pMyPropAllInfo.end();it++)
    {
        if((*it)->m_iPropId == iPropId)
        {
            if(iNum ==0 )
            {
                CDataManager::SaveUserPropToDB(iPropId,0);
            }
            else
            {
                (*it)->m_iPropNum--;
                 CDataManager::SaveUserPropToDB(iPropId,(*it)->m_iPropNum);
            }
            break;
        }
    }
    
}

void CPropLayer::OnSaleCallBack(Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect("music/sale.mp3");
    //是
    log("on sale");
    
    //增加金钱
    std::vector<Sprite*>::iterator it = m_pPropVecSprite.begin();
    std::vector<MyPropInfo*>::iterator itPro = m_pMyPropInfo.begin();
    
    for(int i=0;it!=m_pPropVecSprite.end();it++,i++,itPro++)
    {
        if(i== m_iSelectIndex)
        {
            int iPropId=  m_pMyPropInfo[i]->m_iPropId;
            propInfo* pInfo = CDBMgr::getInstance()->getPropInfoById(iPropId);
            int iSalePrize = pInfo->m_iSalePrize;
            
            //增加钱
//            userInfoInDB* userInfo = CDataManager::getTheUSerInfDB();
            
            int num = CPersonalData::getInstance()->getUserCoin();
            num+=iSalePrize;
            CPersonalData::getInstance()->setUserCoin(num);
            
            m_iSalePropId = iPropId;
            
            m_iSaleIndexId = i;
            
         
            
            //CDataManager::saveUserStoneAndCoinToDB(iSalePrize,0,true);
            
            CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(CPropLayer::OnSaleProp),iSalePrize,0,0);
    
            /*
                m_pMyPropInfo[i]->m_iPropNum--;
            if(m_pMyPropInfo[i]->m_iPropNum <=0)
            {
                OnEraseIt((*itPro)->m_iPropId);
            }*/
            
            //CDataManager::SaveUserPropToDB(iPropId,m_pMyPropInfo[i]->m_iPropNum);
            break;
        }
    }
    ReloadData();
}


void CPropLayer::OnSaleMenu(Ref *pSender)
{
    log("saleMenu");
    
    if(m_pMyPropInfo.size()<=0 || ((guiderManagerLayer::getInstance()->getStepNum() >= 6)
                                   && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ENTER_STORAGE)))
    {
        return;
    }
    //是否确定卖出
    if(m_selected == 1)
    {
        m_iSelectIndex = m_iboxSelected;
    }
    
    
    CommonPropLayer *pDlg = CommonPropLayer::create(SALE_PROP_TYPE, m_pMyPropInfo[m_iSelectIndex]->m_iPropId,true);
    
    //增加金钱
    std::vector<Sprite*>::iterator it = m_pPropVecSprite.begin();
    std::vector<MyPropInfo*>::iterator itPro = m_pMyPropInfo.begin();
    
    propInfo* pInfo = NULL;
    int iAllPrize = 0;
    
    for(int i=0;it!=m_pPropVecSprite.end();it++,i++,itPro++)
    {
        if(i== m_iSelectIndex)
        {
            int iPropId=  m_pMyPropInfo[i]->m_iPropId;
            pInfo = CDBMgr::getInstance()->getPropInfoById(iPropId);
            iAllPrize = pInfo->m_iSalePrize;
            break;
        }
    }
    
    if(pInfo==NULL)
    {
        return ;
    }
    
    
    iAllPrize = iAllPrize;
    char strText[100] = {0};
    sprintf(strText,"是否卖出1个%s,获金%d?",pInfo->m_sItemName.c_str(),iAllPrize);
    pDlg->salePropDialog(this ,menu_selector(CPropLayer::OnSaleCallBack),strText);
    addChild(pDlg,111);
    
    ReloadData();
}



void CPropLayer::OnSaleAllMenu(Ref *pSender)
{
    
    if(m_pMyPropInfo.size()<=0 || ((guiderManagerLayer::getInstance()->getStepNum() >= 6)
                                   && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ENTER_STORAGE)))
    {
        return;
    }
    
    
    if(m_selected == 1)
    {
        m_iSelectIndex = m_iboxSelected;
    }
    CommonPropLayer *pDlg = CommonPropLayer::create(SALE_PROP_TYPE, m_pMyPropInfo[m_iSelectIndex]->m_iPropId,true);
    
    //增加金钱
    std::vector<Sprite*>::iterator it = m_pPropVecSprite.begin();
    std::vector<MyPropInfo*>::iterator itPro = m_pMyPropInfo.begin();
    
    propInfo* pInfo = NULL;
    
    int iAllPrize = 0;
    
    for(int i=0;it!=m_pPropVecSprite.end();it++,i++,itPro++)
    {
        if(i== m_iSelectIndex)
        {
            int iPropId=  m_pMyPropInfo[i]->m_iPropId;
            pInfo = CDBMgr::getInstance()->getPropInfoById(iPropId);
            iAllPrize = pInfo->m_iSalePrize;
            break;
        }
    }
    if(pInfo==NULL)
    {
        return ;
    }
    
    int iNum = (*itPro)->m_iPropNum;
    iAllPrize = iAllPrize*iNum;
    char strText[100] = {0};
    sprintf(strText,"是否全部卖出%s道具%d个",pInfo->m_sItemName.c_str(),iNum);
    pDlg->salePropDialog(this ,menu_selector(CPropLayer::OnSaleAllCallBack),strText, iNum);
    addChild(pDlg,100);
    ReloadData();
    log("saleMenuall");
}


void CPropLayer::OnRemoveNotice()
{
    Label *  p =  (Label *)getChildByTag(1000);
    p->removeFromParent();
}


void CPropLayer::OnUseMenu(Ref *pSender)
{
    log("useMenu");
    
    //打开添加道具
    
    //36木质宝箱,39木质钥匙
    
    if(m_pMyPropInfo.size()<=0 || ((guiderManagerLayer::getInstance()->getStepNum() >= 6)
                                   && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ENTER_STORAGE)))
    {
        return ;
    }
    
    int iIndex = 0;
    
    iIndex = m_iSelectIndex;
    
    int iPropId=  m_pMyPropInfo[m_iSelectIndex]->m_iPropId;
    
    if(m_selected == 1)
    {
        iIndex = m_iboxSelected;
        iPropId=  m_pMyPropInfo[m_iboxSelected]->m_iPropId;
    }
    
    log("iPropId = %d",iPropId);
    
    propInfo* pInfo = CDBMgr::getInstance()->getPropInfoById(iPropId);
    int iNeedPropId = 1;
    
    if(pInfo)
    {
        iNeedPropId = pInfo->m_iNeedPropId;
    }
    
    //道具个数
    m_iNeedNum = pInfo->m_iNeedPropNum;
    
    m_iHaveNum = CDBMgr::getInstance()->HavePropNumById(iNeedPropId);
    
    if(m_iNeedNum>m_iHaveNum)
    {
        SimpleAudioEngine::getInstance()->playEffect("music/noBtn.mp3");
        Sprite * thetarget = CFightManager::getTheFlyWord("打开材料不够哟!!","fonts/cuti.ttf");
        thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
        addChild(thetarget, 100);
        return;
    }
    
    propInfo* PInfoNow  = CDataManager::getPropInfoById(iPropId);
    
    int iKeyNum =  CDBMgr::getInstance()->HavePropNumById(PInfoNow->m_iNeedPropId);
    
    if((iKeyNum-m_iNeedNum)<=0)
    {
        iKeyNum = m_iNeedNum;
    }
    
    OnEraseIt(PInfoNow->m_iNeedPropId,iKeyNum-m_iNeedNum);

    std::vector<PropOpenInfo*>  pOpenInfo = CDBMgr::getInstance()->GetPropOpenInfo();
    PropOpenInfo * pInfoOpen = CDataManager::getPropOpenInfoById(iPropId);
    
    //自己道具个数减1
    m_pMyPropInfo[iIndex]->m_iPropNum--;
    //保存开启黄金宝箱次数
    if(PInfoNow->m_itemId == 38)
    {
        CDataManager::saveTaskInfoToDB(12);
    }
    
    if(m_pMyPropInfo[iIndex]->m_iPropNum<=0)
    {
        std::vector<Sprite*>::iterator it = m_pPropVecSprite.begin();
        std::vector<MyPropInfo*>::iterator itPro = m_pMyPropInfo.begin();
        
        for(int i=0;itPro!=m_pMyPropInfo.end();it++,i++,itPro++)
        {
            if((*itPro)->m_iPropId == iPropId)
            {
                //m_pMyPropInfo.erase(itPro);
                OnEraseIt(iPropId);
        
                
                if(m_selected == 1)
                {
                    m_iboxSelected = 0;
                }
                else
                {
                    m_iSelectIndex = 0;
                }
                break;
            }
        }
    }
    else
    {
        CDataManager::SaveUserPropToDB(iPropId, m_pMyPropInfo[iIndex]->m_iPropNum);
    }
    
    //拥有需要
    {
        m_iHaveNum--;
        
        if(m_iHaveNum<0)
        {
            m_iHaveNum = 0;
        }
        char strHaveNum[100] = {0};
        sprintf(strHaveNum,"%d",m_iHaveNum);
        m_pLabelHaveNum->setString(strHaveNum);
        if(m_iHaveNum<m_iNeedNum)
        {
            m_pLabelHaveNum->setColor(Color3B::RED);
        }
    }
    
    //增加道具
    int id = pInfoOpen->m_iGotItem;
    
    if(id == -1)
    {
        int iNum = 1;
        //CDataManager::saveUserStoneAndCoinToDB(iNum,0,1);
        
        
        
        m_pMenuSale->setEnabled(false);
        m_pMenuSaleAll->setEnabled(false);
        m_pMenuUse->setEnabled(false);
        
       CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(CPropLayer::OnUserProp),iNum,0,0);
        
        
        CommonPropLayer* pLayer = CommonPropLayer::create(USER_CHEST_TYPE,-1,1,iNum);
        addChild(pLayer,100);
        
    }
    else if(id==0)
    {
        int iNum = pInfoOpen->m_iGotMoney;
       // CDataManager::saveUserStoneAndCoinToDB(iNum,0,1);
        CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(CPropLayer::OnUserProp),iNum,0,0);
        
        CommonPropLayer* pLayer = CommonPropLayer::create(USER_CHEST_TYPE,0,1,iNum);
        addChild(pLayer,100);
    }
    else
    {
        propInfo* PInfo  = CDataManager::getPropInfoById(id);
        
        int iType = pInfoOpen->m_itemType;
        
        //0为道具 1为碎片
        if(iType == 1)
        {
            //CDataManager::saveUserDebrisToDB(id,1,0);
        }
        
        int iItemNum = pInfoOpen->m_iGotItemNum;
        
        bool bNoHaveProp = true;

        char strText[100] = {0};
        
        if(iType == 0)
        {
            sprintf(strText,"恭喜您,获得%d个%s道具",iItemNum,PInfo->m_sItemName.c_str());
        }
        else
        {
            sprintf(strText,"恭喜您,获得%d个%s碎片",iItemNum,PInfo->m_sItemName.c_str());
        }
        
        int iNum = CDBMgr::getInstance()->HavePropNumById(id);
        CDataManager::SaveUserPropToDB(id,iNum+iItemNum);
        CommonPropLayer* pLayer = CommonPropLayer::create(USER_CHEST_TYPE, id,iItemNum,iItemNum);
        addChild(pLayer,100);
    }
    
    ReloadData();
    
}

void CPropLayer::addCloseGuide(Ref* pSender)
{
    NotificationCenter::getInstance()->removeObserver(this, "storageGuide");
    LYMenuItemSprite* closeItem = (LYMenuItemSprite*)m_pMenuClose->getChildByTag(10);
    Vec2 pos = closeItem->convertToWorldSpaceAR(Vec2::ZERO);
    guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
}

void CPropLayer::OnLeftMenu(Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect("music/back.mp3");
    log("leftmenu");
    
    if(m_pPropMap.size()<=0)
    {
        return;
    }
    
    if(m_iMapIndex<=0)
    {
        return ;
    }
    
    m_iMapIndex--;
    
    char strMapIndex[50] = {0};
    sprintf(strMapIndex,"%d",m_iMapIndex+1);
    m_pCurIndex->setString(strMapIndex);
    
    m_pMyPropInfo =  m_pPropMap[m_iMapIndex];
    
    ReloadData();
}

void CPropLayer::OnRightMenu(Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect("music/next.mp3");
    log("Rightmenu");
    
    if(m_pPropMap.size()<=0)
    {
        return;
    }
    if(m_iMapIndex>=m_pPropMap.size()-1)
    {
        return ;
    }
    
    m_iMapIndex++;
    char strMapIndex[50] = {0};
    sprintf(strMapIndex,"%d",m_iMapIndex+1);
    m_pCurIndex->setString(strMapIndex);
    m_pMyPropInfo =  m_pPropMap[m_iMapIndex];
    
    ReloadData();
}


void CPropLayer::OnExitLvLayer(Ref *pSender)
{
    CPersonalData::getInstance()->setUpdateUserInfo(true);
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ENTER_STORAGE)
       && (guiderManagerLayer::getInstance()->getStepNum() == 6))
    {
        CPersonalData::getInstance()->setisOpenNewLevel(true);
        guiderManagerLayer::getInstance()->setStepNum(0);
        guiderManagerLayer::getInstance()->removeGuiderLayer();
    }
    CPersonalData::getInstance()->setrestartPlayMusic(false);
    removeFromParent();
}