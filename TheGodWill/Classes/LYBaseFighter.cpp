//
//  LYBaseFighter.cpp
//  TheGodWill
//
//  Created by user on 14/12/1.
//
//

#include "LYBaseFighter.h"

#include "AllHead.h"

#include "LYFightData.h"

#include "cocos2d.h"
#include "BillBoardLable.h"

#include "LYBaseAttack.h"
#include "SmallMap.h"

#include "LYBaseBuff.h"
#include "DataManager.h"

#include "Common.h"

#include "LYFightTalk.h"
#include "guiderManagerLayer.h"

#include "CSharedPlist.h"

#include "LYFightScenePVP.h"

USING_NS_CC;

static int fighterNum = 0;

CBaseFighter::CBaseFighter(){
    
    m_pAction_IDle=NULL;
    m_pAction_Walk = NULL;
    m_pAction_Attack=NULL;
    m_pAction_specialAttack = NULL;
    m_pAction_Hurt = NULL;
    m_pAction_Die = NULL;
    m_pAction_Dizzy = NULL;
    
    m_strTheInfo = NULL;
    
    m_pAttackTarget = NULL;
    
    m_pDeadlyTarget = NULL;
    
    m_pSkillTarget = NULL;
    
    m_iNextSkill = NULL;
    
    m_pPassiveSkill = NULL;
    
    m_pTheSrite = NULL;
    
    m_pTheHpBar = NULL;
    
    walkingSmoke = NULL;
    
    m_pShadow = NULL;
    
    m_pShadow2 = NULL;
    
    eleteStar = NULL;
    
    thePathInfo = NULL;
    
    m_pTheTalk = NULL;
    
    m_pTheActiveCircle = NULL;
    
    m_pMapSprite = NULL;

    m_fReDamagexishu = -1;
    
    m_bIfDied=false;
    m_bIfSearchMode=false;
    m_bIfReborn =false;
    m_fAttackColdNow=0;
    m_MoveTime = 0;
    m_offSet = 0;
    m_ifValid = false;
    m_isCut = false;
    m_v2MoveControll = Vec2(0,0);
    
    m_fAddMoveSpeed = 0;
    
    m_distance = 0;
    
    m_bIfCampMonster = false;
    
    m_bIfLevelBoss = false;
    
    m_vec2IdlePos= Vec2::ZERO;
    
    m_fSpriteScale=1;
    
        
    m_fNowTalkCold=0;
    m_fMaxTalkCold = 5;
    
    m_bIfElite = false;
    
    
    m_iAddHpDaoBin = 0;
    
    m_bIfReinforce =false;
    
    CSharedPlist::getInstance()->pushPlist("plist/skillName.plist");
    
    fighterNum++;
    
    m_bIfActive = false;
    
    CCLOG("剩余士兵数%d",fighterNum);
    
    
    m_fLiveTime = 0;
    
    m_FightStateTime=0;
    

    skilltimes=0;
    
    skillRatios[0] = 1.0f;
    skillRatios[1] = 1.0f;
    skillRatios[2] = 1.0f;
    skillRatios[3] = 1.0f;

    m_bWanshi = true;
    m_bKulouDie = true;
    
    m_bShixue = false;
    
    m_iHuoYanSkinNum = 1;
    
    m_pEliteCircle = NULL;
    
    m_bzhaohuan = false;
    
    _hurtShader = NULL;
    
    _hurtShaderBuilding = NULL;
    
    bossShader = NULL;
    
    eliteShader = NULL;
    
    m_bSoul = true;
    
    m_iTrueId=-1;
    m_pvpShadow = NULL;
    m_pvpDizuo = NULL;
    
    m_bIfBombed = false;
    m_fWushuXishu = 0;
    m_fZiranXishu = 0;
    m_fYexinXishu = 0;
    m_bStop = false;
    
    m_bIfSmallBoss = false;
    
}

CBaseFighter::~CBaseFighter(){
    
    fighterNum--;
    CCLOG("剩余士兵数%d",fighterNum);
    
    CSharedPlist::getInstance()->erasePlist("plist/skillName.plist");
    
    
    clearAllSkillAndBulletAndBuff(true);
    
}


CBaseFighter *CBaseFighter::create(FighterInfo *theInfo,FightGroup theGroup,bool ifBoss,bool ifElite){
    
    CBaseFighter * temp = new CBaseFighter();
    
//    CCLOG("pvpNowFightTime--addsoldier::%f",CFightData::getInstance()->getAllFightTime());
    
    
    if (temp) {
        temp->m_iAttackType = eAttackType_monster;
        
        temp->m_bIfLevelBoss = ifBoss;
        
        temp->m_bIfElite = ifElite;
    }
    
    if (temp && temp->initWithInfo(theInfo,theGroup))
    {
        
        temp->autorelease();
        return temp;
    }else{
        
        delete temp;
        temp = NULL;
        return NULL;
    }
    
}

bool CBaseFighter::initWithInfo(FighterInfo *theInfo,FightGroup theGroup){
    
    if (!Node::init()) {
        return false;
    }
    
    
    //测试用数据，看到就删
    //    theInfo.attk = 1;
    
    //预加载所有shader效果
//    CFightManager::preloadShader();

    
    
    m_strTheInfo = theInfo;
    
    
    m_iFightGroup = theGroup;
    
    
    /*
     数据初始化
     */
    
   
    m_iMaxHP = m_iNowHP = m_strTheInfo->maxHP;
    
    //m_iMaxHP = 1;
    //m_iNowHP = 1;
    
    
    m_iNowAttack = m_strTheInfo->attk;
    
    m_iNowRadius = m_strTheInfo->theModelInfo->radius;
    
    //此处测试用
    //    SkillInfo temp ;
    //    temp.skillId=1;
    //    temp.damageRatio=3;
    //    temp.triggerRatio=0.1f;
    //
    //    m_VecSkillInfo.push_back(temp);
    

    
    /*
     表现初始化
     */
    
    
    //初始化模型
    initModelWithPath(m_strTheInfo->theModelInfo->modelName.getCString());
    
    
    
    //初始化血条
    m_pTheHpBar = CHPBar::create();
    
    
    //怪物血条和士兵血条做区分
    if (m_iFightGroup==eFightGroup_Freind) {
        
        if (theInfo==CFightData::getInstance()->getTheFriendCityInfo()) {//英雄血条和其他普通士兵血条不同
            m_pTheHpBar->setHPSpriteWithPath("Fight_monsterVirtualHP.png", "Fight_monsterHPBG.png");
        }else{
            m_pTheHpBar->setHPSpriteWithPath("Fight_soldierHP.png", "Fight_enemyBg.png");
        }
        
//        m_pTheHpBar->setScale(0.2f);
    }else if (m_iFightGroup==eFightGroup_Enemy) {
        m_pTheHpBar->setHPSpriteWithPath("Fight_enemyHP.png", "Fight_enemyBg.png");
//        m_pTheHpBar->setScale(0.5f);
    }else{
        m_pTheHpBar->setHPSpriteWithPath("Fight_enemyHP.png", "Fight_enemyBg.png");
//        m_pTheHpBar->setScale(0.5f);
    }
    
    addChild(m_pTheHpBar,100);
//    m_pTheHpBar->setGlobalZOrder(999);
    
     m_pTheHpBar->setPosition(Vec2( 0, m_pTheSrite->getBoundingBox().size.height+m_pTheHpBar->getBGHP()->getBoundingBox().size.height*0.5f)+m_pTheSrite->getPosition());
//    m_pTheHpBar->setBlendFunc(cocos2d::BlendFunc::ALPHA_NON_PREMULTIPLIED);
    
    CFightManager::addCamera(m_pTheHpBar);
    
    
    if(m_bIfElite){ //精英相关初始化
        setElite(true);
    }
   
//    m_pTheHpBar->setPosition(0, m_pTheSrite->getBoundingBox().size.height);
    
    //    Label *test = Label::createWithSystemFont("aaaaaa", "Arial", 1000);
    //    test->setPosition(Point(0,150));
    //    addChild(test,11111);
    
    
    
    //测试用，先删除
    //小地图显示
    if(!CPersonalData::getInstance()->getPvpMainLayer())
    {
        if (m_iAttackType==eAttackType_monster)
        {
            m_pMapSprite =  CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(Vec3::ZERO,theInfo->monsterType, theGroup,eAttackType_monster,m_bIfLevelBoss);
            
        }else if (m_iAttackType==eAttackType_walker)
        {
            m_pMapSprite =  CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(Vec3::ZERO,eFighterType_line,eFightGroup_Freind,eAttackType_walker);
        }
        else{
            
            m_pMapSprite =  CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(Vec3::ZERO,theInfo->monsterType, theGroup);
        }
    }
    
    

    
    initAnimate();
    
    //bornAnimate();
    
    
    if(!CPersonalData::getInstance()->getPvpMainLayer())//pvp主界面下不调用建筑更新函数
    {
        scheduleUpdate();
    }
    
    
    //预加载受击的闪白shader效果
//    initShader();   //暂时弃用，切屏幕会引起bug  by ly
    
    
    
//    CFightManager::addShaderToSprite3D(m_pTheSrite,eShadeType_3D_Elite);

    return true;
}

void CBaseFighter::onEnterTransitionDidFinish(){
    
    Node::onEnterTransitionDidFinish();
    
//    //预加载受击的闪白shader效果
//    _hurtShader = (CSprite3dAnimateShaderOnce*)CFightManager::addShaderToSprite3D(m_pTheSrite, eShadeType_3D_beAttacked);
    
}


void CBaseFighter::setAddHpBlood(CBaseFighter *pFighter,int iAddHP)
{
    int iTmpMaxHp = pFighter->m_iMaxHP;
    
    pFighter->m_iMaxHP += iAddHP;
    
    float fMaxHp = pFighter->m_iMaxHP;
    
    pFighter->m_iNowHP =   pFighter->m_iNowHP *(fMaxHp/iTmpMaxHp);
}


void CBaseFighter::RefreshFighter()
{
    
    for(int i=0;i<m_strTheInfo->theFighterSkillInfo.size();i++)
    {
        
        if(m_strTheInfo->theFighterSkillInfo[i].skillLevel==0)
        {
            continue;
            
        }
        
        float fDamageRation = m_strTheInfo->theFighterSkillInfo[i].damageRatio;
        
        

        if(m_strTheInfo->theFighterSkillInfo[i].theSkillInfo->skillId == eAllSkills_GuduYanxi)
        {
            m_iNowAttack +=  m_iNowAttack*fDamageRation;

        }
        else if(m_strTheInfo->theFighterSkillInfo[i].theSkillInfo->skillId == eAllSkills_wushuyanxi)
        {
            m_fWushuXishu = fDamageRation;
        }
        else if(m_strTheInfo->theFighterSkillInfo[i].theSkillInfo->skillId == eAllSkills_yexinggrow)
        {
            m_fYexinXishu = fDamageRation;
        }
        else if(m_strTheInfo->theFighterSkillInfo[i].theSkillInfo->skillId == eAllSkills_ziranyuhe)
        {
            m_fZiranXishu = fDamageRation;
            
        }
       

        
        else if(m_strTheInfo->theFighterSkillInfo[i].theSkillInfo->skillId == eAllSkills_yushou)
        {
            m_iNowAttack=  m_strTheInfo->attk + m_iNowHP*fDamageRation;
            
        }
    }
    
    
    
    cocos2d::Vector<CBaseFighter*>  pSolodier  = *CFightData::getInstance()->getAllSoldier();
    cocos2d::Vector<CBaseFighter*> pDaoBin;

    
    bool bTemp = ifSkillStart(3);
 
    if (bTemp&&m_iNextSkill)
    {
        float fDamage = m_iNextSkill->damageRatio;
        
        switch (m_iNextSkill->theSkillInfo->skillId)
        {
            case eAllSkills_Bintuanyoushi:
            {
                for(int i=0;i<pSolodier.size();i++)
                {
                    if(pSolodier.at(i)->m_strTheInfo->monsterId == 1)
                    {
                        pDaoBin.pushBack(pSolodier.at(i));
                    }
                }
                
                ssize_t iSize = pDaoBin.size();
                
                for(int i=0;i< pDaoBin.size();i++)
                {
                    if(iSize>1)
                    {
                        if(!pDaoBin.at(i)->getIfDied())
                        {
                            pDaoBin.at(i)->skillNameShow(m_iNextSkill->theSkillInfo->skillNamePic);
                            m_iAddHpDaoBin = pDaoBin.at(i)->m_iNowAttack*fDamage;
                            pDaoBin.at(i)->m_iAddHpDaoBin = m_iAddHpDaoBin;
                            pDaoBin.at(i)->m_iNowAttack += m_iAddHpDaoBin;
                        }
                        
                    }
                }
            }
                break;
                
            case eAllSkills_juDun:  //举盾
            {
                m_fReDamagexishu =  fDamage;
                break;
            }
    
            case eAllSkills_linTI:
            {
                m_fReDamagexishu = fDamage;
                break;
            }
            
            case  eAllSkills_Huanjiakaijia:
            {
                m_fReDamagexishu = fDamage;
                break;
            }
                
            case eAllSkills_xixue:
            {
                m_fReDamagexishu = fDamage;
            }
                break;

                
            case eAllSkills_Shixue:
            {
                m_fReDamagexishu = fDamage;
            }
                break;

                
            default:
                break;
    
                
        }
    }
    
}


void CBaseFighter::bornAnimate()
{
  

}


void CBaseFighter::initModelWithPath(std::string path){
    
    /*
     表现初始化
     */
    m_pTheSrite  =  Sprite3D::create(path);
    m_pTheSrite->setScale(m_strTheInfo->theModelInfo->modelScale);
    
    addChild(m_pTheSrite,1);
    CFightManager::addCamera(m_pTheSrite);
    
    

    
    asyncLoad_Callback(NULL,NULL);
    
//    Sprite3D::createAsync(path, CC_CALLBACK_2(CBaseFighter::asyncLoad_Callback, this), NULL);
    

    //攻击开始位置偏移初始化
    m_v2AttackPosOffset = Vec3(10,0,-10);
    

}


void CBaseFighter::asyncLoad_Callback(cocos2d::Sprite3D *sprite, void *param){
    
    
//    m_pTheSrite = sprite;
    

    
    //初始化精灵
    
    
    
    
    
    //是否空军，空军的话，位置上移
    if (m_strTheInfo->theMonsterInfo->ifFly) {
        m_pTheSrite->setPosition3D(Vec3(0, 130, 0));
        
    }
    
//    //抗锯齿
//    if(m_pTheSrite&&m_pTheSrite->getMeshCount()>0){
//        Texture2D* tem= m_pTheSrite->getMesh()->getTexture();
//        tem->generateMipmap();
//        cocos2d::Texture2D::TexParams tempOne ={GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE};
//        tem->setTexParameters(tempOne);
//    }
    
    
    
    
//    m_pTheSrite->setScale(m_strTheInfo->theModelInfo->modelScale);
    
    
    
    float scale = 1;
    
    //加入阴影
    if (m_strTheInfo->monsterType!=eFighterType_Tower&&m_strTheInfo->monsterType!=eFighterType_Trap) {
        
        m_pShadow = Sprite3D::create();
        Sprite* temp = Sprite::createWithSpriteFrameName("touying.png");
        m_pShadow->addChild(temp);
        
        temp ->setOpacity(170);
        
        scale = m_pTheSrite->getBoundingBox().size.width/temp->getBoundingBox().size.width*0.9f;
        if((m_strTheInfo->monsterId == 14) || (m_strTheInfo->monsterId == 15))
        {
            scale = m_pTheSrite->getBoundingBox().size.width/temp->getBoundingBox().size.width*0.6f;
        }
        m_pShadow->setRotation3D(Vec3(90,0,0));
        
        temp->setScale(scale);
        addChild(m_pShadow);
    }else{
        if(!((CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE) || (CPersonalData::getInstance()->getPvpMainLayer())))
        {
            //        m_pShadow = Sprite3D::create("models/Others/jianzhuyy.c3b");
            m_pShadow = Sprite3D::create();
            Sprite* temp = Sprite::createWithSpriteFrameName("fight_light.png");   //fight_light.png
            m_pShadow->addChild(temp);
            
            scale = m_pTheSrite->getBoundingBox().size.width/temp->getBoundingBox().size.width*1.2f;
            temp->setScale(scale);    //ly::这里有个神奇的坑，如果放大的是承载的sprite3d，则稍微大点就显示不正确，如果放大的是sprite，则不会出现这种情况
            
            m_pShadow->setRotation3D(Vec3(90,0,0));   //ly： 这里有个神奇的坑，如果角度转为-90度的话，就会出现渲染层级出错的bug。
            m_pShadow->setPosition3D(Vec3(0, 2, 0));
            //        m_pShadow->setOpacity(200);
            addChild(m_pShadow);
        }
//        m_pShadow2 = Sprite3D::create();//屏蔽模型的阴影 add by zxh 2015-08-07 11:57:52
//        Sprite* temp2 = Sprite::createWithSpriteFrameName("touying.png");
//        m_pShadow2->addChild(temp2);
//        temp2->setOpacity(190);
//        float scale2 = m_pTheSrite->getBoundingBox().size.width/temp2->getBoundingBox().size.width*1.5f+0.3f;
//        temp2->setScale(scale2);
//        
//        m_pShadow2->setRotation3D(Vec3(90,0,0));
//        m_pShadow2->setPosition3D(Vec3(0, 10, 0));
//        addChild(m_pShadow2);
//        if(!CPersonalData::getInstance()->getPvpMainLayer()&&CFightData::getInstance()->getFightType()!=FIGHT_PVP_TYPE){  //非pvp时，阴影透明度降低。不然太难看
//            
//            temp2->setOpacity(40);
//        }

        
//            if (m_iAttackType==eAttackType_wall) { //城墙由于不是一个圆形，所以圆形光太抢，需要透明度降低
//                temp->setOpacity(100);
//                temp2->setOpacity(30);
//            }
//        }
        
        
    }
    
//    CFightManager::addCamera(m_pShadow);
    
//    m_pShadow->setScale(scale);
    
    
    Vec3 targetVec = m_pTheSrite->getRotation3D()+Vec3(0,90,0);
    
    m_pTheSrite->setRotation3D(targetVec);
    
    

    //英雄相关初始化
    
    if (m_iAttackType==eAttackType_hero) {
        //脚底光圈
        
        Sprite3D* theCircle = Sprite3D::create();
        
        Sprite* tempSprite = Sprite::createWithSpriteFrameName("fight_HeroCircle.png");
        
        
        //    tempSprite->setOpacity(255*0.8f);
        
        tempSprite->setScale(m_pTheSrite->getBoundingBox().size.width/tempSprite->getBoundingBox().size.width*2.5);
        theCircle->addChild(tempSprite);
        
        //    tempSprite->runAction(RepeatForever::create(RotateBy::create(0.5f, 30)));
        
        theCircle->setRotation3D(Vec3(90, 0, 0));
        CFightManager::addCamera(theCircle);
        addChild(theCircle);

    }
    

}



void CBaseFighter::initAnimate(){

    
    //测试用，修改动画
    //    m_strTheInfo->theModelInfo->hurtFrame.duration=44;
    
    //一些建筑是不需要动画的
    
    
    m_pAction_IDle=NULL;
    m_pAction_Walk = NULL;
    m_pAction_Attack=NULL;
    m_pAction_specialAttack = NULL;
    m_pAction_Hurt = NULL;
    m_pAction_Die = NULL;
    m_pAction_Dizzy = NULL;
    
    
    
//    if (m_iAttackType==eAttackType_city||m_iAttackType==eAttackType_tower) {
//        return;
//    }
    
    
    
    
    auto animation = Animation3D::create(m_strTheInfo->theModelInfo->modelName.getCString());
    if (animation)
    {
        
        Animate3D *animate=NULL;
        //动画初始化
        if (m_strTheInfo->theModelInfo->idleFrame.duration!=0&&m_strTheInfo->theModelInfo->idleFrame.duration!=1) {
            animate = Animate3D::create(animation,m_strTheInfo->theModelInfo->idleFrame.start/60,m_strTheInfo->theModelInfo->idleFrame.duration/60);
            //        animate->setSpeed(0.2f);//测试用，换模型了就删掉
            
            if(m_strTheInfo->monsterId==14||m_strTheInfo->monsterId == 15)
            {
               animate->setSpeed(0.2f);
            }
            else if(m_strTheInfo->monsterId>=17 && m_strTheInfo->monsterId<=28)
             {
                  animate->setSpeed(0.1f);
             }
            m_pAction_IDle = animate;
            m_pAction_IDle->retain();
            m_pAction_IDle->setTag(SOLDIER_ANIMATE_TAG);
        }
        
        animate=NULL;
        if (m_strTheInfo->theModelInfo->walkFrame.duration!=0&&m_strTheInfo->theModelInfo->walkFrame.duration!=1) {
            animate = Animate3D::create(animation,m_strTheInfo->theModelInfo->walkFrame.start/60,m_strTheInfo->theModelInfo->walkFrame.duration/60);
            //        animate->setSpeed(0.5f);//测试用，换模型了就删
            if(m_strTheInfo->monsterId>=14 && m_strTheInfo->monsterId<=28)
            {
                animate->setSpeed(0.4f);
            }
            
            m_pAction_Walk = animate;
            m_pAction_Walk->retain();
            m_pAction_Walk->setTag(SOLDIER_ANIMATE_TAG);
        }
        
        animate=NULL;
        if (m_strTheInfo->theModelInfo->attackFrame.duration!=0&&m_strTheInfo->theModelInfo->attackFrame.duration!=1) {
            animate = Animate3D::create(animation,m_strTheInfo->theModelInfo->attackFrame.start/60,m_strTheInfo->theModelInfo->attackFrame.duration/60);
            
            if(m_strTheInfo->monsterId>=14 && m_strTheInfo->monsterId<=28)
            {
                animate->setSpeed(0.5f);
            }
        
            
            m_pAction_Attack = animate;
            m_pAction_Attack->retain();
            m_pAction_Attack->setTag(SOLDIER_ANIMATE_TAG);
        }
        
        animate=NULL;
        if (m_strTheInfo->theModelInfo->specialAttackFrame.duration!=0&&m_strTheInfo->theModelInfo->specialAttackFrame.duration!=1) {
            animate = Animate3D::create(animation,m_strTheInfo->theModelInfo->specialAttackFrame.start/60,m_strTheInfo->theModelInfo->specialAttackFrame.duration/60);
            //        animate->setSpeed(0.5f);//测试用，换模型了就删掉
             if(m_strTheInfo->monsterId>=14 && m_strTheInfo->monsterId<=28)
            {
                animate->setSpeed(0.5f);
            }
            
            m_pAction_specialAttack = animate;
            m_pAction_specialAttack->retain();
            m_pAction_specialAttack->setTag(SOLDIER_ANIMATE_TAG);
        }
        
        animate=NULL;
        if (m_strTheInfo->theModelInfo->hurtFrame.duration!=0&&m_strTheInfo->theModelInfo->hurtFrame.duration!=1) {
            animate = Animate3D::create(animation,m_strTheInfo->theModelInfo->hurtFrame.start/60,m_strTheInfo->theModelInfo->hurtFrame.duration/60);
            //        animate->setSpeed(0.5f);//测试用，换模型了就删掉
          
            if(m_strTheInfo->monsterId>=14 && m_strTheInfo->monsterId<=28)
            {
                animate->setSpeed(0.5f);
            }
            m_pAction_Hurt = animate;
            m_pAction_Hurt->retain();
            m_pAction_Hurt->setTag(SOLDIER_ANIMATE_TAG);
        }
        
        animate=NULL;
        if (m_strTheInfo->theModelInfo->DieFrame.duration!=0&&m_strTheInfo->theModelInfo->DieFrame.duration!=1) {
            animate = Animate3D::create(animation,m_strTheInfo->theModelInfo->DieFrame.start/60,m_strTheInfo->theModelInfo->DieFrame.duration/60);
            //        animate->setSpeed(0.5f);//测试用，换模型了就删掉
            
            if(m_strTheInfo->monsterId>=14 && m_strTheInfo->monsterId<=28)
            {
                animate->setSpeed(0.5f);
            }
            
            m_pAction_Die = animate;
            m_pAction_Die->retain();
            m_pAction_Die->setTag(SOLDIER_ANIMATE_TAG);
        }
        
        animate=NULL;
        if (m_strTheInfo->theModelInfo->DizzyFrame.duration!=0&&m_strTheInfo->theModelInfo->DizzyFrame.duration!=1) {
            animate = Animate3D::create(animation,m_strTheInfo->theModelInfo->DizzyFrame.start/60,m_strTheInfo->theModelInfo->DizzyFrame.duration/60);
            //        animate->setSpeed(0.5f);//测试用，换模型了就删掉
            m_pAction_Dizzy = animate;
            m_pAction_Dizzy->retain();
            m_pAction_Dizzy->setTag(SOLDIER_ANIMATE_TAG);
        }
        
        
        
        
        //默认为空闲
        m_iFightState = eFighteState_Idle;
        m_iAnimateState = eFighteState_None;
        //        m_pTheSrite->runAction(m_pAction_IDle);
        
        
        
        // _swim->retain();
        // cocos2d::Animate3D*_hurt = Animate3D::create(animation, 1.933f, 2.8f);
        //_hurt->retain();
        
    }
    
    
    //初始化走路粒子效果
//    walkingSmoke = ParticleSystemQuad::create("particleSystem/walkingPuff.plist") ;
////    walkingSmoke->setScale(0.3f);
//    //    explodePar->setPosition3D(Vec3(0, sprite->gettheSprite()->getBoundingBox().size.height / 2, 0)) ;
//    walkingSmoke->setCameraMask((unsigned short )CameraFlag::USER1);
//    walkingSmoke->setAutoRemoveOnFinish(true);
//    addChild(walkingSmoke, 11, 100) ;
//    walkingSmoke->setGlobalZOrder(1001);
    
    
    
}


void CBaseFighter::update(float time){
    
    
   /* if (m_pShadow && !m_pShadow->isVisible()) {
        m_pShadow->setVisible(true);
    }*/
    
    if(CPersonalData::getInstance()->getPvpMainLayer())
    {
        log("pvp主界面不进行更行哦");
        return;
    }
    
    m_fLiveTime+=time;
    
    
    if (!m_bIfActive)
    {
        
        judgeActive();
        
        return;
    }
    if((!CFightData::getInstance()->getTheFightScene()->m_bJishiStart) && (m_iAttackType != eAttackType_Partol))
    {
        return;//计时未开始之前不调用
    }
    
    if (m_vecAllBullets.size()>3) {
        CCLOG("bulletNum:%zd",m_vecAllBullets.size());
    }
    
    m_fNowTalkCold+=time;
    
    //火巨人火焰皮肤0.5s产生一个CBaseAttk
    
    if(m_strTheInfo->monsterId == 6)
    {
        
            if(m_fLiveTime/1>=m_iHuoYanSkinNum)
            {
                bool bTemp = ifSkillStart(4);
                
                if(bTemp&&m_pPassiveSkill)
                {
                    m_iHuoYanSkinNum++;
                    CBaseAttack* theAttack = CBaseAttack::create();
                    theAttack->setAttackerGroup(m_iFightGroup);
                    theAttack->setCameraMask((unsigned short )CameraFlag::USER1);
                    m_vecAllAttackes.pushBack(theAttack);
                    theAttack->setSkillIdAndStart(m_pPassiveSkill,this,this);
                    getParent()->addChild(theAttack);
            }
        }
    }
    
    //
    m_stMonsterDebuff.ifFreeze = false;
    
    m_stMonsterDebuff.ifDizzy = false;
    
    m_stMonsterDebuff.ifBeatBack = false;
    
//    m_stMonsterDebuff.ifBeatBack = false;
    
    m_stMonsterDebuff.ifCharge = false;
    
    m_stMonsterDebuff.ifSlow = false;
    
    m_stMonsterDebuff.ifDu = false;
    
    m_stMonsterDebuff.ifhuoyanSkin = false;
    
    //更新异常状态
    for (int i=0; i<m_vecAllBuffes.size(); i++) {
        
        if (m_vecAllBuffes.at(i)->getBuffType()==eAllBuffes_bingdong) {
            m_stMonsterDebuff.ifFreeze = true;
//            gettheSprite()->setColor(Color3B::BLUE);
        }
        
        if (m_vecAllBuffes.at(i)->getBuffType()==eAllBuffes_xuanyun) {
            m_stMonsterDebuff.ifDizzy = true;
        }
        
        if (m_vecAllBuffes.at(i)->getBuffType()==eAllBuffes_jitui) {
            
            m_stMonsterDebuff.ifBeatBack = true;
        }
        
        if (m_vecAllBuffes.at(i)->getBuffType()==eAllBuffes_chongfeng) {
            m_stMonsterDebuff.ifCharge = true;
        }
        
        if (m_vecAllBuffes.at(i)->getBuffType()==eAllBuffes_jiansu) {
            m_stMonsterDebuff.ifSlow = true;
            setAddMoveSpeed(-0.5f,1);
        }
        
        if(m_vecAllBuffes.at(i)->getBuffType() == eAllBuffes_du)
        {
            m_stMonsterDebuff.ifDu = true;
            
        }
        
        if(m_vecAllBuffes.at(i)->getBuffType() == eAllBuffes_huoyanskin)
        {
            m_stMonsterDebuff.ifhuoyanSkin = true;
        }
    }
    
    
    
    //颜色更新
    if (m_bIfDied) { //死了会变黑色，不要做处理
        
        
    }else if (m_stMonsterDebuff.ifSlow) {
        gettheSprite()->setColor(Color3B(4, 183, 241));
    }else if (m_stMonsterDebuff.ifFreeze){
        gettheSprite()->setColor(Color3B(4, 183, 241));
    }else if(m_bIfElite){  //精英模式有变色动画，此处就不要做处理了

//        m_pTheSrite->setColor(Color3B(238, 122, 19));
    }else if(m_bIfSmallBoss){  //小boss有变色动画，此处就不要做处理了
        
        //        m_pTheSrite->setColor(Color3B(238, 122, 19));
    }else if(m_strTheInfo->monsterType!=eFighterType_Tower&&m_strTheInfo->monsterType!=eFighterType_Trap){  //只要不是塔就可以变白色，因为塔有被攻击变红的动画
        gettheSprite()->setColor(Color3B::WHITE);
    }
    
    
    
    if(!m_stMonsterDebuff.ifSlow&&getAddMoveSpeed()<0){
        setAddMoveSpeed(0);
    }

    
    
    //为了防止被击飞过程中死去的怪物会停留在空中，因此，被击飞的怪物，只要y轴不为0，则继续移动
    if ((getPositionY()>0) && (CFightData::getInstance()->getFightType() != FIGHT_PVP_TYPE)) {
        m_stMonsterDebuff.ifBeatBack=true;
    }
    
    
    
    if (m_stMonsterDebuff.ifBeatBack) {
        //击退位移更新
        beatBackUpdate(time);
    }
    
    
    if (m_stMonsterDebuff.ifFreeze||m_stMonsterDebuff.ifDizzy||m_stMonsterDebuff.ifBeatBack) {
        return;
    }
    //    gettheSprite()->setColor(Color3B::WHITE);
    
    //从眩晕中恢复,测试用注释
    if (m_iAnimateState==eFighteState_Dizzy)
    {
        stateChange(eFighteState_Idle);
    }

    
    if (!m_stMonsterDebuff.ifCharge) { //当有冲锋buff在时，不判断碰撞。
        collidUpdate(time);
    }
    
    stateUpdate(time);
    
    
    //更新对话框位置
    CFightScene* theScene = CFightData::getInstance()->getTheFightScene();
    if (m_pTheTalk) {
        
        Vec2 thePos = CommonData::GetSingletonPtr()->getPosOnScreen(theScene->getTheCamera(), getPosition3D());
        
        m_pTheTalk->setPosition(thePos+Vec2(0, m_pTheTalk->getTheContent()->getBoundingBox().size.height*0.9f) );
        
        m_pTheTalk->getTheContent()->updateContent();
        
    }
    
    
}


void CBaseFighter::stateUpdate(float time)
{
    
    if (m_bIfDied) {
        return;
    }
    if(!m_bWanshi)
    {
        return;
    }
    
    
    if (eleteStar) {
        eleteStar->setVisible(false);//精英星星隐藏
    }
    
    
    switch (m_iFightState) {
        case eFighteState_Dizzy://如果眩晕状态，则苏醒
        case eFighteState_Idle:
        {
            if(m_bStop)
            {
                log("pvp主界面逻辑");
                break;
            }
            //如果空闲状态，则判断是否有人在攻击范围内
            Vector<CBaseFighter*> target;
            
            target = chooseTarget();
            
            
            //            if (m_iFightGroup==eFightGroup_Freind) {
            //                target =CFightData::getInstance()->getAllEnemy();
            //            }else if (m_iFightGroup==eFightGroup_Enemy){
            //                target =CFightData::getInstance()->getAllSoldier();
            //            }
            
            
            float minDistance=99999;
            
            CBaseFighter* tempTarget = NULL;
            
            for (int i=0; i<target.size(); i++) {
                
                //如果死掉了就不判断了
                if (target.at(i)->getIfDied()) {
                    continue;
                }
                
                float distance = getFightDistance(target.at(i));
                
                
                
                
                if (distance<minDistance) {
                    minDistance = distance;
                    tempTarget = target.at(i);
                    
//                    m_pAttackTarget = target.at(i);
                }
                
            }
            
            if (tempTarget&&tempTarget!=m_pAttackTarget) {
                setAttackTarget(tempTarget);
            }
            
            if (minDistance<=m_strTheInfo->attackRang) {
                
                
                minDistance=-1;//-1代表进入攻击
                stateChange(eFighteState_Attack);
            }
            
            
            
            //如果没有人可以攻击，则进入移动状态
            if (minDistance!=-1&&!getMonsterDebuff()->ifShake) {
                
                stateChange(eFighteState_Walk);
                
            }
            
            
            
            break;
        }
        case eFighteState_Walk:
        {
            
            if (eleteStar) {
                eleteStar->setVisible(true);//精英星星隐藏
            }
            
            //如果移动状态，则判断是否有人在攻击范围内
            Vector<CBaseFighter*> target;
            
            target = chooseTarget();
            
            //            if (m_iFightGroup==eFightGroup_Freind) {
            //                target =CFightData::getInstance()->getAllEnemy();
            //            }else if (m_iFightGroup==eFightGroup_Enemy){
            //                target =CFightData::getInstance()->getAllSoldier();
            //            }
            
            
            const float defaultMaxDistance = 99999;
            float minDistance=defaultMaxDistance;
            
            CBaseFighter* tempTarget = NULL;
            
            for (int i=0; i<target.size(); i++) {
                
                //如果死掉了就不判断了
                if (target.at(i)->getIfDied()) {
                    continue;
                }
                
                
                float distance = getFightDistance(target.at(i));
                
                if (distance<minDistance) {
                    minDistance = distance;
                    tempTarget = target.at(i);
                    
//                    setAttackTarget(target.at(i));
//                    m_pAttackTarget = target.at(i);
                }
                
            }
            
            if (tempTarget&&tempTarget!=m_pAttackTarget) {
                setAttackTarget(tempTarget);
            }

            
            
            if (minDistance<=m_strTheInfo->attackRang) {   //找到最近的敌人进行攻击
                minDistance=-1;//-1代表进入攻击
                stateChange(eFighteState_Attack);
            }

            
            
            //横行兵种在离敌人一定距离时会冲刺。此处做冲刺判断
            if(CFightData::getInstance()->getTheLevelCondition()!=eLevelCondition_TowerDefense&&(m_strTheInfo->monsterType==eFighterType_line||m_strTheInfo->monsterId==16)){//塔防模式下除外
                if (minDistance<=LINE_CHARGE_DIST&&m_iCollidTimes<=0) {
                    
                    setAddMoveSpeed(5);
                }else{
                    setAddMoveSpeed(0);
                }
            }
            
            
            
            
            
            
            
            
            
            //如果没有攻击，也没有敌人，则目标标志为空
            if (minDistance==defaultMaxDistance) {
                setAttackTarget(NULL);
//                m_pAttackTarget=NULL;
            }
            
            
            //如果移动状态下，动画不是移动。则更换移动动画
            if (m_iAnimateState!=eFighteState_Walk&&m_iFightState!=eFighteState_Attack) {
                animateChange(eFighteState_Walk);
            }
            
            
            //在当前动画状态是移动时，移动
            
            if (m_iAnimateState==eFighteState_Walk) {
                positionUpdate(time);
                
                //说话
                if (ifCanTalk()) {
                    CFightManager::addTheFightTalkToFighter(this, 1);
                }
                
                
            }
            
            
            roatationUpdate(time);
            break;
        }
        case eFighteState_Attack:
        {
            setAddMoveSpeed(0);
            
            
            //判断是否攻击动画结束了
            bool ifAnimateOver=false;
            if (m_iAnimateState!=eFighteState_Attack&&m_iAnimateState!=eFighteState_SpecialAttack&&m_iAnimateState!=eFighteState_Hurt) {//这就是传说中的攻击后摇
                ifAnimateOver=true;
            }
            
            if (m_strTheInfo->monsterId==SOLDIER_DOCTOR){
                Vector<CBaseFighter*> target=chooseTarget();
                if (target.size()>0) {
                    setAttackTarget(target.at(0));
//                    m_pAttackTarget = target.at(0);
                }else{
                    setAttackTarget(NULL);
//                    m_pAttackTarget = NULL;
                }
            }
            
            
            if(m_pAttackTarget==NULL||m_pAttackTarget->getReferenceCount()<=0||m_pAttackTarget->getIfDied()){//如果没目标或者目标死掉了
                
                setAttackTarget(NULL);
                
                if (ifAnimateOver) {//并且攻击动画已经结束了
                    
                    stateChange(eFighteState_Idle);
                    
                }
                
                break;
                
            }
            
        
            //此处逻辑防止开始攻击基地后就攻击个不停,因此如果是在攻击基地，则需要重新寻找一下目标。
            if (m_pAttackTarget->getAttackType()==eAttackType_city) {
                setAttackTarget(chooseTheCloseOneInVec(chooseTarget()));
//                m_pAttackTarget=;
            }
            
            
            
            float distance = getFightDistance(m_pAttackTarget);
            
            float attackRangeOffset = 1;
            //如果是选中的友方目标，为了防止目标走动而自身跟着走动，此处把战斗状态的攻击距离延长一些
            if(m_pAttackTarget->getFightGroup()==m_iFightGroup){
                
                attackRangeOffset = 1.3f;
            }
            
            
            if (ifAnimateOver&&distance>m_strTheInfo->attackRang*attackRangeOffset) {//如果超过距离，并且攻击动画结束,则重新寻找目标
                setAttackTarget(NULL);
//                m_pAttackTarget=NULL;
                stateChange(eFighteState_Idle);
                break;
            }
            
//            //如果是治疗者，则当前目标为满血时，则重新寻找目标
//            if (m_strTheInfo->monsterId==SOLDIER_DOCTOR&&m_pAttackTarget->getNowHP()==m_pAttackTarget->m_strTheInfo->maxHP) {
//                m_pAttackTarget = chooseTheCloseOneInVec(chooseTarget());
//            }
            
            
            m_fAttackColdNow-=time;
        

    
            if (m_fAttackColdNow<0 &&ifAnimateOver)
            {
                //攻击时间到了后，判断是释放被动技能还是普通攻击
                if (ifSkillStart())
                {
                    m_fAttackColdNow=m_strTheInfo->attackCD;
                    
                    if(m_iNextSkill)
                    {
                        if(m_iNextSkill->theSkillInfo->skillId == eAllSkills_renShitou)
                        {
                            //float fDamage = m_iNextSkill->damageRatio;
                            log("fDamage\n");
                        }
                        else if(m_iNextSkill->theSkillInfo->skillId == eAllSkills_Xuanfengzhan)
                        {
                            log("xuanfengzhan\n");
                        }
                         
                        skillPrepare();
                        
                    }
                
                }
                else
                {
                    
                   
        
                    animateChange(eFighteState_Attack);
                }
                
            }
            
            roatationUpdate(time);
            
            break;
        }
        case eFighteState_SpecialAttack:
        {
            
            
            break;
        }
            //        case eFighteState_Hurt:
            //        {
            //
            //            break;
            //        }
        case eFighteState_Die:
        {
            
            break;
        }
            
            
        default:
            break;
    }
    
}


void CBaseFighter::setAddMoveSpeed(float var,int addOrmulti){
    
    
    if(addOrmulti==0 ){
        
        m_fAddMoveSpeed = var;
        

        
    }else if(addOrmulti==1){
        
        m_fAddMoveSpeed = var*m_strTheInfo->theMonsterInfo->walkSpeed;
    }
    

    //根据行走速度来改变行走动画
    float ratio = (m_fAddMoveSpeed+m_strTheInfo->theMonsterInfo->walkSpeed)/m_strTheInfo->theMonsterInfo->walkSpeed;
    
    //骑兵的话特殊处理，防止动画变得太夸张
    if (m_strTheInfo->monsterId==9) {
        ratio = ratio/2;
    }
    
    
    
    ((Animate3D*)m_pAction_Walk)->setSpeed(ratio);
    
//    CCLOG("move ratio::%f",ratio);
    
//    //行走速度增快时，加个小烟雾
//    if (var>0) {
//        walkingSmoke->setVisible(true);
//    }else{
//        walkingSmoke->setVisible(false);
//    }
    
    
    
    
}





cocos2d::Vector<CBaseFighter*> CBaseFighter::chooseTarget(){
    
    
    Vector<CBaseFighter*> target;
    
    
    
    //如果是持续治疗的士兵，则返回的怪物类型，应该是自己阵营.否则返回的就是敌人
    
    if(m_strTheInfo->monsterId==SOLDIER_DOCTOR){//因为治疗士兵不多，此处写死
        
        target = CFightData::getInstance()->getFriendMonstersByGroup(m_iFightGroup);
        
        
        Vector<CBaseFighter*> tempTarget;
        
        float hpRatio = 1;
        
        //治疗目标逻辑为：寻找不满血目标。如果都满血。则返回最靠前的友军。
        
        CBaseFighter* theTarget=NULL;
        
        for (int i=0; i<target.size(); i++) {
            
            CBaseFighter* fighter =target.at(i);
            
            float tempRatio = fighter->getNowHP()*1.0f/fighter->getMaxHP();
            
            if (tempRatio<hpRatio) {
                hpRatio = tempRatio;
                theTarget=fighter;
                
            }
            
        }
        
        if (theTarget) {
            tempTarget.pushBack(theTarget);
        }
        
        
        
        
        
        if (tempTarget.size()>0) {
            return tempTarget;
            
        }else{ //如果没有空血目标，则往最前面的士兵靠近
            
            
            CBaseFighter*temp = CFightManager::getTheFrontSoldier(m_iFightGroup);
            
            tempTarget.pushBack(temp);
            
            return tempTarget;
        }
        
        
        
        
        
    }else{
        
        bool ifFightFly = true;
        
        //是否进攻飞行单位
        if (m_strTheInfo->monsterId>=10011&&m_strTheInfo->monsterId<=10015) {//如果是塔防下的炮塔，则不对空
            ifFightFly = false;
        }
        
        if (m_strTheInfo->monsterId>=20004&&m_strTheInfo->monsterId<=20005) {//如果是塔防下的炮塔，则不对空
            ifFightFly = false;
        }
        
        if (m_strTheInfo->theMonsterInfo->attackRange<100) { //如果攻击距离小于100，则不对空
            ifFightFly = false;
        }
        
        
        
        
        if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense) {
            target = CFightData::getInstance()->getFightTargetByGroup(m_iFightGroup,false,ifFightFly);
        }else{
            target = CFightData::getInstance()->getFightTargetByGroup(m_iFightGroup,true,ifFightFly);
        }
        
        
    }
    
    return target;
}

CBaseFighter* CBaseFighter::chooseTheCloseOneInVec(cocos2d::Vector<CBaseFighter *> target){
    
    
    CBaseFighter* targetFighter= NULL;
    
    float minDistance = 99999;
    
    for (int i=0; i<target.size(); i++) {
        
        //如果死掉了就不判断了
        if (target.at(i)->getIfDied()) {
            continue;
        }
        
        float distance = getFightDistance(target.at(i));
        
        
        
        
        if (distance<minDistance) {
            minDistance = distance;
            targetFighter = target.at(i);
        }
    }
    
    
    return targetFighter;
    
}



void  CBaseFighter::kuloureborn()
{


    skillNameShow();
    reborn();
    m_bIfReborn = false;

}



void CBaseFighter::XixueAnimate()
{
    Sprite* soul = Sprite::createWithSpriteFrameName("xixue1.png");
    addChild(soul,11,100);
    
    soul->setCameraMask((unsigned short )CameraFlag::USER1);
    soul->setPosition(Vec2(0,50));
    
    char buf[128];
    auto* pAnimation = Animation::create();
    for (int i = 1; i < 8; i++)
    {
        sprintf(buf, "xixue%d.png", i);
        pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
    }
    pAnimation->setDelayPerUnit(0.1f);
    Animate* pAction = Animate::create(pAnimation);
    soul->runAction(Sequence::create(pAction,RemoveSelf::create(),NULL));
    
}

void CBaseFighter::KulouRebornAnimate()
{
    
    {
//特效
        
        CFightScene* pScene = CFightData::getInstance()->getTheFightScene();
        
        
        Layer *pLayer3D = pScene->gettheLayer3D();
        
        Sprite3D *pReborn = Sprite3D::create();
        
        pLayer3D->addChild(pReborn,10);
        
        Sprite* soul = Sprite::createWithSpriteFrameName("zhongshen1.png");
        soul->setAnchorPoint(Vec2(0.5,0.3));
        
        soul->cocos2d::Node::setScale(6);
        pReborn->addChild(soul);
        pReborn->setPosition3D(getPosition3D());
        
        pReborn->setCameraMask((unsigned short )CameraFlag::USER1);
        
        char buf[128];
        auto* pAnimation = Animation::create();
        for (int i = 1; i < 8; i++)
        {
            sprintf(buf, "zhongshen%d.png", i);
            pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
        }
        
        pAnimation->setDelayPerUnit(0.2f);
        Animate* pAction = Animate::create(pAnimation);
        soul->runAction(Sequence::create(pAction,CallFunc::create(CC_CALLBACK_0(CBaseFighter::kuloureborn, this)),RemoveSelf::create(),NULL));
    }
    
}

void CBaseFighter::stateChange(FighteState targetState){
    
    m_iFightState = targetState;
    
    
    m_FightStateTime = 0;
    

    switch (targetState) {
        case eFighteState_Attack:
            
            
            m_FightStateTime=CFightData::getInstance()->getAllFightTime();
            
            animateChange(eFighteState_Idle);
            
            
            m_fAttackColdNow = m_strTheInfo->attackCD;
            
            
            //如果是横行兵种，则上来直接攻击一次，因为有冲锋
            if (m_strTheInfo->theMonsterInfo->monsterType==eFighterType_line) {
                m_fAttackColdNow=0;
            }
            
            //如果是炸弹人，则上来直接攻击一次，只自爆一次
            if (m_strTheInfo->theMonsterInfo->monsterId==16&&!m_bIfBombed) {
                m_fAttackColdNow=0;
            }

            break;
            
        case eFighteState_Walk:
        {
            
            
            
            //测试用
//            BillBoard* temp2 = BillBoard::createWithTexture(m_pTheSrite->getMesh()->getTexture(),BillBoard::Mode::VIEW_PLANE_ORIENTED);
//            addChild(temp2);
//            temp2->setCameraMask((unsigned short )CameraFlag::USER1);
            
            
            //开始移动时触发
            bool temp =  ifSkillStart(2);
            
            if (temp&&m_iNextSkill) {
                
                switch (m_iNextSkill->theSkillInfo->skillId){
                        
                    case eAllSkills_chongfeng:
                        
                        skillNameShow();
                        //附加一个buff，持续加速。并在状态改变时，删除此buff，造成额外伤害
                        CFightManager::addTheBuffToFighter(this,(AllBuffes)m_iNextSkill->theSkillInfo->buffId,-1,m_iNowAttack*m_iNextSkill->damageRatio);//根据技能提升伤害系数，计算初始伤害。
                        
                        break;
                        
                    default:
                        
                        
                        break;
                }
                
                
            }
            
            
            
            
            
            animateChange(eFighteState_Walk);
            break;
        }
            
        case eFighteState_Dizzy:
            animateChange(eFighteState_Dizzy);
            break;
            
        case eFighteState_Idle:
            animateChange(eFighteState_Idle);
            break;
            
        case eFighteState_Die:
        {
            m_pTheHpBar->setScale(0);
            
            if (m_pMapSprite) {
                m_pMapSprite->stopAllActions();
                m_pMapSprite->removeFromParent();
                m_pMapSprite = NULL;
            }
            
            animateChange(eFighteState_Die);
            
                bool temp =  ifSkillStart(5);
            
                if (temp&&m_iNextSkill)
                {
                    if(m_iNextSkill->theSkillInfo->skillId == eAllSkills_reborn&&m_bKulouDie)//骷髅
                    {
                        m_bIfReborn = true;
                        m_bKulouDie = false;
                    
                    }
                    else if(m_iNextSkill->theSkillInfo->skillId == eAllSkills_shihua&&m_bWanshi) //石化
                    {
                        m_bIfReborn = true;
                        m_bWanshi = false;
                        
                        m_fReDamagexishu = m_iNextSkill->damageRatio;
                    
                    }
                    
            
                }
              
        
        }
            break;
            
        default:
            break;
    }
    
    
    
    
}

void CBaseFighter::set3DPosition(cocos2d::Vec3 pos, float dt)
{
    m_distance += getNowSpeedWithOutFrame(dt);
    if(m_distance > 60)
    {
        m_distance = 0;
        //SimpleAudioEngine::getInstance()->playEffect("music/walk.mp3");
    }
    setPosition3D(pos);
}


void CBaseFighter::positionUpdate(float time){
    
    //如果寻路模式，则开始寻路。 有目标则面向目标移动，没目标则直接移动
    
    //如果是塔防关卡，则一直是寻路模式
    if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense) {
        
        
        m_bIfSearchMode = true;
        
        
        if (!thePathInfo||m_iCollidTimes>MAX_COLLID_TIMES) {
            m_iCollidTimes=0;
            thePathInfo = CFightData::getInstance()->findPathPosByPos(getFightPosition());
        }
        
        if (thePathInfo) {
            
            m_v3SearchPos = CFightData::getInstance()->convertPathPosToPos(thePathInfo);
            float dist = m_v3SearchPos.distance(getPosition3D());
            
                 //判断距离，如果到达目标点，则开始下个目标点的寻路
            
            if (thePathInfo==CFightData::getInstance()->getTheHead()&&dist<getNowSpeedWithOutFrame(time*3)) {
                
                setVisible(false);
                
                CFightData::getInstance()->addLife(-1,this);
                
                onDied(eDamageType_System);
                
                ParticleSystem*explodePar = ParticleSystemQuad::create("particleSystem/TDMonsterDie.plist") ;
                explodePar->setScale(0.5f);
                explodePar->setPosition3D(Vec3(0, gettheSprite()->getBoundingBox().size.height / 10, 0)+getPosition3D()) ;
                explodePar->setCameraMask((unsigned short )CameraFlag::USER1);
                explodePar->setAutoRemoveOnFinish(true);
                
                if( CFightData::getInstance()->getTheFightScene()){
                    CFightData::getInstance()->getTheFightScene()->addChild(explodePar,1000) ;
                }
                
                
                explodePar->setGlobalZOrder(1000);
                
                
                
                
                return;
                
            }
            
            
            if (dist<getNowSpeedWithOutFrame(time)) {
                
                pathPos* temp = CFightData::getInstance()->findNextPath(thePathInfo,m_strTheInfo->theMonsterInfo->ifFly);
                
                if (temp==thePathInfo) {
                    temp = CFightData::getInstance()->findNextPath(thePathInfo,m_strTheInfo->theMonsterInfo->ifFly);
                }
                
                
                thePathInfo = temp;
                
                
                
            }
            
            m_v3SearchPos = CFightData::getInstance()->convertPathPosToPos(thePathInfo);
            
//            thePathInfo->temp->setScale(5);
            
            
        }else{
            CCLOG("塔防模式寻路错误");
            return;
        }
        
    }
    
    
    
    if (m_bIfSearchMode||m_pAttackTarget||(m_v2MoveControll!=Vec2(0, 0)||m_vec2IdlePos!=Vec2::ZERO))
    {
        
        Point myPos ;
        Point targetPos ;
        
        
        
        
        myPos = Point(getPositionX(),getPositionZ());
        
        if (m_v2MoveControll!=Vec2(0, 0)) {//如果有控制位移
            targetPos = m_v2MoveControll+myPos;
        }else if (m_bIfSearchMode){//两者区别就是目标位置不同
            
            targetPos = Point(m_v3SearchPos.x,m_v3SearchPos.z);
            
        }else if(m_pAttackTarget)
        {
            
            targetPos = Point( m_pAttackTarget->getPositionX(), m_pAttackTarget->getPositionZ());
        }else if (m_vec2IdlePos!=Vec2::ZERO){
            targetPos = m_vec2IdlePos;
        }
        
        
        
        //边界判断
        if (m_iAttackType!=eAttackType_NPC&&m_iAttackType!=eAttackType_Partol) {
            targetPos = CFightManager::getThePosInMap(targetPos);
        }
        
        
        
        //位置修正
        float dist = myPos.distance(targetPos);
        
        
        float speedX=0;
        float speedZ=0;
        
        if (dist>= getNowSpeedWithOutFrame(time))
        {
            speedX = getNowSpeedWithOutFrame(time)*(targetPos.x-myPos.x)/dist;
            speedZ = getNowSpeedWithOutFrame(time)*(targetPos.y-myPos.y)/dist;
        }
        else
        {
            speedX = targetPos.x-myPos.x;
            speedZ = targetPos.y-myPos.y;
            
            //距离很近了，就取消寻路模式
            m_bIfSearchMode=false;
            
        }
        
        CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(getPosition3D()+Vec3(speedX, 0, speedZ),m_pMapSprite);
        set3DPosition(getPosition3D()+Vec3(speedX, 0, speedZ), time);
        
    }else
    {
        if(!m_pAttackTarget && (m_iAttackType==eAttackType_Partol))
        {
            playAniamteAction(m_pAction_IDle,true);
        }
        
        if (m_iAttackType!=eAttackType_hero) {//当不是敌人的时候，才能行动
            
            
            if (m_iAttackType==eAttackType_protector) {
                
                //如果是守护者，则啥也不做
                
                
                
            }else if (m_iFightGroup==eFightGroup_Enemy)
            {
                Vec3 targetVec = Vec3(0,-90,0);
                m_pTheSrite->setRotation3D(targetVec);
                
                
                Vec3 PosVec =getPosition3D()+Vec3(-getNowSpeedWithOutFrame(time), 0, 0);
                
                if (PosVec.x>=Start3DPOINT) {//不到边界则继续移动
                    set3DPosition(getPosition3D()+Vec3(-getNowSpeedWithOutFrame(time), 0, 0), time);
                    CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(PosVec,m_pMapSprite);
                }
                //调用小地图地方单位
            }else if (m_iFightGroup==eFightGroup_Freind)
            {
                Vec3 targetVec = Vec3(0,90,0);
                m_pTheSrite->setRotation3D(targetVec);
                
                Vec3 PosVec =getPosition3D()+Vec3(getNowSpeedWithOutFrame(time), 0,0);
                
                //log("PosVec = %f",m_strTheInfo->walkSpeed);
                
                
                Vec3 target= CFightManager::getThePosInMap(PosVec);
                
                
                //                if (PosVec.x>=Start3DPOINT&&PosVec.x<=END3DPOINT&&PosVec.z<=STARTZPOINT&&PosVec.z>=ENDZPOINT)//不到边界则继续移动
                //                {
                set3DPosition(target, time);
                CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(target,m_pMapSprite);
                //                }
                
                //调用小地图我方单位
            }
            

        }
        
        
    }
    
}


void CBaseFighter::roatationUpdate(float time){
    
    if(getMonsterDebuff()->ifShake)
    {
        Vec3 target = m_pTheSrite->getRotation3D();
        target.y=0;
        
        m_pTheSrite->setRotation3D(target);
    }else if (m_bIfSearchMode||m_pAttackTarget||(m_v2MoveControll!=Vec2(0, 0))||m_vec2IdlePos!=Vec2::ZERO)
    {
        
        Point myPos ;
        Point targetPos ;
        myPos = Point(getPositionX(),getPositionZ());
        if (m_v2MoveControll!=Vec2(0, 0)) {//如果有控制位移
            targetPos = m_v2MoveControll+myPos;
        }else if (m_bIfSearchMode&&m_iFightState!=eFighteState_Attack)//两者区别就是目标位置不同
        {
            targetPos = Point(m_v3SearchPos.x,m_v3SearchPos.z);
        }
        else if(m_pAttackTarget)
        {
            targetPos = Point( m_pAttackTarget->getPositionX(), m_pAttackTarget->getPositionZ());
        }else if (m_vec2IdlePos!=Vec2::ZERO){
            
            targetPos = m_vec2IdlePos;
        }
        
        float radians = CFightManager::getTheDegreeWith(myPos, targetPos,false);
        
        
        //        //如果目标就是自己，那么友军面向右边，敌军面向左边
        //        if(m_pAttackTarget==this){
        //
        //            if (m_iFightGroup==eFightGroup_Enemy) {
        //                Vec3 targetVec = Vec3(0,-90,0);
        //                m_pTheSrite->setRotation3D(targetVec);
        //            }else if(m_iFightGroup==eFightGroup_Freind){
        //                Vec3 targetVec = Vec3(0,90,0);
        //                m_pTheSrite->setRotation3D(targetVec);
        //
        //            }
        //
        //        }
        
        
        //        //面向修正
        //        Vec2 tempPoint;
        //
        //        tempPoint.set(myPos,targetPos);
        //
        //        float tempAngle = tempPoint.getAngle(Vec2(0,1));//计算角度
        //
        //
        //        float radians = CC_RADIANS_TO_DEGREES(tempAngle);
        
        Vec3 targetVec = m_pTheSrite->getRotation3D();
        
        targetVec.y = radians;
        
        if(targetVec!=m_pTheSrite->getRotation3D())
        {
            m_pTheSrite->setRotation3D(targetVec);
        }
        
    }
    
    
}

void CBaseFighter::collidUpdate(float time){
    
    
    if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense) {//塔防模式下卡死时，不碰撞检测
        return;
    }
    
    
    
    //如果自己是尸体或者战斗状态，则不会移动
    if (m_iFightState==eFighteState_Die) //
    {
        return;
    }
    
    //如果自己是boss
    if (m_bIfLevelBoss) {
        return ;
    }
    
    //如果自己是护送者
    if ((m_iAttackType==eAttackType_walker) || (m_iAttackType==eAttackType_Partol)) {
        return ;
    }
    
    
    
    Vector<CBaseFighter*> theVector = CFightData::getInstance()->getAllBaseFight();
    
    bool ifCollid=false;
    
    //    if (m_iFightGroup==eFightGroup_Freind) {
    
    
    //此处防止和敌人位置重叠，因此，第一个循环处理所有友军，然后再处理敌人的判断放在第二个for循环里面
    for (int i=0; i<theVector.size(); i++) {
        
        CBaseFighter* tempTarget = theVector.at(i);
        
        if(m_iFightGroup!=tempTarget->getFightGroup()){
            continue;
        }
        
        //为了防止远程兵种堵住近战兵种的路，因此做个判断，自己不会被比自己射程远的怪物挡住
        if (tempTarget->getTheInfo()->attackRang>getTheInfo()->attackRang) {
            continue;
        }
        
        if(collidWithOther(tempTarget)){
            ifCollid=true;
        }
        
    }
    
    //处理敌人的判断
    for (int i=0; i<theVector.size(); i++) {
        
        CBaseFighter* tempTarget = theVector.at(i);
        
        if(m_iFightGroup==tempTarget->getFightGroup()){
            continue;
        }
        
        //为了防止远程兵种堵住近战兵种的路，因此做个判断，自己不会被比自己射程远的怪物挡住
        if (tempTarget->getTheInfo()->attackRang>getTheInfo()->attackRang) {
            continue;
        }
        
        if(collidWithOther(tempTarget)){
            ifCollid=true;
        }
        
    }
    
    
    
    //    }
    
    
    //    Vector<CBaseFighter*>* theVector2 = CFightData::getInstance()->getAllEnemy();
    //
    ////    if (m_iFightGroup==eFightGroup_Enemy) {
    //        for (int i=0; i<theVector2->size(); i++) {
    //
    //            CBaseFighter* tempTarget = theVector2->at(i);
    //
    //            if(collidWithOther(tempTarget)){
    //                ifCollid=true;
    //            }
    //
    //        }
    ////    }
    
    
    
    //如果没有发生任何碰撞
    if (!ifCollid) {
        m_iCollidTimes=0;
    }else if(ifCollid&&m_fAddMoveSpeed>0){
        setAddMoveSpeed(0);//碰撞时就没有额外速度了
        m_iCollidTimes++;
    }
    
    
    
}

bool CBaseFighter::collidWithOther(CBaseFighter *tempTarget){
    
    //如果是自己，则返回
    if (tempTarget&&tempTarget==this) {
        return false;
    }
    
    //如果敌人已死，则返回
    if (tempTarget&&tempTarget->getIfDied()) {
        return false;
    }
    
    
    //如果自己的飞行状态和敌人不符合，则不碰撞。
    if(m_strTheInfo->theMonsterInfo->ifFly!=tempTarget->getTheInfo()->theMonsterInfo->ifFly){
        
        return false;
    }
    
    
    
    //如果只有自己是战斗状态，敌人不是。则自己不移动
//    if (m_iFightState==eFighteState_Attack&&tempTarget->getFightState()!=eFighteState_Attack) {
//        return false;
//    }
    
    
    
    //如果是敌对状态，则按优先时间移动（如果自己比对方早进入战斗状态，则自己不移动）
    if (m_iFightGroup!=tempTarget->getFightGroup()&&getFightStateTime()<tempTarget->getFightStateTime()) {
        return false;
    }
    
    //如果自己不是战斗状态，而对方是，则自己不移动（仅限友军）。
    if (m_iFightState!=eFighteState_Attack&&tempTarget->getFightState()==eFighteState_Attack&&m_iFightGroup==tempTarget->getFightState()) {
        return false;
    }
    
    
    Point myPos = Point(getPositionX(),getPositionZ());
    Point targetPos = Point( tempTarget->getPositionX(), tempTarget->getPositionZ());
    
    
    //最小的间隔
    float minDistance = getNowRadius()+tempTarget->getNowRadius();
    
    
    //位置修正,防止重叠
    float dist = myPos.distance(targetPos);
    
    
    if (dist>=minDistance) {//如果不到最小距离，则不做处理
        return false;
    }
    
    float offsetDist = minDistance-dist+1;
    
    float speedX=0;
    float speedZ=0;
    
    //    offsetDist = MIN(offsetDist, getNowSpeed()*2);
    
    
    if (dist>= getNowSpeed()) {
        speedX = offsetDist*(targetPos.x-myPos.x)/dist;
        speedZ = offsetDist*(targetPos.y-myPos.y)/dist;
    }else{
        speedX = targetPos.x-myPos.x;
        speedZ = targetPos.y-myPos.y;
    }
    
    
    
    
    setPosition3D(getPosition3D()-Vec3(speedX, 0, speedZ));
    
    //    CCLOG("collide position: x:%f,z:%f",getPositionX(),getPositionY());
    
    //    //如果已经寻路模式，则寻路的最终点变化
    //    if(m_bIfSearchMode){
    //        m_v3SearchPos-=Vec3(speedX, 0, speedZ)*3;
    //    }
    
    
    //记录碰撞修正次数，大于一定次数则开启寻路模式
    if (m_iCollidTimes>MAX_COLLID_TIMES) {
        m_iCollidTimes=0;
//        SearchingStart();
    }
    
    
    return true;
}


void CBaseFighter::SearchingStart(){
    
    m_bIfSearchMode = true;
    
    /*
     寻路逻辑，寻找目标点
     */
    
    //遍历所有正在攻击的友军，寻找离自己最近的“端点”     “端点”是一个友军，他周围正在攻击的友军最多只有一个人。
    
    Vector<CBaseFighter*>* aliveVector=NULL;
    
    if (m_iFightGroup==eFightGroup_Freind) {
        aliveVector = CFightData::getInstance()->getAllSoldier();
    }else if (m_iFightGroup==eFightGroup_Enemy) {
        aliveVector = CFightData::getInstance()->getAllEnemy();
    }
    
    
    //判断自己是近战还是远程，后面会使用
    bool ifCloseFight=true;
    int maxCloseFightDist = 100;  //某些近战士兵攻击距离较远，但是不会超过100
    if (m_strTheInfo->theMonsterInfo->attackRange>maxCloseFightDist) {
        ifCloseFight=false;
    }
    
    
    //1。寻找所有战斗中的友军
    Vector<CBaseFighter*> fightVector;
    for (int i=0; i<aliveVector->size(); i++) {
        
        CBaseFighter* tempTarget = aliveVector->at(i);
        
        if (tempTarget->getFightState()!=eFighteState_Attack) {//不在攻击则跳过
            continue;
        }
        
        
        //为了防止近战兵种以远程兵种为端点而出现的错误。因此远程兵种不和近战兵种一起比对。当自身是近战时，只能比较所有的近战士兵
        bool ifCloseFightTemp=true;
        if(tempTarget->getTheInfo()->theMonsterInfo->attackRange>maxCloseFightDist){
            ifCloseFightTemp=false;
        }
        
        if ((ifCloseFight&&!ifCloseFightTemp)
            ||(!ifCloseFight&&ifCloseFightTemp)) {//如果战斗距离不一致，则不能拿来当参考
            continue;
        }
        
        
        
        
        
        
        fightVector.pushBack(tempTarget);
    }
    
    
    
    
    //3。寻找最近的“端点”
    float minDistance = 9999;
    CBaseFighter* CloseFighter=NULL;
    CBaseFighter* CloseFighterOther=NULL;  //离得最近的端点所靠近的“士兵”
    
    for (int i=0; i<fightVector.size(); i++) {
        
        CBaseFighter* tempI= fightVector.at(i);
        
        //判断是否端点
        int conectFighter = 0;//彼此靠近的士兵
        for (int j=0; j<fightVector.size(); j++) {
            CBaseFighter* tempJ = fightVector.at(j);
            
            if (tempJ==tempI) {
                continue;
            }
            
            //最小的间隔
            float minDistance = tempI->getNowRadius()+tempJ->getNowRadius()+3;//
            //实际间隔
            float trueDistance = tempI->getPosition3D().distance(tempJ->getPosition3D());
            
            //如果实际间隔比最小间隔小，则加1
            if (trueDistance<minDistance) {
                CloseFighterOther = tempJ;
                conectFighter++;
                
            }
            if (conectFighter>=2) {
                CloseFighterOther = NULL;
                continue;
            }
        }
        
        if (conectFighter>=2) {
            CloseFighterOther = NULL;
            continue;
        }
        
        //实际间隔
        float trueDistance = tempI->getPosition3D().distance(getPosition3D());
        //如果彼此靠近的士兵数小于2，则为端点。记录最小距离，和节点。
        if (trueDistance<minDistance) {
            minDistance=trueDistance;
            CloseFighter = tempI;
        }
        
    }
    
    
    //如果没找到，则不进入寻路模式,并且寻路cd延长。
    if (!CloseFighter) {
        m_bIfSearchMode = false;
        m_iCollidTimes = -MAX_COLLID_TIMES;
        
        
        
#ifdef COCOS2D_DEBUG
        //        m_pTheSrite->setColor(Color3B::RED);
#endif
        
        return;
    }
    
    //    m_pTheSrite->setColor(Color3B::WHITE);
    
    
    //找到他的敌人，就出发吧 黎明号~
    //    if (CloseFighter->getAttackTarget()) {
    //        m_v3SearchPos = CloseFighter->getAttackTarget()->getPosition3D();
    //    }else{
    //        m_v3SearchPos = CloseFighter->getPosition3D();
    //    }
    
    
    //端点临近的友军的对称点就是我们的目的地，如果端点没有临近点，则目的地直接是端点的攻击者
    if(!CloseFighterOther&&CloseFighter->getAttackTarget()&&CloseFighter->getAttackTarget()->getReferenceCount()>0&&CloseFighter->getAttackTarget()->getReferenceCount()<999){//这里是不科学的，但是暂时只能这么做。
        m_v3SearchPos = CloseFighter->getAttackTarget()->getPosition3D();
    }else if(CloseFighterOther){
        
        Vec3 offsetPoint = CloseFighterOther->getPosition3D()-CloseFighter->getPosition3D();
        
        float distance = offsetPoint.distance(Vec3::ZERO);
        
        float ratio = 1;
        
        if (distance>getNowRadius()*3) { //如果距离大于自身半径的3倍。则做出削减
            
            ratio = (getNowRadius()*3)/distance;
            
        }
        
        //防止跨越的距离太远，此处做出距离判断。距离端点的距离太远时，按比例减少
        m_v3SearchPos = CloseFighter->getPosition3D() - offsetPoint*ratio;
        
        //出发吧黎明号
        
        //是否出现异常值判断
        float dist = getPosition3D().distance(m_v3SearchPos);
        CCLOG("lylog:: searchingDist-- %f",dist);
        
    }else{
        
        //如果什么都不做，就退出寻路模式
        m_bIfSearchMode = false;
        
    }
    
    
    
    
    fightVector.clear();
    
}








void CBaseFighter::animateUpdate()
{
    
    
    
    
}


void CBaseFighter::animateChange(FighteState targetAniamte,bool ifDoit){
    
    
    //如果当前状态是技能动画。而改变的动画不是死亡动画。则不作操作。
    //动画优先级  死亡-》眩晕、技能-》攻击、受伤-》走路、空闲
    
    if (m_iAnimateState==eFighteState_SpecialAttack&&targetAniamte!=eFighteState_Die&&!ifDoit) {
        return;
    }
    
    
    //如果是塔防模式，则怪物不会播放受伤动画
    if(CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense&&targetAniamte==eFighteState_Hurt){
        return;
    }
    
    
    //如果是boss，则怪物不会播放受伤动画
    if(m_bIfLevelBoss==true&&targetAniamte==eFighteState_Hurt){
        return;
    }
    
    //如果是pvp，则怪物不会播放受伤动画
    if (CFightData::getInstance()->getFightType()==FIGHT_PVP_TYPE&&targetAniamte==eFighteState_Hurt) {
        return;
    }
    
    
    
    
    //    m_pTheSrite->stopAllActions();
    
    
    if (targetAniamte==m_iAnimateState&&!ifDoit) {
        return;
    }
    
    switch (targetAniamte) {
        case eFighteState_Walk:
            
            if(m_iAnimateState==eFighteState_Idle||m_iAnimateState==eFighteState_None){
                playAniamteAction(m_pAction_Walk,true);
                m_iAnimateState = targetAniamte;
            }
            
            
            break;
        case eFighteState_Idle:
            
            playAniamteAction(m_pAction_IDle,true);
            
            m_iAnimateState = targetAniamte;
            break;
            
        case eFighteState_Dizzy:
            
            playAniamteAction(m_pAction_Dizzy,true);
            m_iAnimateState = targetAniamte;
            break;
            
        case eFighteState_Attack:
            
            
            if (m_iAnimateState==eFighteState_Idle||m_iAnimateState==eFighteState_Walk||ifDoit) {//如果是空闲动画时，则攻击
                
                //                m_pTheSrite->stopActionByTag(SOLDIER_ANIMATE_TAG);
                
                
                //说话
                if (ifCanTalk()) {
                    CFightManager::addTheFightTalkToFighter(this, 3);
                }
                
                
                float duration = 0.5f;
                
                if(m_pAction_Attack){
                    duration = m_pAction_Attack->getDuration();
                }
                
                
                Sequence *theSequence = Sequence::create(DelayTime::create(duration),CallFunc::create(CC_CALLBACK_0(CBaseFighter::animationIdleCallBack, this)) , NULL);
                theSequence->setTag(101);
                //                theSequence->setTag(SOLDIER_ANIMATE_TAG);
                playAniamteAction(m_pAction_Attack);
                m_pTheSrite->runAction(theSequence);
                

                m_fAttackColdNow=m_strTheInfo->attackCD;
                m_iAnimateState = targetAniamte;
                
                
                if (m_pAttackTarget&&m_pAttackTarget->getReferenceCount()>0&&getFightDistance(m_pAttackTarget)<m_strTheInfo->attackRang) {//如果有敌人在攻击范围内
                    float hurtTime = m_strTheInfo->theModelInfo->attackDamageTime;
                    m_pAttackTarget->runAction(Sequence::create(DelayTime::create(hurtTime),CallFunc::create(CC_CALLBACK_0(CBaseFighter::attackAction, this)), NULL));
                }
                
        
                
            }
            
            break;
            
        case eFighteState_SpecialAttack:
        {
            m_fAttackColdNow=m_strTheInfo->attackCD;
            
            //            m_pTheSrite->stopActionByTag(SOLDIER_ANIMATE_TAG);
            
            float duration = 0.5f;
            
            if(m_pAction_specialAttack)
            {
                duration = m_pAction_specialAttack->getDuration();
            }
            
            Sequence *theSequence = Sequence::create(DelayTime::create(duration),CallFunc::create(CC_CALLBACK_0(CBaseFighter::animationIdleCallBack, this)) , NULL);
            theSequence->setTag(101);
            
            playAniamteAction(m_pAction_specialAttack);
            //            theSequence->setTag(SOLDIER_ANIMATE_TAG);
            m_pTheSrite->runAction(theSequence);
            m_iAnimateState = targetAniamte;
        }
            break;
            
            
        case eFighteState_Hurt:
            
            //如果没有摇杆位移时，才播放动画。（不能在摇杆位移时还受伤动画，太怪）
            if (m_v2MoveControll==Vec2(0, 0))
            {
                if (m_iAnimateState==eFighteState_Idle||m_iAnimateState==eFighteState_Walk||ifDoit)
                {//如果是空闲动画时，则受伤
                    
                    //                    m_pTheSrite->stopActionByTag(SOLDIER_ANIMATE_TAG);
                    
                    float duration = 0.5f;
                    
                    if(m_pAction_Hurt)
                    {
                        duration = m_pAction_Hurt->getDuration();
                    }
                    
                    Sequence *theSequence = Sequence::create(DelayTime::create(duration),CallFunc::create(CC_CALLBACK_0(CBaseFighter::animationIdleCallBack, this)) , NULL);
                    theSequence->setTag(101);
                    
                    playAniamteAction(m_pAction_Hurt);
                    m_pTheSrite->runAction(theSequence);
                    m_iAnimateState = targetAniamte;
                    
                }
            }
            break;
            
        case eFighteState_Die:
        {
            
            if(m_iAnimateState!=eFighteState_Die){
                
                //                m_pTheSrite->stopActionByTag(SOLDIER_ANIMATE_TAG);
                
                //                Sequence *theSequence = Sequence::create(DelayTime::create(m_pAction_Die->getDuration()),DelayTime::create(5),CallFunc::create([=](){
                //
                //                    if (m_bIfReborn) {//如果复活，则走复活逻辑
                //                        reborn();
                //                    }else{
                //                        CFightData::getInstance()->removeAttacker(this);
                //
                //                        this->runAction(Sequence::create(MoveBy::create(0.8f, Point(0,-100)),RemoveSelf::create(), NULL));
                //                    }
                //
                //                }) , NULL);
                
                
                
                playAniamteAction(m_pAction_Die);
           
                
                
                //如果是飞行单位，要让它掉到地上
                if(m_strTheInfo->theMonsterInfo->ifFly){
                    
                    float allTime = m_pAction_Die->getDuration();
                    
                    m_pTheSrite->runAction(MoveTo::create(allTime, Vec2::ZERO));
                }
                
             
                
                //theSequence->setTag(SOLDIER_ANIMATE_TAG);
                //m_pTheSrite->runAction(theSequence);
                
                m_iAnimateState = targetAniamte;
    
                if(m_strTheInfo->monsterId==11)
                  {
                    
                    m_pTheSrite->runAction(Sequence::create(
                                                            Spawn::create(MoveTo::create(0.5,Vec2(m_pTheSrite->getPositionX(),m_pTheSrite->getPositionY()+20)),FadeOut::create(0.5), NULL),DelayTime::create(0.1),CallFunc::create([=](){
                       
                        if(m_strTheInfo->rare>=4)
                        {
                            m_pTheSrite->setVisible(false);
                            bool temp = ifSkillStart(7);
                           
                            if(temp)
                           {
                                CBaseAttack* theAttack = CBaseAttack::create();
                           theAttack->setAttackerGroup(m_iFightGroup);
                            theAttack->setCameraMask((unsigned short )CameraFlag::USER1);
                            m_vecAllAttackes.pushBack(theAttack);
                            theAttack->setSkillIdAndStart(m_pPassiveSkill,this,this);
                            getParent()->addChild(theAttack);
                            }
                            
                           
                       }
                    }),NULL));
                      
                  }
            }
            
            break;
        }
            
        default:
            break;
    }
    
    
}

void CBaseFighter::attackAction(){
    
    
    //此处暂时这么写。因为如果击杀怪物以后，就有了新的目标。此时如果不做距离判断，则可以“无视距离进行攻击”
    
    
    if (m_pAttackTarget&&m_pAttackTarget->getReferenceCount()>0&&getFightDistance(m_pAttackTarget)<m_strTheInfo->attackRang)
    {
        
        float distance = getFightDistance(m_pAttackTarget);
        
        //如果有子弹信息，并且距离大于危险距离，则发射子弹，否则就直接攻击
        if (m_strTheInfo->theBulletInfo) { //&&distance>m_strTheInfo->theBulletInfo->bulletMinRange
            
            this->bulletStart();
            
        }else{
            m_pAttackTarget->beAttacked(this,0,1,m_iFightGroup);
            
                //此处大坑，要加受伤效果，但是要自己被弹死，因为自己被弹死，将会把m_pAttackTarget清掉
            if (m_pAttackTarget) {
                
                
                
                //给目标加上被攻击的效果。两种随机
                int which = rand()%2;
                
                Sprite3D* hurt3D = Sprite3D::create();
                float delayTime = 0;
                
                if (which) {
                    Sprite*temp = Sprite::createWithSpriteFrameName("fight_knifeHurt_1.png");
                    
                    Animate* theAnimate = CFightManager::getAnimateWithPath("fight_knifeHurt_", 1, 6);
                    
                    delayTime = theAnimate->getDuration();
                    
                    temp->runAction(Sequence::create(theAnimate,RemoveSelf::create(), NULL));
                    hurt3D->addChild(temp);
                    //                CFightManager::addCamera(temp);
                    
                    temp->setScale(1.5f);
                    
                    if (m_pAttackTarget->getPositionX()>getPositionX()) {
                        temp->setFlippedX(true);
                    }
                    //                temp->setPosition3D(Vec3(0,m_pAttackTarget->getPhysicalPosition().y,0));
                    
                    
                }else{
                    auto hurtAction = Animate::create(AnimationCache::getInstance()->getAnimation("hurtAnimation"));
                    
                    delayTime = hurtAction->getDuration();
                    //    auto billBord = BillBoard::create();
                    auto billBord = Sprite::createWithSpriteFrameName("hit1.png");
                    
                    //                billBord->setPosition3D(Vec3(0,3+m_pAttackTarget->getPhysicalPosition().y,0));
                    billBord->setScale(1);
                    //                billBord->setCameraMask((unsigned short )CameraFlag::USER1);
                    
                    billBord->runAction(Sequence::create(hurtAction,RemoveSelf::create(), NULL));
                    //                m_pAttackTarget->addChild(billBord,100);
                    hurt3D->addChild(billBord);
                }
                
                CFightManager::addCamera(hurt3D);
                hurt3D->setPosition3D(Vec3(0,m_pAttackTarget->getPhysicalPosition().y+30,0));
                m_pAttackTarget->addChild(hurt3D);
                hurt3D->runAction(Sequence::create(DelayTime::create(delayTime),RemoveSelf::create(), NULL));
                
                if (m_pAttackTarget->getAttackType()!=eAttackType_city) {
                    m_pAttackTarget->animateChange(eFighteState_Hurt);
                }

            }
            
            
    }
    
    }
    
}




void CBaseFighter::animationIdleCallBack(){
    
    m_pTheSrite->stopActionByTag(101);
    
    if (!m_bIfDied) {//如果动画结束了，并且没死，则进入空闲动画
        //                    m_iAnimateState = eFighteState_Idle;
        animateChange(eFighteState_Idle,true);//因为空闲动画优先级比特殊攻击低，因此需要强制播放动画
    }
    
}




CBaseBullet* CBaseFighter::bulletStart(int bulletId){
    
    
    //如果已经死亡，则不发子弹了
    if (m_bIfDied) {
        return NULL;
    }
    
    
    
    if (!m_pAttackTarget) {
        return NULL;
    }
    
    CBaseBullet* temp = CBaseBullet::create();
    
    temp->setPosition3D(getPhysicalPosition()+Vec3(0, m_pTheSrite->boundingBox().size.height*0.4f, 0));
    
    
    
    if (getParent()) {
        getParent()->addChild(temp);
        temp->setAttackerAndPositionAndRun(this, m_pAttackTarget->getPhysicalPosition()+Vec3(0, m_pAttackTarget->gettheSprite()->boundingBox().size.height*0.4f, 0),bulletId);
        m_vecAllBullets.pushBack(temp);
    }else{
//        CC_SAFE_DELETE(temp); //此处大坑。不可贸然使用delete，哪怕你有个safe！！！
        temp = NULL;
        
    }
    
    
    
    return temp;
    
}




void CBaseFighter::playAniamteAction(cocos2d::ActionInterval *theAction,bool ifloop,bool bReverse){
    
    
    if ((m_stMonsterDebuff.ifFreeze||m_stMonsterDebuff.ifDizzy)
        &&theAction!=m_pAction_Die) {//冰冻状态下不播放除死亡意外的动画
        
        return;
    }
    
    if (!theAction) {
        return;
    }
    
    if (!m_pTheSrite) {
        return;
    }
    
    
    m_pTheSrite->stopAllActions();
   m_pTheSrite->stopActionByTag(SOLDIER_ANIMATE_TAG);
    
    if(ifloop){
        
        //        cocos2d::ActionInterval * temp = RepeatForever::create(theAction->clone());
        //
        //        temp->setTag(SOLDIER_ANIMATE_TAG);
        
        //        m_pTheSrite->runAction( temp);
        
        cocos2d::ActionInterval * temp = RepeatForever::create(theAction);
        
        if(bReverse)
        {
             temp = temp->reverse();
        }
        m_pTheSrite->runAction( temp);
        
        
    }else{
        
        //        cocos2d::ActionInterval * temp =theAction->clone();
        //
        //        temp->setTag(SOLDIER_ANIMATE_TAG);
        
        //        m_pTheSrite->runAction(temp);
        
        if(bReverse)
        {
            theAction = theAction->reverse();
        }
        
        m_pTheSrite->runAction( theAction);
    }
    
    
}



void CBaseFighter::beAttacked(CBaseFighter* target,int attk,float damagePercent,FightGroup attackerGroup ,DamageType type){

    
//    CCLOG("pvpNowFightTime--beattacked::%f",CFightData::getInstance()->getAllFightTime());
//    CCLOG("pvpNowFightPos--beattacked::%f,%f",getPositionX(),getPositionY());
//    
    
    char buf[128];
    if(target)
    {
        monsterInfo* pMonster = CDataManager::getMonsterInfoByMonsterId(target->getTheInfo()->monsterId);
        sprintf(buf, "music/%s", pMonster->attackEffect.c_str());
        SimpleAudioEngine::getInstance()->playEffect(buf);
    }
    
    //说话
    if (ifCanTalk()) {
        CFightManager::addTheFightTalkToFighter(this, 4);
    }
    
    
    //被攻击时触发
    bool temp = ifSkillStart(1);
    
    
    float damageReduceRatio =0;
    float damageBacklashRatio = 0;
    
    
    if(temp&&m_iNextSkill){
        
        switch (m_iNextSkill->theSkillInfo->skillId) {
            case eAllSkills_dundang:
            {
                skillNameShow();
                damageReduceRatio = m_iNextSkill->damageRatio;
                
                //加入特效
                Sprite* temp = Sprite::createWithSpriteFrameName("fight_shield_1.png");
                
                temp->setScale(1);
                
                temp->setPosition3D(Vec3(0, m_pTheSrite->getBoundingBox().size.height*0.5f, 0));
                
                Animate* theAnimate = CFightManager::getAnimateWithPath("fight_shield_", 1, 10);
                
                temp->runAction(Sequence::create(theAnimate,RemoveSelf::create(), NULL));
                
                CFightManager::addCamera(temp);
                
                addChild(temp);
            }
                
                break;
                
            case eAllSkills_dunfan:
                skillNameShow();
                damageBacklashRatio = m_iNextSkill->damageRatio;
                
                //测试用，反弹1000倍伤害
                //                damageBacklashRatio = 1000;
                
                break;
                
            case eAllSkills_jushipifu:
                skillNameShow();
                
                if (target&&target->getTheInfo()->attackRang<100) {//对近战攻击者，添加眩晕buff
                    
                    
                    CFightManager::addTheBuffToFighter(target,(AllBuffes)m_iNextSkill->theSkillInfo->buffId);
        
                }
                
                
                break;

                
            default:
                break;
        }
    }
    
    float random = (rand()%20)*1.0/100+0.9f;   //攻击浮动范围为0.9~1.1
    
    if (CFightData::getInstance()->getTheFightScene()&&CFightData::getInstance()->getTheFightScene()->gettheFightSceneType()==eFightSceneType_PVP) { //pvp时，不随机
        random=1;
    }

    int hurtNum=attk*damagePercent*(1-damageReduceRatio)*random;

    m_iDeadlyGroup=attackerGroup;
    
    
    if (target) {
        
        if (attk==0) {//如果没有传攻击力，则按攻击者的攻击力计算
            hurtNum = target->getNowAttack()*damagePercent*(1-damageReduceRatio)*random;
        }
        m_iDeadlyGroup = target->getFightGroup();
        
        float ratio = CFightManager::getTheAtrEffect(target->getTheInfo()->monsterType,getTheInfo()->monsterType); //获得属性相克系数
        
        if(m_strTheInfo->monsterId == 5)
        {
            if(m_fReDamagexishu>0)
            {
                float fScale = m_iNowHP*1.0f/m_iMaxHP*1.0f;
                
                if(fScale<=0.5&&!m_bShixue)
                {
                    //全身变红，攻击力提升50%
                    
                    std::string strName = m_strTheInfo->theFighterSkillInfo[0].theSkillInfo->skillNamePic;
                
                    m_iNowAttack += m_iNowAttack*m_fReDamagexishu;
                    
                     int iAtk = target->getNowAttack();

                    if(iAtk <=m_iNowHP)
                    {
                        std::string strName = m_strTheInfo->theFighterSkillInfo[0].theSkillInfo->skillNamePic;
                        m_iNowAttack += m_iNowAttack*m_fReDamagexishu;
                        
                        skillNameShow(strName);
                        
                        m_pTheSrite->setOpacity(255);
                        
                        float fScale = m_pTheSrite->getScale();
                        
                        m_pTheSrite->setScale(fScale*1.2);
                        
                        m_bShixue = true;
                    }
                }

            }
        }
        
        if(target->m_strTheInfo->monsterId == 3) //骷髅吸血
        {

            if(target->getTheXishu()>0)
            {
                float fAtkNum =  hurtNum*target->getTheXishu();
                
                if(fAtkNum<1)
                {
                    fAtkNum = 1;
                }
                
                target->becured((int)fAtkNum,1);
                
            }
            
        }
        
        if(m_fReDamagexishu>0)
        {
            if(m_strTheInfo->monsterId == 11)  //亡灵牧师灵体近战伤害
            {
                //m_strTheInfo->attackRang<100
                
                if(target->m_strTheInfo->attackRang<100)
                {
                    ratio -= m_fReDamagexishu;
                }
            }
            else
            {
                ratio -= m_fReDamagexishu;
                
            }
            
        }
        
        hurtNum = hurtNum* ratio;
        
        hurtNum = MAX(hurtNum, 1);

        //如果有攻击者，则反弹伤害
        if(damageBacklashRatio!=0){
            
            int damageNum = MAX(target->getNowAttack()*damageBacklashRatio, 1) ;  //测试用，看到就删掉10000
            target->beAttacked(this,damageNum,1,m_iFightGroup,eDamageType_SoliderSkill);//注意，此处使用攻击者攻击力来算反弹伤害。而不是用所受到的伤害来算反弹伤害。类似lol及dota的处理方式
        }
    }
    
    
    
    //如果是pvp录像回放，则不会在被攻击时改变血量
    CFightScene* theScene = CFightData::getInstance()->getTheFightScene();
    if (theScene&&theScene->getthePVPMode()!=EPVPMode_REC){
        
        //改变血条及死亡判断
        if(type!=eDamageType_GeneralAttack||m_iFightGroup!=eFightGroup_Freind)   //type != eDamageType_GeneralAttack &&CFightData::getInstance()->getTheLevelCondition()!=eLevelCondition_TowerDefense)//一直显示伤血数字,则为true
        {
            
            changeHP(hurtNum, true,type,target,0);
            
        }else{
            
//            if (type==eDamageType_GeneralAttack) {
//                changeHP(hurtNum, false,type,target,0);
//            }
            changeHP(hurtNum, false,type,target,0);
        }
        
    }
    
    
    
    
    //受击的效果表现,懒加载
//    if (!_hurtShader) {
//        _hurtShader = (CSprite3dAnimateShaderOnce*)CFightManager::addShaderToSprite3D(m_pTheSrite, eShadeType_3D_beAttacked);
//    }
    
    
    
    
    //    BillBoard * theHurt = BillBoard::create("CloseNormal.png");
    //
    
    
    
    //受击效果
//    auto hurtAction = Animate::create(AnimationCache::getInstance()->getAnimation("hurtAnimation"));
////    auto billBord = BillBoard::create();
//    auto billBord = Sprite::createWithSpriteFrameName("hit1.png");
//    
//    billBord->setPosition3D(Vec3(0,30+m_pTheSrite->getPositionY(),0));
//    billBord->setScale(1, 1);
//    billBord->setCameraMask((unsigned short )CameraFlag::USER1);
//    
//    billBord->runAction(Sequence::create(hurtAction,RemoveSelf::create(), NULL));
//    addChild(billBord,100);
    
    
    //
    //    theHurt->setScale(5, 5);
    //
    //    Label * thehurt = Label::createWithSystemFont("999", "Arial", 100);
    //
    //    theHurt->setPosition3D(getPosition3D()+Vec3(0,50,0));
    //
    //
    ////    thehurt->setPosition(Point(0,100));
    //
    ////    thehurt->enableOutline(Color4B(100,100,200,255));
    ////    thehurt->setRotation3D(Vec3(90,0,0));
    //
    ////    thehurt->setPosition3D(getPosition3D());
    //
    //
    ////    thehurt->setScale(100);
    //
    //    theHurt->addChild(thehurt);
    //
    ////    thehurt->setCameraMask((unsigned short )CameraFlag::USER1);
    //////
    //
    //    thehurt->setCameraMask((unsigned short )CameraFlag::USER1);
    //
    //    theHurt->setCameraMask((unsigned short )CameraFlag::USER1);
    //
    //    theHurt->setMode(BillBoard::Mode::VIEW_POINT_ORIENTED);
    //
    //    getParent()->addChild(theHurt);
    
    
}

void CBaseFighter::changeHP(int hurtNum, bool ifText,DamageType type,CBaseFighter* target,int ifForceDied){
    
    
    //pvp记录伤害数值
    CFightScene* theScene = CFightData::getInstance()->getTheFightScene();
    if (theScene&&theScene->getthePVPMode()==EPVPMode_PLAY&&m_iTrueId!=-1) {
        
        bool ifDied = false;
        
        if(m_iNowHP -hurtNum<0){
            ifDied=true;
        }
        
        ((CFightScenePVP*)theScene)->addRecordStringWithChangeHP(m_iTrueId, hurtNum, type,ifDied);
    }
    
    
    
    
    
    //伤血数字 ，己方所受伤害的普通攻击则去掉
    
    if(ifText)   //type != eDamageType_GeneralAttack &&CFightData::getInstance()->getTheLevelCondition()!=eLevelCondition_TowerDefense)//一直显示伤血数字,则为true
    {
        //        Sprite* minValue = Sprite::createWithSpriteFrameName("blood_minus.png");
        //        addChild(minValue, 100);
        //        minValue->setAnchorPoint(Vec2(0, 0.5));
        //        minValue->setPosition3D(Vec3(minValue->getContentSize().width, 70, 0));
        //        minValue->setCameraMask((unsigned short )CameraFlag::USER1);
        //        minValue->setScale(0.5f);
        //        minValue->runAction(Sequence::create(Spawn::create(MoveBy::create(0.2f, Point(0,10)),ScaleTo::create(0.2f, 1), NULL),MoveBy::create(0.5f,Vec2(0, 20)) ,CallFunc::create([=](){
        //            minValue->removeFromParentAndCleanup(true);
        //        }),  NULL));
        
        
        __String* hurtStr = __String::createWithFormat("%d",hurtNum);
        
        Label* theHurt = NULL;
        
        if (m_iFightGroup==eFightGroup_Freind) {
            theHurt = Label::createWithCharMap("fonts/Damage_num.png", 24, 36, '0');
        }else{
            theHurt = Label::createWithCharMap("fonts/Damage_num2.png", 24, 36, '0');
        }
        
        
        //伤害字大小
        float startScale = 0.2f;
        float endScale = 0.5;
        
        if (type!=eDamageType_GeneralAttack &&CFightData::getInstance()->getTheLevelCondition()!=eLevelCondition_TowerDefense) {
            startScale = 0.5f;
            endScale = 1;
        }
        
        //        theHurt->setColor(Color3B::RED);
        //        theHurt->setAnchorPoint(Vec2(0, 0.5));
        //        theHurt->setPosition3D(Vec3(minValue->getPositionX() + minValue->getContentSize().width,70,0));
        theHurt->setPosition3D(Vec3(0, m_pTheSrite->getBoundingBox().size.height+getPhysicalPosition().y, 30));
        
        theHurt->setString(hurtStr->getCString());
        theHurt->setCameraMask((unsigned short )CameraFlag::USER1);
        addChild(theHurt,11);
        
        theHurt->setGlobalZOrder(1000);
        
        theHurt->setBlendFunc(cocos2d::BlendFunc::ALPHA_NON_PREMULTIPLIED);
        //theHurt->enableOutline(Color4B(0,0,0,255));
        
        
        EaseSineOut *theMoveAction = EaseSineOut::create(MoveBy::create(0.4f, Vec3(0, 30, 0)));   //  EaseElasticOut 是有个回弹。   EaseBackOut 也是有回弹，但是不是那么快的回弹，比较柔和。 CCEaseBounceOut 是震动
        
        //        Spawn *flyAction = Spawn::create(theMoveAction,FadeOut::create(0.6f), NULL);
        
        
        
        theHurt->setScale(startScale);
        theHurt->runAction(Sequence::create(Spawn::create(MoveBy::create(0.1f, Point(0,10)),ScaleTo::create(0.1f, endScale), NULL),theMoveAction,CallFunc::create([=](){
            theHurt->removeFromParentAndCleanup(true);
        }),  NULL));
    }
    
    
    
    if(m_strTheInfo->monsterId == 10&&m_fReDamagexishu>0)
    {
        
    }
    
    else
    {
        //如果血量降到一定百分比，则会被击飞
        
        if (ifClickFly(hurtNum)&&target&&m_strTheInfo->theMonsterInfo->walkSpeed!=0&&!m_bIfLevelBoss&&CFightData::getInstance()->getTheLevelCondition()!=eLevelCondition_TowerDefense) {
            
            float beatBackDist = 200;
            float beatFlyDist = 100;
            float beatTime = 1;
            
            this->beatBackFrom(target->getPosition3D(), beatBackDist, beatTime,beatFlyDist);
            CFightManager::addTheBuffToFighter(this, (AllBuffes)eAllBuffes_jitui,beatTime);
        }
    }
    
    
    
    
    
    //血条变化
    m_iNowHP -= hurtNum;
    m_iNowHP = MAX(m_iNowHP, 0);
    
    
    //如果是强制死亡，则血量直接归0
    if (ifForceDied==1) {
        m_iNowHP=0;
    }
    
    
    
    //if(m_strTheInfo->monsterId == 6||m_st)
    {
        LevelCondition target = CFightData::getInstance()->getTheLevelCondition();
        
        if(target == eLevelCondition_killBoss)
        {
            CFightScene* pScene = CFightData::getInstance()->getTheFightScene();
            
            
            
            float tx = m_iNowHP*1.0/m_iMaxHP;
            
             if(m_bIfLevelBoss)
             {
                 pScene->gettheBossProgress()->setPercentage(tx*100);
             }
            //pScene->gettheWhiteBoss()->runAction(ScaleTo::create(1,tx,1));
            
        }
    }
    
    float hpScale = m_iNowHP*1.0f/m_iMaxHP;
    
    m_pTheHpBar->toHPScale(hpScale*100);
    
    
    //状态变化
    if (m_iNowHP<=0) {
        if (!m_isCut) {
            setDeadlyTarget(target);
            //            m_pDeadlyTarget = target;
            onDied(type);
            
            //如果是死了，则致死者也会说话
            if (target&&target->ifCanTalk()) {
                CFightManager::addTheFightTalkToFighter(target, 6);
            }
            
            if (target&&target->ifCanTalk()) {
                CFightManager::addTheFightTalkToFighter(target, 6);
            }
        }
        else{
            if(!(CPersonalData::getInstance()->getCutGuide()) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT)
               && (guiderManagerLayer::getInstance()->getStepNum() == 5))
            {
                CPersonalData::getInstance()->setCutGuide(true);
                NotificationCenter::getInstance()->postNotification("guideCut");
            }
        }
    }else{
        
        
        
    }
    
    
    
    
    //受击效果表现
    if (_hurtShader&&_hurtShader->getFadeState()==2) {
        _hurtShader->setFadeState(0);
        _hurtShader->setShiningDuration(0);
    }
    
    if (_hurtShaderBuilding&&_hurtShaderBuilding->getFadeState()==2) {
        _hurtShaderBuilding->setFadeState(0);
        _hurtShaderBuilding->setShiningDuration(0);
    }

    
    
    
    
}







bool CBaseFighter::ifClickFly(int damage){
    
    
    
//    if (m_stMonsterDebuff.ifBeatBack) {//如果已经处于击飞状态，则不会再被击飞
//        return false;
//    }
    
    
    
    float hpNum[1] = {0.2};
    
    
    for (int i=0; i<1; i++) {
        
        float oldRatio = m_iNowHP*1.0/m_iMaxHP;
        
        float newRatio = (m_iNowHP-damage)*1.0/m_iMaxHP;
        
        if (oldRatio>hpNum[i]&&newRatio<=hpNum[i]) {
            
            //满足要求后，一定几率会被击飞,
            
            int ifbeatFly = rand()%3; // 1/3的几率会被击飞
            
            
            if (CFightData::getInstance()->getTheFightScene()&&CFightData::getInstance()->getTheFightScene()->gettheFightSceneType()==eFightSceneType_PVP) { //pvp时，不击飞
                ifbeatFly=0;
            }

            
            if (ifbeatFly==1) {
                return true;
            }
            
        }
    }
    
    
    return false;
}


void CBaseFighter::becured(int number,bool bxixue){
    
  
    //血条变化
    m_iNowHP += number;
    m_iNowHP = MIN(m_iNowHP, m_iMaxHP);
    
    float hpScale = m_iNowHP*1.0f/m_iMaxHP;
    
    m_pTheHpBar->toHPScale(hpScale*100);
    
    //治疗特效
    
    ParticleSystem * explodePar = NULL;
    
    if(bxixue)
    {
        //explodePar = ParticleSystemQuad::create("particleSystem/VampiricCure.plist");
        
        XixueAnimate();
    }
    else
    {
        SimpleAudioEngine::getInstance()->playEffect("music/blood.mp3");
        
        explodePar = ParticleSystemQuad::create("particleSystem/cure2.plist");
        
        explodePar->setScale(0.5f);
        explodePar->setPosition3D(Vec3(0, gettheSprite()->getBoundingBox().size.height / 10, 0)) ;
        explodePar->setCameraMask((unsigned short )CameraFlag::USER1);
        explodePar->setAutoRemoveOnFinish(true);
        addChild(explodePar, 11, 100) ;
        
    }

  
    //    explodePar->setGlobalZOrder(1001);
    
    
    //治疗数字
    __String* hurtStr = __String::createWithFormat("%d",number);
    
    Label* theHurt = NULL;
    
    theHurt = Label::createWithCharMap("fonts/cure_num.png", 24, 36, '0');
    
//    theHurt->setAnchorPoint(Vec2(0, 0.5));
    theHurt->setPosition3D(Vec3(0, m_pTheSrite->getBoundingBox().size.height+getPhysicalPosition().y, 30));
    theHurt->setString(hurtStr->getCString());
    theHurt->setCameraMask((unsigned short )CameraFlag::USER1);
    addChild(theHurt,11);
    
    theHurt->setGlobalZOrder(1000);
    
    theHurt->setBlendFunc(cocos2d::BlendFunc::ALPHA_NON_PREMULTIPLIED);
    //theHurt->enableOutline(Color4B(0,0,0,255));
    
    theHurt->setScale(0.5f);
    theHurt->runAction(Sequence::create(Spawn::create(MoveBy::create(0.2f, Point(0,10)),ScaleTo::create(0.2f, 1), NULL),MoveBy::create(0.5f,Vec3(0, 20,0)) ,CallFunc::create([=](){
        theHurt->removeFromParentAndCleanup(true);
    }),  NULL));
    
    
}







void CBaseFighter::skillPrepare(bool ifShowName){
    
    
    
    //技能名字show
    if (ifShowName) {
        skillNameShow();
    }
    
    

    
    
    //确定目标对象
    if(m_iNextSkill){
        
        
        if(m_strTheInfo->monsterId==SOLDIER_DOCTOR){
            

            setSkillAttackTarget(m_pAttackTarget);//医生的攻击目标，就是医生的技能目标
            
//            m_pSkillTarget = m_pAttackTarget;//医生的攻击目标，就是医生的技能目标
            
        }else{
            
            //根据释放目标来更新面向
            
            //        if (m_iNextSkill->theSkillInfo->skillId==eAllSkills_huichunshu) {
            //
            //            //如果是回春术，则技能目标为攻击目标。特殊处理
            //            m_pSkillTarget = m_pAttackTarget;
            //        }else
            {
                if (m_iNextSkill->theSkillInfo->positionGroup==-1) {//如果目标点为自己。则选择自己
                    setSkillAttackTarget(this);
                }else{
                    CBaseFighter* temp = CFightData::getInstance()->getAttackerByGroupAndCondition(m_iFightGroup, m_iNextSkill->theSkillInfo->positionCondition, m_iNextSkill->theSkillInfo->positionGroup,this);
                    
                    setSkillAttackTarget(temp);
//                    m_pSkillTarget= CFightData::getInstance()->getAttackerByGroupAndCondition(m_iFightGroup, m_iNextSkill->theSkillInfo->positionCondition, m_iNextSkill->theSkillInfo->positionGroup,this);
                }
            }
        }
        
        
        if(m_iAttackType!=eAttackType_tower){//防御塔不会在释放技能时更新面向
            roatationUpdate(0);
        }
        
        animateChange(eFighteState_SpecialAttack);
        
        
        //到了伤害触法点在生成技能
        runAction(Sequence::create(DelayTime::create(m_strTheInfo->theModelInfo->SpecialDamageTime),CallFunc::create(CC_CALLBACK_0(CBaseFighter::skillStart, this)), NULL));
        
    }
    
    
    
    
    
}

void CBaseFighter::blowShake(bool ifShake)
{
    stopActionByTag(SOLDIER_SHAKE_TAG);
    
    if (ifShake) {
        Vec3 enemyPos = getPosition3D();
        ActionInterval* target = RepeatForever::create(Sequence::create(MoveTo::create(0.08, Vec3(enemyPos.x - 1, enemyPos.y,enemyPos.z)),
                                                                        MoveTo::create(0.08, Vec3(enemyPos.x + 1, enemyPos.y,enemyPos.z)),
                                                                        NULL));
        target->setTag(SOLDIER_SHAKE_TAG);
        runAction(target);
    }
    
    
    //        stateChange(eFighteState_Idle);
    
}


void CBaseFighter::skillStart(){
    
    //如果是炸弹人，则标志位更改，表示已自爆
    if (m_strTheInfo->theMonsterInfo->monsterId==16&&!m_bIfBombed) {
        m_bIfBombed = true;
    }
    
    
    
    switch (m_iNextSkill->theSkillInfo->skillId) {
        case eAllSkills_luanshe:
        {
            runAction(Repeat::create(Sequence::create(CallFunc::create([=](){
                
                CBaseBullet* temp = bulletStart();
                
                if (temp) {
                    temp->setDamagePercent(m_iNextSkill->damageRatio);
                }
             
            }),DelayTime::create(0.1f), NULL), 3));
            
            break;
            
        }
            
        case eAllSkills_zhimingsheji:
        {
            CBaseBullet* temp = bulletStart(9);
            
            if (temp) {
//                temp->setScale(1);//测试用
                temp->setDamageLifePercent(m_iNextSkill->damageRatio);
            }
            
            break;
        }
        case eAllSkills_xixue:
            break;
        
    
        case eAllSkills_wushuyanxi:
            break;
//        case eAllSkills_jurenhuiji:
//         //  break;
//        case eAllSkills_jushipifu:
         // break;
            
     
        case eAllSkills_silinqiyue:
        {
            
            Vec3  t3 = getPosition3D();
            int i1 = random(-100, 100);
            int i2 = random(-100, 100);
            
            if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
            {
                i1 = 10;
                i2= 0;
                
            }
            Vec3 VPos = Vec3(t3.x+i1,t3.y,t3.z+i2);
    
            CFightScene* pFightScene =  CFightData::getInstance()->getTheFightScene();
            
            CFightScene *pScene =  CFightData::getInstance()->getTheFightScene();
            
            Layer *pLayer3D = pScene->gettheLayer3D();
        
            Sprite3D *pFazhe = Sprite3D::create();
            Sprite * pTemp = Sprite::create("call_method.png");
            pFazhe->setRotation3D(Vec3(90,0,0));
            pTemp->cocos2d::Node::setScale(0.8);
            pFazhe->addChild(pTemp);
            
            pFazhe->setCameraMask((unsigned short )CameraFlag::USER1);
            
            pFazhe->setVisible(false);
            
            pTemp->runAction(RepeatForever::create(RotateBy::create(0.1f, 10)));
            pFazhe->runAction(Sequence::create(DelayTime::create(1),RemoveSelf::create(),NULL));
            
            if(pFightScene)
            {
                FighterInfo* pInfo = NULL;
            
                if(m_iFightGroup == eFightGroup_Freind )
                {
                    pInfo  = CFightData::getInstance()->getTheCallKulouFriend();
                    
                    
                    if(CPersonalData::getInstance()->getEnemyType() == SPECIFIED_ENEMY) //pvp录像回放模式的话，骷髅召唤取敌人的
                    {
                        pInfo  = CFightData::getInstance()->getTheCallKulouEnemy();
                        
                    }
                    
                    CBaseFighter* pKulou = pFightScene->addMonsterWithIdAndGroupAndPosition(pInfo,eFightGroup_Freind,Vec2(VPos.x,VPos.z),
                                                                                        eAttackType_monster,false,false,false,true);
                   
                    
            
                    pKulou->setPositionY(-50);
                    
                    pKulou->runAction(MoveTo::create(2,Vec3(pKulou->getPositionX(),0,pKulou->getPositionZ())));
        
                    Vec3 pPosition =  pKulou->getPosition3D();
                    
                    pFazhe->setPosition3D(Vec3(pPosition.x,0,pPosition.z));
                    
                    pLayer3D->addChild(pFazhe);
                    
                    
                }
                else if(m_iFightGroup == eFightGroup_Enemy)
                {
                    pInfo  = CFightData::getInstance()->getTheCallKulouEnemy();
                    CBaseFighter* pKulou=  pFightScene->addMonsterWithIdAndGroupAndPosition(pInfo,eFightGroup_Enemy,Vec2(VPos.x,VPos.z),eAttackType_monster,false,false,false,true);
                    
                    pKulou->setPositionY(-50);
                    
                    pKulou->runAction(MoveTo::create(2,Vec3(pKulou->getPositionX(),0,pKulou->getPositionZ())));
                    Vec3 pPosition =  pKulou->getPosition3D();
                    
                    pFazhe->setPosition3D(Vec3(pPosition.x,0,pPosition.z));
                    
                    pLayer3D->addChild(pFazhe);
                    
                   
                    
                }
            }
        }
         break;
        
        case eAllSkills_jushipifu:
            break;
      

//        case 1005://测试用，看到就删
//        {
//            CCLOG("投石机fire");
//        }
            
        
        default:
        {
            
            if(m_iNextSkill->theSkillInfo->skillId == eAllSkills_jushipifu)
            {
                log("jushipifu");
                
                log("2222333");
                
                
                
            }
        
            CBaseAttack* theAttack = CBaseAttack::create();
            theAttack->setAttackerGroup(m_iFightGroup);
            
            theAttack->setCameraMask((unsigned short )CameraFlag::USER1);
            
            
            
            if(m_pSkillTarget&&m_pSkillTarget->getReferenceCount()>0&&m_pSkillTarget->getReferenceCount()<100&&getParent()){ //防止野指针，以此判断
                
                m_vecAllAttackes.pushBack(theAttack);
                theAttack->setSkillIdAndStart(m_iNextSkill,this,m_pSkillTarget);
                getParent()->addChild(theAttack);
                
                
                
                
            }
//            else{
//                
//                CC_SAFE_DELETE(theAttack);  //此处大坑。不可贸然使用delete，哪怕你有个safe！
//            }
            
            
            
            break;
        }
            
    }
    
    
    
    
    //    else{
    //        theAttack->setSkillIdAndStart(m_iNextSkill,this,NULL);//不移动的话，则目标位置为0
    //    }
    
    
    //    __String *target = __String::createWithFormat("%s1.png",m_strTheInfo->skillPath.getCString());
    //
    //    Sprite* theSprite = Sprite::createWithSpriteFrameName(target->getCString());
    //
    
    //    theSprite->setVisible(false);
    //    theSprite->setScaleX(scale);
    //    theSprite->setRotation(startRotate);
    //    theSprite->setOpacity(0);
    
    //测试用
    //    theSprite->setScale(0.05f);
    //
    //
    //
    //
    //    theSprite->setAnchorPoint(Point(0.5f,0.5f));
    //
    //    theSprite->setPosition3D(m_v2AttackPosOffset);
    //
    //    theSprite->setRotation3D(-Vec3(0,90,0));
    //
    //
    //    theSprite->setGlobalZOrder(1000);
    //    m_pTheSrite->addChild(theSprite);
    //
    //    theSprite->setCameraMask((unsigned short )CameraFlag::USER1);
    //
    //
    //    Animation * skillAnimation = Animation::create();
    //
    //    for (int i=1; i<=m_strTheInfo->skillFrameNum; i++) {
    //
    //        __String *temp = __String::createWithFormat("%s%d.png",m_strTheInfo->skillPath.getCString(),i);
    //        skillAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(temp->getCString()));
    //
    //    }
    //    skillAnimation->setDelayPerUnit(0.05f);
    //
    //
    //    theSprite->runAction(Sequence::create(Animate::create(skillAnimation),RemoveSelf::create(),CallFunc::create([=](){
    //
    //
    //
    //        //如果子弹属于动画完成就消失的，则判断
    //
    //        if (theAttack&&theAttack->getReferenceCount()>0&&theAttack->getAttackInfo().MaxlifeTime==0) {
    //            theAttack->removeFromParentAndCleanup(true);
    //        }
    //
    //    }) , NULL));
    
    
    
    
    //    theSprite->setBlendFunc(cocos2d::BlendFunc::ALPHA_PREMULTIPLIED);
    
    //    auto spawn1 = Spawn::create(ScaleTo::create(speed,2.5, 2.5),RotateBy::create(speed, rotate),FadeIn::create(0), NULL);
    //
    //    auto spawn2 = Spawn::create(FadeOut::create(0.5f),ScaleTo::create(0,scale, 2.5),RotateTo::create(0, startRotate), NULL);
    //
    //
    //    theSprite->runAction(Sequence::create(spawn1,spawn2, NULL));
    
    
    
}

bool CBaseFighter::ifSkillStart(int startCondition){

 
    std::vector<FighterSkillInfo*> StartSkillVec;//备选的触发技;
    std::vector<FighterSkillInfo>* Allskill = &(m_strTheInfo->theFighterSkillInfo);

    
    int i=-1;
    for (std::vector<FighterSkillInfo>::iterator ite = Allskill->begin(); ite!=Allskill->end(); ite++)
    {
        i++;
        if ((*ite).skillLevel==0)
        {
            continue;
        }
        
        if ((*ite).theSkillInfo->startCondition!=startCondition)
        {
            continue;
        }
        
        float possible = rand_0_1();
        
        //测试技能用

       //possible = 0;

    
        if (CFightData::getInstance()->getTheFightScene()&&CFightData::getInstance()->getTheFightScene()->gettheFightSceneType()==eFightSceneType_PVP) { //pvp时，不随机
            
            if (m_strTheInfo->monsterId==1) {
//                CCLOG("I'm in");
            }
            
            skillRatios[i]-=0.2001f;
            possible=skillRatios[i];
        }
        
        if (possible<(*ite).startRatio) {
            skillRatios[i]=1.0f;
            StartSkillVec.push_back(&(*ite));
        }
        
    }
    
    
    //从中挑选最终触发的技能
    if (StartSkillVec.size()>0)
    {
        int randSkill = rand()%StartSkillVec.size();
        

        if (CFightData::getInstance()->getTheFightScene()&&CFightData::getInstance()->getTheFightScene()->gettheFightSceneType()==eFightSceneType_PVP) { //pvp时，不随机
            randSkill = skilltimes%StartSkillVec.size();
            skilltimes++;
        }
        
       // m_iNextSkill = StartSkillVec.at(randSkill);

        
        if(startCondition==4||startCondition == 7)
        {
            m_pPassiveSkill = StartSkillVec.at(randSkill);
        }
        else
        {
            m_iNextSkill = StartSkillVec.at(randSkill);
//            CCLOG("pvp触发技能名：%s",m_iNextSkill->theSkillInfo->skillName.c_str());
        }
        
        return true;
    }
    
    return false;
    
}


void CBaseFighter::OnUpdateEnergy()
{
    int temp = CFightData::getInstance()->getNowEnergy();
    
    temp+=50;
    
    CFightData::getInstance()->setNowEnergy(temp);
    
    /*CCFightScene* pScene =  CFightData::getInstance()->getTheFightScene();
    
    
    Layer* pLayer = pScene->getTheLayer();
    
    cocos2d::ProgressTimer*  pEnergyLine = (ProgressTimer*)pLayer->getChildByTag(FIGHT_ENERGY_CAOTAG);
    
    
    pEnergyLine->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(CBaseFighter::onEnergySetWhite,this)),DelayTime::create(0.1),CallFunc::create(CC_CALLBACK_0(CBaseFighter::onEnergySetNoraml,this)),NULL));
    
    */
    
}


void CBaseFighter::onEnergySetWhite()
{
    CFightScene* pScene =  CFightData::getInstance()->getTheFightScene();
    
    
    Layer* pLayer = pScene->getTheLayer();
    
    
    cocos2d::ProgressTimer*  pEnergyLine = (ProgressTimer*)pLayer->getChildByTag(FIGHT_ENERGY_CAOTAG);
    
    Sprite *pEnergyCon = pEnergyLine->getSprite();;

    
    pEnergyCon->setSpriteFrame("enemgyline3.png");
    
}


void CBaseFighter::onEnergySetNoraml()
{

     CFightScene* pScene =  CFightData::getInstance()->getTheFightScene();
     
     
     Layer* pLayer = pScene->getTheLayer();
     
     
     cocos2d::ProgressTimer*  pEnergyLine = (ProgressTimer*)pLayer->getChildByTag(FIGHT_ENERGY_CAOTAG);
     
     Sprite *pEnergyCon = pEnergyLine->getSprite();;
    
     pEnergyCon->setSpriteFrame("enemgyline.png");


     pEnergyLine->setPercentage(pEnergyLine->getPercentage()+0.01f);  //cocos2dx坑，改变精灵帧后，如果不改变percent，就不会重新绘制
}


void CBaseFighter::onAddEnergy()
{
    if (CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE) {
    
        CFightScene* pScene =  CFightData::getInstance()->getTheFightScene();
        
        Layer* pLayer = pScene->getTheLayer();
        
        Vec2  t = CommonData::GetSingletonPtr()->getPosOnScreen(pScene->getTheCamera(),getPosition3D());
    
        LabelAtlas* pExpLable= LabelAtlas::create("50","fonts/KillCoinNum.png", 36, 41,'0');
        pExpLable->setScale(1);
        
        pExpLable->setPosition( t);
        pLayer->addChild(pExpLable,99999);

        
        pExpLable->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.2,pExpLable->getScale()*1.5),ScaleTo::create(0.2,pExpLable->getScale()),NULL),DelayTime::create(0.2),RemoveSelf::create(),NULL));
    }
    
    
}


void CBaseFighter::onDied(DamageType type)
{
    
    CFightData::getInstance()->setlastDiedTarget(this);
    
    setAttackTarget(NULL);//因为是强引用，所以在死前必须清除，以免都删不掉
    setSkillAttackTarget(NULL);

    if (m_bIfDied) {//已经死了就不调用了
        return;
    }
    
    //如果是死了，则致死者也会说话
    if (ifCanTalk()) {
        CFightManager::addTheFightTalkToFighter(this, 7);
    }
    
    char buf[128];
    monsterInfo* pMonster = CDataManager::getMonsterInfoByMonsterId(m_strTheInfo->monsterId);
    sprintf(buf, "music/%s", pMonster->dieEffect.c_str());
    SimpleAudioEngine::getInstance()->playEffect(buf);
    
    m_bIfDied=true;
    stateChange(eFighteState_Die);
    
    //移除底座和阴影
    if(m_pvpShadow)
    {
        m_pvpShadow->removeFromParent();
        m_pvpShadow = NULL;
    }
    if(m_pvpDizuo)
    {
        m_pvpDizuo->removeFromParent();
        m_pvpDizuo = NULL;
    }
    
    if((m_iFightGroup==eFightGroup_Freind) && !m_bzhaohuan)
    {//计算我方阵亡士兵数量
        CFightData::getInstance()->setFightData(1);
        if(CFightData::getInstance()->getNowMonsterNum() > 0&&m_iAttackType==eAttackType_monster&&!m_bIfReinforce)
        {
            CFightData::getInstance()->setNowMonsterNum(CFightData::getInstance()->getNowMonsterNum() - 1);
            CCLOG("当前士兵数量%d", CFightData::getInstance()->getNowMonsterNum());
        }
        
        
        if(m_bSoul&&m_iAttackType!=eAttackType_min)
        {
            Sprite* soul = Sprite::createWithSpriteFrameName("soliderSoul_1.png");
            addChild(soul,10);
            soul->setPosition3D(Vec3(soul->getContentSize().width * 0.45f, 50, 0));
            soul->setCameraMask((unsigned short )CameraFlag::USER1);
            char buf[128];
            auto* pAnimation = Animation::create();
            for (int i = 1; i < 4; i++) {
                sprintf(buf, "soliderSoul_%d.png", i);
                pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
            }
            pAnimation->setDelayPerUnit(0.2f);
            Animate* pAction = Animate::create(pAnimation);
            ActionInterval* move = MoveBy::create(1.2f, Vec2(5, 100));
            ActionInterval* fade = FadeOut::create(1.5f);
            
            soul->runAction(RepeatForever::create(pAction));
            soul->runAction(move);
            soul->runAction(Sequence::create(fade,RemoveSelf::create(), NULL));
        }
        
        

//        soul->runAction(Sequence::create(Repeat::create(pAction, 2), RemoveSelf::create(), NULL));
//        soul->runAction(move);
//        soul->runAction(fade);
    }
    if((CDataManager::getGuideStepNum() == GUIDE_ADD_CIRCLE) && (m_iFightGroup==eFightGroup_Enemy) && (guiderManagerLayer::getInstance()->getStepNum() == 0) && (CFightData::getInstance()->getNowLevelId() == 2))
    {
        CFightData::getInstance()->setFirstSolider(true);
    }
    if((m_iFightGroup==eFightGroup_Enemy) && (type == eDamageType_HeroSkill))
    {
        CFightData::getInstance()->setFightData(7);
    }
    if((m_iFightGroup==eFightGroup_Enemy) && m_pDeadlyTarget && (m_pDeadlyTarget->m_iAttackType == eAttackType_monster))
    {
        CFightData::getInstance()->setFightData(5, m_pDeadlyTarget->m_strTheInfo->monsterId);//刀兵杀死的怪物数量
    }
    if (((m_iFightGroup==eFightGroup_Enemy) && (type == eDamageType_HeroSkill)) || (m_iDeadlyGroup==eFightGroup_Freind)){


        Vec3 deadPos = Vec3(-40, 30+getPhysicalPosition().y, 5);//m_pTheSrite->getBoundingBox().size.height;
        //CFightData::getInstance()->setFightData(5, m_pDeadlyTarget->m_strTheInfo->monsterId);//刀兵杀死的怪物数量
        
        
        //如果是普通战斗，则掉金币。特殊战斗，则掉魔法石
        if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense) {
            
            //根据怪物的耗费和等级，来计算获得魔法石数量
            
             if( eDamageType_Upgrade != type)
             {
                 int theNum = m_strTheInfo->theMonsterInfo->energyCost * m_strTheInfo->level;
                 CFightData::getInstance()->getTheFightScene()->gotTheSton(theNum, getPosition3D());
                 CFightData::getInstance()->addStone(theNum);
                 }
  
            //塔防金币掉落效果
            
            
            
        }else{
            //添加金币掉落效果
            
            
            if (CFightData::getInstance()->getFightType()!=FIGHT_PVP_TYPE&&m_iAttackType!=eAttackType_wall) {  //pvp模式不掉金币
                Sprite3D* baseNode = Sprite3D::create();
                addChild(baseNode);
                baseNode->setPosition3D(deadPos);
                
        
                char buf[128];
                auto* pAnimation = Animation::create();
                for (int i = 1; i < 6; i++) {
                    sprintf(buf, "coin_%d.png", i);
                    pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
                }
                pAnimation->setDelayPerUnit(0.1f);
                Animate* pAction = Animate::create(pAnimation);
                Sprite* coin1 = Sprite::createWithSpriteFrameName("coin_5.png");
                baseNode->addChild(coin1, 100);
                //            coin1->setGlobalZOrder(1000);
                
                float offsety = 85;
                float offsety2 = 100;
                
                if (m_strTheInfo->theMonsterInfo->ifFly) { //因为如果是空军，再弹那么高的金币就显得很难看到了。所以高度降低
                    offsety = 55;
                    offsety2 = 70;
                }
                
                ActionInterval* moveTop1 = MoveBy::create(0.5f, Vec2(0, offsety));
                ActionInterval* moveTop2 = MoveBy::create(0.5f, Vec2(0, offsety2));
                
                EaseBackOut* act1 = EaseBackOut::create(moveTop1);
                EaseBackOut* act2 = EaseBackOut::create(moveTop2);
                coin1->runAction(RepeatForever::create(pAction));
                baseNode->runAction(Sequence::create(act1, FadeOut::create(0.8f), RemoveSelf::create(),NULL));
                
                Sprite* addCoin = Sprite::createWithSpriteFrameName("add.png");
                baseNode->addChild(addCoin, 100);
                addCoin->setPosition3D(Vec3( coin1->getPositionX() + coin1->getBoundingBox().size.width , 0, 0));
                //            addCoin->runAction(Hide::create());
                //            addCoin->runAction(Sequence::create(Show::create(), act2, FadeOut::create(0.8f),
                //                                                RemoveSelf::create(), NULL));
                //            addCoin->setCameraMask((unsigned short )CameraFlag::USER1);
                
                int levelCoin = CDataManager::getTheLevelInfoWithId(CFightData::getInstance()->getNowLevelId())->goldNum;
                int coinValue =int(pMonster->energyCost*levelCoin/10000.0);
                
    
    
                Label* coinNum = Label::createWithCharMap("fonts/coin_figure.png", 30, 36, '0');
                baseNode->addChild(coinNum, 100);
                sprintf(buf, "%d", coinValue);
                //            coinNum->setGlobalZOrder(1000);
                coinNum->setString(buf);
                coinNum->setAnchorPoint(Vec2(0, 0.5));
                coinNum->setPosition3D(Vec3( addCoin->getPositionX()+ addCoin->getBoundingBox().size.width , 0, 0));
                //            coinNum->runAction(Hide::create());
                //            coinNum->runAction(Sequence::create(Show::create(), act2->clone(), FadeOut::create(0.8f),
                //                                                RemoveSelf::create(), NULL));
                //            coinNum->setCameraMask((unsigned short )CameraFlag::USER1);
                CFightData::getInstance()->setAddCoin(CFightData::getInstance()->getAddCoin() + coinValue);
                CCLOG("addcoin::%d",CFightData::getInstance()->getAddCoin());
                
                CFightManager::addCamera(baseNode);

            }
            
        }
        
        
    }
    
    
    //刀兵死亡后扣除加成

    cocos2d::Vector<CBaseFighter*>  pSolodier  = *CFightData::getInstance()->getAllSoldier();
    cocos2d::Vector<CBaseFighter*> pDaoBin;
    
    if (m_iAddHpDaoBin>0)
    {
        
        
        {
            for(int i=0;i<pSolodier.size();i++)
            {
                if(pSolodier.at(i)->m_strTheInfo->monsterId == 1)
                {
                    pDaoBin.pushBack(pSolodier.at(i));
                }
            }
            

            ssize_t iSize = pDaoBin.size();

            for(int i=0;i< pDaoBin.size();i++)
            {
                 if(iSize>1)
                 {
                     pDaoBin.at(i)->m_iNowAttack -= m_iAddHpDaoBin;
                     
                 }
            }
        }
    }
    
    if (m_strTheInfo->theModelInfo->modelId==8) {//特殊死亡动作
    
        
        float duration = 0.5f;
        
        if(m_pAction_Die)
        {
            duration = m_pAction_Die->getDuration();
        }
        
        
        m_pTheSrite->runAction(FadeOut::create(duration));
        if (m_pShadow)
        {
            m_pShadow->runAction(FadeOut::create(duration));
        }
        
        
        runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(CBaseFighter::onDieing, this)),DelayTime::create(duration),CallFunc::create(CC_CALLBACK_0(CBaseFighter::afterDied, this)), NULL));
        

    }else if (m_pDeadlyTarget&&m_pDeadlyTarget->getTheInfo()->monsterType==eFighterType_Trap){ //如果致死者是陷阱，则不显示尸体，直接死亡
        
        float duration = 2.5f;
        
        m_pTheSrite->setVisible(false);
        
        if (m_pShadow) {
            m_pShadow->setVisible(false);
        }
        
        if (m_pEliteCircle) {
            m_pEliteCircle->setVisible(false);
        }
        
        runAction(Sequence::create(DelayTime::create(duration),CallFunc::create(CC_CALLBACK_0(CBaseFighter::afterDied, this)), NULL));
        
    }else{
        m_pTheSrite->setColor(Color3B::GRAY);
        
        float fadeTime = 2;
        
        
        float duration = 0.5f;
        
        if(m_pAction_Die){
            duration = m_pAction_Die->getDuration();
        }
        
    
        if((m_strTheInfo->monsterId == 3&&!m_bKulouDie)||(m_strTheInfo->monsterId == 10&&!m_bWanshi)||m_strTheInfo->monsterType==eFighterType_Tower||m_strTheInfo->monsterType==eFighterType_Trap)
        {
            
            if (m_strTheInfo->monsterType==eFighterType_Tower||m_strTheInfo->monsterType==eFighterType_Trap)
            {
                m_pTheSrite->setVisible(false);
            }

        
            
            Sprite *pEnergySui  = NULL;
            
            CFightScene* pScene =  CFightData::getInstance()->getTheFightScene();
            
            Layer* pLayer = pScene->getTheLayer();
            
            Sprite *pEnergyQiu  =(Sprite*)pLayer->getChildByTag(FIGHT_ENERGY_QIUTAG);
            
            if(CPersonalData::getInstance()->getEnemyType() != SPECIFIED_ENEMY&&CFightData::getInstance()->getFightType()==FIGHT_PVP_TYPE)
            {
                pEnergySui  = Sprite::createWithSpriteFrameName("energysui.png");
                pEnergySui ->setScale(0);
              
                
                Vec3 vNow = getPosition3D();
                
                Vec2 tpp =  CommonData::GetSingletonPtr()->getPosOnScreen(pScene->getTheCamera(),vNow);
                
                Layer *pLayer3D = pScene->gettheLayer3D();
                pEnergySui->setPosition(tpp);
                
                pLayer3D->addChild(pEnergySui,9999);
                

            }
        
            if(pEnergyQiu)
            {
                
              
            
                duration = 1.0f;
                
                //死亡后的消失动画
                
                //陷阱不加能量
                if(CFightData::getInstance()->getFightType()==FIGHT_PVP_TYPE &&(m_strTheInfo->monsterType==eFighterType_Tower))
                {
                    if(m_strTheInfo->monsterId != 20000&&CPersonalData::getInstance()->getEnemyType()!=FIGHT_BACK_VIDEO)
                    {
                        
                      Vec2 t = pEnergyQiu->getPosition(); 
                       pEnergySui->runAction(Spawn::create(MoveBy::create(0.2,Vec2(0,50)),ScaleTo::create(0.2,1),CallFunc::create(CC_CALLBACK_0(CBaseFighter::onAddEnergy,this)), NULL));
                        
                        
                        pEnergySui->runAction(Sequence::create(DelayTime::create(0.5),Spawn::create(
                        MoveTo::create(0.2,Vec2(t.x+10,t.y)),ScaleTo::create(0.2,0.5),NULL),CallFunc::create(CC_CALLBACK_0(CBaseFighter::OnUpdateEnergy,this)),DelayTime::create(0.2),
                                                               RemoveSelf::create(),NULL));
                        
            
                        runAction(Sequence::create(DelayTime::create(0.7),CallFunc::create(CC_CALLBACK_0(CBaseFighter::onEnergySetWhite,this)),DelayTime::create(0.1),CallFunc::create(CC_CALLBACK_0(CBaseFighter::onEnergySetNoraml,this)),NULL));
                        

                        
                        
                        duration = 1.8;
                        
                        runAction(Sequence::create(DelayTime::create(duration),CallFunc::create(CC_CALLBACK_0(CBaseFighter::afterDied, this)), NULL));
                          
                          pEnergyQiu->runAction(Sequence::create(DelayTime::create(0.7),ScaleTo::create(0.2,pEnergyQiu->getScale()*1.5),ScaleTo::create(0.2, 1.5),NULL));
                      }
                    else
                    {
                        duration =2.0f;
                        runAction(Sequence::create(DelayTime::create(duration),CallFunc::create(CC_CALLBACK_0(CBaseFighter::afterDied, this)), NULL));
                        
                    }
                }
                
                else
                {
    
                    duration =2.0f;
                    runAction(Sequence::create(DelayTime::create(duration),CallFunc::create(CC_CALLBACK_0(CBaseFighter::afterDied, this)), NULL));
                
                }
            }
            
            
        }
        else
        {
            
            
        //死亡后的消失动画
        runAction(Sequence::create(DelayTime::create(duration),CallFunc::create(CC_CALLBACK_0(CBaseFighter::onDieing, this)),DelayTime::create(fadeTime),CallFunc::create(CC_CALLBACK_0(CBaseFighter::afterDied, this)), NULL));
        }
    }
    if(m_pShadow)
    {
        m_pShadow->runAction(Sequence::create(FadeOut::create(0.25f), CallFunc::create([=]{
            m_pShadow->removeFromParent();
            m_pShadow = NULL;
        }), NULL));
    }
    
    
    
    
    
    
//    clearAllSkillAndBulletAndBuff();
    
    
    
    if (m_iFightGroup==eFightGroup_Enemy && m_iDeadlyGroup==eFightGroup_Freind) {
//        CFightData::getInstance()->addTheFightConditon(eLevelCondition_kill, 1);
    }
    
    
    if (m_bIfLevelBoss) {
        //关卡boss死了就赢了
        CFightData::getInstance()->setWinState(1,this);
    }
    
    
    CFightData* theFightData = CFightData::getInstance();
    
    if (theFightData->getTheLevelCondition()==eLevelCondition_TowerDefense) {
        
        theFightData->changeTowerObb(false, this);
        
        
        //判断胜利，如果是第十波了，而且一个活怪都没有了。则战斗胜利
        long monsterNum = theFightData->getFightMonstersByGroup(eFightGroup_Freind,1).size();
        if (theFightData->getNowRound()==11&&monsterNum<=0&&m_iFightGroup==eFightGroup_Enemy&&CFightData::getInstance()->getWinState()==0) {//round=11 表示已经把最后一波怪全出完了
            CFightData::getInstance()->setWinState(1,this);
        }
    }
    
    CFightScene* targetScene = theFightData->getTheFightScene();
    if (targetScene) {
        targetScene->removeOBBWithFighter(this);
    }
    
}

void CBaseFighter::onDieing()
{    
    if (m_pShadow) {
        m_pShadow->runAction(FadeOut::create(2));
    }
    
//    resetShader(0);  //暂时弃用，切屏幕会引起bug  by ly
    
    m_pTheSrite->runAction(FadeOut::create(2));
    
}

void CBaseFighter::afterDied(){
    
    
    
//    clearAllSkillAndBulletAndBuff();
    
    setSkillAttackTarget(NULL);
    
    if (CFightData::getInstance()->getlastDiedTarget()==this) {
        CFightData::getInstance()->setlastDiedTarget(NULL);
    }
    
    if (CFightData::getInstance()->getEndTarget()==this) {
        CFightData::getInstance()->setEndTarget(NULL);
    }
    
//    if (m_pSkillTarget) {
//        CC_SAFE_RELEASE(m_pSkillTarget);
//        m_pSkillTarget = NULL;
//    }
    
    
    if (m_bIfReborn) {//如果复活，则走复活逻辑
        
       if(m_strTheInfo->monsterId ==3)
       {
           KulouRebornAnimate();
           
       }
      else{
          
        skillNameShow();
        reborn();
        if(m_strTheInfo->monsterId == 10)
        {
            m_bIfReborn = false;
        }
    }
        
    }else{
        //                CFightData::getInstance()->removeAttacker(this);
        
        removeFromParentAndCleanup(true);
        
    }
    
}




void CBaseFighter::clearAllSkillAndBulletAndBuff(bool ifClean){
    
    //移除所有子弹，防止野指针
    for ( int i=0; i<m_vecAllBullets.size();) {
        
        CBaseBullet* temp = m_vecAllBullets.at(i);
        
        temp->stopAllActions();
        
        temp->onDestroy(ifClean);
        
        //        m_vecAllBullets.eraseObject(temp);
        //        temp->removeFromParentAndCleanup(true);
        
    }
    
    m_vecAllBullets.clear();
    
    
    //移除所有buff
    for (int i=0; i<m_vecAllBuffes.size(); ) {
        
        CBaseBuff* temp = m_vecAllBuffes.at(i);
        
        temp->stopAllActions();
        
        temp->onDestroy(ifClean);
        
    }
    
    //移除所有技能
    
    for (int i=0; i<m_vecAllAttackes.size(); ) {
        
        CBaseAttack* temp = m_vecAllAttackes.at(i);
        
        temp->stopAllActions();
        
        m_vecAllAttackes.eraseObject(temp);
        temp->onDestroy(ifClean);
        
    }
    
}



void CBaseFighter::reborn(){
    
    m_bIfDied = false;
    
    
    m_pTheHpBar->setScale(1);
    
    
    if(m_strTheInfo->monsterId == 3) //骷髅反着播放死亡动画
    {
        playAniamteAction(m_pAction_Die,false,true);
    }
    
    if(m_strTheInfo->monsterId != 10) //山岭石化一直为石头状态
    {
        stateChange(eFighteState_Idle);
        //m_stMonsterDebuff.ifBeatBack = false;
        
    }
  
    //    animateChange(eFighteState_Idle);


    
    m_iNowHP = m_iMaxHP;
    
    m_pTheHpBar->toHPScale(100);
    
    m_pTheSrite->stopAllActions();
    
    m_pTheSrite->setOpacity(255);
    

    m_pTheSrite->setColor(Color3B::WHITE);

    //小地图显示
    if (m_pMapSprite) {
        m_pMapSprite->stopAllActions();
        m_pMapSprite->removeFromParent();
        m_pMapSprite = NULL;
    }
    
    if (m_iAttackType==eAttackType_monster) {
        m_pMapSprite =  CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(Vec3::ZERO,m_strTheInfo->monsterType, m_iFightGroup,eAttackType_monster,m_bIfLevelBoss);
    }else if (m_iAttackType==eAttackType_walker)
    {
        m_pMapSprite =  CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(Vec3::ZERO,eFighterType_line,eFightGroup_Freind,eAttackType_walker);
    }
    else{
        m_pMapSprite =  CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(Vec3::ZERO,m_strTheInfo->monsterType, m_iFightGroup);
    }
    
}




void CBaseFighter::onExit(){
    
    
    m_pTheSrite->stopAllActions();   //此处尝试修复动画爆得bug，防止回调中又重新使用动画。
    m_pTheSrite->unscheduleAllCallbacks();
    
    if(m_pAction_IDle){
        m_pAction_IDle->release();
        m_pAction_IDle = NULL;
    }
    if(m_pAction_Walk){
        m_pAction_Walk->release();
        m_pAction_Walk = NULL;
    }
    if(m_pAction_Attack){
        m_pAction_Attack->release();
        m_pAction_Attack = NULL;
    }
    if(m_pAction_specialAttack){
        m_pAction_specialAttack->release();
        m_pAction_specialAttack = NULL;
    }
    if(m_pAction_Hurt){
        m_pAction_Hurt->release();
        m_pAction_Hurt = NULL;
    }
    if(m_pAction_Die){
        m_pAction_Die->release();
        m_pAction_Die = NULL;
    }
    if(m_pAction_Dizzy){
        m_pAction_Dizzy->release();
        m_pAction_Dizzy = NULL;
    }
    
    
//    clearAllSkillAndBulletAndBuff(false); //如果是士兵的onexit逻辑，则此处不删除，免得重复删除
    
    
    
    //    m_pAction_Walk->release();
    //    m_pAction_Attack->release();
    //    m_pAction_specialAttack->release();
    //    m_pAction_Hurt->release();
    //    m_pAction_Die->release();
    //    m_pAction_Dizzy->release();
    
    onRemoved();
    
    
    Node::onExit();
    
    
    
//    if(m_pTheTalk){
//        m_pTheTalk->removeFromParentAndCleanup(true);
//    }
    
}

void CBaseFighter::onRemoved(bool ifRemoveSelf){
    
    //    CFightData::getInstance()->addTheFightConditon(eLevelCondition_kill, 1);
    
    setAttackTarget(NULL);
    setDeadlyTarget(NULL);
    setSkillAttackTarget(NULL);
    
    stopAllActions();
    unscheduleAllCallbacks();
    unscheduleUpdate();
    
    /*if (m_pMapSprite) {
        m_pMapSprite->stopAllActions();
        m_pMapSprite->removeFromParent();
        m_pMapSprite = NULL;
    }*/
    
    
    CFightData::getInstance()->removeAttacker(this);
    
    if (ifRemoveSelf) {
        removeFromParent();
    }
    
}

float CBaseFighter::getFightDistance(CBaseFighter* target){
    
    return getFightPosition().getDistance(target->getFightPosition())-target->getNowRadius()-getNowRadius();//要减去此人和自身的radius
    
}


MonsterDebuff* CBaseFighter::getMonsterDebuff(){
    
    return &m_stMonsterDebuff;
}



void CBaseFighter::skillNameShow(std::string strSkillPic)
{
    
    BillBoard* theHurt = (BillBoard*)Sprite::createWithSpriteFrameName(strSkillPic.c_str());
    
    theHurt->setScale(2.0);
    theHurt->setPosition3D(Vec3(0,m_pTheSrite->getBoundingBox().size.height,0));
    theHurt->setCameraMask((unsigned short )CameraFlag::USER1);
    addChild(theHurt,11);
    
    theHurt->setGlobalZOrder(1000);
    
    theHurt->setBlendFunc(cocos2d::BlendFunc::ALPHA_NON_PREMULTIPLIED);
    theHurt->setOpacity(0);
    ScaleTo *act1 = ScaleTo::create(0.2f, 1.0f);
    FadeIn* act2 = FadeIn::create(0.2f);
    
    
    MoveBy* act3 = MoveBy::create(0.6f, Vec2(0, 10));
    
    
    ScaleTo *act4 = ScaleTo::create(0.2f, 1.0f,2.0f);
    FadeOut* act5 = FadeOut::create(0.2f);
    
    theHurt->runAction(Sequence::create(Spawn::create(act1,act2,NULL),act3,Spawn::create(act4,act5, NULL),RemoveSelf::create(),  NULL));
    
}


void CBaseFighter::skillNameShow()
{
    
    //如果类型是防御塔，或者是一直治疗的技能，则不显示技能名，免得太频繁
    if (m_iAttackType==eAttackType_tower) {
        return;
    }
    
    if (m_iNextSkill&&m_iNextSkill->theSkillInfo->skillId==eAllSkills_huichunshu) {
        return;
    }
    
    
    if (m_iNextSkill) {
    
        skillNameShow(m_iNextSkill->theSkillInfo->skillNamePic.c_str());
        
    }
    
}





void CBaseFighter::beatBackFrom(cocos2d::Vec3 originPos, float dist, float time,float highDist){
    
    
    
    //被击退的前提是，自己有速度
    if (m_strTheInfo->walkSpeed==0||CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense){//塔防模式下除外) {
        return;
    }
    
    
    //    stopAllActions();
   // m_stMonsterDebuff.ifBeatBack = true;
    
    //    被击退时，先眩晕，再位移
    //    CFightManager::addTheBuffToFighter(this, eAllBuffes_xuanyun,time);
    
    
    
    //计算击退位置
    Vec3 startPoint = originPos;
    
    Vec3 endPoint = getPosition3D();
    
    
    //判断，如果起始点和自己位置一样，则不位移。
    if (startPoint==endPoint) {
        return;
    }
    
    
    
    Vec3 moveVec = endPoint-startPoint;
    
    m_v3BeatMaxtime = time;
    
    m_v3BeatNowtime = 0;
    
    float movedist = moveVec.length();
    
    //各个方向上每秒位移
    float speedX =  moveVec.x/movedist  *  dist/time ;
    float speedZ = moveVec.z/movedist  *  dist/time ;
    
    m_fBeatFlyAccelerated = -highDist /(0.5 * (time/2) * (time/2)); //击飞以后还需要掉下来
    
    float speedY = -m_fBeatFlyAccelerated*(time/2) ;//高度上的速度
    
    
    m_v3BeatSpeed = Vec3(speedX, speedY, speedZ);
    
    
    
    
}


void CBaseFighter::beatBackUpdate(float time){
    
    
    
    //变速
    float reduceSpeed = 0.93f;
    
    m_v3BeatSpeed.x = m_v3BeatSpeed.x*reduceSpeed;
    m_v3BeatSpeed.y += m_fBeatFlyAccelerated*time;
    m_v3BeatSpeed.z = m_v3BeatSpeed.z*reduceSpeed;
    
    
    
    m_v3BeatNowtime+=time;
    
    
    
//    float posX = getPositionX()+m_v3BeatSpeed.x*time;
//    float posY = getPositionY()+m_v3BeatSpeed.y*time;
//    float posZ = getPositionZ()+m_v3BeatSpeed.z*time;
    
    
    Vec3 targetPos = getPosition3D()+ m_v3BeatSpeed*time;
    
    
    targetPos = CFightManager::getThePosInMap(targetPos);//边界判断
    
    CCLOG("position(%f,%f)",targetPos.x,targetPos.z);
    
    
    setPosition3D(targetPos);
    

    if (m_v3BeatNowtime>=m_v3BeatMaxtime&&targetPos.y<=0) {//当时间够了。并且当前高度为0时，才能结束
        
        m_stMonsterDebuff.ifBeatBack = false;
    }
    
    
    
}

Vec3 CBaseFighter::getPhysicalPosition(){
    
    
    if (m_strTheInfo->theMonsterInfo->ifFly) {
        
        return getPosition3D()+m_pTheSrite->getPosition3D();
    }
    
    return getPosition3D();
    
}


bool CBaseFighter::ifCanTalk(){
    
    if(CFightTalk::talkNumber>1){
        return false;
    }
    
    if(m_strTheInfo->monsterType==eFighterType_Tower){//如果是建筑就不说话
        return false;
    }
    
    
    
    //最多同时有一个talk
    if (!m_pTheTalk&&CFightData::getInstance()->getNowTalkTime()>m_fMaxTalkCold*2&&m_fNowTalkCold>m_fMaxTalkCold) {
        
        m_fNowTalkCold=0;
        
        //就算满足cd，也要有个判断条件，满足则出现
        int possible = rand()%3;
        
        if (possible==1) {
            
            CFightData::getInstance()->setNowTalkTime(0);
            return true;
        }
        
    }
    
    
    return false;
}


void CBaseFighter::setElite(bool ifElite){
    
    //如果士兵是精英,体型增大1.3倍，带上火光，变色，并且攻击和生命值*1.5倍
    
    m_bIfElite = ifElite;
    
    if(ifElite){
        
        setMaxHP(getMaxHP()*1.5f);
        setNowHP(getMaxHP());
        
        setNowAttack(getNowAttack()*1.5f);
        
        changeSpriteScale(1.3f);
//        setNowRadius(getNowRadius()*1.3f);
//        
//        m_pTheSrite->setScale(gettheSprite()->getScale()*1.3);
        
//        gettheSprite()->setColor(Color3B(255, 240, 0));
        
        
        //添加星星粒子特效
//        eleteStar = ParticleSystemQuad::create("particleSystem/starLight.plist") ;
//        //    explodePar->setScale(0.5f);
//        eleteStar->setPosition3D(Vec3(0, gettheSprite()->getBoundingBox().size.height*0.9f, 0)) ;
//        eleteStar->setCameraMask((unsigned short )CameraFlag::USER1);
//        
//        addChild(eleteStar);
        
        
        //脚底光圈
        
        m_pEliteCircle = Sprite3D::create();
        
        Sprite* tempSprite = Sprite::createWithSpriteFrameName("fight_bornCircle2.png");
        
        tempSprite->setScale(m_pTheSrite->getBoundingBox().size.width/tempSprite->getBoundingBox().size.width*1.5f);
        m_pEliteCircle->addChild(tempSprite);
        
        tempSprite->runAction(RepeatForever::create(RotateBy::create(0.5f, 50)));
        
        m_pEliteCircle->setRotation3D(Vec3(90, 0, 0));
        CFightManager::addCamera(m_pEliteCircle);
        addChild(m_pEliteCircle);
        
        
        //shader效果,无法和受击效果重复，因此需要删除,  骑兵不能加上效果
        if (m_strTheInfo->theMonsterInfo->monsterId!=9) {
            
//            changeShader(eShadeType_3D_Elite);  //暂时弃用，切屏幕会引起bug  by ly
            
//            eliteShader = (CSprite3dAnimateShaderWithFile*)CFightManager::addShaderToSprite3D(m_pTheSrite,eShadeType_3D_Elite,"shaderFade");
//            if (_hurtShader) {
//                _hurtShader->removeFromParent();
//                _hurtShader = NULL;
//            }
//            if (_hurtShaderBuilding) {
//                _hurtShaderBuilding->removeFromParent();
//                _hurtShaderBuilding=NULL;
//            }
        }
        
        
    }
    
}


void CBaseFighter::judgeActive(){
    
    
    
    
    //判断是否激活
    if (m_pTheActiveCircle) {
        
        long number = (long)m_pTheActiveCircle->getUserData();
        
        if (number==1) {  //当它被置为1时，就开始激活士兵
            
            removeChild(m_pTheActiveCircle);
            
            m_pTheActiveCircle = NULL;
            
            m_bIfActive=true;
            
        }
        
    }else{
        m_bIfActive = true;
    }
    
    
    //判断激活后的动作
    if (m_bIfActive) {
        
        if (getTheInfo()->monsterType==eFighterType_Tower) {
            
            //建筑升起时候的回弹
            float offsetBack = 20;
            
            gettheSprite()->runAction(Sequence::create(EaseSineOut::create(MoveBy::create(0.3f, Vec2(0, gettheSprite()->getBoundingBox().size.height*1.2f+offsetBack))),MoveBy::create(0.1f, Vec2(0, -offsetBack)),CallFunc::create([=](){
                m_pShadow->setVisible(true);
                
                
            }), NULL));
            
        }else{
            
            if (CFightData::getInstance()->getWinState()==0) {
                
                setVisible(true);
                scheduleUpdate();
            }
            
            CCLOG("pvpNowFightTime--bornTime::%f",CFightData::getInstance()->getAllFightTime());
        }
        RefreshFighter();
        
    }
    
    
    
}

void CBaseFighter::changeSpriteScale(float targetScale){
    
    m_pTheSrite->setScale(m_pTheSrite->getScale()*targetScale);
    
    setNowRadius(getNowRadius()*1.3f);
    
    resetHPPosition();

}

void CBaseFighter::resetHPPosition(){
    
    
    m_pTheHpBar->setPosition(Vec2( 0, m_pTheSrite->getBoundingBox().size.height+m_pTheHpBar->getBGHP()->getBoundingBox().size.height*0.5f)+m_pTheSrite->getPosition());
    
}

void CBaseFighter::initShader(){
    

    //初始化战斗相关的shader信息
    
    
    //因为shader对不再一个贴图的模型进行变化时会出错，因此骑兵（使用3张贴图）就不进行初始化了
    if (m_strTheInfo->theMonsterInfo->monsterId==9||m_strTheInfo->theMonsterInfo->monsterId==16) {
        return;
    }
    
    
    //shader对投石机初始化会出错，因此不对其进行初始化了 ,暂时先这么处理，回头再查原因
    if (m_strTheInfo->theModelInfo->modelId==2022) {
        return;
    }
    
    //shader对陷阱初始化会出错，因此不对其进行初始化了 ,暂时先这么处理，回头再查原因
    if (m_strTheInfo->monsterType==eFighterType_Trap) {
        return;
    }
    
    
    //如果是守军类型，则不进行初始化
    if (m_iAttackType==eAttackType_Partol){
     
        return;
    }
    
    
    
//    temp->initWithFormat("");
    
    if (_hurtShader) {
        _hurtShader->removeFromParent();
        _hurtShader = NULL;
    }
    
    if (_hurtShaderBuilding) {
        _hurtShaderBuilding->removeFromParent();
        _hurtShaderBuilding=NULL;
    }
    
    
    
    if (!m_bIfElite&&m_strTheInfo->monsterType!=eFighterType_Tower&&!m_bIfLevelBoss) {
        _hurtShader = (CSprite3dAnimateShaderOnce*)CFightManager::addShaderToSprite3D(m_pTheSrite, eShadeType_3D_beAttacked,"shaderOnce");
    }else if (m_strTheInfo->monsterType==eFighterType_Tower) {
        _hurtShaderBuilding = (CSprite3dShaderWhite*)CFightManager::addShaderToSprite3D(m_pTheSrite, eShadeType_TurnWhite);
    }else if (m_bIfLevelBoss){
        _hurtShader = (CSprite3dAnimateShaderOnce*)CFightManager::addShaderToSprite3D(m_pTheSrite, eShadeType_3D_BossBeAttacked,"shaserOnceBoss");
    }
    
    //测试用
//    resetShader(0);
}

void CBaseFighter::changeShader(ShadeType theType){
    
    if (_hurtShader) {
        _hurtShader->removeFromParent();
        _hurtShader = NULL;
    }
    
    if (_hurtShaderBuilding) {
        _hurtShaderBuilding->removeFromParent();
        _hurtShaderBuilding=NULL;
    }
    
    if (bossShader) {
        bossShader->removeFromParent();
        bossShader=NULL;
    }
    
    if (eliteShader) {
        eliteShader->removeFromParent();
        eliteShader=NULL;
    }
    
    
    m_pTheSrite->genGLProgramState();
    
    
    switch (theType) {
        case eShadeType_3D_beAttacked:
            _hurtShader = (CSprite3dAnimateShaderOnce*)CFightManager::addShaderToSprite3D(m_pTheSrite, eShadeType_3D_beAttacked,"shaderOnce");
            break;
            
        case eShadeType_TurnWhite:
            _hurtShaderBuilding = (CSprite3dShaderWhite*)CFightManager::addShaderToSprite3D(m_pTheSrite, eShadeType_TurnWhite);
            break;
            
        case eShadeType_3D_BossBeAttacked:
            _hurtShader = (CSprite3dAnimateShaderOnce*)CFightManager::addShaderToSprite3D(m_pTheSrite, eShadeType_3D_BossBeAttacked,"shaserOnceBoss");
            break;
            
        case eShadeType_3D_Boss:
            bossShader = (CSprite3dAnimateShaderWithFile*)CFightManager::addShaderToSprite3D(m_pTheSrite, eShadeType_3D_Boss,"shaderFade");
            break;
            
        case eShadeType_3D_Elite:
            eliteShader = (CSprite3dAnimateShaderWithFile*)CFightManager::addShaderToSprite3D(m_pTheSrite,eShadeType_3D_Elite,"shaderFade");
            break;
            
        default:
            break;
    }
    
    
    
    
}


void CBaseFighter::reloadShader(){
    
    if (_hurtShader) {
        _hurtShader->removeFromParent();
        _hurtShader = NULL;
    }
    if (_hurtShaderBuilding) {
        _hurtShaderBuilding->removeFromParent();
        _hurtShaderBuilding=NULL;
    }
    
    if (bossShader) {
        bossShader->removeFromParent();
        bossShader=NULL;
    }
    
    if (eliteShader) {
        eliteShader->removeFromParent();
    }
    
    
    //如果是小boss，则会变红，属性提高2倍
    if(m_bIfSmallBoss) {

        bossShader= (CSprite3dAnimateShaderWithFile*)CFightManager::addShaderToSprite3D(m_pTheSrite, eShadeType_3D_Boss,"shaderFade");
    }else if (!m_bIfElite&&m_strTheInfo->monsterType!=eFighterType_Tower&&!m_bIfLevelBoss) {
        _hurtShader = (CSprite3dAnimateShaderOnce*)CFightManager::addShaderToSprite3D(m_pTheSrite, eShadeType_3D_beAttacked,"shaderOnce");
    }else if (m_strTheInfo->monsterType==eFighterType_Tower) {
        _hurtShaderBuilding = (CSprite3dShaderWhite*)CFightManager::addShaderToSprite3D(m_pTheSrite, eShadeType_TurnWhite);
    }else if (m_bIfLevelBoss){
        _hurtShader = (CSprite3dAnimateShaderOnce*)CFightManager::addShaderToSprite3D(m_pTheSrite, eShadeType_3D_BossBeAttacked,"shaserOnceBoss");
    }else if (m_bIfElite){
        eliteShader = (CSprite3dAnimateShaderWithFile*)CFightManager::addShaderToSprite3D(m_pTheSrite,eShadeType_3D_Elite,"shaderFade");
    }
    
    
    
    
}




void CBaseFighter::resetShader(float time){
    
    
    
    
    
    if (_hurtShader) {
        _hurtShader->removeFromParent();
        _hurtShader = NULL;
    }
    if (_hurtShaderBuilding) {
        _hurtShaderBuilding->removeFromParent();
        _hurtShaderBuilding=NULL;
    }
    
    if(bossShader){
        bossShader->removeFromParent();
        bossShader = NULL;
    }
    
    if (eliteShader) {
        eliteShader->removeFromParent();
        eliteShader = NULL;
    }
    
//    m_pTheSrite->setGLProgramState(GLProgramState::create(GLProgramCache::getInstance()->getGLProgram("Shader3DPosition")));
    
    m_pTheSrite->genGLProgramState();
    
}

void CBaseFighter::setAttackTarget(CBaseFighter *target,bool ifRec){
    
    
    if (target==m_pAttackTarget&&m_iAttackType!=eAttackType_tower) { //如果攻击目标和目标一致，则不做任何操作（建筑除外，建筑的每次调用都是攻击一次，所以不能return）
        return;
    }
    
    CFightScene* theScene = CFightData::getInstance()->getTheFightScene();
    
    
    
    
    
    //pvp相关处理
    if (target!=NULL) {
        
        if (theScene&&theScene->getthePVPMode()==EPVPMode_PLAY){
            ((CFightScenePVP*)theScene)->addRecordStringWithchangeTarget(m_iTrueId, target->getTrueId());
        }
        
        if (theScene&&theScene->getthePVPMode()==EPVPMode_REC&&ifRec==false) {//录像模式下，不会根据新的目标进行操作
            return;
        }
    }
    
    
    
    if (m_pAttackTarget != target)
    {
        CC_SAFE_RETAIN(target);
        CC_SAFE_RELEASE(m_pAttackTarget);
        m_pAttackTarget = target;
    }

    
}
