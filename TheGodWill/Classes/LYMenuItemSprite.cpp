//
//  LYMenuItemSprite.cpp
//  game1
//
//  Created by user on 13-8-20.
//
//

#include "LYMenuItemSprite.h"
#include "LYPersonalData.h"
using namespace cocos2d;

void LYMenuItemSprite::activate(){
    
    CCMenuItemSprite::activate();

    if(UserDefault::getInstance()->getBoolForKey("effectMusic", true))
    {
        SimpleAudioEngine::getInstance()->setEffectsVolume(1);
    }
    else{
        SimpleAudioEngine::getInstance()->setEffectsVolume(0);
    }
    if (PlaySoundName != "")
    {
        int effectId = SimpleAudioEngine::getInstance()->playEffect(PlaySoundName.c_str());
        CPersonalData::getInstance()->setEffectId(effectId);
    }
}

//LYMenuItemSprite * LYMenuItemSprite::create(Node *normalSprite, Node *selectedSprite, Node *disabledSprite, const ccMenuCallback& callback)
//{
//    LYMenuItemSprite *pRet = new LYMenuItemSprite();
//    pRet->initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, callback);
//    pRet->autorelease();
//    return pRet;
//}

LYMenuItemSprite * LYMenuItemSprite::create(Node* normalSprite, Node* selectedSprite, const ccMenuCallback& callback, std::string soundName)
{
    LYMenuItemSprite *pRet = new LYMenuItemSprite();
    pRet->initWithNormalSprite(normalSprite, selectedSprite, NULL, callback);
    pRet->autorelease();
    pRet->PlaySoundName = soundName;
    return pRet;
}

LYMenuItemSprite * LYMenuItemSprite::create(Node* normalSprite, Node* selectedSprite, Node* disabledSprite, const ccMenuCallback& callback, std::string soundName)
{
    LYMenuItemSprite *pRet = new LYMenuItemSprite();
    pRet->initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, callback);
    pRet->autorelease();
    pRet->PlaySoundName = soundName;
    return pRet;
}
