//
//  LYBaseCity.cpp
//  TheGodWill
//
//  Created by user on 14/12/17.
//
//

#include "LYBaseCity.h"

#include "LYFightData.h"


USING_NS_CC;


CBaseCity::CBaseCity(){
    
    
    
}

CBaseCity::~CBaseCity(){
    
    
}


CBaseCity *CBaseCity::create(FighterInfo *theInfo,FightGroup theGroup){
    
    CBaseCity * temp = new CBaseCity();
    
    temp->m_iAttackType = eAttackType_city;
    
    if (temp && temp->initWithInfo(theInfo,theGroup)) {
        temp->autorelease();
        return temp;
    }else{
        
        delete temp;
        temp = NULL;
        return NULL;
    }
    
}

bool CBaseCity::initWithInfo(FighterInfo *theInfo,FightGroup theGroup){
    
        
    
    //此处测试用
//    SkillInfo temp ;
//    temp.skillId=-1;
//    temp.damageRatio=1;
//    temp.triggerRatio=1;
    
//    m_VecSkillInfo.push_back(temp);
    
    
    
    if (!CBaseBuilding::initWithInfo(theInfo,theGroup)) {
        return false;
    };
    
    if (theGroup==eFightGroup_midFight) {
        m_bIfReborn = true;//中立塔死后会复活，变为摧毁者一方
    }
    
    
//    m_pTheSrite->setScale(6);
    
    m_pTheHpBar->setScale(1);
    
    m_pTheHpBar->setPosition(0, m_pTheSrite->getBoundingBox().size.height);
    
    m_iDeadlyGroup = theGroup;//防止错误，默认为致死阵营是自己阵营
    
    
    m_v2AttackPosOffset = Vec3(10, 100, 10);
    
    m_vecBaseRotation=Vec3(0,90,-90);
    m_pTheSrite->setRotation3D(m_vecBaseRotation);
    
    
    

    //脚底光圈
    
    Sprite3D* theCircle = Sprite3D::create();
    
    Sprite* tempSprite = Sprite::createWithSpriteFrameName("fight_CityCircle.png");

    
//    tempSprite->setOpacity(255*0.8f);
    
    tempSprite->setScale(m_pTheSrite->getBoundingBox().size.width/tempSprite->getBoundingBox().size.width*1.5);
    theCircle->addChild(tempSprite);
    
    tempSprite->runAction(RepeatForever::create(RotateBy::create(0.5f, 10)));
    
    theCircle->setRotation3D(Vec3(90, 0, 0));
    CFightManager::addCamera(theCircle);
    
    theCircle->setPosition3D(Vec3(0, 1, 0));
    addChild(theCircle);
    
    
    
    
    return true;
    
}

void CBaseCity::collidUpdate(float time){
    
    //重写此方法，什么都不做。因为资源点不会位移
    
    
    
}


void CBaseCity::stateUpdate(float time){
    
    
    CBaseBuilding::stateUpdate(time);
    
    //测试用
    //    ifSkillStart();
    //    skillPrepare();
    //    return;
    
    
    
    
    //重写此方法，因为资源点只判断最后一个经过他的人的属性。
    
    
    
    
    
    
}

void CBaseCity::onDied(DamageType type){
    
    CBaseBuilding::onDied(type);
    
    CFightData::getInstance()->addTheFightConditon(eLevelCondition_destroyCity, 1,this);
    
    CFightData::getInstance()->setTheEnemyCity(NULL);
    
    //城市死了就赢了
    CFightData::getInstance()->setWinState(1,this);
    
    //添加主基地死亡动画
    Sprite* bomb = Sprite::createWithSpriteFrameName("enemy_tower_1.png");
    CFightManager::addCamera(bomb);
    bomb->setGlobalZOrder(1000);
    bomb->setScale(3);
    addChild(bomb);
    char buf[257];
    Animation* pAnimation = Animation::create();
    for (int i = 1; i < 10; i++)
    {
        sprintf(buf, "enemy_tower_%d.png", i);
        pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
    }
    pAnimation->setDelayPerUnit(0.1f);
    if(pAnimation)
    {
        bomb->runAction(Sequence::create(Animate::create(pAnimation), RemoveSelf::create(), NULL));
    }
}

