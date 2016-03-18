//
//  gameStoryLayer.h
//  WarOfGod
//
//  Created by user on 15/4/13.
//
//

#ifndef __WarOfGod__gameStoryLayer__
#define __WarOfGod__gameStoryLayer__

#include <stdio.h>
#include "BaseLayer.h"
#include "AllDefine.h"
#include "DataManager.h"

class gameStoryLayer:public CBaseLayer
{
public:
    gameStoryLayer();
    ~gameStoryLayer();
    static gameStoryLayer* create();
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    bool init();
    
    void initUI();
private:
    cocos2d::Size m_winSize;
    EventListenerTouchOneByOne* listener;
    storyState m_type;
    std::vector<gameStory> m_storyInfo;
    LayerColor* m_back;
    Sprite* m_girl;
    Label* m_dialogue;
    Label* m_name;
    bool m_bRomoveFlag;
};


#endif /* defined(__WarOfGod__gameStoryLayer__) */
