//
//  LYSprite3dShaderWhite.cpp
//  WarOfGod
//
//  Created by user on 15/7/10.
//
//

#include "LYSprite3dShaderWhite.h"


using namespace cocos2d;



CSprite3dShaderWhite::CSprite3dShaderWhite(){
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//    _backToForegroundListener = NULL;
//#endif

    fadeState=0;
    
    
    
}

CSprite3dShaderWhite::~CSprite3dShaderWhite(){
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//    if (_backToForegroundListener) {
//        Director::getInstance()->getEventDispatcher()->removeEventListener(_backToForegroundListener);
//    }
//#endif
    
    
}


void CSprite3dShaderWhite::setTarget(cocos2d::Sprite3D *target){
    
    
    _cylinder_texture_offset = 0;
    _shining_duraion = 0;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //use custom camera
//    auto camera = Camera::createPerspective(60, visibleSize.width/visibleSize.height, 0.1f, 200);
//    camera->setCameraFlag(CameraFlag::USER1);
    
    //create cylinder
    auto cylinder = target;
    
    //create and set our custom shader
//    auto shader =GLProgram::createWithFilenames("Sprite3DTest/cylinder.vert","Sprite3DTest/fadeColor.frag");
//    _state = GLProgramState::create(shader);
//    cylinder->setGLProgramState(_state);
    
    
    __String* temp3 = __String::createWithFormat("shaderOnceBuilding");
    GLProgram* glprogram3 = GLProgramCache::getInstance()->getGLProgram(temp3->getCString());
    if (!glprogram3) {
        glprogram3 = GLProgram::createWithFilenames("Sprite3DTest/cylinder.vert","Sprite3DTest/fadeColor.frag");
        GLProgramCache::getInstance()->addGLProgram(glprogram3, temp3->getCString());
    }
    
    _state = GLProgramState::create(glprogram3);
    cylinder->setGLProgramState(_state);
    
//    _state->setUniformFloat("offset",_cylinder_texture_offset);
    _state->setUniformFloat("duration",_shining_duraion);
    //pass mesh's attribute to shader
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
    auto shining_texture = Director::getInstance()->getTextureCache()->addImage("Sprite3DTest/white.png");
    Texture2D::TexParams tRepeatParams;//set texture parameters
    tRepeatParams.magFilter = GL_NEAREST;
    tRepeatParams.minFilter = GL_NEAREST;
    tRepeatParams.wrapS = GL_REPEAT;
    tRepeatParams.wrapT = GL_REPEAT;
    shining_texture->setTexParameters(tRepeatParams);
    //pass the texture sampler to our custom shader
    _state->setUniformTexture("caustics",shining_texture);
    
    
//    this->setCameraMask(target->getCameraMask());
//    this->addChild(camera);
    
    //adjust cylinder's position & rotation
//    cylinder->setPosition3D(Vec3(0,-15,-50));
//    cylinder->setRotation3D(Vec3(-90,0,0));
    
    //the callback function update cylinder's texcoord
    schedule(schedule_selector(CSprite3dShaderWhite::cylinderUpdate));
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//    _backToForegroundListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
//                                                            [this](EventCustom*)
//                                                            {
//                                                                auto glProgram = _state->getGLProgram();
//                                                                glProgram->reset();
//                                                                glProgram->initWithFilenames("Sprite3DTest/cylinder.vert", "Sprite3DTest/fadeColor.frag");
//                                                                glProgram->link();
//                                                                glProgram->updateUniforms();
//                                                                auto shining_texture = Director::getInstance()->getTextureCache()->addImage("Sprite3DTest/caustics.png");
//                                                                Texture2D::TexParams tRepeatParams;//set texture parameters
//                                                                tRepeatParams.magFilter = GL_NEAREST;
//                                                                tRepeatParams.minFilter = GL_NEAREST;
//                                                                tRepeatParams.wrapS = GL_REPEAT;
//                                                                tRepeatParams.wrapT = GL_REPEAT;
//                                                                shining_texture->setTexParameters(tRepeatParams);
//                                                                //pass the texture sampler to our custom shader
//                                                                _state->setUniformTexture("caustics",shining_texture);
//                                                                //_state->setUniformFloat("offset",_cylinder_texture_offset);
//                                                                _state->setUniformFloat("duration",_shining_duraion);
//                                                            }
//                                                            );
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backToForegroundListener, -1);
//#endif
//

}





void CSprite3dShaderWhite::cylinderUpdate(float dt){
    
    
//    _cylinder_texture_offset += 0.3*dt;
//    _cylinder_texture_offset = (_cylinder_texture_offset >1) ? 0 : _cylinder_texture_offset;
    if(fadeState==0)
    {
        _shining_duraion += 5*dt;
        if(_shining_duraion>1) fadeState = 1;
        _state->setUniformFloat("duration",_shining_duraion);
    }
    else if(fadeState==1)
    {
        _shining_duraion -= 5*dt;
        if(_shining_duraion<0) fadeState = 2;
        _state->setUniformFloat("duration",_shining_duraion);
    }
    
    
    
    //pass the result to shader
    //    _state->setUniformFloat("offset",_cylinder_texture_offset);
    
    
    
    if (fadeState==2)  //如果状态为2，则啥也不做
    {
        _shining_duraion+=dt;
        _state->setUniformFloat("duration",0);
        
//        if(_shining_duraion>0.5)
//            fadeState = 3;
        
        //        m_Sprite->setGLProgramState(GLProgramState::getOrCreateWithGLProgram(GLProgramCache::getInstance()->getGLProgram("Shader3DSkinPositionTexture")));
        //        removeFromParent();
    }

}
