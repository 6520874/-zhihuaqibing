//
//  LYMenuItemSprite.h
//  game1
//
//  Created by user on 13-8-20.
//
//

#ifndef __game1__LYMenuItemSprite__
#define __game1__LYMenuItemSprite__

#include <iostream>
#include "cocos2d.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

class LYMenuItemSprite:public MenuItemSprite {
    
public:
    virtual void activate();
    
    /** creates a menu item with a normal,selected  and disabled image with target/selector */
    static LYMenuItemSprite * create(Node* normalSprite, Node* selectedSprite, const ccMenuCallback& callback, std::string soundName = "music/click_btn.mp3");
    static LYMenuItemSprite * create(Node* normalSprite, Node* selectedSprite, Node* disabledSprite, const ccMenuCallback& callback, std::string soundName = "music/click_btn.mp3");
    
    
    int m_iLevelId;
    void SetLevelId(int iId){m_iLevelId = iId;}
    int GetLevelId(){return m_iLevelId;}

public:
    std::string PlaySoundName;
};

#endif /* defined(__game1__LYMenuItemSprite__) */
