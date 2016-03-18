//
//  LYBaseMonsterMine.cpp
//  TheGodWill
//
//  Created by user on 15/1/19.
//
//

#include "LYBaseMonsterMine.h"

#include "LYFightData.h"

#include "DataManager.h"

USING_NS_CC;

CBaseMonsterMine::CBaseMonsterMine(){
    
    m_pSolderNumLabel = NULL;
    
}



CBaseMonsterMine::~CBaseMonsterMine(){
    
    
    
}

CBaseMonsterMine *CBaseMonsterMine::create(FighterInfo *theInfo,FightGroup theGroup){
    
    CBaseMonsterMine * temp = new CBaseMonsterMine();
    
    temp->m_iAttackType = eAttackType_min;
    
    if (temp && temp->initWithInfo(theInfo,theGroup)) {
        temp->autorelease();
        return temp;
    }else{
        
        delete temp;
        temp = NULL;
        return NULL;
    }
    
}

bool CBaseMonsterMine::initWithInfo(FighterInfo *theInfo,FightGroup theGroup){
    
    //测试用，有模型了就删掉
    //    theInfo.theModelInfo->modelName= "model/piglet/piglet.c3b";
    //    theInfo.theModelInfo->radius = 50;
    
    if (!CBaseResourceMine::initWithInfo(theInfo,theGroup)) {
        return false;
    };
    
    
//    m_fSpriteScale = 1;
//    m_pTheSrite->setScale(m_fSpriteScale);
    
    m_pTheHpBar->setPosition(0, m_pTheSrite->getBoundingBox().size.height);
    
    //根据id初始化产生怪物id和产生cd
    int monsterId = 1;
    switch (m_strTheInfo->monsterId) {
        case 1001:
            monsterId =3;
            break;
            
        default:
            break;
    }
    
    levelInfo* tempInfo = CFightData::getInstance()->getTheLevelInfo();
    CDataManager::CalcFighterInfoWith(&m_stTheFighterInfo, monsterId,  tempInfo->monsterLevel,1,0,false,tempInfo->monsterSkillLevel,tempInfo->monsterSkillLevel,tempInfo->monsterSkillLevel,tempInfo->monsterSkillLevel);
    
    
    
    m_fMaxProudceTime = m_stTheFighterInfo.energyCost/10;
    m_fNowProudceTime = 0;
    m_iMaxSoldierNum = 5;
    m_iNowSoldierNum = 0;
    
    
    
    //初始化怪物数量，暂时用
    __String * tempStr = __String::createWithFormat("%d/%d",m_iNowSoldierNum,m_iMaxSoldierNum);
    
    m_pSolderNumLabel = Label::createWithSystemFont(tempStr->getCString(), "Arial", 36);
    m_pSolderNumLabel->setColor(Color3B::RED);
    m_pSolderNumLabel->setAnchorPoint(Vec2(0, 0.5));
    m_pSolderNumLabel->setPosition3D(Vec3(0,70,0));
    m_pSolderNumLabel->setCameraMask((unsigned short )CameraFlag::USER1);
    addChild(m_pSolderNumLabel,11);
    
    m_pSolderNumLabel->setGlobalZOrder(1000);
    
    m_pSolderNumLabel->setBlendFunc(cocos2d::BlendFunc::ALPHA_NON_PREMULTIPLIED);

    
    
    

    
    return true;
    
}

void CBaseMonsterMine::stateUpdate(float time){
    
    
    CBaseResourceMine::stateUpdate(time);
    
    
    //时间增长，判断什么时候出兵
    
    m_fNowProudceTime+=time;
    
    
    if (m_fNowProudceTime>=m_fMaxProudceTime&&m_iFightGroup!=eFightGroup_midNotFight) {
        
        m_fNowProudceTime =0;
        
        
        CFightScene* target = CFightData::getInstance()->getTheFightScene();

        if (target&&target->gettheLayer3D()) {
            
            CBaseFighter* theOne;
            
            m_iNowSoldierNum++;

            theOne =target->addMonsterWithIdAndGroupAndPosition(&m_stTheFighterInfo, m_iFightGroup, getFightPosition());

            allSoldier.pushBack(theOne);
            
        }
        
    }
    
    
    for (int i=0; i<allSoldier.size(); ) {//遍历一下士兵死活，如果死掉了则减一
        
        CBaseFighter* theOne = allSoldier.at(i);
        
        if (theOne&&theOne->getReferenceCount()>0&&!theOne->getIfDied()) {//如果怪没死，则啥事儿没有
            i++;
        }else{//如果怪死了，则删除
            allSoldier.eraseObject(theOne);
        }
        
    }
    
    
    //显示数量刷新
    __String * tempStr = __String::createWithFormat("%d/%d",m_iNowSoldierNum,m_iMaxSoldierNum);
    
    m_pSolderNumLabel->setString(tempStr->getCString());
    
    
}



void CBaseMonsterMine::onChangeOwner(FightGroup target){
    
    //测试用注释
    
//    m_pTheSrite->removeFromParentAndCleanup(true);
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
//        //        modelPath.append("_blue");
//    }else if (m_iFightGroup == eFightGroup_Enemy){
//        temp.replace(offset,5,"red");
//        //        modelPath.append("_red");
//    }
//    
//    initModelWithPath(temp);
    
    
//    refreshTheOwner(target);
    
    
//    //修改拥有者
//    m_iFightGroup = target;
    
//    m_fNowTime = 0;//生产时间归0
    
}


void CBaseMonsterMine::reborn(){
    
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
    
    
    
    //减去原先拥有者的能量增长值

//    refreshTheOwner();
    
    allSoldier.clear();
    

    CBaseResourceMine::reborn();
    
    
    m_iNowSoldierNum = 0;
    m_fNowProudceTime = 0;//生产时间归0
    
    //    m_pTheHpBar->setScale(10);
}


