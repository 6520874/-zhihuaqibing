//
//  LYFightManager.cpp
//  TheGodWill
//
//  Created by user on 14/12/9.
//
//

#include "LYFightManager.h"




#include "DataManager.h"

#include "LYBaseFighter.h"

#include "LYBaseBuff.h"

#include "PublicDefine.h"

#include "BillBoardLable.h"

#include "LYFightTalk.h"

#include "LYFightData.h"


#include "LYSprite3dShaderWhite.h"
#include "LYSprite3dShaderGold.h"
#include "LYSprite3dAnimateShaderGold.h"

#include "LYSprite3dAnimateShaderWithFile.h"

#include "LYSprite3dAnimateShaderOnce.h"
#include "guiderManagerLayer.h"
USING_NS_CC;

void CFightManager::addCamera(cocos2d::Node *target){
    
    target->setCameraMask((unsigned short )CameraFlag::USER1);
}


float CFightManager::getTheDegreeWith(cocos2d::Vec2 StartPos, cocos2d::Vec2 endPos,bool b3D, cocos2d::Vec2 defaultVector){
    
    
    //面向修正
    Vec2 tempPoint;
    
    if(b3D)
    {
        StartPos.y = -StartPos.y;
        endPos.y = -endPos.y;
    }
    tempPoint.set(StartPos,endPos);
    
    float tempAngle = tempPoint.getAngle(defaultVector);//计算角度
    
    
    float radians = CC_RADIANS_TO_DEGREES(tempAngle);
    
    return radians;
}

Sprite* CFightManager::getRewardWord(std::string text, float delayTime, std::string font, float fontSize)
{
    Sprite* showBg = Sprite::createWithSpriteFrameName("rewardBg.png");
    showBg->setCascadeOpacityEnabled(true);
    showBg->setOpacity(0);
    Label* pLabel = Label::createWithTTF(text.c_str(), font, fontSize);
    showBg->addChild(pLabel);
    pLabel->setPosition(Vec2(showBg->getBoundingBox().size.width * 0.5f, showBg->getBoundingBox().size.height * 0.5f));
    ActionInterval* fade = FadeOut::create(0.6);
    showBg->runAction(Sequence::create(DelayTime::create(delayTime), FadeTo::create(0, 255), MoveBy::create(1.5f, Point(0, 210)), fade, RemoveSelf::create(), NULL));
    return showBg;
}

Sprite* CFightManager::getTheFlyWord(std::string text,std::string font,float fontSize,float upSpeed, float waitTime,std::string BGPath, Color3B textColor){
    
    Label * thetarget = NULL;
    
    if(font=="fonts/cuti.ttf"){
        
        thetarget = Label::createWithTTF(text, font, fontSize);
        
    }else{
        
        thetarget =   Label::createWithSystemFont(text, font, fontSize);

    }
    
    Sprite* theBG = Sprite::createWithSpriteFrameName(BGPath.c_str());
    
    thetarget->setColor(textColor);
    thetarget->setPosition(Vec2(theBG->getBoundingBox().size.width*0.5f, theBG->getBoundingBox().size.height*0.5f));
    
    theBG->addChild(thetarget);
    
    Spawn* startAction = Spawn::create(FadeIn::create(0.3f),EaseSineOut::create(MoveBy::create(0.5f, Vec2(0, 100))), NULL);
    Spawn* startAction2 = Spawn::create(FadeIn::create(0.3f),MoveBy::create(0.5f, Vec2(0, 1)), NULL); //至于为什么文字和背景动作不同步，是因为，文字的父节点是背景。但是如果自己不调用动作的话，他又不会动
    
    theBG->runAction(Sequence::create(startAction,DelayTime::create(0.3f),FadeOut::create(0.3f),RemoveSelf::create(),NULL));
//    thetarget->updateContent();
    thetarget->runAction(Sequence::create(startAction2,DelayTime::create(0.3f),FadeOut::create(0.3f),RemoveSelf::create(),NULL));
    
    
    
    
    
    return theBG;
}

void CFightManager::getTheFlyAction(cocos2d::Node *target){
    
    
    target->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(0, 100)),DelayTime::create(0.5f),RemoveSelf::create(),NULL));
    
}



float CFightManager::getTheAtrEffect(FighterType attacker, FighterType beAttacker){
    
    
    //只有三角形、圆形、方形参与兵种相克，其余的不参与
    if (attacker>eFighterType_square||attacker<eFighterType_triangle||beAttacker>eFighterType_square||beAttacker<eFighterType_triangle) {
        return 1;
    }
    

    //相克系数
    float theRatio = 1;
    
    
    
    int offset = attacker - beAttacker ;
    
    if (offset==1||offset == eFighterType_square-eFighterType_triangle) {
        theRatio = 1+AtrEffect;
    }else if (offset==-1||offset ==eFighterType_triangle- eFighterType_square){
        theRatio = 1- AtrEffect;
    }
    
    
//    CCLOG("攻击属性增益系数 %f",theRatio);
    
    
    return theRatio;
    
}


bool CFightManager::ifDropItem(int type, int id){
    
    
    
    if(type==0){
        propInfo* target = CDataManager::getPropInfoById(id);
        
        
        int theratio = target->m_fDropRatio*10000;
        
        
        int theRand = rand()%10000;
        
        if (theRand<theratio) {
            return true;
        }
        
    }else if (type==1){
        
        //如果是碎片，暂定掉率为1/10
        
        int theRatio = 10000*0.1;
        
        int theRand = rand()%10000;
        
        if (theRand<theRatio) {
            return true;
        }
        
    }
    
    return false;
}

void CFightManager::addTheBuffToFighter(CBaseFighter *target, AllBuffes buffId,float duration,float baseDamage){
    
    
    CBaseBuff* temp = CBaseBuff::create();
    
    target->addChild(temp,100);
    target->getAllBuffes()->pushBack(temp);
    
    temp->setDamageBase(baseDamage);
    
    temp->setTheTarget(target);
    
    temp->setBuffIdAndStart(buffId,duration);

}

Vec3 CFightManager::getThePosInMap(cocos2d::Vec3 tempPos){
    
    
    
    if (tempPos.x<Start3DPOINT-0.5*ONESIDE) {
        tempPos.x = Start3DPOINT-0.5*ONESIDE;
    }
    
    if (tempPos.x>END3DPOINT+0.5*ONESIDE) {
        tempPos.x = END3DPOINT+0.5*ONESIDE;
    }
    
    if (tempPos.z>STARTZPOINT) {
        tempPos.z = STARTZPOINT;
    }
    
    if (tempPos.z<ENDZPOINT) {
        tempPos.z = ENDZPOINT;
    }
    
    if (tempPos.y<0) {
        tempPos.y = 0;
    }
    
    
    
    
    
    return tempPos;
    
}

Vec2 CFightManager::getThePosInMap(cocos2d::Vec2 tempPos){
    
    Vec3 temp = Vec3(tempPos.x,0,tempPos.y);
    
    
    Vec3 target = getThePosInMap(temp);
    
    
    return Vec2(target.x, target.z);

}


void CFightManager::addTheFightTalkToFighter(CBaseFighter *target, int type,int talkId){
    
    fightTalkingInfo* theInfo = NULL;
    
    if (talkId!=0) { //没有确切的对话id就是用对话类型来获取对话信息
        theInfo = CDataManager::getTheFightTalkingInfoWithId(talkId);
    }else{
        theInfo = CDataManager::getTheRandomFightTalkingInfoByType(type);
    }
    
    if (!target||!theInfo) {
        CCASSERT(0, "LYAssert: fightTalking-- target or Info Null");
        return;
    }
    
    
    CFightTalk* theTalk = CFightTalk::create();
    theTalk->setTheInfo(theInfo,target);
    
    target->setTheFightTalk(theTalk);
    
    
    
    
    
//    std::string tempValue = theInfo->content;
//    
//    Sprite* theBG = Sprite::create("fightTalkIcon/FT_BG.png");
//    
//    
//    target->addChild(theBG);
//    
//    
//    
//    float textSize=30;
//    
//    
//    
//    
//    float BG_Width = theBG->getBoundingBox().size.width;
//    
//    long textNum = tempValue.length();//总字数`
//    
//    int lineText = BG_Width/(textSize+2); //每行最多字数
//    
//    float lineNum = int(textNum/lineText) +  ((textNum/lineText-int(textNum/lineText))>0? 1:0);  //所有字行数
//    
//    float textHight = lineNum* (textSize+5);
//    
//    float scaleY = textHight/theBG->getBoundingBox().size.height;
//    
//    theBG->setScale(1, scaleY);
//    
//    theBG->setPosition3D(Vec3(0,target->gettheSprite()->getBoundingBox().size.height+theBG->getBoundingBox().size.height*0.5f,0));
//    
//    
//    Label* theContent = BillBoardLable::createWithSystemFont(tempValue.c_str(), "Arial", textSize);
//    addCamera(theContent);
//    target->addChild(theContent);
//    theContent->setPosition3D(theBG->getPosition3D());
    
    
//    addCamera(theBG);
    
}

void CFightManager::addTheFightTalkToGroupAndId(FightGroup targetGroup, int talkId){
    
    
    
    
    
    
}


void CFightManager::RunShakeAction(cocos2d::Node *target,float dist,float shakeTime,float shakeTimes,float time){
    
    
    //卡牌抖动

    //            CCSequence *shake1 = CCSequence::create(CCMoveBy::create(shakeTime, ccp(0,dist)),CCMoveBy::create(shakeTime, ccp(0,-dist)),NULL);
    Sequence *shake2 = Sequence::create(CCMoveBy::create(shakeTime/2, Point(dist/2,0)),CCMoveBy::create(shakeTime, Point(-dist,0)),CCMoveBy::create(shakeTime/2, Point(dist/2,0)),NULL);
    
    Sequence *TheAction = CCSequence::create(CCDelayTime::create(time),CCRepeat::create(shake2, shakeTimes),NULL);
    
    TheAction->setTag(888);
    //    CCSpawn *shake = CCSpawn::create(shake1,shake2);
    //            solder->runAction(CCSequence::create(CCDelayTime::create(time),CCRepeat::create(shake1, shakeTimes),NULL) );
    target->stopActionByTag(888);
    target->runAction(TheAction);
    

}

void CFightManager::RunTumblerAction(cocos2d::Node *target,float angle,float shakeTime,float shakeTimes,bool ifWeak,float time,Vec3 defaultRoatation){
    
    
    RotateBy::create(shakeTime/2, Vec3(0, 0, angle/2));
    
    Sequence *shake2 = NULL;
    
    
    if (ifWeak) { //振幅是否越来越小, 注：只支持一回合的越来越小
        shake2 = Sequence::create(RotateBy::create(shakeTime/2, Vec3(0, 0, angle/2)),RotateBy::create(shakeTime, Vec3(0, 0, -angle*3/4)),RotateBy::create(shakeTime/2, Vec3(0, 0, angle*1/4)),NULL);
    }else{ //来回摇动
        shake2 = Sequence::create(RotateBy::create(shakeTime/2, Vec3(0, 0, angle/2)),RotateBy::create(shakeTime, Vec3(0, 0, -angle)),RotateBy::create(shakeTime/2, Vec3(0, 0, angle/2)),NULL);
    }
    
    Sequence *TheAction = CCSequence::create(CCDelayTime::create(time),CCRepeat::create(shake2, shakeTimes),NULL);
    
    TheAction->setTag(889);
    //    CCSpawn *shake = CCSpawn::create(shake1,shake2);
    //            solder->runAction(CCSequence::create(CCDelayTime::create(time),CCRepeat::create(shake1, shakeTimes),NULL) );
    target->stopActionByTag(889);
    
    target->setRotation3D(defaultRoatation);
    
    target->runAction(TheAction);

}


Sprite3D* CFightManager::getTheModelWithPath(std::string theBulletPath){
    
    
    Sprite3D* target = NULL;
    
    long flag = theBulletPath.find(".png");
    
    
    if (flag!=-1) { //如果是png类型
        
        target = Sprite3D::create();
        //CCLOG("%s", theBulletPath.c_str());
        Sprite* temp = Sprite::createWithSpriteFrameName(theBulletPath.c_str());
        target->addChild(temp,1,100);
        
    }else{  //如果是c3b类型
        
        target = Sprite3D::create(theBulletPath.c_str());
        
    }
    
    return target;
}


Animate* CFightManager::getAnimateWithPath(std::string path1, int startNumber, int allNumber,std::string path2, float interval){


    
    Animation * hurtAnimation = Animation::create();
    
    for (int i=startNumber; i<(allNumber+startNumber); i++)
    {
        
        __String *temp = __String::createWithFormat("%s%d%s",path1.c_str(),i,path2.c_str());
        
//        CCLOG("%s",temp->getCString());
        //                Texture2D* hero = TextureCache::getInstance()->addImage(temp->getCString());
        //
        //
        //                SpriteFrame* temp2 =SpriteFrame::createWithTexture(hero, hero->getContentSize());
        //
        //                Sprite* temp3 = Sprite::create(temp->getCString());
        
        SpriteFrame* tempFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(temp->getCString());
        if (tempFrame) {
            hurtAnimation->addSpriteFrame(tempFrame);
        }else{
            CCASSERT(0, "animate error");
        }
        
        
        
    }
    
    hurtAnimation->setDelayPerUnit(interval);

    return Animate::create(hurtAnimation);


}

void CFightManager::getMemoryList(){
    
    
    std::string logDetail =   Director::getInstance()->getTextureCache()->getCachedTextureInfo();
    CCLOG("%s",logDetail.c_str());
    
    
}

void CFightManager::removeUnusedMemory(){
    
    Animation3DCache::getInstance()->removeUnusedAnimation();//卸掉动画内存
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();//卸掉纹理内存

    
}


void CFightManager::addSprite3dAnimationBeforeFight(){
    
    
    std::vector<std::string> allVec = CFightData::getInstance()->getNeededPngPath(true);
    
    for (int i=0; i<allVec.size(); i++) {
        
        Animation3D::create(allVec.at(i));
    }
    
}

CBaseFighter* CFightManager::getTheFrontSoldier(FightGroup theGroup){
    
    //获取最靠前的敌人，将摄像机缓慢移动到敌人位置上
    cocos2d::Vector<CBaseFighter*> targetVec = CFightData::getInstance()->getFriendMonstersByGroup(theGroup);
    
    CBaseFighter* target = NULL;
    float Xposition = 0;
    
    if (theGroup==eFightGroup_Enemy) { //如果是敌人，则x轴越小越靠前，如果是友军，则x轴越大越靠前
        Xposition=99999;
    }
    
    for (int i=0; i<targetVec.size(); i++) {
        
        CBaseFighter* tempTarget = targetVec.at(i);
        
        if (theGroup==eFightGroup_Freind&&tempTarget->getPositionX()>Xposition) {
            
            target = tempTarget;
            Xposition = tempTarget->getPositionX();
        }else if (theGroup==eFightGroup_Enemy&&tempTarget->getPositionX()<Xposition){
            target = tempTarget;
            Xposition = tempTarget->getPositionX();
        }
        
    }

    return target;
}

CBaseFighter* CFightManager::getTheFrontSoldierAndBuilding(FightGroup theGroup){
    
    //获取最靠前的敌人，将摄像机缓慢移动到敌人位置上
    cocos2d::Vector<CBaseFighter*> targetVec = CFightData::getInstance()->getFriendMonstersByGroup(theGroup);
    
    cocos2d::Vector<CBaseFighter*> tempVec = CFightData::getInstance()->getFriendTowerByGroup(theGroup);
    
    targetVec.pushBack(tempVec);
    
    
    CBaseFighter* target = NULL;
    float Xposition = 0;
    
    if (theGroup==eFightGroup_Enemy) { //如果是敌人，则x轴越小越靠前，如果是友军，则x轴越大越靠前
        Xposition=99999;
    }
    
    for (int i=0; i<targetVec.size(); i++) {
        
        CBaseFighter* tempTarget = targetVec.at(i);
        
        if (theGroup==eFightGroup_Freind&&tempTarget->getPositionX()>Xposition) {
            
            target = tempTarget;
            Xposition = tempTarget->getPositionX();
        }else if (theGroup==eFightGroup_Enemy&&tempTarget->getPositionX()<Xposition){
            target = tempTarget;
            Xposition = tempTarget->getPositionX();
        }
        
    }
    
    return target;
    
}

CBaseFighter* CFightManager::getTheWalker(){
    
    
    cocos2d::Vector<CBaseFighter*> targetVec = CFightData::getInstance()->getFriendMonstersByGroup(eFightGroup_Freind);
    
    CBaseFighter* target = NULL;
    
    for (int i=0; i<targetVec.size(); i++) {
        
        CBaseFighter* tempTarget = targetVec.at(i);
        
        if (tempTarget->getAttackType()==eAttackType_walker) {
            
            return tempTarget;
        }
        
        
    }
    
    return NULL;
    
}


Sprite3D* CFightManager::getModelWithPVPBuildingId(int PVPbuildingId,int type,int globZorder){
    
    
//    PVPbuildingId = 21;
    
    
    pvpBuildInfo* temp = CDataManager::getBuildInfo(PVPbuildingId);
    
    
    Sprite3D* targetSprite=NULL;
    
    Sprite3D* upSprite=NULL;
    
    
    
    if (temp->staticModelId!=0) {
        modelInfo* downtemp = CDataManager::getModelInfoByModelId(temp->staticModelId);
        
        //        m_pTheSrite->removeFromParentAndCleanup(true);
        //    m_pTheSrite = Sprite3D::create("models/newBuilding/kuang.c3b");
        
        if (temp->texturePath!="0") {
            targetSprite = Sprite3D::create(downtemp->modelName.getCString(),temp->texturePath);
        }else{
            targetSprite = Sprite3D::create(downtemp->modelName.getCString());
        }
        
        
        if(temp->texturePath == "0")
        {
            targetSprite = Sprite3D::create(downtemp->modelName.getCString());
        }
        targetSprite->setScale(downtemp->modelScale);
        
        if (globZorder!=0) {
            targetSprite->setGlobalZOrder(globZorder);
        }
        
    }
    
    
    if (temp->activeModelId!=0) {
        modelInfo* uptemp = CDataManager::getModelInfoByModelId(temp->activeModelId);
        
        upSprite = Sprite3D::create(uptemp->modelName.getCString());
//        CFightManager::addCamera(m_pTowerUp);
        
        upSprite->setRotation3D(Vec3(0, -90, 0));
        
        upSprite->setScale(uptemp->modelScale);

        if (globZorder!=0) {
            upSprite->setGlobalZOrder(globZorder);
        }
    }
    
    
    
//    if (targetSprite) {
//        float height = targetSprite->getBoundingBox().size.height;
//        upSprite->setPosition3D(Vec3(0, height, 0));
//        targetSprite->addChild(upSprite);
//    }else{
//        targetSprite = upSprite;
//    }
    
    
    if(type==0&&upSprite){  //如果返回的是全部
        
        if (targetSprite) {
            float height = targetSprite->getBoundingBox().size.height/targetSprite->getScale()*0.90f;
            upSprite->setPosition3D(Vec3(0, height, 0));
            upSprite->setScale(upSprite->getScale()/targetSprite->getScale());
            targetSprite->addChild(upSprite);
            

        }else{
            targetSprite=upSprite;
        }
        
    }else if (type==1){   //如果只返回上面模型
        
        targetSprite=upSprite;

    }
    
    
//    //测试用
//    if(targetSprite){
//        targetSprite->setScale(75);
//    }

    return targetSprite;
    
    
    
}



void CFightManager::closeCloud(cocos2d::Ref *target, cocos2d::SEL_CallFuncO handler,LayerColor *m_pLayer)
{
    if(m_pLayer)
    {
        Sprite *pCloud1=  (Sprite*)m_pLayer->getChildByTag(ePvp_Cloud1Tag);
        Sprite *pCloud2=  (Sprite*)m_pLayer->getChildByTag(ePvp_Cloud2Tag);
        Sprite *pCloud3=  (Sprite*)m_pLayer->getChildByTag(ePvp_Cloud3Tag);
        Sprite *pCloud4=  (Sprite*)m_pLayer->getChildByTag(ePvp_Cloud4Tag);
        Sprite *pCloud5=  (Sprite*)m_pLayer->getChildByTag(ePvp_Cloud5Tag);
        Sprite *pCloud6=  (Sprite*)m_pLayer->getChildByTag(ePvp_Cloud6Tag);
        
        Size  m_WinSize = Director::getInstance()->getWinSize();
        
        
        float fTime1 = 2;
        
        
        pCloud1->runAction(Sequence::create(DelayTime::create(fTime1*0.2),Spawn::create(EaseSineOut::create(MoveTo::create(fTime1*0.8,Vec2(-pCloud1->getBoundingBox().size.width*0.5,pCloud1->getBoundingBox().size.height*0.5))),FadeTo::create(fTime1*0.8,255*0.5),NULL),NULL));
        
        pCloud3->runAction(Sequence::create(DelayTime::create(fTime1*0.3),Spawn::create(EaseSineOut::create(MoveTo::create(fTime1/2,Vec2(-pCloud3->getBoundingBox().size.width*0.5,pCloud3->getBoundingBox().size.height*0.5))),FadeTo::create(fTime1/2,255*0.7),NULL),NULL));
        
        pCloud5->runAction(Sequence::create(Spawn::create(EaseSineOut::create(MoveTo::create(fTime1*3/4,Vec2(-pCloud5->getBoundingBox().size.width*0.5,pCloud5->getBoundingBox().size.height*0.5))),FadeTo::create(fTime1/4,255*0.9),NULL),NULL));
        
        
        pCloud2->runAction(Sequence::create(DelayTime::create(fTime1*0.2),Spawn::create(EaseSineOut::create(MoveTo::create(fTime1*0.8,Vec2(m_WinSize.width+pCloud2->getBoundingBox().size.width*0.5,m_WinSize.height-pCloud2->getBoundingBox().size.height*0.5))),FadeTo::create(fTime1*0.8,255*0.5),NULL),NULL));
        
        pCloud4->runAction(Sequence::create(DelayTime::create(fTime1*0.3),Spawn::create(EaseSineOut::create(MoveTo::create(fTime1/2,Vec2(m_WinSize.width+pCloud4->getBoundingBox().size.width*0.5,m_WinSize.height- pCloud4->getBoundingBox().size.height*0.5))),FadeTo::create(fTime1/2,255*0.7),NULL),NULL));
        
        pCloud6->runAction(Sequence::create(Spawn::create(EaseSineOut::create(MoveTo::create(fTime1*3/4,Vec2(m_WinSize.width+pCloud6->getBoundingBox().size.width*0.5,m_WinSize.height-pCloud6->getBoundingBox().size.height*0.5))),FadeTo::create(fTime1/4,255*0.9),NULL),CallFunc::create([=]{
            
    
            if(target&&target)
            {
                (target->*handler)(NULL);
            }
        }),NULL));
    }
    
    
    
}

void  CFightManager::addCloud(cocos2d::Ref *target, cocos2d::SEL_CallFuncO handler,LayerColor *m_pLayer)
{
   
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_ROB) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
    {
        guiderManagerLayer::getInstance()->removeGuiderLayer();
    }
    Size  m_WinSize = Director::getInstance()->getWinSize();
    
    
    Sprite *pCloud5 = Sprite::createWithSpriteFrameName("pvpcloud5.png");
    pCloud5->setScale(1/0.4);
    pCloud5->setPosition(-pCloud5->getBoundingBox().size.width*0.5,pCloud5->getBoundingBox().size.height*0.5);
    pCloud5->setOpacity(255*0.9);
    m_pLayer->addChild(pCloud5,3,ePvp_Cloud5Tag);
 
    Sprite *pCloud3 = Sprite::createWithSpriteFrameName("pvpcloud3.png");
    pCloud3->setScale(1.7/0.4);
    pCloud3->setPosition(-pCloud3->getBoundingBox().size.width*0.5,pCloud3->getBoundingBox().size.height*0.5);
    m_pLayer->addChild(pCloud3,1,ePvp_Cloud3Tag);
    pCloud3->setOpacity(255*0.7);

    
    
    Sprite *pCloud1 =  Sprite::createWithSpriteFrameName("pvpcloud1.png");
    pCloud1->setScale(2/0.4);
    pCloud1->setPosition(-pCloud1->getBoundingBox().size.width*0.5,pCloud1->getBoundingBox().size.height*0.5);
    m_pLayer->addChild(pCloud1,0,ePvp_Cloud1Tag);
    pCloud1->setOpacity(255*0.5);
    
    

    Sprite *pCloud6 = Sprite::createWithSpriteFrameName("pvpcloud6.png");
    pCloud6->setScale(1/0.4);
    pCloud6->setPosition(m_WinSize.width+pCloud6->getBoundingBox().size.width*0.5,m_WinSize.height-pCloud6->getBoundingBox().size.height*0.5);
    m_pLayer->addChild(pCloud6,2,ePvp_Cloud6Tag);
    pCloud6->setOpacity(255*0.9);
   
    
    Sprite *pCloud4 = Sprite::createWithSpriteFrameName("pvpcloud4.png");
       pCloud4->setScale(1.7/0.4);
    pCloud4->setPosition(m_WinSize.width+pCloud4->getBoundingBox().size.width*0.5,m_WinSize.height-pCloud4->getBoundingBox().size.height*0.5);
    m_pLayer->addChild(pCloud4,1,ePvp_Cloud4Tag);
    pCloud4->setOpacity(255*0.7);
 
    
    Sprite *pCloud2 = Sprite::createWithSpriteFrameName("pvpcloud2.png");
    pCloud2->setScale(2/0.4);
    pCloud2->setPosition(m_WinSize.width+pCloud2->getBoundingBox().size.width*0.5,m_WinSize.height-pCloud2->getBoundingBox().size.height*0.5);
    m_pLayer->addChild(pCloud2,0,ePvp_Cloud2Tag);
    pCloud2->setOpacity(255*0.5);
 
    
    //速度 1-2-3
    //1 50-10
    // 2  70 -30
    //3  50-90
    
    float fTime1 = 2;
   
    pCloud1->runAction(Sequence::create(Spawn::create(EaseSineIn::create(MoveTo::create(fTime1*0.8,Vec2(pCloud1->getBoundingBox().size.width*0.5,pCloud1->getBoundingBox().size.height*0.5))),FadeTo::create(fTime1*0.8,255*0.1),NULL),NULL));
    
    pCloud3->runAction(Sequence::create(DelayTime::create(fTime1/2),Spawn::create(EaseSineIn::create(MoveTo::create(fTime1/2,Vec2(pCloud3->getBoundingBox().size.width*0.5,pCloud3->getBoundingBox().size.height*0.5))),FadeTo::create(fTime1/2,255*0.3),NULL),NULL));
    
    pCloud5->runAction(Sequence::create(DelayTime::create(fTime1*3/4),Spawn::create(EaseSineIn::create(MoveTo::create(fTime1/4,Vec2(pCloud5->getBoundingBox().size.width*0.5,pCloud5->getBoundingBox().size.height*0.5))),FadeTo::create(fTime1/4,255),NULL),NULL));
    
    
    pCloud2->runAction(Sequence::create(Spawn::create(EaseSineIn::create(MoveTo::create(fTime1*0.8,Vec2(m_WinSize.width-pCloud2->getBoundingBox().size.width*0.5,m_WinSize.height-pCloud2->getBoundingBox().size.height*0.5))),FadeTo::create(fTime1*0.8,255*0.1),NULL),NULL));
    
    
    pCloud4->runAction(Sequence::create(DelayTime::create(fTime1/2),Spawn::create(EaseSineIn::create(MoveTo::create(fTime1/2,Vec2(m_WinSize.width-pCloud4->getBoundingBox().size.width*0.5,m_WinSize.height-pCloud4->getBoundingBox().size.height*0.5))),FadeTo::create(fTime1/2,255*0.3),NULL),NULL));
    
    pCloud6->runAction(Sequence::create(DelayTime::create(fTime1*3/4),Spawn::create(EaseSineIn::create(MoveTo::create(fTime1/4,Vec2(m_WinSize.width-pCloud6->getBoundingBox().size.width*0.5,m_WinSize.height-pCloud6->getBoundingBox().size.height*0.5))),FadeTo::create(fTime1/4,255),NULL),CallFunc::create([=]{
        
        if(target&&target)
        {
            (target->*handler)(NULL);
        }
        
    }),NULL));

}


Node* CFightManager::addShaderToSprite3D(cocos2d::Sprite3D *temp,ShadeType targetShader,std::string GLProgramKey){
    
    //测试用
//    if(targetShader!=eShadeType_TurnWhite){
//        
//        return NULL;
//    }
    
    
    cocos2d::Node* effect=NULL;
    
    switch (targetShader) {
        case eShadeType_TurnWhite:
        {
            effect = CSprite3dShaderWhite::create();
            ((CSprite3dShaderWhite*)effect)->setTarget(temp);
            temp->addChild(effect);

        }
            break;
            
//        case eShadeType_TurnGold:
//        {
//            effect = CSprite3dShaderGold::create();
//            ((CSprite3dShaderGold*)effect)->setTarget(temp);
//            temp->addChild(effect);
//        }
//            break;
            
        case eShadeType_3D_Elite:
        {
            effect = CSprite3dAnimateShaderWithFile::create();
            ((CSprite3dAnimateShaderWithFile*)effect)->setTarget(temp,"Sprite3DTest/gold.png",GLProgramKey);
            temp->addChild(effect);
        }
            break;
        case eShadeType_3D_Boss:
        {
            effect = CSprite3dAnimateShaderWithFile::create();
            ((CSprite3dAnimateShaderWithFile*)effect)->setTarget(temp,"Sprite3DTest/stone.png",GLProgramKey);
            temp->addChild(effect);
        }
            break;
            
        case eShadeType_3D_beAttacked:
        case eShadeType_3D_BossBeAttacked:
        {
            effect = CSprite3dAnimateShaderOnce::create();
            ((CSprite3dAnimateShaderOnce*)effect)->setTarget(temp,"Sprite3DTest/white.png",GLProgramKey);
            temp->addChild(effect);
        }
            break;
            
        default:
            break;
    }
    
    
    return effect;
}


void CFightManager::preloadShader(bool ifRefresh){
    
    
    
    
    __String* temp = __String::createWithFormat("shaderOnce");
    
    __String* temp2 = __String::createWithFormat("shaderFade");
    
    __String* temp3 = __String::createWithFormat("shaderOnceBuilding");
    
    __String* temp4 = __String::createWithFormat("shaserOnceBoss");
    
    
    auto glprogram = GLProgramCache::getInstance()->getGLProgram(temp->getCString());
    auto glprogram2 = GLProgramCache::getInstance()->getGLProgram(temp2->getCString());
    
    auto glprogram3 = GLProgramCache::getInstance()->getGLProgram(temp3->getCString());
    
    auto glprogram4 = GLProgramCache::getInstance()->getGLProgram(temp4->getCString());
    
    
    if (!glprogram||ifRefresh) {
        auto glprogram = GLProgram::createWithFilenames("Sprite3DTest/fish/UVAnimation.vsh", "Sprite3DTest/fish/flashColor.fsh");
        GLProgramCache::getInstance()->addGLProgram(glprogram, temp->getCString());
    }else{

        if (ifRefresh) { //如果是刷新shader，则重新初始化一次
            if(glprogram){
                glprogram->reset();
                glprogram->initWithFilenames("Sprite3DTest/fish/UVAnimation.vsh", "Sprite3DTest/fish/flashColor.fsh");
                glprogram->link();
                glprogram->updateUniforms();
            }
        }
    }
    
    if (!glprogram2||ifRefresh) {
        auto glprogram2 = GLProgram::createWithFilenames("Sprite3DTest/fish/UVAnimation.vsh", "Sprite3DTest/fish/addColor.fsh");
        GLProgramCache::getInstance()->addGLProgram(glprogram2, temp2->getCString());
    }else{
        
        if (ifRefresh) { //如果是刷新shader，则重新初始化一次
            if(glprogram){
                glprogram->reset();
                glprogram->initWithFilenames("Sprite3DTest/fish/UVAnimation.vsh", "Sprite3DTest/fish/addColor.fsh");
                glprogram->link();
                glprogram->updateUniforms();
            }
        }
    }
    
    if (!glprogram3||ifRefresh) {
        auto glprogram3 = GLProgram::createWithFilenames("Sprite3DTest/cylinder.vert","Sprite3DTest/fadeColor.frag");
        GLProgramCache::getInstance()->addGLProgram(glprogram3, temp3->getCString());
    }else{
        
        if (ifRefresh) { //如果是刷新shader，则重新初始化一次
            if(glprogram){
                glprogram->reset();
                glprogram->initWithFilenames("Sprite3DTest/cylinder.vert","Sprite3DTest/fadeColor.frag");
                glprogram->link();
                glprogram->updateUniforms();
            }
        }
    }
    
    if (!glprogram4||ifRefresh) {
        auto glprogram4 = GLProgram::createWithFilenames("Sprite3DTest/fish/UVAnimation.vsh", "Sprite3DTest/fish/flashColorBoss.fsh");
        GLProgramCache::getInstance()->addGLProgram(glprogram4, temp4->getCString());
    }else{
        
        if (ifRefresh) { //如果是刷新shader，则重新初始化一次
            if(glprogram){
                glprogram->reset();
                glprogram->initWithFilenames("Sprite3DTest/fish/UVAnimation.vsh", "Sprite3DTest/fish/flashColorBoss.fsh");
                glprogram->link();
                glprogram->updateUniforms();
            }
        }
    }
    
    
    
}

//void CFightManager::reloadAllFighterShader(){
//    
//    
//    cocos2d::Vector<CBaseFighter*> temp = CFightData::getInstance()->getAllAttacker();
//    
//    for (int i =0; i<temp.size(); i++) {
//        
//        CBaseFighter* target = temp.at(i);
//        target->reloadShader();
//    }
//    
//}
