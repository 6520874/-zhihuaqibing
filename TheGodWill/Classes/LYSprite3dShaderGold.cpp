//
//  LYSprite3dShaderGold.cpp
//  WarOfGod
//
//  Created by user on 15/7/10.
//
//

#include "LYSprite3dShaderGold.h"
using namespace cocos2d;



CSprite3dShaderGold::CSprite3dShaderGold(){
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    _backToForegroundListener = NULL;
#endif
    
    
}

CSprite3dShaderGold::~CSprite3dShaderGold(){
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    if (_backToForegroundListener) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(_backToForegroundListener);
    }
#endif
    
}


void CSprite3dShaderGold::setTarget(cocos2d::Sprite3D *target){

    
//    _cylinder_texture_offset = 0;
    _shining_duraion = 0;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //use custom camera
    //    auto camera = Camera::createPerspective(60, visibleSize.width/visibleSize.height, 0.1f, 200);
    //    camera->setCameraFlag(CameraFlag::USER1);
    
    //create cylinder
    auto cylinder = target;
    
    //create and set our custom shader
    auto shader =GLProgram::createWithFilenames("Sprite3DTest/cylinder.vert","Sprite3DTest/fadeColor.frag");
    _state = GLProgramState::create(shader);
    cylinder->setGLProgramState(_state);
    
//    _state->setUniformFloat("offset",_cylinder_texture_offset);
    _state->setUniformFloat("duration",_shining_duraion);
    //pass mesh's attribute to shader.
    long offset = 0;
    auto attributeCount = cylinder->getMesh()->getMeshVertexAttribCount();
    for (auto i = 0; i < attributeCount; i++) {
        auto meshattribute = cylinder->getMesh()->getMeshVertexAttribute(i);
        _state->setVertexAttribPointer(s_attributeNames[meshattribute.vertexAttrib],
                                       meshattribute.size,
                                       meshattribute.type,
                                       GL_FALSE,
                                       cylinder->getMesh()->getVertexSizeInBytes(),
                                       (GLvoid*)offset);
        offset += meshattribute.attribSizeBytes;
    }
    
    //create the second texture for cylinder
    auto shining_texture = Director::getInstance()->getTextureCache()->addImage("Sprite3DTest/gold.png");
    Texture2D::TexParams tRepeatParams;//set texture parameters
    tRepeatParams.magFilter = GL_NEAREST;
    tRepeatParams.minFilter = GL_NEAREST;
    tRepeatParams.wrapS = GL_REPEAT;
    tRepeatParams.wrapT = GL_REPEAT;
    shining_texture->setTexParameters(tRepeatParams);
    //pass the texture sampler to our custom shader
    _state->setUniformTexture("caustics",shining_texture);
    
    
    this->setCameraMask(target->getCameraMask());
    //    this->addChild(camera);
    
    //adjust cylinder's position & rotation
    //    cylinder->setPosition3D(Vec3(0,-15,-50));
    //    cylinder->setRotation3D(Vec3(-90,0,0));
    
    //the callback function update cylinder's texcoord
    schedule(schedule_selector(CSprite3dShaderGold::cylinderUpdate));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    _backToForegroundListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
                                                            [this](EventCustom*)
                                                            {
                                                                auto glProgram = _state->getGLProgram();
                                                                glProgram->reset();
                                                                glProgram->initWithFilenames("Sprite3DTest/cylinder.vert", "Sprite3DTest/fadeColor.frag");
                                                                glProgram->link();
                                                                glProgram->updateUniforms();
                                                                auto shining_texture = Director::getInstance()->getTextureCache()->addImage("Sprite3DTest/gold.png");
                                                                Texture2D::TexParams tRepeatParams;//set texture parameters
                                                                tRepeatParams.magFilter = GL_NEAREST;
                                                                tRepeatParams.minFilter = GL_NEAREST;
                                                                tRepeatParams.wrapS = GL_REPEAT;
                                                                tRepeatParams.wrapT = GL_REPEAT;
                                                                shining_texture->setTexParameters(tRepeatParams);
                                                                //pass the texture sampler to our custom shader
                                                                _state->setUniformTexture("caustics",shining_texture);
                                                                //_state->setUniformFloat("offset",_cylinder_texture_offset);
                                                                _state->setUniformFloat("duration",_shining_duraion);
                                                            }
                                                            );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backToForegroundListener, -1);
#endif



}


void CSprite3dShaderGold::cylinderUpdate(float dt){
    
    
    //    _cylinder_texture_offset += 0.3*dt;
    //    _cylinder_texture_offset = (_cylinder_texture_offset >1) ? 0 : _cylinder_texture_offset;
    if(m_bFateState)
    {
        _shining_duraion += 0.5*dt;
        if(_shining_duraion>1) m_bFateState = false;
    }
    else
    {
        _shining_duraion -= 0.5*dt;
        if(_shining_duraion<0) m_bFateState = true;
    }
    
    //pass the result to shader
    //    _state->setUniformFloat("offset",_cylinder_texture_offset);
    _state->setUniformFloat("duration",_shining_duraion);
    
}
