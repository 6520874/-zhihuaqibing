//
//  Common.h
//  TheGodWill
//
//  Created by prefer_shan on 14/12/4.
//
//

#ifndef __TheGodWill__Common__
#define __TheGodWill__Common__

#include "cocos2d.h"

USING_NS_CC;


#include <stdio.h>

class CommonData:public Node
{
public:
    static CommonData* GetSingletonPtr();
    
    Vec2 getPosOnLayer3D(Camera * _camera,cocos2d::Vec2 cameraPoint);//根据镜头上点击的坐标，转化为layer3d上的坐标
    
    Vec2 getPosOnScreen(Camera *_camera,cocos2d::Vec3 position3D);//根据layer3d上的坐标，转化为镜头上点击的坐标
    
    static void changeRotation3DWithVec(Vec3 originDirection, Vec3 targetDirection,cocos2d::Node* targetNode);
    
    static void enlargeItemSprite(MenuItemSprite* target,float ratio); //放大按钮的实际相应区域，但是图片大小不变。
    
    //女主角是否显示
    bool IsGirlHeroVisable();


    //男主角是否显示
    bool IsManHeroVisable();
    
    std::map<int,int>  GetPropMap();
    
    std::map<int,int> m_PropMap;  //道具id，需要的id
private:
    CommonData();
    ~CommonData();

    static CommonData *t;
    
};
#endif /* defined(__TheGodWill__Common__) */
