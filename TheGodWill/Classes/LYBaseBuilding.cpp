//
//  LYBaseBuilding.cpp
//  TheGodWill
//
//  Created by user on 15/2/4.
//
//

#include "LYBaseBuilding.h"

#include "SmallMap.h"

#include "LYFightData.h"

#include "BillBoardLable.h"

#include "DataManager.h"
//#include "Effect3DOutline.h"

USING_NS_CC;




CBaseBuilding::CBaseBuilding(){
    
    m_pTowerUp= NULL;
    
    m_pAtkRangeCircle = NULL;
    
    m_pMenuLayer = NULL;
    
    
    m_pRuinsSprite = NULL;
    
    m_pRuinsParticle=NULL;
    m_fScale = 0;
    m_vecBaseRotation = Vec3(0, 90, 0);
}

CBaseBuilding::~CBaseBuilding(){
    
    
}


CBaseBuilding *CBaseBuilding::create(FighterInfo *theInfo,FightGroup theGroup){
    
    CBaseBuilding * temp = new CBaseBuilding();
    
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

bool CBaseBuilding::initWithInfo(FighterInfo *theInfo,FightGroup theGroup){
    
    
    
    //此处测试用
    //    SkillInfo temp ;
    //    temp.skillId=-1;
    //    temp.damageRatio=1;
    //    temp.triggerRatio=1;
    //
    //    m_VecSkillInfo.push_back(temp);
    
    
    
    if (!CBaseFighter::initWithInfo(theInfo,theGroup)) {
        return false;
    };
    
    
    //塔防模型新增小物件
    fightBuildingInfo* buildInfo = CDataManager::getTheFightBuildingInfoWithId(m_strTheInfo->monsterId);
    
    
    //测试用
//    if (buildInfo&&buildInfo->towerType!=0){
//        
//        m_pTheSrite->setScale(1);
//    }
    
    
    if (buildInfo&&buildInfo->towerType==1) {
        
        m_pTowerUp = Sprite3D::create("models/building_ta/tower_arrow/jianta-arrow.c3b");
        
        m_pTheSrite->addChild(m_pTowerUp);
        
        CFightManager::addCamera(m_pTowerUp);
        
        
        float height = m_pTheSrite->getBoundingBox().size.height;
        
        m_pTowerUp->setPosition3D(Vec3(0, height, 0));
        
//        m_pTheSrite->setScale(0.7);
        
        
    }else if (buildInfo&&buildInfo->towerType==2) {
        
        m_pTowerUp = Sprite3D::create("models/building_ta/tower_magic/book.c3b");
        
        m_pTheSrite->addChild(m_pTowerUp);
        
        CFightManager::addCamera(m_pTowerUp);
        
        
        float height = 48;
        
        m_pTheSrite->setPosition3D(Vec3(0, -height, 0));
        
        
        if (buildInfo->monsterId==10006) {//模型微调高度
            float height = 5;
            m_pTheSrite->setPosition3D(m_pTheSrite->getPosition3D()+ Vec3(0, height, 0));
        }else if (buildInfo->monsterId==10007) {//模型微调高度
            float height = 5;
            m_pTheSrite->setPosition3D(m_pTheSrite->getPosition3D()+ Vec3(0, height, 0));
        }else if (buildInfo->monsterId==10008) {//模型微调高度
            float height = 5;
            m_pTheSrite->setPosition3D(m_pTheSrite->getPosition3D()+ Vec3(0, height, 0));
            m_pTheSrite->setRotation3D(Vec3(0, 20, 0));
        }else if (buildInfo->monsterId==10009) {//模型微调高度
            float height = 5;
            m_pTheSrite->setPosition3D(m_pTheSrite->getPosition3D()+ Vec3(0, height, 0));
            m_pTheSrite->setScale(1.2f);
        }else if (buildInfo->monsterId==10010) {//模型微调高度
            float height = 5;
            m_pTheSrite->setPosition3D(m_pTheSrite->getPosition3D()+ Vec3(0, height, 0));
            float bookHeight = m_pTheSrite->getBoundingBox().size.height*0.2f;
            m_pTowerUp->setPosition3D(Vec3(0, bookHeight, 0));
        }
        
        
        
//        if (buildInfo->monsterId==10010) {
//            float bookHeight = m_pTheSrite->getBoundingBox().size.height*0.2f;
//            m_pTowerUp->setPosition3D(Vec3(0, bookHeight, 0));
//        }
        
        
        
    }else if (buildInfo&&buildInfo->towerType==3) {
        
        m_pTowerUp = Sprite3D::create("models/building_ta/tower_cannon/pao.c3b");
        
        m_pTheSrite->addChild(m_pTowerUp);
        
        CFightManager::addCamera(m_pTowerUp);
        
        float height = m_pTheSrite->getBoundingBox().size.height;
        
        m_pTowerUp->setPosition3D(Vec3(0, height, 0));
        
        
    }
    
    
    
//    m_pMenuLayer = Sprite::create("toushichenormal.png");
//    m_pMenuLayer->setScale(0.5f);
//    addChild(m_pMenuLayer);
    
//    CFightManager::addCamera(m_pMenuLayer);
    
//    m_pMenuLayer->setGlobalZOrder(10000);
    
//    m_pMenuLayer->setVisible(false);
    
    
    
    
    
    refreshTheOwner(m_iFightGroup);
    
    
    //    m_fSpriteScale = 0.2f;
    //    m_pTheSrite->setScale(m_fSpriteScale);
    
    
    
    //    if (m_iAttackType==eAttackType_wall) {//测试用，小一点
    //        setScale(0.5f);
    //    }
    
//    m_pTheHpBar->setPosition(0, m_pTheSrite->getBoundingBox().size.height);
    m_pTheHpBar->setPosition(Vec2( 0, m_pTheSrite->getBoundingBox().size.height+m_pTheHpBar->getBGHP()->getBoundingBox().size.height*0.5f)+m_pTheSrite->getPosition());
    
    
    //    m_pTheHpBar->setScale(1);
    
    //    m_pTheHpBar->setPosition3D(Vec3(0,60,0));
    
    m_iDeadlyGroup = theGroup;//防止错误，默认为致死阵营是自己阵营
    
    
    //    setRotation3D(Vec3(0, 0, -90));
    
    //    m_v2AttackPosOffset = Vec3(10, 100, 10);
    
    
    //测试用
//    __String *tempStr = __String::createWithFormat("%d",m_strTheInfo->monsterId);
//    
//    Label* tempName  = Label::createWithCharMap("fight/coin_figure.png", 30, 36, '0');
//    tempName->setString(tempStr->getCString());
//    tempName->setPosition3D(Vec3(0, 100, 0));
//    CFightManager::addCamera(tempName);
//    addChild(tempName);
//    
//    tempName->setGlobalZOrder(1000);
    
    
    //攻击区域框
    m_pAtkRangeCircle = Sprite3D::create();
    
    Sprite* temp = Sprite::createWithSpriteFrameName("fight_attackRange.png");
    m_pAtkRangeCircle->addChild(temp);
    temp->getTexture()->setAntiAliasTexParameters();
    
//    m_pAtkRangeCircle = Sprite3D::create("models/Others/Circle.c3b");
    CFightManager::addCamera(m_pAtkRangeCircle);
    addChild(m_pAtkRangeCircle);
    
    
    float width = temp->getBoundingBox().size.width*0.5f;
    m_fScale =  m_strTheInfo->theMonsterInfo->attackRange*1.0f/width;
    m_pAtkRangeCircle->setScale(m_fScale);
    
    m_pAtkRangeCircle->setOpacity(150);
    
    m_pAtkRangeCircle->setRotation3D(Vec3(-90,0,0));
    
    m_pAtkRangeCircle->setPosition3D(Vec3(0, 4, 0));
    
    m_pAtkRangeCircle->setVisible(false);
    
    
//    if(m_pAtkRangeCircle&&m_pAtkRangeCircle->getMeshCount()>0){
//        Texture2D* tem= m_pAtkRangeCircle->getMesh()->getTexture();
//        tem->generateMipmap();
//        cocos2d::Texture2D::TexParams tempOne ={GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE};
//        tem->setTexParameters(tempOne);
//    }
    
    
    return true;
    
}

void CBaseBuilding::collidUpdate(float time){
    
    //重写此方法，什么都不做。建筑不会位移
    
    
    
}


void CBaseBuilding::stateUpdate(float time){
    
    
    //测试用
    //    ifSkillStart();
    //    skillPrepare();
    //    return;
    
    
    
    
    //重写此方法，因为资源点只判断最后一个经过他的人的属性。
    
    
    
    //如果是塔防模式，则查找自己上面、左上、右上方向三个路点是否有敌人，有的话则进虚化
    if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense) {
        
        bool ifVirtual = ifCoverSomeOne();
        
        virtualAction(ifVirtual);
        
    }
    
    
    //pvp模式中，建筑上面的炮台根据目标旋转
    if (CFightData::getInstance()->getFightType()==FIGHT_PVP_TYPE&&m_pTowerUp&&getAttackTarget()) {
        Vec2 targetPos = Point( getAttackTarget()->getPositionX(), getAttackTarget()->getPositionZ());
        Vec2 myPos = Point(getPositionX(),getPositionZ());
        
        float radians = CFightManager::getTheDegreeWith(myPos, targetPos,false);
        
        Vec3 targetVec = m_pTowerUp->getRotation3D();
        
        targetVec.y = radians;
        
        if(targetVec!=m_pTowerUp->getRotation3D())
        {
            m_pTowerUp->setRotation3D(targetVec);
        }
    }
    
    
    
    
    
    
}

void CBaseBuilding::reborn(){
    
    //更换模型
    
    //    m_pTheSrite->removeFromParentAndCleanup(true);
    //
    //
    //    if (m_pDeadlyTarget) {
    //        m_iFightGroup = m_pDeadlyTarget->getFightGroup();
    //
    //    }else{
    //
    //        m_iFightGroup = m_iDeadlyGroup;
    //    }
    //
    //
    //    __String modelPath = m_strTheInfo->theModelInfo->modelName;
    //
    //    std::string temp = modelPath.getCString();
    //
    //    long offset = temp.find("white");
    //
    //    if (m_iFightGroup == eFightGroup_Freind) {
    //
    //        temp.replace(offset,5,"blue");
    ////        modelPath.append("_blue");
    //    }else if (m_iFightGroup == eFightGroup_Enemy){
    //        temp.replace(offset,5,"red");
    ////        modelPath.append("_red");
    //    }
    //    
    //    initModelWithPath(temp);
    
    
    CBaseFighter::reborn();
    
    
    refreshTheOwner();
    

    //    m_pTheHpBar->setScale(10);
}

void CBaseBuilding::refreshTheOwner(FightGroup target){
    
    
    //测试用
    //    if (target==efightGroup_err) {//没有指定拥有者时
    //        if (m_pDeadlyTarget&&m_pDeadlyTarget->getReferenceCount()>0) {
    //            m_iFightGroup =  m_pDeadlyTarget->getFightGroup();
    //
    //        }else{
    //
    //            m_iFightGroup = m_iDeadlyGroup;
    //        }
    //    }else{
    //
    //        m_iFightGroup = target;
    //    }
    //
    //    if (m_iFightGroup == eFightGroup_Freind) {
    //
    //        m_pTheSrite->setColor(Color3B::BLUE);
    //    }else if (m_iFightGroup == eFightGroup_Enemy){
    //        m_pTheSrite->setColor(Color3B::RED);
    //    }
    
    
    if (target==efightGroup_err) {//没有指定拥有者时
        CBaseFighter* deadTarget = getDeadlyTarget();
        if (deadTarget) {
            m_iFightGroup = deadTarget->getFightGroup();
            
        }else{
            
            m_iFightGroup = m_iDeadlyGroup;
        }
    }else{
        
        m_iFightGroup = target;
    }

    
    
    
    //小地图显示
    if (m_pMapSprite) {
        m_pMapSprite->stopAllActions();
        m_pMapSprite->removeFromParent();
        m_pMapSprite = NULL;
    }
    
    if(!CPersonalData::getInstance()->getPvpMainLayer())
    {
        m_pMapSprite =  CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(getPosition3D(),m_strTheInfo->monsterType, m_iFightGroup);
        CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(getPosition3D(),m_pMapSprite);
    }
    
    
//    if (m_pMapSprite) {
//        CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(getPosition3D(),m_pMapSprite);
//        
//        if(m_iFightGroup == eFightGroup_Freind)
//        {
//            m_pMapSprite->setSpriteFrame("lan.png");
//            CCLOG("变蓝塔图标");
//        }
//        else if(m_iFightGroup == eFightGroup_Enemy)
//        {
//            m_pMapSprite->setSpriteFrame("hong.png");
//            CCLOG("变红塔图标");
//        }
//        else{
//            m_pMapSprite->setSpriteFrame("huang.png");
//            CCLOG("变黄塔图标");
//        }
//
//    }
    
    
    //    cocos2d::Vector<CBaseFighter*> towerAll = CFightData::getInstance()->getAllTowers();
    //    for(int i = 0; i < towerAll.size(); i++)
    //    {
    //        m_pMapSprite;
    //
    //
    //        if(towerAll.at(i)->m_iFightGroup == eFightGroup_Freind)
    //        {
    //            towerAll.at(i)->m_pMapSprite->setTexture("shape/lan.png");
    //        }
    //        else if(towerAll.at(i)->m_iFightGroup == eFightGroup_Enemy)
    //        {
    //            towerAll.at(i)->m_pMapSprite->setTexture("shape/hong.png");
    //        }
    //        else{
    //            towerAll.at(i)->m_pMapSprite->setTexture("shape/huang.png");
    //        }
    //    }
    
    
    
    
    
    
    
    
    __String modelPath = m_strTheInfo->theModelInfo->modelName;
    
    std::string temp = getTheModelPathByGroup();
    
//    long offset = temp.find("white");
    
    if(temp!=""){ //如果找得到white则代表模型会变色
//        if (m_iFightGroup == eFightGroup_Freind) {
//            
//            temp.replace(offset,5,"blue");
//            //        modelPath.append("_blue");
//        }else if (m_iFightGroup == eFightGroup_Enemy){
//            temp.replace(offset,5,"red");
//            //        modelPath.append("_red");
//        }
        
        
//        resetShader(0);
        m_pTheSrite->setTexture(temp);
        m_pTheSrite->setVisible(true);
        
//        m_pTheSrite->removeFromParentAndCleanup(true);
//        
//        m_pTheSrite = NULL;
//        
//        _hurtShader=NULL;
//        _hurtShaderBuilding=NULL;
//        
//        initModelWithPath(temp);
//        
//        m_pTheSrite->setScale(m_strTheInfo->theModelInfo->modelScale);
    }
    
    
    if (m_pRuinsSprite) {
        m_pRuinsSprite->removeFromParent();
        m_pRuinsSprite=NULL;
    }
    
    if (m_pRuinsParticle) {
        m_pRuinsParticle->removeFromParent();
        m_pRuinsParticle=NULL;
    }
//
//    
//    initShader();
    
    //    m_pTheSrite->setScale(m_fSpriteScale);
    
    
    
}

std::string CBaseBuilding::getTheModelPathByGroup(){
    
    
    __String modelPath = m_strTheInfo->theModelInfo->modelName;
    
    std::string temp = modelPath.getCString();
    
    long offset = temp.find("_white.c3b");   //只要模型的后缀为_white.c3b 则默认为一定有蓝色和红色贴图，类似为XXX_blue.png
    
    if(offset!=-1){ //如果找得到white则代表模型会变色
        if (m_iFightGroup == eFightGroup_Freind) {
            
            temp.replace(offset,10,"_blue.png");
            //        modelPath.append("_blue");
        }else if (m_iFightGroup == eFightGroup_Enemy){
            temp.replace(offset,10,"_red.png");
            //        modelPath.append("_red");
        }else{
            return "";
        }
        
    }else{
        return "";
    }
    
    return temp;
}



void CBaseBuilding::buildingSelect(){
    
//    m_pMenuLayer->setVisible(true);
    
    m_pTheSrite->setColor(Color3B(255, 41, 28));
    
    if (m_pAtkRangeCircle) {
        m_pAtkRangeCircle->setVisible(true);
    }
    
    
    
}

void CBaseBuilding::buildingUnselect(){
    
    m_pTheSrite->setColor(Color3B::WHITE);
//    m_pMenuLayer->setVisible(false);
    
    if (m_pAtkRangeCircle) {
        m_pAtkRangeCircle->setVisible(false);
    }
    
}

bool CBaseBuilding::ifCoverSomeOne(){
    
    
    pathPos* temp = CFightData::getInstance()->findPathPosByPos(getFightPosition());
    
    if (temp) {
        return temp->ifNeedVirtual;
    }
    
    return false;
}

void CBaseBuilding::virtualAction(bool ifVirtual){
    
    
    if (ifVirtual) {
        
        m_pTheSrite->setOpacity(120);
        
    }else{
        m_pTheSrite->setOpacity(255);
    }
    
    
}

void CBaseBuilding::onDied(DamageType type)
{
   CBaseFighter::onDied(type);
    
    if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
    {
        pvpCommonData::getInstance()->setDestroyedBuildNum(pvpCommonData::getInstance()->getDestroyedBuildNum() + 1);
    }
//    SimpleAudioEngine::getInstance()->playEffect("music/tower.mp3");
    
    Sprite* temp=Sprite::createWithSpriteFrameName("tower_die_1.png");
    CFightManager::addCamera(temp);
    temp->setGlobalZOrder(1000);
    addChild(temp);
    temp->setScale(1.5f);
    char buf[256];
    Animation* pAnimation = Animation::create();
    for (int i = 1; i < 11; i++)
    {
        sprintf(buf, "tower_die_%d.png", i);
        pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
    }
    pAnimation->setDelayPerUnit(0.1f);
    temp->runAction(Sequence::create(Animate::create(pAnimation),RemoveSelf::create(), NULL) );
    
    
    
    if (type!=eDamageType_Upgrade) { //塔防升级时不需要废墟和冒烟
        
        
        //破坏后的废墟
        m_pRuinsSprite = Sprite3D::create();
        
        //随机给个废墟图
        int randFlag = rand()%3+1;
        __String *tempStr = __String::createWithFormat("ruins%d.png",randFlag);
        
        Sprite* ruinSprite = Sprite::createWithSpriteFrameName(tempStr->getCString());
        m_pRuinsSprite->addChild(ruinSprite);
        //    m_pRuinsSprite = CFightManager::getTheModelWithPath("ruins.png");
        
        m_pRuinsSprite->setPosition3D(getPosition3D());
        m_pRuinsSprite->setRotation3D(Vec3(90, 0, 0));
        CFightManager::addCamera(m_pRuinsSprite);
        
        //    //随机的转动一下，防止重复
        //    int randRotate = rand()%360;
        //    m_pRuinsSprite->setRotation3D(Vec3(90, randRotate, 0));
        m_pRuinsSprite->setRotation3D(Vec3(90, 0, 0));
        
        m_pRuinsSprite->setScale(m_pTheSrite->getBoundingBox().size.width/ruinSprite->getBoundingBox().size.width*1.2f);
        
        if (getParent()) {
            getParent()->addChild(m_pRuinsSprite,100);
        }
        
        //废墟的粒子特效
        
        //随机给个废墟特效
        int randParticle = rand()%1+1;
        __String *tempStr2 = __String::createWithFormat("particleSystem/ruinsSmoke%d.plist",randParticle);
        
        m_pRuinsParticle = ParticleSystemQuad::create(tempStr2->getCString());
        CFightManager::addCamera(m_pRuinsParticle);
        m_pRuinsParticle->setPosition3D(getPosition3D());
        m_pRuinsParticle->setScale(m_pTheSrite->getBoundingBox().size.width/ruinSprite->getBoundingBox().size.width*1);
        if (getParent()) {
            getParent()->addChild(m_pRuinsParticle,100);
        }
        m_pRuinsParticle->setGlobalZOrder(1001);
        
    
       
        
        //pBg->setPosition(Vec2(0,0));
        
        
        
        /*
         *****PVP相关***
         */
        
        if (CFightData::getInstance()->getPVPTheCity()==this) { //如果pvp主基地对象和自己想同，则判断fightdata的胜利条件
            
            CFightData::getInstance()->setWinState(1,this);
            
        }

    }
    
    
    
    
    
    
}


void CBaseBuilding::beAttacked(CBaseFighter* target,int attk,float damagePercent,FightGroup attackerGroup ,DamageType type){
    
    //建筑受到投石类型伤害时，所受伤害翻5倍
    if (type==eDamageType_StoneSkill) {
        damagePercent=damagePercent*5;
    }
    
    CBaseFighter::beAttacked(target, attk, damagePercent, attackerGroup, type);
    
    //建筑变色
    auto theAction = Sequence::create(TintTo::create(0.35f, 255, 0, 0),TintTo::create(0.35f, 255, 255, 255), NULL);
    theAction->setTag(999);
    
    m_pTheSrite->stopActionByTag(999);
    m_pTheSrite->runAction(theAction);
    
    CFightManager::RunTumblerAction(m_pTheSrite, 20, 0.15, 1,true,0,m_vecBaseRotation);

}

void CBaseBuilding::ChangeModelWithPVPBuildingId(int thePVPBuildingId){
    
    
    if (_hurtShader) {
        _hurtShader->removeFromParent();
        _hurtShader=NULL;
    }
    
    if (_hurtShaderBuilding) {
        _hurtShaderBuilding->removeFromParent();
        _hurtShaderBuilding=NULL;
    }
    
    m_pTheSrite->removeFromParentAndCleanup(true);
    m_pTheSrite=NULL;
    
    
    //测试用
//    thePVPBuildingId = 21;
    
    
    pvpBuildInfo* temp = CDataManager::getBuildInfo(thePVPBuildingId);
    
    
    m_pTheSrite = CFightManager::getModelWithPVPBuildingId(thePVPBuildingId,2);
    if (m_pTheSrite) {
        addChild(m_pTheSrite,1);
        CFightManager::addCamera(m_pTheSrite);
    }
    
    
    
    m_pTowerUp = CFightManager::getModelWithPVPBuildingId(thePVPBuildingId,1);
    if (m_pTowerUp) {
        CFightManager::addCamera(m_pTowerUp);
        if (m_pTheSrite) {
            float height = m_pTheSrite->getBoundingBox().size.height/m_pTheSrite->getScale()*0.9f;
            m_pTowerUp->setPosition3D(Vec3(0, height, 0));
            float scale = m_pTowerUp->getScale()/m_pTheSrite->getScale();
            m_pTowerUp->setScale(scale);
            m_pTheSrite->addChild(m_pTowerUp);
//            m_pTheSrite->addChild(m_pTowerUp);
        }else{
            m_pTheSrite = m_pTowerUp;
            addChild(m_pTheSrite,1);
        }

    }
    
    
//    //测试用
//    if (m_strTheInfo->monsterId==20000) {
//        m_pTheSrite->setScale(3.7);
//    }
    
    
    
    
//    m_pTheSrite->setScale(m_pTheSrite->getScale()*0.5f);
    
    
//    if (temp->staticModelId!=0) {
//        modelInfo* downtemp = CDataManager::getModelInfoByModelId(temp->staticModelId);
//        
//        m_pTheSrite->removeFromParentAndCleanup(true);
//        //    m_pTheSrite = Sprite3D::create("models/newBuilding/kuang.c3b");
//        
//        if (temp->texturePath!="0") {
//            m_pTheSrite = Sprite3D::create(downtemp->modelName.getCString(),temp->texturePath);
//        }else{
//            m_pTheSrite = Sprite3D::create(downtemp->modelName.getCString());
//        }
//        
//        
//        if(temp->texturePath == "0")
//        {
//            m_pTheSrite = Sprite3D::create(downtemp->modelName.getCString());
//        }
//        addChild(m_pTheSrite,1);
//        CFightManager::addCamera(m_pTheSrite);
//        m_pTheSrite->setScale(downtemp->modelScale);
//        
//        
//
//    }
    
    
//    if (temp->activeModelId!=0) {
//        modelInfo* uptemp = CDataManager::getModelInfoByModelId(temp->activeModelId);
//        
//        
//        
//        m_pTowerUp = Sprite3D::create(uptemp->modelName.getCString());
//        CFightManager::addCamera(m_pTowerUp);
//        
//        float height = m_pTheSrite->getBoundingBox().size.height;
//        
//        m_pTowerUp->setPosition3D(Vec3(0, height, 0));
//        
//        m_pTowerUp->setRotation3D(Vec3(0, -90, 0));
//        
//        m_pTowerUp->setScale(uptemp->modelScale);
//
//        
//        if (m_pTheSrite) {
//            m_pTheSrite->addChild(m_pTowerUp);
//        }else{
//            m_pTheSrite = m_pTowerUp;
//            addChild(m_pTheSrite,1);
//        }
//        
//        
//        
//    }
    
    
    if(CPersonalData::getInstance()->getPvpMainLayer()==false)
    {
        initShader();
    }
    
}

void CBaseBuilding::resetHPPosition(){
    
    CBaseFighter::resetHPPosition();
    
    
    if (m_pTowerUp&&m_pTheSrite!=m_pTowerUp) {    //如果有两个模型，则放大
        
        float high = m_pTowerUp->getBoundingBox().size.height+m_pTheSrite->getBoundingBox().size.height;
        
        m_pTheHpBar->setPosition(Vec2(0, high)+m_pTowerUp->getPosition());
        
//        m_pTheHpBar->setPosition(Vec2( 0, m_pTowerUp->getBoundingBox().size.height+m_pTheHpBar->getBGHP()->getBoundingBox().size.height*0.5f)+m_pTowerUp->getPosition());
        
    }
    
    
    
    
    
    
}


