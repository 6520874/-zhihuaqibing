//
//  CEvolutionLayer.cpp
//  WarOfGod
//
//  Created by user on 15/8/20.
//
//

#include "evolutionLayer.h"

CEvolutionLayer::CEvolutionLayer()
{
    m_pListen = NULL;
    m_bEvolutionEnd = false;
}

CEvolutionLayer::~CEvolutionLayer()
{
    _eventDispatcher->removeEventListener(m_pListen);
}

bool CEvolutionLayer::init()
{
    if(!CBaseLayer::initWithColor(Color4B(0, 0, 0, 200)))
    {
        return false;
    }
    
    m_pListen = EventListenerTouchOneByOne::create();
    m_pListen->setSwallowTouches(true);
    m_pListen->onTouchBegan = CC_CALLBACK_2(CEvolutionLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListen, this);
    
    m_winWize = Director::getInstance()->getWinSize();
    char buf[256];
    Sprite* pNode = Sprite::createWithSpriteFrameName("pvp_evolution_1.png");
    addChild(pNode);
    pNode->setScale(2.0f);
    pNode->setPosition(Vec2(m_winWize.width * 0.5f, m_winWize.height * 0.65f));
    Animation* pEvolution = Animation::create();
    for(int i = 1; i < 13; i++)
    {
        sprintf(buf, "pvp_evolution_%d.png", i);
        pEvolution->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
    }
    pEvolution->setDelayPerUnit(0.2f);
    ActionInterval* pAction = Animate::create(pEvolution);
    pNode->runAction(Sequence::create(pAction, CallFunc::create([=]{
        m_bEvolutionEnd = true;
    }), DelayTime::create(0.5f), CallFunc::create([=]{
        removeFromParent();
    }), NULL));
    return true;
}

bool CEvolutionLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
    if(m_bEvolutionEnd)
    {
        removeFromParent();
    }
    return true;
}