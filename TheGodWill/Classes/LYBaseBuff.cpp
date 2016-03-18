//
//  LYBaseBuff.cpp
//  TheGodWill
//
//  Created by user on 15/1/7.
//
//

#include "LYBaseBuff.h"

#include "LYFightData.h"

#include "LYFightManager.h"

USING_NS_CC;

static int m_count =0;

CBaseBuff::CBaseBuff(){
    
    
    m_pTheBuffSprite= NULL;
    m_pTheTarget = NULL;
    
    ifNeedRotate = false;
    RotateOffset = Vec3::ZERO;
    
    m_iDuWave = 0;
    
    m_count++;
    
    CCLOG("总共buff数%d",m_count);
    
}

CBaseBuff::~CBaseBuff(){
    
    m_count--;
    
    CCLOG("总共buff数%d",m_count);
    
}


void CBaseBuff::setBuffIdAndStart(AllBuffes theType, float theTime){
    
    
    m_iBuffType = theType;
    
    m_fNowLifeTime =  m_fMaxLifeTime = theTime;
    
    
    onStart();
    
    scheduleUpdate();
}


void CBaseBuff::onStart(){
    
    
    switch (m_iBuffType) {
        case eAllBuffes_bingdong:
        {
            m_pTheBuffSprite = CFightManager::getTheModelWithPath("bingdong8.png");
            
            
            m_pTheBuffSprite->setCameraMask((unsigned short )CameraFlag::USER1);
            
            m_pTheBuffSprite->setPosition3D(Vec3(0, 30, 0));

            
            m_pTheTarget->getTheDebuff()->ifFreeze = true;
            
//            m_pTheTarget->unscheduleUpdate();
            m_pTheTarget->stopAllActions();
            
            m_pTheBuffSprite->setScale(2);
        }
            
            break;
            
        case eAllBuffes_huoyanskin:
        {
            m_pTheTarget->stopAllActions();

            
            m_pTheTarget->getTheDebuff()->ifhuoyanSkin = true;
        
            Sprite* dizzy = Sprite::createWithSpriteFrameName("huoyanpifu9.png");
            CFightManager::addCamera(dizzy);
            dizzy->setGlobalZOrder(1000);
            dizzy->setScale(2);
            addChild(dizzy);
            
            dizzy->setOpacity(220);
            
            dizzy->setPosition3D(Vec3(0, m_pTheTarget->gettheSprite()->getBoundingBox().size.height+35, 0));
            
            Animation * hurtAnimation = Animation::create();
            
            for (int i=1; i<10; i++)
            {
                __String *temp = __String::createWithFormat("huoyanpifu%d.png",i);
                hurtAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(temp->getCString()));
            }
            
            hurtAnimation->setDelayPerUnit(0.1f);
            dizzy->runAction(RepeatForever::create(Animate::create(hurtAnimation)));
            break;
        }
            
        case eAllBuffes_xuanyun:
        {
            
            m_pTheTarget->stopAllActions();
            //            m_pTheTarget->unscheduleUpdate();
            
            m_pTheTarget->animateChange(eFighteState_Dizzy);
            m_pTheTarget->getTheDebuff()->ifDizzy = true;
            
            
            Sprite* dizzy = Sprite::createWithSpriteFrameName("fight_dizzy1.png");
            CFightManager::addCamera(dizzy);
            dizzy->setGlobalZOrder(1000);
            dizzy->setScale(1);
            addChild(dizzy);
            
            dizzy->setOpacity(220);
            
            dizzy->setPosition3D(Vec3(0, m_pTheTarget->gettheSprite()->getBoundingBox().size.height+15, 0));
            
            Animation * hurtAnimation = Animation::create();
            
            for (int i=1; i<8; i++)
            {
                __String *temp = __String::createWithFormat("fight_dizzy%d.png",i);
                hurtAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(temp->getCString()));
            }
            hurtAnimation->setDelayPerUnit(0.1f);
            
            dizzy->runAction(RepeatForever::create(Animate::create(hurtAnimation)));

            break;
        }
        case eAllBuffes_jitui:
        {
            
            //boss免疫击退效果
            if (m_pTheTarget->getIfLevelBoss()) {
                break;
            }
            
            
//            m_pTheTarget->stopAllActions();
            if (!m_pTheTarget->getIfDied()) {
                m_pTheTarget->animateChange(eFighteState_Dizzy);
            }
            m_pTheTarget->getTheDebuff()->ifBeatBack = true;
            
            
            

//            Sprite* dizzy = Sprite::create("fight/fight_dizzy1.png");
//            CFightManager::addCamera(dizzy);
//            dizzy->setGlobalZOrder(1000);
//            dizzy->setScale(0.5f);
//            addChild(dizzy);
//            
//            dizzy->setPosition3D(Vec3(0, m_pTheTarget->gettheSprite()->getBoundingBox().size.height, 0));
//            
//            
//            Animation * hurtAnimation = Animation::create();
//            
//            for (int i=1; i<8; i++)
//            {
//                
//                __String *temp = __String::createWithFormat("fight/fight_dizzy%d.png",i);
//                hurtAnimation->addSpriteFrameWithFile((temp->getCString()));
//            }
//            hurtAnimation->setDelayPerUnit(0.1f);
//            
//            dizzy->runAction(RepeatForever::create(Animate::create(hurtAnimation)));
            
            
            break;
        }
            
            
        case eAllBuffes_chongfeng:
        {
            
            m_pTheBuffSprite = Sprite3D::create();

            Sprite* temp  = Sprite::createWithSpriteFrameName("fight_charge_1.png");
            temp->setScale(2);
            m_pTheBuffSprite->addChild(temp);
            
//            m_pTheBuffSprite->setScale(3);
            
            m_pTheBuffSprite->setCameraMask((unsigned short )CameraFlag::USER1);
            
            m_pTheBuffSprite->setPosition3D(Vec3(0, temp->boundingBox().size.height*0.5f, 0));
            
            
            Animate* theAnimate = CFightManager::getAnimateWithPath("fight_charge_", 1, 8);
            
            temp->runAction(RepeatForever::create(theAnimate));
            
            RotateOffset = Vec3(0,-90,0);

            ifNeedRotate = true;
        }

            break;
            
        case eAllBuffes_du:
        {
            log("发毒减血");
            m_pTheTarget->stopAllActions();


            m_pTheTarget->getTheDebuff()->ifhuoyanSkin = true;
            
            Sprite* dizzy = Sprite::createWithSpriteFrameName("qiansuan1.png");
            CFightManager::addCamera(dizzy);
            dizzy->setGlobalZOrder(1000);
            dizzy->setScale(2);
            //dizzy->setScale(1);
            
            addChild(dizzy);
            
            dizzy->setOpacity(220);
            
            dizzy->setPosition3D(Vec3(0, m_pTheTarget->gettheSprite()->getBoundingBox().size.height-50, 0));
            
            Animation * hurtAnimation = Animation::create();
            
            for (int i=1; i<7; i++)
            {
                __String *temp = __String::createWithFormat("qiansuan%d.png",i);
                hurtAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(temp->getCString()));
            }
            
            hurtAnimation->setDelayPerUnit(0.1f);
            dizzy->runAction(RepeatForever::create(Animate::create(hurtAnimation)));
    
        }
            break;
            
        default:
            break;
    }
    
    if (m_pTheBuffSprite) {
        addChild(m_pTheBuffSprite,100);
    }
    
    //调整buff大小
//    if(m_pTheBuffSprite){
//        
//        float temp1 = m_pTheTarget->gettheSprite()->getBoundingBox().size.height;
//        
//        float temp2 = m_pTheBuffSprite->getBoundingBox().size.height;
//        if (m_pTheBuffSprite->getChildByTag(100)) {//如果是帧动画，则不能使用模型来获取height，而应该用模型里的sprite
//            temp2 = m_pTheBuffSprite->getChildByTag(100)->getBoundingBox().size.height;
//        }
//        
//        float tempScale = temp1/temp2*0.5f;
//        m_pTheBuffSprite->setScale(tempScale*m_pTheBuffSprite->getScale());
//    }
    
    
}

void CBaseBuff::onEnd(){
    
    
    switch (m_iBuffType) {
        case eAllBuffes_chongfeng:
        {
            
            //对敌人造成范围伤害。并且根据加成速度值造成击退效果
            Vector<CBaseFighter*>targetVec = CFightData::getInstance()->getFightTargetByGroup(m_pTheTarget->getFightGroup());
            
            
            for (int i=0; i<targetVec.size(); i++) {
                
                
                
                CBaseFighter* theTarget = targetVec.at(i);
                
                
                if (theTarget->getIfDied()) {//如果敌人死亡了，则跳过
                    continue;
                }
                
                //伤害距离内的造成伤害
                float distance = m_pTheTarget->getPosition3D().distance(theTarget->getPosition3D()) - theTarget->getNowRadius();
                
                if (distance<120) {//影响范围50
                    
                    float addRatio = m_pTheTarget->getAddMoveSpeed()/6;//每6点速度，代表加速了3秒
                    
                    //造成伤害,伤害值根据速度加成
                    float damageRatio = addRatio*1.2f;//每6的速度，加0.5倍攻击力
                    
                    
                    theTarget->beAttacked(m_pTheTarget,m_fdamageBase*(damageRatio),1,m_pTheTarget->getFightGroup(),eDamageType_SoliderSkill);
                    
                    
                    //如果怪物没死，则按速度百分比造成击退效果
                    
                    if (!theTarget->getIfDied()||true) {
                        if (addRatio>1) {
                            
                            float dist = addRatio*200;//每6点速度，弹出去100码
                            float time = 0.5f;
                            
                            theTarget->beatBackFrom(m_pTheTarget->getPosition3D(), dist, time,50);
                            CFightManager::addTheBuffToFighter(theTarget, (AllBuffes)eAllBuffes_jitui,time);
                            
                            
                        }else{
                            theTarget->animateChange(eFighteState_Hurt);
                        }
                    }
                    

                }

            }
            
            m_pTheTarget->setAddMoveSpeed(0);
            
            
        }
            break;
            
            
        case eAllBuffes_jiansu:
            break;
            
        case eAllBuffes_du:

            break;
            
        default:
            break;
    }
    
    
    onDestroy();
    
}

void CBaseBuff::onDestroy(bool ifRemoveSelf){
    
    unscheduleUpdate();
    
    if (m_pTheTarget) {
        m_pTheTarget->getAllBuffes()->eraseObject(this);
        m_pTheTarget = NULL;
    }
    
    
    if(ifRemoveSelf){
        removeFromParentAndCleanup(true);
    }
    
    
    
}



void CBaseBuff::update(float time){
    
    
    if (m_fMaxLifeTime!=-1) {//当最大生命周期不是-1时，则计算生命时间
        
        if((int)(m_fMaxLifeTime-m_fNowLifeTime)/1>m_iDuWave&&m_iBuffType==eAllBuffes_du)
        {
            m_iDuWave++;

            //毒技能每秒减血
            
            m_pTheTarget->beAttacked(NULL,m_fdamageBase);
            
        }
        
        m_fNowLifeTime -= time;
        
        if (m_fNowLifeTime<=0) {
            
            onEnd();
            
        }

    }

    
    
    if (m_pTheBuffSprite&&ifNeedRotate)
    {
        Vec3 rotation = m_pTheTarget->gettheSprite()->getRotation3D()+RotateOffset;
        
        m_pTheBuffSprite->setRotation3D(rotation);
    }

    
    
    switch (m_iBuffType) {
        case eAllBuffes_chongfeng:
        {
            
            //如果怪物是移动状态，则加速，否则终结自己。并对敌人造成伤害
            
            if (m_pTheTarget->getFightState()==eFighteState_Walk) {
                float addSpeedNow = m_pTheTarget->getAddMoveSpeed();
                addSpeedNow+=time*(4);
                m_pTheTarget->setAddMoveSpeed(addSpeedNow);
                
                if (m_pTheTarget->getAddMoveSpeed()>3) { //冲锋效果在满足一定条件时才会出现
                    m_pTheBuffSprite->setVisible(true);
                }else{
                    m_pTheBuffSprite->setVisible(false);
                }
                
                
            }else{
                
                //撞击时新增粒子特效
                if (m_pTheTarget->getAddMoveSpeed()>3) { //在满足一定条件时才会出现
                    ParticleSystem*explodePar = ParticleSystemQuad::create("particleSystem/impact.plist") ;
                    explodePar->setScale(1);
                    explodePar->setPosition3D(Vec3(0, m_pTheTarget->gettheSprite()->getBoundingBox().size.height / 10, 0)) ;
                    explodePar->setCameraMask((unsigned short )CameraFlag::USER1);
                    explodePar->setAutoRemoveOnFinish(true);
                    m_pTheTarget->addChild(explodePar, 11, 100) ;
                    
                    explodePar->setPosition3D(Vec3(m_pTheTarget->gettheSprite()->getBoundingBox().size.width*0.5f,m_pTheTarget->gettheSprite()->getBoundingBox().size.height*0.5f,0));
                    
                }
                
                
                
                
                
                onEnd();
            }
            
            
            
            
            
            
            break;
        }
        case eAllBuffes_du:
            
            //每秒造成多少伤害

            
        
            break;
            
        default:
            break;
    }

    
    
    
}


void CBaseBuff::onExit(){
    
    
    onDestroy(false);
    
    Node::onExit();
    
    
}

