//
//  LYBaseTower.cpp
//  TheGodWill
//
//  Created by user on 14/12/8.
//
//

#include "LYBaseTower.h"
#include "LYFightData.h"


USING_NS_CC;




CBaseTower::CBaseTower(){
    
    m_bUpdating = false;
    
    m_vecBaseRotation = Vec3(0, 0, 0);
    
}

CBaseTower::~CBaseTower(){
    
    
}


CBaseTower *CBaseTower::create(FighterInfo *theInfo,FightGroup theGroup){
    
    CBaseTower * temp = new CBaseTower();
    
    temp->m_iAttackType = eAttackType_tower;
    
    if (temp && temp->initWithInfo(theInfo,theGroup)) {
        temp->autorelease();
        return temp;
    }else{
        
        delete temp;
        temp = NULL;
        return NULL;
    }
    
}

bool CBaseTower::initWithInfo(FighterInfo *theInfo,FightGroup theGroup){

    
    
    //此处测试用
//    SkillInfo temp ;
//    temp.skillId=-1;
//    temp.damageRatio=1;
//    temp.triggerRatio=1;
//    
//    m_VecSkillInfo.push_back(temp);
    
    
    
    if (!CBaseBuilding::initWithInfo(theInfo,theGroup)) {
        return false;
    };
    
    if (theGroup==eFightGroup_midFight||theGroup==eFightGroup_midNotFight) {
        m_bIfReborn = true;//中立塔死后会复活，变为摧毁者一方
    }
    
    
    
    refreshTheOwner(m_iFightGroup);
    
    
    
    
//    m_fSpriteScale = 0.2f;
//    m_pTheSrite->setScale(m_fSpriteScale);
    

    
//    if (m_iAttackType==eAttackType_wall) {//测试用，小一点
//        setScale(0.5f);
//    }

    
    m_pTheHpBar->setPosition(0, m_pTheSrite->getBoundingBox().size.height);
    
    
//    m_pTheHpBar->setScale(1);
    
//    m_pTheHpBar->setPosition3D(Vec3(0,60,0));
    
    m_iDeadlyGroup = theGroup;//防止错误，默认为致死阵营是自己阵营
    
    
//    setRotation3D(Vec3(0, 0, -90));
    
//    m_v2AttackPosOffset = Vec3(10, 100, 10);

    
    
    
    
    
    return true;
    
}

void CBaseTower::collidUpdate(float time){
    
    //重写此方法，什么都不做。因为资源点不会位移
    
    

}


void CBaseTower::stateUpdate(float time){
    
    
    //测试用
//    ifSkillStart();
//    skillPrepare();
//    return;
    
    
    
    
    //重写此方法，因为资源点只判断最后一个经过他的人的属性。
    
    
    
    CBaseBuilding::stateUpdate(time);
    
    
    
    //如果死了，则返回
    if (getIfDied()||m_bUpdating) {
        return;
    }
    
    //如果没有攻击范围，则返回
    if (m_strTheInfo->attackRang<=1) {
        return;
    }
    
    //如果是录像模式，则不走攻击判断
    CFightScene* theScene = CFightData::getInstance()->getTheFightScene();
    if (theScene&&theScene->getthePVPMode()==EPVPMode_REC) {
        return;
    }

    
    
    //判断是否到攻击cd，到了则攻击。
    m_fAttackColdNow-=time;
    if (m_fAttackColdNow<0 ) {
        
        
        
        
        //判断是否有敌人，有则攻击
        cocos2d::Vector<CBaseFighter*> targetVec = chooseTarget();
        
        float minDistance = m_strTheInfo->attackRang;
        CBaseFighter* tempTarget = NULL;
        
        for (int i =0; i<targetVec.size(); i++) {
            
            CBaseFighter* target = targetVec.at(i);
            
            float distatce = getFightDistance(target);
            
            if (distatce<=minDistance) {
                
                tempTarget = target;
                minDistance = getFightDistance(target);
                
  
                //此处需注意，在普通战斗模式下 如果塔已经开始攻击，而坐标点却没归零的话，则手动归零
                if (m_pTheSrite->getPosition3D()!=Vec3::ZERO&&CFightData::getInstance()->getFightType()==FIGHT_COMMON_TYPE) {
                    m_pTheSrite->setPosition3D(Vec3::ZERO);
                }

            }
 
        }
        
        
        //获取离得最近的目标后开始攻击
        if (tempTarget) {
            setAttackTarget(tempTarget);
            
            if(tempTarget==NULL||tempTarget->getReferenceCount()<=0||tempTarget->getIfDied()){
                setAttackTarget(NULL);
            }else{
                
                //攻击时间到了后，判断是释放被动技能还是普通攻击
                if (ifSkillStart()) {
                    m_fAttackColdNow = m_strTheInfo->attackCD;
                    skillPrepare();
                }
//                else{
//                    CCLOG("技能既然没开始？果断bug了");
//                }
                
                if (m_strTheInfo->theModelInfo->modelId==2022) { //投石机才会更新面向
                    roatationUpdate(time);
                }
            }
            
            
        }
        
        

        

    }
    
    
}

void CBaseTower::setAttackTarget(CBaseFighter *target,bool ifRec){
    
    
//    if (target==m_pAttackTarget) { //如果攻击目标和目标一致，则不做任何操作
//        return;
//    }
    
    
    CBaseFighter::setAttackTarget(target,ifRec);
    
    
    CFightScene* theScene = CFightData::getInstance()->getTheFightScene();
    
    
    if (target==NULL) { //如果是设置技能目标位空，则无论什么时候都设置
        setSkillAttackTarget(NULL);
    }
    

    
    
    if (theScene&&theScene->getthePVPMode()==EPVPMode_REC&&ifRec==false) {
        return;
    }
    
    setSkillAttackTarget(target); //如果是设置技能目标，则只能在回放模

    
    //回放模式下，跳过攻击cd的判断，直接攻击
    if(ifRec){
        
        
        //攻击时间到了后，判断是释放被动技能还是普通攻击
        if (ifSkillStart()) {
            m_fAttackColdNow = m_strTheInfo->attackCD;
            skillPrepare();
        }
        
        if (m_strTheInfo->theModelInfo->modelId==2022) { //投石机才会更新面向
            roatationUpdate(0);
        }
        
    }
    
    
    

}



void CBaseTower::onDied(DamageType type ){
    
    
    CBaseBuilding::onDied(type);
    
    
    //如果是pvp模式的话，塔死掉后会给玩家加能量
    

    
    
    
    
}

