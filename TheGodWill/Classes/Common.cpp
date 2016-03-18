//
//  Common.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/12/4.
//
//

#include "Common.h"
#include "LYPersonalData.h"
#include "fightScene.h"
#include "DBMgr.h"
#include "SysTime.h"
#include "LYFightData.h"

#include "NetWorkDataSingle.h"

 CommonData *CommonData::t = NULL;

CommonData::CommonData()
{


}

CommonData::~CommonData()
{


}

CommonData* CommonData::GetSingletonPtr()
{
    if (!t)
    {
        t = new CommonData();
        
    }
    return t;
}


std::map<int,int>  CommonData::GetPropMap()
{
    return   m_PropMap;
}



bool CommonData::IsManHeroVisable()
{
    int levelId = CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel;
    
    if (levelId>= (LEVEL_START_MAN))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CommonData::IsGirlHeroVisable()
{
    int levelId = CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel;
    
    if (levelId>= (LEVEL_START_PROTECT))
    {
        return true;
    }
    else
    {
        return false;
    }
}

Vec2 CommonData::getPosOnLayer3D(Camera * _camera,cocos2d::Vec2 cameraPoint)
{
    
    //将屏幕坐标转化为对应点的世界坐标。
    
    auto size = Director::getInstance()->getWinSize();
    
    Vec3 targetNear;
    Vec3 targetFar;
    Vec3 target;
    
    targetNear.x = cameraPoint.x;
    targetNear.y = cameraPoint.y;
    targetNear.z = -1;
    
    
    targetFar.x = cameraPoint.x;
    targetFar.y = cameraPoint.y;
    targetFar.z = 1;
    
    
    _camera->unproject(size, &targetNear, &targetNear);
    _camera->unproject(size, &targetFar, &targetFar);
    
    
    Vec3 dir(targetFar-targetNear);   //光标的向量
    
    float dist = 0.0f;
    
    float ndd = Vec3::dot(Vec3(0,1,0), dir);
    
    if (ndd==0)
    {
        dist=0;
    }
    
    float ndo = Vec3::dot(Vec3(0,1,0),targetNear);
    
    dist = (0-ndo)/ndd;
    
    target = targetNear + dist*dir;
    
    
    return Vec2(target.x, target.z);
    
}

Vec2 CommonData::getPosOnScreen(cocos2d::Camera *_camera, cocos2d::Vec3 position3D){
    
    Size winsize = Director::getInstance()->getWinSize();
    
    Mat4 viewMat = _camera->getViewMatrix();
    Mat4 projMat = _camera->getProjectionMatrix();
    
    Vec4 in = Vec4(position3D.x,position3D.y,position3D.z,1.0f);
    
    Vec4 out = viewMat * in;
    out = projMat * out;
    
    out.x /= out.w;
    out.y /= out.w;
    out.z /= out.w;
    
    out.x = out.x*0.5 +0.5;
    out.y = out.y*0.5 +0.5;
    out.z = out.z*0.5 +0.5;
    
    out.x = out.x*winsize.width;
    out.y = (out.y)*winsize.height;
    
    
    return  Vec2(out.x, out.y);
    
}

void CommonData::changeRotation3DWithVec(cocos2d::Vec3 originDirection, cocos2d::Vec3 targetDirection, cocos2d::Node *targetNode){
    
    
    Mat4 rotation;
    
    Vec3 origin = originDirection;
    origin.normalize();
    
    Vec3 target = targetDirection;
    target.normalize();
    
    Vec3 crossVec;
    Vec3::cross(origin, target, &crossVec);
    crossVec.normalize();
    
    float angle = acos(Vec3::dot(origin, target));
    
    
    Quaternion  quaternion;
    //    Quaternion::createFromRotationMatrix(rotation,&quaternion);
    Quaternion::createFromAxisAngle(crossVec, angle, &quaternion);
    
    targetNode->setRotationQuat(quaternion);
    
}

void CommonData::enlargeItemSprite(cocos2d::MenuItemSprite *target,float ratio){
    
    
    float posOffset = (ratio-1)*0.5f;
    
    Node* normal = target->getNormalImage();
    
    Node* select = target->getSelectedImage();
    
    target->setContentSize(target->getContentSize()*ratio);
    
    normal->setPosition(normal->getContentSize().width*posOffset,normal->getContentSize().height*posOffset);
    
    select->setPosition(select->getContentSize().width*posOffset,select->getContentSize().height*posOffset);
    
}
