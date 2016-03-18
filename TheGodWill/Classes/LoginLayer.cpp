//
//  LoginLayer.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/12/19.
//
//

#include "LoginLayer.h"
#include "LoginIntro.h"
#include "sysTime.h"


CLoginLayer::CLoginLayer()
{
    m_pListener = NULL;
    m_pBig = NULL;
    m_pBig2=  NULL;
    m_pBig3 = NULL;
    m_pTextInfo = NULL;
    m_iIndex = 0;
    m_iAutorIndex =0;
    m_lAutorTime = 0;
    m_iChangeState = 0;
}

CLoginLayer::~CLoginLayer()
{
    
}


cocos2d::Scene*  CLoginLayer::createScene()
{
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = CLoginLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);

    
    return scene;
}


bool CLoginLayer::onTouchBegan(Touch *touch, Event *event)
{
    
    unscheduleUpdate();
    m_pTextAuthor->setVisible(true);
    m_pTextAuthor->setString(m_sAuthor);
    m_pTextInfo->setString(m_strText);
    
    
    log("%d,%d,%d",m_pBig->getOpacity(),m_pBig2->getOpacity(),m_pBig3->getOpacity());
    
    
    
//    if(m_pBig2->getOpacity()<10&&m_pBig3->getOpacity()<10)
//    {
//        m_pBig->setOpacity(0);
//        m_pBig2->setOpacity(255);
//        m_pBig->stopAllActions();
//        m_pBig2->stopAllActions();
//    
//    }
//    else if(m_pBig->getOpacity()<10&&m_pBig2->getOpacity()>200&&m_pBig3->getOpacity()<10)
//    {
//        m_pBig2->setOpacity(0);
//        m_pBig3->setOpacity(255);
//        m_pBig3->stopAllActions();
//        b3 = true;
//        
//    }
    
    
    if (m_iChangeState==0) {
        
        m_iChangeState=1;
        m_pBig->setOpacity(0);
        m_pBig2->setOpacity(255);
        m_pBig->stopAllActions();
        m_pBig2->stopAllActions();
        
    }else if(m_iChangeState==1) {
        
        m_iChangeState=2;
        
        m_pBig2->setOpacity(0);
        m_pBig3->setOpacity(255);
        m_pBig2->stopAllActions();
        m_pBig3->stopAllActions();
        
    }else if(m_iChangeState==2)
    {
        m_iChangeState=3;
        CLayerManager::GetSingletonPtr()->createLayer(eLayer_LoginSec, NULL,true);
    }
    
    
    
    
    
    
    
    return  true;
}


void CLoginLayer::onTouchMoved(Touch *touch, Event *event)
{
    
}

void CLoginLayer::onTouchEnded(Touch *touch, Event *event)
{
    
}

void CLoginLayer::onTouchCancelled(Touch *touch, Event *event)
{
    
}

bool CLoginLayer::init()
{
    CBaseLayer::initWithColor(Color4B(26,11,24, 255));
    
    m_pListener = EventListenerTouchOneByOne::create();
    m_pListener->setSwallowTouches(true);
    m_pListener->onTouchBegan = CC_CALLBACK_2(CLoginLayer::onTouchBegan, this);
    m_pListener->onTouchMoved = CC_CALLBACK_2(CLoginLayer::onTouchMoved, this);
    m_pListener->onTouchEnded = CC_CALLBACK_2(CLoginLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener,this);
    
    m_pBig = Sprite::create("login/login_d1.png");
    m_pBig->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(m_pBig);
    
    
    m_pBig2 = Sprite::create("login/login_d2.png");
    m_pBig2->setOpacity(0);
    m_pBig2->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(m_pBig2);
    
    m_pBig3 = Sprite::create("login/login_d3.png");
    m_pBig3->setOpacity(0);
    m_pBig3->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(m_pBig3);
    
    Size tBg = m_pBig->getBoundingBox().size;
    
    
    m_lBeginTime = 0;
    
    m_pBig->runAction(Sequence::create(DelayTime::create(2.5),FadeOut::create(0.5),CallFunc::create([=](){
        
        m_iChangeState = 1;
    }),  NULL));
    
    m_pBig2->runAction(Sequence::create(DelayTime::create(3.5),FadeIn::create(1),DelayTime::create(2.5),FadeOut::create(0.5),CallFunc::create([=](){
        
        m_iChangeState = 2;
    }),NULL));
    
    m_pBig3->runAction(Sequence::create(DelayTime::create(8),FadeIn::create(1),NULL));
    
    //1024X196
    
    LayerColor *pLayer = LayerColor::create(Color4B(0,0,0,255*0.6),m_winSize.width,196);

    pLayer->setPosition(0,-m_winSize.height*0.025);
    addChild(pLayer,1);
    
    
    m_strText = "那是一个众神消逝的时代，那是一片危机四伏的大陆。很难想象，如果没有他，相比其他种族而言如此弱小的人类，到底会如何惨淡的结束。是时代造就了英雄，还是英雄造就了时代？\n";
    
    m_sAuthor = "出自《唯一魔导召唤师的一生》 王国卫队长卡尔 著";
    
    Size tbg = pLayer->getBoundingBox().size;
    

    m_pTextInfo = Label::createWithTTF(m_strText.substr(0,1),"fonts/cuti.ttf",30,Size(tbg.width*0.9,0));
    m_pTextInfo->setAnchorPoint(Vec2(0.5,1));
    
    m_pTextInfo->setColor(Color3B(205,196,161));
    m_pTextInfo->setPosition(tbg.width*0.5,tbg.height*0.85);
    pLayer->addChild(m_pTextInfo);

    //,Size(tbg.width*0.9,0)
    m_pTextAuthor = Label::createWithTTF(m_sAuthor,"fonts/cuti.ttf",25);
    m_pTextAuthor->setColor(Color3B(205,196,161));
    m_pTextAuthor->setAnchorPoint(Vec2(0.5,1));
    
    //-tbg.width*0.05
    m_pTextAuthor->setPosition(m_winSize.width-m_pTextAuthor->getContentSize().width*0.5-tbg.width*0.05,m_winSize.height*0.05);
    
    m_pTextAuthor->setVisible(false);
    
    addChild(m_pTextAuthor,10);
    
    scheduleUpdate();
    
    
    return true;
}


void CLoginLayer::OnFadeOut()
{

    m_pBig->removeFromParent();
    

    m_pBig2 = Sprite::create("login/login_d2.png");
    
    m_pBig2->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    
    m_pBig2->setOpacity(0);
    
    addChild(m_pBig2);
    
    
    CallFunc* getOff = CallFunc::create(CC_CALLBACK_0(CLoginLayer::OnFadeIn, this));
    
    m_pBig2->runAction(Sequence::create(FadeIn::create(3),getOff,NULL));
    
    
}

void CLoginLayer::OnFadeIn()
{

    
}


void CLoginLayer::update(float ft)
{
    m_lBeginTime += ft;

    if(m_iIndex>= m_strText.length())
    {
        m_pTextAuthor->setVisible(true);

        m_lAutorTime+= ft;
         m_iAutorIndex = m_lAutorTime/2+m_sAuthor.length();
        
        std::string str = m_sAuthor.substr(0,m_iAutorIndex);
        m_pTextAuthor->setString(str);
        //m_iAutorIndex ++;
    }
    else
    {
        m_iIndex = (float)m_lBeginTime/9*m_strText.length();

        std::string str = m_strText.substr(0,m_iIndex);
        
        m_pTextInfo->setString(str);
    }
  
    

    
    
}

/*
void CLoginLayer::UpdateTime1(float ft)
{
    m_iIndex++;
    
    std::string str = m_strText.substr(0,m_iIndex);
    
    m_pTextInfo->setString(str);
    
}*/

void CLoginLayer::UpdateTime2(float ft)
{
    
}

void CLoginLayer::onExit()
{
    _eventDispatcher->removeEventListener(m_pListener);
    CBaseLayer::onExit();
}


