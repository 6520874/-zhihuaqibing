//
//  getNewHeroLayer.cpp
//  TheGodWill
//
//  Created by user on 15/2/11.
//
//

#include "getNewHeroLayer.h"
#include "DataManager.h"
#include "LYFightData.h"
#include "guiderManagerLayer.h"
#include "FightWinLayer.h"

CGetNewHeroLayer::CGetNewHeroLayer()
{
    m_monsterId = 1;
    listen = NULL;
}

CGetNewHeroLayer::~CGetNewHeroLayer()
{
    
}

CGetNewHeroLayer* CGetNewHeroLayer::create(int monsterId)
{
    CGetNewHeroLayer* pRet = new CGetNewHeroLayer();
    pRet->m_monsterId = monsterId;
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else{
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool CGetNewHeroLayer::init()
{
    if(!CBaseLayer::initWithColor(Color4B(0, 0, 0, 150)))
    {
        return false;
    }
    m_winSize = Director::getInstance()->getWinSize();
    
    listen = EventListenerTouchOneByOne::create();
    listen->setSwallowTouches(true);
    listen->onTouchBegan = CC_CALLBACK_2(CGetNewHeroLayer::onTouchBegan, this);
    listen->onTouchEnded = CC_CALLBACK_2(CGetNewHeroLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);
    initUI();
    return true;
}

void CGetNewHeroLayer::onExit()
{
    _eventDispatcher->removeEventListener(listen);
    CBaseLayer::onExit();
}

void CGetNewHeroLayer::initUI()
{
    SimpleAudioEngine::getInstance()->playEffect("music/lvlSucess.mp3");
    monsterInfo* pInfo = CDataManager::getMonsterInfoByMonsterId(m_monsterId);
    //光
    Sprite* light = Sprite::createWithSpriteFrameName("backlight3.png");
    addChild(light);
    light->setScale(4.5);
    light->setPosition(Vec2(m_winSize.width / 2, m_winSize.height * 0.5f));
    light->runAction(RepeatForever::create(RotateBy::create(2.5, 90)));
    
    Sprite* advanceBg = Sprite::createWithSpriteFrameName("getHero_bg.png");
    addChild(advanceBg);
    advanceBg->setScale(3.0f);
    advanceBg->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
    Size adSize = Size(advanceBg->getContentSize().width * 0.6f, advanceBg->getContentSize().height * 0.6f);
    
    Sprite* word = Sprite::createWithSpriteFrameName("unlock.png");
    addChild(word);
    word->setScale(2.8f);
    word->setPosition(Vec2(m_winSize.width * 0.62, m_winSize.height * 0.695));
    
    //英雄
    modelInfo* pModelInfo = CDataManager::getModelInfoByModelId(pInfo->modelId);
    Sprite3D* m_hero = Sprite3D::create(pModelInfo->modelName.getCString());
    advanceBg->addChild(m_hero);
    m_hero->setGlobalZOrder(100);
    Size modeSize = m_hero->getBoundingBox().size;
    
    
    float scale = adSize.width/modeSize.width;
    
    
    if(scale > adSize.height/modeSize.height)
    {
        scale = adSize.height/modeSize.height;
    }
    m_hero->setScale(scale*4);
    
    m_hero->setPosition(Vec2(advanceBg->getContentSize().width / 2, advanceBg->getContentSize().height * 0.23f));
    auto animation = Animation3D::create(pModelInfo->modelName.getCString());
    if (animation)
    {
        auto animate = Animate3D::create(animation, pModelInfo->idleFrame.start/60, 1/60);
        cocos2d::Action* _swim = RepeatForever::create(animate);
        m_hero->runAction(_swim);
    }
    
    //暂停3秒之后自动关闭
    CallFunc* act = CallFunc::create(CC_CALLBACK_0(CGetNewHeroLayer::removeSelf, this));
    runAction(Sequence::create(DelayTime::create(4.0f), act, NULL));
}

void CGetNewHeroLayer::removeSelf()
{
    CFightWinLayer* parent = (CFightWinLayer*)getParent();
    parent->addShowGuide();
    removeFromParent();
}

bool CGetNewHeroLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

void CGetNewHeroLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
    CallFunc* act = CallFunc::create(CC_CALLBACK_0(CGetNewHeroLayer::removeSelf, this));
    runAction(Sequence::create(DelayTime::create(0.25f), act, NULL));
}