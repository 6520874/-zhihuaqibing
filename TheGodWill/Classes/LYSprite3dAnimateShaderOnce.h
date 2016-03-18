//
//  LYSprite3dAnimateShaderOnce.h
//  WarOfGod
//
//  Created by user on 15/7/13.
//
//

#ifndef __WarOfGod__LYSprite3dAnimateShaderOnce__
#define __WarOfGod__LYSprite3dAnimateShaderOnce__

#include <stdio.h>

#include "cocos2d.h"
#include "3d/CCAnimation3D.h"
#include "3d/CCAnimate3D.h"
#include "3d/CCMesh.h"
#include "Math/Vec2.h"


class CSprite3dAnimateShaderOnce : public cocos2d::Node
{
public:
    
    CREATE_FUNC(CSprite3dAnimateShaderOnce);
    
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4 &transform, uint32_t flags);
    
    void setTarget(cocos2d::Sprite3D* target,std::string shaderFilePath,std::string glProgramKey);
    
//    void setFadeState(int temp){fadeState = temp;};
    
protected:
    
    cocos2d::Sprite3D*		m_Sprite;
    
    cocos2d::Vec2			m_LightAni;
    
    
//    int fadeState;
    
    CC_SYNTHESIZE(int, fadeState, FadeState);
    CC_SYNTHESIZE(float, _shining_duraion, ShiningDuration);
//    float _shining_duraion;
    
    cocos2d::GLProgramState * m_pProgramstate;
    
    void cylinderUpdate(float dt);
}
;


#endif /* defined(__WarOfGod__LYSprite3dAnimateShaderOnce__) */
