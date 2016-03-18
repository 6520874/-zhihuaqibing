//
//  LYBaseProtector.cpp
//  TheGodWill
//
//  Created by user on 15/1/26.
//
//

#include "LYBaseProtector.h"

#include "LYFightData.h"

#include "LYBaseHero.h"

USING_NS_CC;


CBaseProtector::CBaseProtector(){
    
    
    
}

CBaseProtector::~CBaseProtector(){
    
    
}

CBaseProtector *CBaseProtector::create(FighterInfo *theInfo,FightGroup theGroup){
    
    CBaseProtector * temp = new CBaseProtector();
    
    temp->setAttackType(eAttackType_protector);
    
    //    temp->m_iAttackType = eAttackType_hero;
    
    if (temp && temp->initWithInfo(theInfo,theGroup)) {
        temp->autorelease();
        return temp;
    }else{
        
        delete temp;
        temp = NULL;
        return NULL;
    }
    
}



bool CBaseProtector::initWithInfo(FighterInfo *theInfo,FightGroup theGroup){
    
    //测试用，有模型了就删掉
    
    if (!CBaseFighter::initWithInfo(theInfo,theGroup)) {
        return false;
    };
    
    
    m_fNowWaitTime = 0;
    
    
//    setScale(1.5f);
    
    stateChange(eFighteState_Idle);
    animateChange(eFighteState_Idle);
    
    
    //脚底光圈
    
    Sprite3D* theCircle = Sprite3D::create();
    
    Sprite* tempSprite = Sprite::createWithSpriteFrameName("fight_ProtectCircle.png");
    
    
    //    tempSprite->setOpacity(255*0.8f);
    
    tempSprite->setScale(m_pTheSrite->getBoundingBox().size.width/tempSprite->getBoundingBox().size.width*2.5);
    theCircle->addChild(tempSprite);
    
    //    tempSprite->runAction(RepeatForever::create(RotateBy::create(0.5f, 30)));
    
    theCircle->setRotation3D(Vec3(90, 0, 0));
    CFightManager::addCamera(theCircle);
    addChild(theCircle);
    
    
    return true;
    
}

void CBaseProtector::collidUpdate(float time){
    
    //重写此方法，什么都不做。因为资源点不会位移
    
    
}

void CBaseProtector::stateUpdate(float time){
    
    
    if (m_bIfDied) {
        return;
    }

    
    CBaseHero* theHero = CFightData::getInstance()->getTheFriendHero();
    
    if (!theHero||theHero->getIfDied()) {
        return;
    }
    
    
    switch (m_iFightState) {
        case eFighteState_Idle:
        {
            
            
            bool ifAttack = findTargetLogic();
            
            
            if (ifAttack) {
                break;
            }
            
             //如果没有攻击目标，则每隔一定时间，就会随机一个位置进行位移。
            m_fNowWaitTime+=time;
            
            if (m_fNowWaitTime>=IDLE_WAIT_TIME) {
                
                m_fNowWaitTime=0;
                
                int tempDist = m_strTheInfo->theMonsterInfo->walkSpeed*120;//大概就是最多移动二秒的距离
                
                float x = rand()%(tempDist*2)-tempDist;
                float y = rand()%(tempDist*2)-tempDist;
                
                
                float originX = theHero->getPositionX();
                float originY = theHero->getPositionZ();
                
                
                
                x = originX+x;
                y = originY+y;
                
                //距离限定。不能离英雄太近，也不能离英雄太远
                if (x<originX) {
                    x =  MIN(x,originX-MIN_WALK_RANGE);
                    x =  MAX(x,originX-MAX_WALK_RANGE);
                }else
                {
                    x =  MAX(x,originX+MIN_WALK_RANGE);
                    x =  MIN(x,originX+MAX_WALK_RANGE);
                }
                
                if (y<originY) {
                    y =  MIN(y,originY-MIN_WALK_RANGE);
                    y =  MAX(y,originY-MAX_WALK_RANGE);
                }else
                {
                    y =  MAX(y,originY+MIN_WALK_RANGE);
                    y =  MIN(y,originY+MAX_WALK_RANGE);
                }
                
                //边界判断
                m_vec2IdlePos = Vec2(x, y);
                
                m_vec2IdlePos = CFightManager::getThePosInMap(m_vec2IdlePos);
                
                stateChange(eFighteState_Walk);
            }
            
            break;
        }
            
        case eFighteState_Walk:
        {
            
            bool ifAttack = findTargetLogic();
            
            
            //如果没有敌人，而且已经到了空闲位置点，则改变状态成空闲
            if (!ifAttack&&m_vec2IdlePos.distance(getFightPosition())<5) {
                
                stateChange(eFighteState_Idle);
                break;
            }
            

            //如果移动状态下，动画不是移动。则更换移动动画
            if (m_iAnimateState!=eFighteState_Walk&&m_iFightState!=eFighteState_Attack) {
                animateChange(eFighteState_Walk);
            }
            
            //在当前动画状态是移动时，移动
            
            if (m_iAnimateState==eFighteState_Walk) {
                positionUpdate(time);
            }
            
            
            roatationUpdate(time);

            break;
        }
            
        default:
            
            CBaseFighter::stateUpdate(time);
            
            break;
    }
    

    
//    //但是会实时侦测拉杆，并且产生位移。
//    positionUpdate(time);
//    roatationUpdate(time);
    
}

bool CBaseProtector::findTargetLogic(){
    
    //先判断有没有敌人踏入雷区
    Vector<CBaseFighter*> target;
    
    target = chooseTarget();
    
    
    CBaseHero* theHero = CFightData::getInstance()->getTheFriendHero();
    
    
    float minDistance=MAX_ALERT_RANGE;
    for (int i=0; i<target.size(); i++) {
        
        //如果死掉了就不判断了
        if (target.at(i)->getIfDied()) {
            continue;
        }
        
        float distance = theHero->getFightDistance(target.at(i));
        
        float atkdistance = getFightDistance(target.at(i));
        
        
        //此处是优先挑离英雄最近的人当目标，
        
        if (distance<minDistance) {
            minDistance = distance;
            setAttackTarget(target.at(i));
//            m_pAttackTarget = ;
        }
        
        if (atkdistance<=m_strTheInfo->attackRang) {
            
            minDistance=-1;//-1代表进入攻击
            setAttackTarget(target.at(i));
//            m_pAttackTarget = target.at(i);
            stateChange(eFighteState_Attack);
            return true;
        }
    }
    
    
    if (minDistance<MAX_ALERT_RANGE) { //如果最小距离小于警戒距离，则进入走路
        
        stateChange(eFighteState_Walk);
        return true;
        
    }else{
        setAttackTarget(NULL);
//        m_pAttackTarget=NULL;
    }

    return false;
}



