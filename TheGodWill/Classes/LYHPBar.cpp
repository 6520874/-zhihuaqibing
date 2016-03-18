//
//  LYHPBar.cpp
//  TheGodWill
//
//  Created by user on 14/12/3.
//
//

#include "LYHPBar.h"

USING_NS_CC;

void CHPBar::setHPSpriteWithPath(const char *HPName,const char *HPBGName){
    
    
    m_pNowHP = Sprite::createWithSpriteFrameName(HPName);
    m_pNowHP->setAnchorPoint(Vec2(0, 0.5f));
    
    m_pNowHP->setPosition(-m_pNowHP->getBoundingBox().size.width*0.5f, 0);
    
//    m_pNowHP = ProgressTimer::create(hpSprite);
//    m_pNowHP->setType(ProgressTimer::Type::BAR);
//    m_pNowHP->setPercentage(100);
//    
//    
//    m_pNowHP->setMidpoint(Point(0, 0.5));
//
//    m_pNowHP->setBarChangeRate(Point(1, 0));
    
    m_pBGHP = Sprite::createWithSpriteFrameName(HPBGName);
    
    
//    LayerColor* BlackBG = LayerColor::create(Color4B(0,0,0,100), m_pNowHP->getBoundingBox().size.width, m_pNowHP->getBoundingBox().size.height);
//    BlackBG->setPosition(-m_pNowHP->getBoundingBox().size.width*0.5f, -m_pNowHP->getBoundingBox().size.height*0.5f);
    
    
    
    addChild(m_pBGHP,4);
    addChild(m_pNowHP,2);
//    addChild(BlackBG,1);
    
    m_pNowHP->setScale(1);
    
    
    
//    m_pNowHP->setGlobalZOrder(999);
//    m_pBGHP->setGlobalZOrder(999);
//    BlackBG->setGlobalZOrder(999);
    
    setVisible(false);
}

void CHPBar::toHPScale(float scale,float time){
    
    
    
    m_pNowHP->setScale(scale/100,1);
    
    if(scale == 0)
    {
        setVisible(false);
        return;
    }
    else
    {
        setVisible(true);
        Sequence *visibleAction =Sequence::create(DelayTime::create(5),Hide::create(), NULL);
        visibleAction->setTag(VISIBLE_ACTION_TAG);
        
        stopActionByTag(VISIBLE_ACTION_TAG);
        runAction(visibleAction);
    }
}