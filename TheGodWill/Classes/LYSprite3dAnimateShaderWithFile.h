//
//  LYSprite3dAnimateShaderWithFile.h
//  WarOfGod
//
//  Created by user on 15/7/13.
//
//

#ifndef __WarOfGod__LYSprite3dAnimateShaderWithFile__
#define __WarOfGod__LYSprite3dAnimateShaderWithFile__

#include <stdio.h>

#include "cocos2d.h"
#include "3d/CCAnimation3D.h"
#include "3d/CCAnimate3D.h"
#include "3d/CCMesh.h"
#include "Math/Vec2.h"


class CSprite3dAnimateShaderWithFile : public cocos2d::Node
{
public:
    
    CREATE_FUNC(CSprite3dAnimateShaderWithFile);
    
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4 &transform, uint32_t flags);
    
    void setTarget(cocos2d::Sprite3D* target,std::string shaderFilePath,std::string glProgramKey);
    
protected:
    
    cocos2d::Sprite3D*		m_Sprite;
    
    cocos2d::Vec2			m_LightAni;
    
    cocos2d::GLProgramState * glprogramstate;
    
    bool m_bFateState;
    
    float _shining_duraion;
    
    cocos2d::GLProgramState * m_pProgramstate;
    
    void cylinderUpdate(float dt);
}
;


#endif /* defined(__WarOfGod__LYSprite3dAnimateShaderWithFile__) */
