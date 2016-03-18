//
//  costStoneWndLayer.cpp
//  WarOfGod
//
//  Created by user on 15/5/29.
//
//

#include "costStoneWndLayer.h"

costStoneCommonLayer::costStoneCommonLayer()
{
}

costStoneCommonLayer::~costStoneCommonLayer()
{
    
}

bool costStoneCommonLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

bool costStoneCommonLayer::init()
{
    if(!CBaseLayer::initWithColor(Color4B(0, 0, 0, 180)))
    {
        return false;
    }
    
    listen = EventListenerTouchOneByOne::create();
    listen->setSwallowTouches(true);
    listen->onTouchBegan = CC_CALLBACK_2(costStoneCommonLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);
    m_winSize = Director::getInstance()->getWinSize();
    return true;
}

void costStoneCommonLayer::initWndInfo(Ref* target, SEL_MenuHandler hander, std::string content, int num)
{
    Sprite* m_bg = Sprite::createWithSpriteFrameName("fight_bg.png");
    addChild(m_bg);
    m_bg->setScaleY(0.7f);
    m_bg->setScaleX(0.8f);
    Size bgSize = m_bg->getBoundingBox().size;
    m_bg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.5f));
    
    LayerColor* pLayer = LayerColor::create(Color4B(0, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(pLayer);
    pLayer->setPosition(Vec2(m_bg->getPositionX() - bgSize.width * 0.5f, m_bg->getPositionY() - bgSize.height * 0.5f));
    
    Label* pDesc = Label::createWithTTF(content.c_str(), "fonts/cuti.ttf", 30);
    pLayer->addChild(pDesc);
    pDesc->setWidth(370);
    pDesc->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.65f));
    
    std::string tmp = "是否花费";
    std::string::size_type startPos = content.find(tmp);
    
    if(startPos != std::string::npos)
    {
        auto tempStrLabel = Label::createWithTTF(tmp.c_str(), "fonts/cuti.ttf", 30);
        int npos = tempStrLabel->getStringLength();
        startPos += npos;
        
        char buf[128];
        sprintf(buf, "%d", num);
        auto tempNum = Label::createWithTTF(buf, "fonts/cuti.ttf", 30);
        int numLen = tempNum->getStringLength();
        for(int i = npos;i < npos + numLen; i++)
        {
            auto BChar = (Sprite*) pDesc->getLetter(i);
            if(BChar)
            {
                BChar->setColor(Color3B(4, 174, 255));
            }
        }
    }
    
    
    

    
    
    Menu* m_menu = Menu::create();
    pLayer->addChild(m_menu);
    m_menu->setPosition(Vec2::ZERO);
    Sprite* normal = Sprite::createWithSpriteFrameName("redbtn.png");
    Sprite* select = Sprite::createWithSpriteFrameName("redbtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* cancelItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(costStoneCommonLayer::onCancenBtn, this));
    cancelItem->setScale(2.6f);
    cancelItem->setPosition(Vec2(bgSize.width * 0.25, bgSize.height * 0.2));
    m_menu->addChild(cancelItem);
    Label* cancel = Label::createWithSystemFont("取消", "Arial", 32);
    cancelItem->addChild(cancel);
    cancel->setScale(0.35f);
    cancel->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height / 2));
    
    normal = Sprite::createWithSpriteFrameName("greenbtn.png");
    select = Sprite::createWithSpriteFrameName("greenbtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* sureItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(costStoneCommonLayer::onSureBtn, this));
    m_menu->addChild(sureItem);
    sureItem->setScale(2.6f);
    sureItem->setPosition(Vec2(bgSize.width * 0.75, bgSize.height * 0.2));
    Label* sure = Label::createWithSystemFont("确定", "Arial", 32);
    sureItem->addChild(sure);
    sure->setScale(0.35f);
    sure->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height / 2));
    m_pHandler = hander;
    m_pTarget = target;
}

void costStoneCommonLayer::onSureBtn(Ref* pSender)
{
    if(m_pHandler && m_pTarget)
    {
        (m_pTarget->*m_pHandler)(pSender);
    }
    removeFromParent();
}

void costStoneCommonLayer::onCancenBtn(Ref* pSender)
{
    removeFromParent();
}







