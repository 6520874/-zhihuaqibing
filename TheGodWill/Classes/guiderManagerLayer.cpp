//
//  guiderManagerLayer.cpp
//  TheGodWill
//
//  Created by user on 15/1/28.
//
//

#include "guiderManagerLayer.h"

guiderManagerLayer* guiderManagerLayer::m_singleGuide = nullptr;//m_singleton指向空指针

guiderManagerLayer::guiderManagerLayer()
{
    m_curGuiderLayer = NULL;
    //m_state = GUIDE_START_GAME;
    m_bStart = false;
    
}

guiderManagerLayer::~guiderManagerLayer()
{
    
}

guiderManagerLayer* guiderManagerLayer::getInstance()
{
    if (!m_singleGuide) {
        m_singleGuide = new guiderManagerLayer();
        m_singleGuide->initGuideAnimation();
    }
    return m_singleGuide;
}

void guiderManagerLayer::initGuideAnimation()
{
    m_iType = GUIDE_ADD_LINE;
    m_iStep = 1;
    m_bStart = false;
    m_bNextGuide = false;
}

void guiderManagerLayer::removeGuiderLayer()
{
    if(m_curGuiderLayer)
    {
        m_curGuiderLayer->removeFromParent();
        m_curGuiderLayer = NULL;
    }
}

void guiderManagerLayer::createLayer(Node* parent, GuideType type, Vec2 pos,bool ifShadow)
{
    removeGuiderLayer();
    m_curGuiderLayer = guiderLayer::create(type, pos,ifShadow);
    parent->addChild(m_curGuiderLayer, GUIDE_TAG);
    //m_curGuiderLayer->runAction(Sequence::create(FadeOut::create(0),FadeIn::create(3), NULL));
    //m_curGuiderLayer->runAction(Sequence::create(DelayTime::create(0.5f), Show::create(), NULL));
//    m_curGuiderLayer->setOpacity(0);
//    m_curGuiderLayer->runAction(FadeTo::create(0.65f, 255));
}