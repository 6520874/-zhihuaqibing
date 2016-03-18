//
//  storyManagerLayer.h
//  WarOfGod
//
//  Created by user on 15/4/13.
//
//

#ifndef __WarOfGod__storyManagerLayer__
#define __WarOfGod__storyManagerLayer__

#include <stdio.h>
#include "cocos2d.h"

#include "gameStoryLayer.h"
USING_NS_CC;

class storyManagerLayer
{
public:
    storyManagerLayer();
    ~storyManagerLayer();
    
    static storyManagerLayer* getInstance();
    static storyManagerLayer* m_singleStory;
    void createStoryLayer(Node* parent);
    void removeGameStoryLayer();
    gameStoryLayer* m_curLayer;
    CC_SYNTHESIZE(storyState, m_iType, storyType);//剧情类型
    CC_SYNTHESIZE(int, m_iStep, StepNum);//剧情类型的步骤
    static int m_layNum;
};


#endif /* defined(__WarOfGod__storyManagerLayer__) */
