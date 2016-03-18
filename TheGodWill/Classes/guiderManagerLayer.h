//
//  guiderManagerLayer.h
//  TheGodWill
//
//  Created by user on 15/1/28.
//
//

#ifndef __TheGodWill__guiderManagerLayer__
#define __TheGodWill__guiderManagerLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "guiderLayer.h"
USING_NS_CC;

#define GUIDE_TAG 99999

class guiderManagerLayer
{
public:
    guiderManagerLayer();
    ~guiderManagerLayer();
    static guiderManagerLayer* getInstance();
    void initGuideAnimation();
    //void initGuiderUI();
    void createLayer(Node* parent, GuideType type = GUIDE_SHOW_DIALOG, Vec2 pos = Vec2::ZERO,bool ifShadow =true);
    void removeGuiderLayer();
    
private:
    cocos2d::Size m_winSize;
    static guiderManagerLayer* m_singleGuide;
    GuideState m_state;
    
    CC_SYNTHESIZE(guiderLayer*, m_curGuiderLayer, GuiderLayer);
    CC_SYNTHESIZE(GuideState, m_iType, GuideType);//记录引导类型
    CC_SYNTHESIZE(int, m_iStep, StepNum);//记录引导类型的步骤
    
    CC_SYNTHESIZE(bool, m_bStart, start);//标识是否开始游戏
    CC_SYNTHESIZE(bool, m_bNextGuide, NextGuide);//标识是否开始下一个类型的引导
    Vec2 m_pos;//目前用于记录塔防模式引导的手指位置
};

#endif /* defined(__TheGodWill__guiderManagerLayer__) */
