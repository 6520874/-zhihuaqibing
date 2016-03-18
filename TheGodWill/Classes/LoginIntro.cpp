//
//  LoginIntro.cpp
//  WarOfGod
//
//  Created by prefer_shan on 15/4/14.
//
//

#include "LoginIntro.h"


CLoginIntro::CLoginIntro()
{
    m_pBeNameLayer = NULL;
    m_pListener = NULL;
    m_iIndex = 0;
    m_fBeginTime = 0;
    
}

CLoginIntro::~CLoginIntro()
{

}
void CLoginIntro::onExit()
{
    _eventDispatcher->removeEventListener(m_pListener);
    CBaseLayer::onExit();
}


cocos2d::Scene* CLoginIntro::createScene()
{
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = CLoginIntro::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    return scene;
}

bool CLoginIntro::onTouchBegan(Touch *touch, Event *event)
{
    
    if(m_pBeNameLayer)
    {
        
    }
    else
    {
        m_pBeNameLayer = CBeNameLayer::create(0);
        addChild(m_pBeNameLayer,2);
    }
    
    return  true;
}

void CLoginIntro::onTouchMoved(Touch *touch, Event *event)
{

}

void CLoginIntro::onTouchEnded(Touch *touch, Event *event)
{
    
}

void CLoginIntro::onTouchCancelled(Touch *touch, Event *event)
{
    
}

void CLoginIntro::update(float ft)
{
    
    m_fBeginTime +=ft;
    
    m_iIndex = m_fBeginTime/3*m_strText.length();
    
    m_pTextInfo->setString(m_strText.substr(0,m_iIndex));
}


bool CLoginIntro::init()
{

    CBaseLayer::init();
    CBaseLayer::initWithColor(Color4B(26,11,24, 255));
    
    m_pListener = EventListenerTouchOneByOne::create();
    m_pListener->setSwallowTouches(true);
    m_pListener->onTouchBegan = CC_CALLBACK_2(CLoginIntro::onTouchBegan, this);
    m_pListener->onTouchMoved = CC_CALLBACK_2(CLoginIntro::onTouchMoved, this);
    m_pListener->onTouchEnded = CC_CALLBACK_2(CLoginIntro::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener,this);

    Sprite *pBig1 = Sprite::create("login/login_d4.png");
    
    pBig1->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(pBig1);
    
    Size tBg = pBig1->getBoundingBox().size;

    
    LayerColor *pLayer = LayerColor::create(Color4B(0,0,0,255*0.6),m_winSize.width,196);
    pLayer->setPosition(0,-m_winSize.height*0.025);
    addChild(pLayer,2);

    
    Size tLayer = pLayer->getBoundingBox().size;
    
    m_strText = "召唤师峡谷是培养召唤师的摇篮，这里曾经走出过许多强大的召唤师。对大多数人来说，今天不过是普通的一天，而对于某一部分人来说，却是相当不普通的。因为他们。。毕业了。";
    
    m_pTextInfo = Label::createWithTTF("", "fonts/cuti.ttf",30,Size(tLayer.width*0.9,0));
    m_pTextInfo->setAnchorPoint(Vec2(0.5,1));
    m_pTextInfo->setColor(Color3B(205,196,161));
    m_pTextInfo->setPosition(tLayer.width*0.5,tLayer.height*0.85);
    pLayer->addChild(m_pTextInfo);

    scheduleUpdate();
    
    
    return true;
}



