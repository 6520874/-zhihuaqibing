//
//  LYSprite3dAnimateShaderGold.cpp
//  WarOfGod
//
//  Created by user on 15/7/10.
//
//

#include "LYSprite3dAnimateShaderGold.h"

using namespace cocos2d;


void CSprite3dAnimateShaderGold::setTarget(cocos2d::Sprite3D *target){
    
    
    //º”‘ÿƒ£–ÕŒƒº˛
    m_Sprite = target;
    auto s = Director::getInstance()->getWinSize();
//    m_Sprite->setPosition(Vec2(s.width / 2.f, s.height / 2.f));
//    addChild(m_Sprite);
    
    
    // Ω´vsh”Îfsh◊∞≈‰≥…“ª∏ˆÕÍ’˚µƒShaderŒƒº˛°£
    auto glprogram = GLProgram::createWithFilenames("Sprite3DTest/fish/UVAnimation.vsh", "Sprite3DTest/fish/gold.fsh");
    // ”…ShaderŒƒº˛¥¥Ω®’‚∏ˆShader
    glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
    // ∏¯æ´¡È…Ë÷√À˘”√µƒShader
    m_Sprite->setGLProgramState(glprogramstate);
    
    
    glprogramstate->setUniformFloat("duration",_shining_duraion);
    
    //¥¥Ω®∫£πÍÀ˘”√µƒÃ˘Õº°£
//    auto textrue1 = Director::getInstance()->getTextureCache()->addImage("models/soldier/Hero0_M_t3.png");
    auto textrue1 = m_Sprite->getMesh()->getTexture();
    glprogramstate->setUniformTexture("u_texture1", textrue1);
    
    //¥¥Ω®≤®π‚Ã˘Õº°£
    auto textrue2 = Director::getInstance()->getTextureCache()->addImage("Sprite3DTest/caustics.png");
    
    glprogramstate->setUniformTexture("u_lightTexture", textrue2);
    
    //◊¢“‚£¨∂‘”⁄≤®π‚Ã˘Õº£¨Œ“√«œ£Õ˚À¸‘⁄Ω¯––UV∂Øª≠ ±ƒ‹≤˙…˙Àƒ∑Ω¡¨–¯–ßπ˚£¨±ÿ–Î…Ë÷√À¸µƒŒ∆¿ÌUV—∞÷∑∑Ω ΩŒ™GL_REPEAT°£
    Texture2D::TexParams		tRepeatParams;
    tRepeatParams.magFilter = GL_LINEAR_MIPMAP_LINEAR;
    tRepeatParams.minFilter = GL_LINEAR;
    tRepeatParams.wrapS = GL_REPEAT;
    tRepeatParams.wrapT = GL_REPEAT;
    textrue2->setTexParameters(tRepeatParams);
    
    //‘⁄’‚¿Ô£¨Œ“√«…Ë÷√“ª∏ˆ≤®π‚µƒ—’…´£¨’‚¿Ô…Ë÷√Œ™∞◊…´°£
    Vec4  tLightColor(1.0,1.0,1.0,1.0);
    glprogramstate->setUniformVec4("v_LightColor",tLightColor);
    
    schedule(schedule_selector(CSprite3dAnimateShaderGold::cylinderUpdate));
    
    //œ¬√Ê’‚“ª∂Œ£¨ «Œ™¡ÀΩ´Œ“√«◊‘∂®“ÂµƒShader”ÎŒ“√«µƒƒ£–Õ∂•µ„◊È÷Ø∑Ω ΩΩ¯––∆•≈‰°£ƒ£–Õµƒ∂•µ„ ˝æ›“ª∞„∞¸¿®Œª÷√£¨∑®œﬂ£¨…´≤ £¨Œ∆¿Ì£¨“‘º∞π«˜¿∞Û∂®–≈œ¢°£∂¯Shader–Ë“™Ω´ƒ⁄≤øœ‡”¶µƒ∂•µ„ Ù–‘Õ®µ¿”Îƒ£–Õœ‡”¶µƒ∂•µ„ Ù–‘ ˝æ›Ω¯––∞Û∂®≤≈ƒ‹’˝»∑œ‘ æ≥ˆ∂•µ„°£
    long offset = 0;
    auto attributeCount = m_Sprite->getMesh()->getMeshVertexAttribCount();
    for (auto k = 0; k < attributeCount; k++) {
        auto meshattribute = m_Sprite->getMesh()->getMeshVertexAttribute(k);
        glprogramstate->setVertexAttribPointer(s_attributeNames[meshattribute.vertexAttrib],
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

void CSprite3dAnimateShaderGold::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags)
{
    if(m_Sprite)
    {
        
        auto glprogramstate = m_Sprite->getGLProgramState();
        if(glprogramstate)
        {
            m_LightAni.x += 0.01;
            if(m_LightAni.x > 1.0)
            {
                m_LightAni.x-= 1.0;
            }
            m_LightAni.y += 0.01;
            if(m_LightAni.y > 1.0)
            {
                m_LightAni.y-= 1.0;
            }
            glprogramstate->setUniformVec2("v_animLight",m_LightAni);
        }
    }
    Node::draw(renderer,transform,flags);
}

void CSprite3dAnimateShaderGold::cylinderUpdate(float dt){
    
    
    //    _cylinder_texture_offset += 0.3*dt;
    //    _cylinder_texture_offset = (_cylinder_texture_offset >1) ? 0 : _cylinder_texture_offset;
    if(fadeState==0)
    {
        _shining_duraion += 0.5*dt;
        if(_shining_duraion>1) fadeState = 1;
    }
    else if(fadeState==1)
    {
        _shining_duraion -= 0.5*dt;
        if(_shining_duraion<0) fadeState = 2;
    }
    
    //pass the result to shader
    //    _state->setUniformFloat("offset",_cylinder_texture_offset);
    glprogramstate->setUniformFloat("duration",_shining_duraion);
    
    
    if (fadeState==2)  //如果状态为2，则删除自己
    {
//        m_Sprite->genGLProgramState();
        
//        m_Sprite->setGLProgramState(GLProgramState::getOrCreateWithGLProgram(GLProgramCache::getInstance()->getGLProgram("ShaderPositionTextureColor_noMVP")));
        
        m_Sprite->setGLProgramState(GLProgramState::getOrCreateWithGLProgram(GLProgramCache::getInstance()->getGLProgram("Shader3DSkinPositionTexture")));
        removeFromParent();
    }
    
}


