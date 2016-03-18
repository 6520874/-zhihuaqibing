//
//  LYSprite3dShaderWhite.h
//  WarOfGod
//
//  Created by user on 15/7/10.
//
//

#ifndef __WarOfGod__LYSprite3dShaderWhite__
#define __WarOfGod__LYSprite3dShaderWhite__

#include <stdio.h>

#include "cocos2d.h"


class CSprite3dShaderWhite : public cocos2d::Node{
    
public:
    
    CREATE_FUNC(CSprite3dShaderWhite);
    CSprite3dShaderWhite();
    virtual ~CSprite3dShaderWhite();
    
    void setTarget(cocos2d::Sprite3D* target);
    
//    void setFadeState(int temp){fadeState = temp;};
    
protected:
    
    void cylinderUpdate(float dt);
    
    float _cylinder_texture_offset;
    
    CC_SYNTHESIZE(float, _shining_duraion, ShiningDuration);
    
    cocos2d::GLProgramState * _state;
    
//    int fadeState;
    
    CC_SYNTHESIZE(int, fadeState, FadeState);
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//    cocos2d::EventListenerCustom* _backToForegroundListener;
//#endif
    
private:
    
//    bool m_bFateState;

    
};


#endif /* defined(__WarOfGod__LYSprite3dShaderWhite__) */
