//
//  CNpcFighter.cpp
//  WarOfGod
//
//  Created by prefer_shan on 15/8/3.
//
//

#include "CNpcFighter.h"
#include "LYFightData.h"
#include "DBMgr.h"

USING_NS_CC;


CNpcFighter::CNpcFighter()
{
    m_parentLayer = NULL;

    m_iMode = 0;

}

CNpcFighter::~CNpcFighter()
{
    
}


CNpcFighter * CNpcFighter::create(FighterInfo *theInfo,int iMonsterId)
{
 
    CNpcFighter * temp = new CNpcFighter();
    
    temp->m_iAttackType=eAttackType_NPC;
    
    if (temp && temp->initWithInfo(theInfo,iMonsterId))
    {
        
        temp->autorelease();
        return temp;
    }else{
        
        delete temp;
        temp = NULL;
        return NULL;
    }
    
}


void  CNpcFighter::setPvpMainLayer(pvpMainLayer*p)
{
    m_parentLayer = p;
}



bool CNpcFighter::initWithInfo(FighterInfo *theInfo,int iMonsterId)
{
    m_strTheInfo = theInfo;
    

    
    iMonsterId  =  iMonsterId;
    
    m_pInfo = CDBMgr::getInstance()->GetModleInfoById(iMonsterId);

    initModelWithPath(m_pInfo->modelName.getCString());
    
    
    m_pShadow = Sprite3D::create();
    Sprite* temp = Sprite::createWithSpriteFrameName("touying.png");
    m_pShadow->addChild(temp);
    
    temp ->setOpacity(170);
    
    float scale = m_pTheSrite->getBoundingBox().size.width/temp->getBoundingBox().size.width*0.6f;
    
    
    if(iMonsterId== 20 ||iMonsterId == 25)
    {
        scale = scale*5;
        
    }
    
    
    m_pShadow->setRotation3D(Vec3(90,0,0));
    
    temp->setScale(scale);
    addChild(m_pShadow);

    
    return true;
}


void CNpcFighter::initModelWithPath(std::string path)
{
    m_pTheSrite  =  Sprite3D::create(path);
    m_pTheSrite->setScale(m_pInfo->modelScale);

    

    
    addChild(m_pTheSrite,1);
    
   
    
    
    CFightManager::addCamera(m_pTheSrite);
   
    initAnimate();
    
    if(m_iMode == 2)
    {
        playAniamteAction(m_pAction_Attack,true);
    }
    else 
    {
        playAniamteAction(m_pAction_Walk,true);
    }
}


void CNpcFighter::roatationUpdate(float time)
{
     CBaseFighter::roatationUpdate(time);
  
}

void CNpcFighter::setAddMoveSpeed(float var)
{
    m_fAddMoveSpeed = var*m_strTheInfo->theMonsterInfo->walkSpeed;

    
}
    
    


void CNpcFighter::positionUpdate(float time)
{
    
    m_bIfSearchMode  = true;
    

    if(m_iMode == 0)
    {
        float fDist = m_v3SearchPos.distance(getPosition3D());
    
        
        if (fDist<getNowSpeedWithOutFrame(time))
        {
            vector<Vec3>  pp;
            pp =  m_parentLayer->getNpcValidPlace(pp);
            
            int i = rand()%pp.size();
        
            m_v3SearchPos = pp[i];
        }
        
    }
    else  if(m_iMode == 1)
    {
        float fDist = m_v3SearchPos.distance(getPosition3D());
        
        if(fDist<getNowSpeedWithOutFrame(time))
        {
           
            int x =  random(Start3DPOINT,END3DPOINT);
            
            int y =  random(ENDZPOINT,STARTZPOINT);
            
            
            m_v3SearchPos = Vec3(x,0,y);
         
        }
    }
    else if(m_iMode == 2)
    {
    
        if(getPosition3D() == m_v3SearchPos)
        {
            setVisible(false);
              unscheduleUpdate();
            removeFromParent();
          
            
            return ;
            
        }
        
    }

    
    CBaseFighter::positionUpdate(time);
    
}




void CNpcFighter::update(float time)
{


    roatationUpdate(time);
    positionUpdate(time);
    
}


void CNpcFighter::SetDest(Vec3 dst)
{
    if(m_iMode ==2)
    {
          playAniamteAction(m_pAction_Attack,true);
    }
    m_v3SearchPos   = dst;
    
}

void CNpcFighter::SetRunAway()
{
    
}

void CNpcFighter::initAnimate()
{
    m_pAction_IDle=NULL;
    m_pAction_Walk = NULL;
    m_pAction_Attack=NULL;
    m_pAction_specialAttack = NULL;
    m_pAction_Hurt = NULL;
    m_pAction_Die = NULL;
    m_pAction_Dizzy = NULL;
    
    
    auto animation = cocos2d::Animation3D::create(m_pInfo->modelName.getCString());
    if (animation)
    {
        
        cocos2d::Animate3D *animate=NULL;
        //动画初始化
        if (m_pInfo->idleFrame.duration!=0&&m_pInfo->idleFrame.duration!=1) {
            animate = cocos2d::Animate3D::create(animation,m_pInfo->idleFrame.start/60,m_pInfo->idleFrame.duration/60);
            //        animate->setSpeed(0.2f);//测试用，换模型了就删掉
            m_pAction_IDle = animate;
            m_pAction_IDle->retain();
            m_pAction_IDle->setTag(SOLDIER_ANIMATE_TAG);
        }
        
        animate=NULL;
        if (m_pInfo->walkFrame.duration!=0&&m_pInfo->walkFrame.duration!=1) {
            animate = cocos2d::Animate3D::create(animation,m_pInfo->walkFrame.start/60,m_pInfo->walkFrame.duration/60);
        
            animate->setSpeed(0.5f);//测试用，换模型了就删掉
            m_pAction_Walk = animate;
            m_pAction_Walk->retain();
            m_pAction_Walk->setTag(SOLDIER_ANIMATE_TAG);
        }
        
        animate=NULL;
        if (m_pInfo->attackFrame.duration!=0&&m_pInfo->attackFrame.duration!=1) {
            animate = cocos2d::Animate3D::create(animation,m_pInfo->attackFrame.start/60,m_pInfo->attackFrame.duration/60);
                  //animate->setSpeed(0.1f);//测试用，换模型了就删掉
            
             animate->setSpeed(0.5f);
            
            m_pAction_Attack = animate;
            m_pAction_Attack->retain();
            m_pAction_Attack->setTag(SOLDIER_ANIMATE_TAG);
        }
        
        animate=NULL;
        if (m_pInfo->specialAttackFrame.duration!=0&&m_pInfo->specialAttackFrame.duration!=1) {
            animate = cocos2d::Animate3D::create(animation,m_pInfo->specialAttackFrame.start/60,m_pInfo->specialAttackFrame.duration/60);
            //        animate->setSpeed(0.5f);//测试用，换模型了就删掉
            m_pAction_specialAttack = animate;
            m_pAction_specialAttack->retain();
            m_pAction_specialAttack->setTag(SOLDIER_ANIMATE_TAG);
        }
        
        animate=NULL;
        if (m_pInfo->hurtFrame.duration!=0&&m_pInfo->hurtFrame.duration!=1) {
            animate = cocos2d::Animate3D::create(animation,m_pInfo->hurtFrame.start/60,m_pInfo->hurtFrame.duration/60);
            //        animate->setSpeed(0.5f);//测试用，换模型了就删掉
            m_pAction_Hurt = animate;
            m_pAction_Hurt->retain();
            m_pAction_Hurt->setTag(SOLDIER_ANIMATE_TAG);
        }
        
        animate=NULL;
        if (m_pInfo->DieFrame.duration!=0&&m_pInfo->DieFrame.duration!=1) {
            animate = cocos2d::Animate3D::create(animation,m_pInfo->DieFrame.start/60,m_pInfo->DieFrame.duration/60);
            //        animate->setSpeed(0.5f);//测试用，换模型了就删掉
            m_pAction_Die = animate;
            m_pAction_Die->retain();
            m_pAction_Die->setTag(SOLDIER_ANIMATE_TAG);
        }
        
        animate=NULL;
        if (m_pInfo->DizzyFrame.duration!=0&&m_pInfo->DizzyFrame.duration!=1) {
            animate = cocos2d::Animate3D::create(animation,m_pInfo->DizzyFrame.start/60,m_pInfo->DizzyFrame.duration/60);
            //        animate->setSpeed(0.5f);//测试用，换模型了就删掉
            m_pAction_Dizzy = animate;
            m_pAction_Dizzy->retain();
            m_pAction_Dizzy->setTag(SOLDIER_ANIMATE_TAG);
        }
        
        

        //默认为空闲
        m_iFightState = eFighteState_Idle;
        m_iAnimateState = eFighteState_None;
        //        m_pTheSrite->runAction(m_pAction_IDle);
        
    
    }

}





