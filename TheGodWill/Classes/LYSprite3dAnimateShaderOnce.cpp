//
//  LYSprite3dAnimateShaderOnce.cpp
//  WarOfGod
//
//  Created by user on 15/7/13.
//
//

#include "LYSprite3dAnimateShaderOnce.h"

using namespace cocos2d;

void CSprite3dAnimateShaderOnce::setTarget(cocos2d::Sprite3D *target,std::string shaderFilePath,std::string glProgramKey){
    
    
    fadeState=0;
    _shining_duraion=0;
    
    //º”‘ÿƒ£–ÕŒƒº˛
    m_Sprite = target;
    auto s = Director::getInstance()->getWinSize();
    //    m_Sprite->setPosition(Vec2(s.width / 2.f, s.height / 2.f));
    //    addChild(m_Sprite);
    
    
    // Ω´vsh”Îfsh◊∞≈‰≥…“ª∏ˆÕÍ’˚µƒShaderŒƒº˛°£
//    auto glprogram = GLProgram::createWithFilenames("Sprite3DTest/fish/UVAnimation.vsh", "Sprite3DTest/fish/flashColor.fsh");
    // ”…ShaderŒƒº˛¥¥Ω®’‚∏ˆShader
    
    GLProgram* glprogram = NULL;
    
    if (glProgramKey!="") {
         glprogram = GLProgramCache::getInstance()->getGLProgram(glProgramKey.c_str());
    }else{
        glprogram = GLProgram::createWithFilenames("Sprite3DTest/fish/UVAnimation.vsh", "Sprite3DTest/fish/flashColor.fsh");
    }
    
    
//    auto glprogram = GLProgramCache::getInstance()->getGLProgram("shaderOnce");
    m_pProgramstate = GLProgramState::create(glprogram);
    // ∏¯æ´¡È…Ë÷√À˘”√µƒShader
    m_Sprite->setGLProgramState(m_pProgramstate);
    
    
    //¥¥Ω®∫£πÍÀ˘”√µƒÃ˘Õº°£
    //    auto textrue1 = Director::getInstance()->getTextureCache()->addImage("models/soldier/Hero0_M_t3.png");
    auto textrue1 = m_Sprite->getMesh()->getTexture();
    m_pProgramstate->setUniformTexture("u_texture1", textrue1);
    
    //¥¥Ω®≤®π‚Ã˘Õº°£
    //    auto textrue2 = Director::getInstance()->getTextureCache()->addImage("Sprite3DTest/caustics.png");
    
    auto textrue2 = Director::getInstance()->getTextureCache()->addImage(shaderFilePath.c_str());
    
    m_pProgramstate->setUniformTexture("u_lightTexture", textrue2);
    
    //◊¢“‚£¨∂‘”⁄≤®π‚Ã˘Õº£¨Œ“√«œ£Õ˚À¸‘⁄Ω¯––UV∂Øª≠ ±ƒ‹≤˙…˙Àƒ∑Ω¡¨–¯–ßπ˚£¨±ÿ–Î…Ë÷√À¸µƒŒ∆¿ÌUV—∞÷∑∑Ω ΩŒ™GL_REPEAT°£
    Texture2D::TexParams		tRepeatParams;
    tRepeatParams.magFilter = GL_LINEAR_MIPMAP_LINEAR;
    tRepeatParams.minFilter = GL_LINEAR;
    tRepeatParams.wrapS = GL_REPEAT;
    tRepeatParams.wrapT = GL_REPEAT;
    textrue2->setTexParameters(tRepeatParams);
    
    //‘⁄’‚¿Ô£¨Œ“√«…Ë÷√“ª∏ˆ≤®π‚µƒ—’…´£¨’‚¿Ô…Ë÷√Œ™∞◊…´°£
    Vec4  tLightColor(1.0,1.0,1.0,1.0);
    m_pProgramstate->setUniformVec4("v_LightColor",tLightColor);
    
    
    m_pProgramstate->setUniformFloat("duration",_shining_duraion);
    
    schedule(schedule_selector(CSprite3dAnimateShaderOnce::cylinderUpdate));
    
    //œ¬√Ê’‚“ª∂Œ£¨ «Œ™¡ÀΩ´Œ“√«◊‘∂®“ÂµƒShader”ÎŒ“√«µƒƒ£–Õ∂•µ„◊È÷Ø∑Ω ΩΩ¯––∆•≈‰°£ƒ£–Õµƒ∂•µ„ ˝æ›“ª∞„∞¸¿®Œª÷√£¨∑®œﬂ£¨…´≤ £¨Œ∆¿Ì£¨“‘º∞π«˜¿∞Û∂®–≈œ¢°£∂¯Shader–Ë“™Ω´ƒ⁄≤øœ‡”¶µƒ∂•µ„ Ù–‘Õ®µ¿”Îƒ£–Õœ‡”¶µƒ∂•µ„ Ù–‘ ˝æ›Ω¯––∞Û∂®≤≈ƒ‹’˝»∑œ‘ æ≥ˆ∂•µ„°£
    long offset = 0;
    auto attributeCount = m_Sprite->getMesh()->getMeshVertexAttribCount();
    for (auto k = 0; k < attributeCount; k++) {
        auto meshattribute = m_Sprite->getMesh()->getMeshVertexAttribute(k);
        m_pProgramstate->setVertexAttribPointer(s_attributeNames[meshattribute.vertexAttrib],
                                                meshattribute.size,
                                                meshattribute.type,
                                                GL_FALSE,
                                                m_Sprite->getMesh()->getVertexSizeInBytes(),
                                                (GLvoid*)offset);
        offset += meshattribute.attribSizeBytes;
    }
    
    //uvπˆ∂Ø≥ı º÷µ…ËŒ™0
    m_LightAni.x = m_LightAni.y = 0;
    
}

void CSprite3dAnimateShaderOnce::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags)
{
//    if(m_Sprite)
//    {
//        
//        auto glprogramstate = m_Sprite->getGLProgramState();
//        if(glprogramstate)
//        {
//            //            m_LightAni.x += 0.01;
//            //            if(m_LightAni.x > 1.0)
//            //            {
//            //                m_LightAni.x-= 1.0;
//            //            }
////            m_LightAni.y += 0.01;
////            if(m_LightAni.y > 1.0)
////            {
////                m_LightAni.y-= 1.0;
////            }
//            glprogramstate->setUniformVec2("v_animLight",m_LightAni);
//        }
//    }
    Node::draw(renderer,transform,flags);
}


void CSprite3dAnimateShaderOnce::cylinderUpdate(float dt){
    
    
    if(fadeState==0)
    {
        _shining_duraion += 5*dt;
        if(_shining_duraion>1) fadeState = 1;
        m_pProgramstate->setUniformFloat("duration",_shining_duraion);
    }
    else if(fadeState==1)
    {
        _shining_duraion -= 5*dt;
        if(_shining_duraion<0) fadeState = 2;
        m_pProgramstate->setUniformFloat("duration",_shining_duraion);
    }
    
    
    
    //pass the result to shader
    //    _state->setUniformFloat("offset",_cylinder_texture_offset);
    
    
    
    if (fadeState==2)  //如果状态为2，则啥也不做
    {
        _shining_duraion+=dt;
        m_pProgramstate->setUniformFloat("duration",0);
        
//        if(_shining_duraion>0.5)
//            fadeState = 3;
        
//        m_Sprite->setGLProgramState(GLProgramState::getOrCreateWithGLProgram(GLProgramCache::getInstance()->getGLProgram("Shader3DSkinPositionTexture")));
//        removeFromParent();
    }


    
}
