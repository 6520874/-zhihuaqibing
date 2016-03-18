//
//  systemSetLayer.cpp
//  TheGodWill
//
//  Created by user on 15/2/16.
//
//

#include "systemSetLayer.h"
#include "LYPersonalData.h"
#include "mainMenuLayer.h"
#include "Common.h"

CSystemSetLayer::CSystemSetLayer()
{
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plist/set.plist");
}

CSystemSetLayer::~CSystemSetLayer()
{
    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("plist/set.plist");
}

bool CSystemSetLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

bool CSystemSetLayer::init()
{
    if (!CBaseLayer::initWithColor(Color4B(0, 0, 0, 200))) {
        return false;
    }
    
    m_winSize = Director::getInstance()->getWinSize();
    
    auto listen = EventListenerTouchOneByOne::create();
    listen->setSwallowTouches(true);
    listen->onTouchBegan = CC_CALLBACK_2(CSystemSetLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);
    initUI();
    return true;
}

void CSystemSetLayer::initUI()
{
    Sprite* bg = Sprite::createWithSpriteFrameName("common_bg.png");
    addChild(bg);
    bg->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
    Size bgSize = bg->getContentSize();
    
    Label* pTitle = Label::createWithSystemFont("设置", "Arial", 30);
    bg->addChild(pTitle);
    pTitle->setPosition(Vec2(bgSize.width / 2, bgSize.height * 0.89f));
    
    //音效
    Sprite* voice = Sprite::createWithSpriteFrameName("voice.png");
    bg->addChild(voice);
    voice->setPosition(Vec2(bgSize.width * 0.22f, bgSize.height * 0.6f));
    
    std::string str = "notOpen.png";
    if(UserDefault::getInstance()->getBoolForKey("effectMusic", true))
    {
        str = "opened.png";
    }
    Sprite* normal = Sprite::createWithSpriteFrameName(str.c_str());
    Sprite* select = Sprite::createWithSpriteFrameName(str.c_str());
    LYMenuItemSprite* pVoiceItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CSystemSetLayer::btnCallBack, this));
    Menu* pVoiceMenu = Menu::create(pVoiceItem, NULL);
    pVoiceItem->setTag(2);
    bg->addChild(pVoiceMenu);
    pVoiceMenu->setPosition(Vec2(bgSize.width * 0.34f, bgSize.height * 0.61f));
    
    //反馈意见
//    Sprite* advice = Sprite::createWithSpriteFrameName("advice.png");
//    bg->addChild(advice);
//    advice->setPosition(Vec2(bgSize.width * 0.22f, bgSize.height * 0.32f));
//    Label* pAdvice = Label::createWithSystemFont("反馈意见", "Arial", 30);
//    bg->addChild(pAdvice);
//    pAdvice->setPosition(Vec2(bgSize.width * 0.33, bgSize.height * 0.32f));
    
    
    //背景音乐
    Sprite* music = Sprite::createWithSpriteFrameName("music.png");
    bg->addChild(music);
    music->setPosition(Vec2(bgSize.width * 0.67f, bgSize.height * 0.6f));
    
    str = "notOpen.png";
    if(UserDefault::getInstance()->getBoolForKey("bgMusic", true))
    {
        str = "opened.png";
    }
    normal = Sprite::createWithSpriteFrameName(str.c_str());
    select = Sprite::createWithSpriteFrameName(str.c_str());
    LYMenuItemSprite* pMusicItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CSystemSetLayer::btnCallBack, this));
    Menu* pMusicMenu = Menu::create(pMusicItem, NULL);
    pMusicItem->setTag(3);
    bg->addChild(pMusicMenu);
    pMusicMenu->setPosition(Vec2(bgSize.width * 0.785f, bgSize.height * 0.61f));
    
    //官方联系方式
    Label* service = Label::createWithTTF("官方qq群:", "fonts/cuti.ttf", 30);
    bg->addChild(service);
    service->setAnchorPoint(Vec2(0, 0.5));
    service->setColor(Color3B::YELLOW);
    service->setPosition(Vec2(bgSize.width * 0.18f, bgSize.height * 0.32f));
    
    Label* pTel = Label::createWithSystemFont("152768414", "Arial", 30);
    //pTel->setColor(Color3B(103,206,71));
    bg->addChild(pTel);
    pTel->setAnchorPoint(Vec2(0, 0.5));
    pTel->setPosition(Vec2(service->getPositionX() + service->getContentSize().width + 10, service->getPositionY()));
    
    normal = Sprite::createWithSpriteFrameName("closeBtn.png");
    select = Sprite::createWithSpriteFrameName("closeBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pExitItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CSystemSetLayer::btnCallBack, this));
    CommonData::enlargeItemSprite(pExitItem, 2);
    Menu* pSExitMenu = Menu::create(pExitItem, NULL);
    bg->addChild(pSExitMenu);
    pExitItem->setTag(1);
    pSExitMenu->setPosition(Vec2(bgSize.width * 0.955, bgSize.height * 0.88));
}

void CSystemSetLayer::btnCallBack(Ref* Psender)
{
    int tag = ((Node*)Psender)->getTag();
    if(tag == 1)
    {
        removeFromParent();
        CPersonalData::getInstance()->setrestartPlayMusic(false);
    }
    else if(tag == 2)
    {
        LYMenuItemSprite* menuItem = (LYMenuItemSprite*)Psender;
        Sprite* sp = Sprite::createWithSpriteFrameName("opened.png");
        if(UserDefault::getInstance()->getBoolForKey("effectMusic", true))
        {
            sp = Sprite::createWithSpriteFrameName("notOpen.png");
            UserDefault::getInstance()->setBoolForKey("effectMusic", false);
        }
        else{
            UserDefault::getInstance()->setBoolForKey("effectMusic", true);
        }
        if(menuItem)
        {
            menuItem->setNormalImage(sp);
            menuItem->setSelectedImage(sp);
        }
    }
    else if(tag == 3)
    {
        LYMenuItemSprite* menuItem = (LYMenuItemSprite*)Psender;
        Sprite* sp = Sprite::createWithSpriteFrameName("opened.png");
        if(UserDefault::getInstance()->getBoolForKey("bgMusic", true))
        {
            sp = Sprite::createWithSpriteFrameName("notOpen.png");
            UserDefault::getInstance()->setBoolForKey("bgMusic", false);
        }
        else{
            UserDefault::getInstance()->setBoolForKey("bgMusic", true);
        }
        if(menuItem)
        {
            menuItem->setNormalImage(sp);
            menuItem->setSelectedImage(sp);
        }
        CMainMenuLayer* parent = (CMainMenuLayer*)getParent();
        if(parent)
        {
            parent->updateMusicState();
        }
    }

}
