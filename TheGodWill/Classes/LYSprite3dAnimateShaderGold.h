//
//  LYSprite3dAnimateShaderGold.h
//  WarOfGod
//
//  Created by user on 15/7/10.
//
//

#ifndef __WarOfGod__LYSprite3dAnimateShaderGold__
#define __WarOfGod__LYSprite3dAnimateShaderGold__

#include <stdio.h>

#include "cocos2d.h"
#include "3d/CCAnimation3D.h"
#include "3d/CCAnimate3D.h"
#include "3d/CCMesh.h"
#include "Math/Vec2.h"



class CSprite3dAnimateShaderGold : public cocos2d::Node
{
public:

    CREATE_FUNC(CSprite3dAnimateShaderGold);

    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4 &transform, uint32_t flags);
    
    void setTarget(cocos2d::Sprite3D* target);
    
protected:
    
    void cylinderUpdate(float dt);
    

    cocos2d::Sprite3D*		m_Sprite;

    cocos2d::Vec2			m_LightAni;
    
    int fadeState;
    float _shining_duraion;
    
    cocos2d::GLProgramState * glprogramstate;
}
;

#endif /* defined(__WarOfGod__LYSprite3dAnimateShaderGold__) */
