//
//  SkillUpSucessLayer.cpp
//  WarOfGod
//
//  Created by prefer_shan on 15/3/26.
//
//

#include "SkillUpSucessLayer.h"
#include "DataManager.h"
#include "LvUpDetail.h"
#include "LYFightManager.h"

CSKillUpSucessLayer::CSKillUpSucessLayer(std::string strSkillName,std::string strImg,bool bHero)
{
    m_winSize = Director::getInstance()->getVisibleSize();
    m_bHero = bHero;
    m_strSKillName = strSkillName;
    m_strImg = strImg;
    
}


CSKillUpSucessLayer::~CSKillUpSucessLayer()
{
   
}

void CSKillUpSucessLayer::onExit()
{
    _eventDispatcher->removeEventListener(m_plistener);
    Layer::onExit();
}


bool CSKillUpSucessLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

void CSKillUpSucessLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{

}

void CSKillUpSucessLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{

}

void CSKillUpSucessLayer::onTouchCancelled(Touch *touch, Event *event)
{

    
}

bool CSKillUpSucessLayer::init()
{

    m_plistener = EventListenerTouchOneByOne::create();//创建一个触摸监听
    m_plistener->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false
    
    m_plistener->onTouchBegan = CC_CALLBACK_2(CSKillUpSucessLayer::onTouchBegan, this);
    m_plistener->onTouchMoved = CC_CALLBACK_2(CSKillUpSucessLayer::onTouchMoved, this);
    m_plistener->onTouchEnded = CC_CALLBACK_2(CSKillUpSucessLayer::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_plistener,this);
    
    LayerColor *pBlackColor = LayerColor::create();
    pBlackColor->setColor(Color3B::BLACK);
    pBlackColor->setAnchorPoint(Vec2(0,0));
    pBlackColor->setPosition(Vec2(0, 0));
    pBlackColor->setOpacity(255*0.8);
    addChild(pBlackColor,300,900);
    
    Sprite *pLvUpSucessLayer = Sprite::createWithSpriteFrameName("skillupsucess1.png");
    
     pLvUpSucessLayer->setScale(3);
    Animate* theAnimate = CFightManager::getAnimateWithPath("skillupsucess", 1, 12);
    
     Animate* theAnimate1 = CFightManager::getAnimateWithPath("skillupsucess", 5, 8);
    
    pLvUpSucessLayer->runAction(Sequence::create(theAnimate ,theAnimate1, NULL));

    
    pLvUpSucessLayer->setPosition(Point(m_winSize.width / 2, m_winSize.height*0.8));
    //pLvUpSucessLayer->setScale(2);
    addChild(pLvUpSucessLayer,302,901);
    
    Sprite *pMid = Sprite::createWithSpriteFrameName("dilight.png");
    pMid->setPosition(Point(m_winSize.width/2,m_winSize.height*0.5));
    addChild(pMid,304,904);
    
    pMid->setScale(0);
    
    Size  tMid = pMid->getContentSize();
    
    char strIcoPath[50] = {0};
    sprintf(strIcoPath,"%s",m_strImg.c_str());
    
    Sprite *pHero = NULL;
    
     if(m_bHero)
     {
         pHero = CDataManager::getSkillIcoWithId(m_strImg,1);
     }
     else
     {
         pHero = CDataManager::getSkillIcoWithId(m_strImg,0.6);
     }
    pHero->setScale(0.7);
    pHero->setPosition(tMid.width*0.5,tMid.height*0.5);
    pMid->addChild(pHero);

    
    Label *pSkillName = Label::createWithSystemFont(m_strSKillName,"Aril",30);
    pSkillName->setColor(Color3B::YELLOW);
    pSkillName->setScale(0.25);
    pSkillName->setPosition(tMid.width*0.5,tMid.height*0.05);
    pMid->addChild(pSkillName);
    
    CallFunc* getOff = CallFunc::create(CC_CALLBACK_0(CSKillUpSucessLayer::OnRemoveAll, this));
    pMid->runAction(Sequence::create(CCScaleTo::create(0.5f,4),NULL));
    runAction(Sequence::create(DelayTime::create(2.5f),getOff, NULL));
    SimpleAudioEngine::getInstance()->playEffect("music/lvlSucess.mp3");
    return true;
}


void CSKillUpSucessLayer::OnRemoveAll()
{
    Label *p4 = (Label*)getChildByTag(1000);
    
    if(p4)
    {
        p4->removeFromParent();
    }
    
    Sprite  *  p =  (Sprite *)getChildByTag(901);
    if(p)
    {
        p->removeFromParent();
    }
    
    Sprite  *  p1 =  (Sprite *)getChildByTag(902);
    if(p1)
    {
        p1->removeFromParent();
    }
    
    Sprite  *  p2 =  (Sprite *)getChildByTag(903);
    if(p2)
    {
        p2->removeFromParent();
    }
    
    Sprite *p3 = (Sprite*)getChildByTag(900);
    if(p3)
    {
        p3->removeFromParent();
    }
    
    Sprite *p5= (Sprite*)getChildByTag(904);
    if(p5)
    {
        p5->removeFromParent();
    }
    
    LvUpDetail *pLayer = (LvUpDetail*)getParent()->getParent();
    if(pLayer)
    {
        
        if(pLayer->m_pSkillUpLayer)
        {
            log("asdasadsads\n");
            Sprite *p = (Sprite *)pLayer->getChildByTag(101);
            if(p)
            {
                p->setVisible(true);
            }

            
        }
        else
        {
            Sprite *p = (Sprite *)pLayer->getChildByTag(101);
            if(p)
            {
                p->setVisible(true);
            }
        }
    }
    
    removeFromParent();
    NotificationCenter::getInstance()->postNotification("upgradeGuide");
}





