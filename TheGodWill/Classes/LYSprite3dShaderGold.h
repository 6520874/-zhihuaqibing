//
//  LYSprite3dShaderGold.h
//  WarOfGod
//
//  Created by user on 15/7/10.
//
//

#ifndef __WarOfGod__LYSprite3dShaderGold__
#define __WarOfGod__LYSprite3dShaderGold__

#include <stdio.h>

#include "cocos2d.h"

class CSprite3dShaderGold:public cocos2d::Node {
    
public:
    
    CREATE_FUNC(CSprite3dShaderGold);
    CSprite3dShaderGold();
    virtual ~CSprite3dShaderGold();
    
    void setTarget(cocos2d::Sprite3D* target);
    
    void cylinderUpdate(float dt);
    
    float _cylinder_texture_offset;
    float _shining_duraion;
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    cocos2d::EventListenerCustom* _backToForegroundListener;
#endif
    
    cocos2d::GLProgramState * _state;
    
private:
    
    bool m_bFateState;
    
};





#endif /* defined(__WarOfGod__LYSprite3dShaderGold__) */
