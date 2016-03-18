//
//  BaseDrawLayer.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/11/27.
//
//

#include "BaseDrawLayer.h"


BaseDrawLayer::BaseDrawLayer()
{
    
}
BaseDrawLayer::~BaseDrawLayer(void)
{
    
}

void BaseDrawLayer::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    //注意：注意OpenGL的坐标系，起始点为左下角
    Size s=CCDirector::getInstance()->getWinSize();
    
    //绘制一个控制点贝瑟尔曲线
    //参数说明：起点、控制点、终点，分段数
    ccDrawQuadBezier(Point(0,0), Point(s.width/2,s.height),Point(s.width,0), 50);
    
    
    //绘制两个控制点贝瑟尔曲线
   // ccDrawCubicBezier(ccp(s.width/2, s.height/2), ccp(s.width/2+30,s.height/2+50), ccp(s.width/2+60,s.height/2-50),ccp(s.width, s.height/2),100);
    
    // 恢复opengl绘制的原始值
    glLineWidth(1);
    ccDrawColor4B(255,255,255,255);
    ccPointSize(1);
}
