//
//  LYBaseTrap.cpp
//  WarOfGod
//
//  Created by user on 15/6/29.
//
//

#include "LYBaseTrap.h"
#include "LYFightData.h"


USING_NS_CC;


CBaseTrap::CBaseTrap(){
    
    m_iTrapState=0;
    bDingshiBorn = true;
    
    m_bUpdating = false;
  
    
}

CBaseTrap::~CBaseTrap(){
    
    
}

CBaseTrap *CBaseTrap::create(FighterInfo *theInfo,FightGroup theGroup){
    
    
    CBaseTrap * temp = new CBaseTrap();
    
    temp->m_iAttackType = eAttackType_Trap;
    
    if (temp && temp->initWithInfo(theInfo,theGroup)) {
        temp->autorelease();
        return temp;
    }else{
        
        delete temp;
        temp = NULL;
        return NULL;
    }
    
}

bool CBaseTrap::initWithInfo(FighterInfo *theInfo,FightGroup theGroup){
    
    
    
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
    



    
    if(m_strTheInfo->monsterId != 20012)
    {
        m_pTheSrite->setVisible(false);
        setVisible(false);
    }
    
    if (m_pShadow) {
        m_pShadow->setVisible(false);
    }
    if (m_pShadow2) {
        m_pShadow2->setVisible(false);
    }
    

    
    return true;
    
}


void CBaseTrap::stateUpdate(float time){
    
    
    if(m_bUpdating)
    {
        return;
    }
    //如果是隐藏状态，则不显示
    if (m_iTrapState==0&&m_strTheInfo->monsterId != 20012)
    {
        setVisible(false);
    }else{
        return; //否则直接返回
    }
    
    
    
    //判断是否有人在攻击范围内
    Vector<CBaseFighter*> target;
    
    target = chooseTarget();
    
    float minDistance=m_strTheInfo->attackRang;
    
    for (int i=0; i<target.size(); i++) {
        
        //如果死掉了就不判断了
        if (target.at(i)->getIfDied()) {
            continue;
        }
        
        float distance = getFightDistance(target.at(i));
        
        
        if (distance<minDistance) {
            minDistance = distance;
            
            setAttackTarget(target.at(i));
            break;
        }
        
    }
    
    
    if (getAttackTarget()&&m_strTheInfo->monsterId != 20012) {  //有人在攻击范围内，则判断是否触发技能
        
        
//        ifSkillStart(6); //是否技能触发
        
        
        //攻击时间到了后，判断是释放被动技能还是普通攻击
        if (ifSkillStart(6))
        {
            if(m_iNextSkill)
            {
                
                TrapActive();
                
            }
            
        }
        
    }
    
    
    
}



void CBaseTrap::TrapActive(){
    
    setVisible(true);
    m_pTheSrite->setVisible(true);
    if (m_pShadow) {
        m_pShadow->setVisible(true);
    }
    if (m_pShadow2) {
        m_pShadow2->setVisible(true);
    }
    
    m_iTrapState=1;
    
    
    if(m_strTheInfo->monsterId == 20012)
    {
        //m_pTheSrite->setPosition3D(Vec3(0,m_pTheSrite->getPosition3D()));
        
        m_pTheSrite->runAction(Sequence::create(Repeat::create(Sequence::create(TintTo::create(0.15, 255, 0, 0),TintTo::create(0.15, 0, 0, 0), NULL), 4), NULL));
        
    }
    else
    {
        m_pTheSrite->setPosition3D(Vec3(0,-m_pTheSrite->getBoundingBox().size.height,0)+m_pTheSrite->getPosition3D());

        m_pTheSrite->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(0, m_pTheSrite->getBoundingBox().size.height)),Repeat::create(Sequence::create(TintTo::create(0.15, 255, 0, 0),TintTo::create(0.15, 0, 0, 0), NULL), 4), NULL));
    }
    
    scheduleOnce(schedule_selector(CBaseTrap::TrapSkillStart), 1.7);
    
}



void CBaseTrap::TrapSkillStart(float time){
    
    skillPrepare(false);
}


void CBaseTrap::update(float time)
{

    stateUpdate(time);

    
     if(m_strTheInfo->monsterId == 20012)
     {
         int iTime = CFightData::getInstance()->getThePvpTime();
         
        
         int iBornTime = CFightData::getInstance()->getTheBornTime();
         
         
         int iAlTime  = CFightData::getInstance()->getThePvpAllTime();
         
         if(iTime<=(iAlTime-iBornTime))
         {
             if(bDingshiBorn)
             {
                 if (ifSkillStart(6))
                 {
                     if(m_iNextSkill)
                     {
                         TrapActive();
                     }
                 }
                 bDingshiBorn = false;
                 
             }
    }
     }
    
}
