//
//  storyManagerLayer.cpp
//  WarOfGod
//
//  Created by user on 15/4/13.
//
//

#include "storyManagerLayer.h"

int storyManagerLayer::m_layNum = 0;
storyManagerLayer* storyManagerLayer::m_singleStory = nullptr;

storyManagerLayer::storyManagerLayer()
{
    m_curLayer = NULL;
}

storyManagerLayer::~storyManagerLayer()
{
    
}

storyManagerLayer* storyManagerLayer::getInstance()
{
    if (!m_singleStory) {
        m_singleStory = new storyManagerLayer();
    }
    return m_singleStory;
}

void storyManagerLayer::removeGameStoryLayer()
{
    if(m_curLayer)
    {
        m_curLayer->removeFromParent();
        m_curLayer = NULL;
    }
}

void storyManagerLayer::createStoryLayer(Node* parent)
{
    if(m_curLayer)
    {
        removeGameStoryLayer();
    }
    m_layNum++;
    m_curLayer = gameStoryLayer::create();
    parent->addChild(m_curLayer, 99999);
}