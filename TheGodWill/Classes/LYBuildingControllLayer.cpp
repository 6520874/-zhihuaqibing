//
//  LYBuildingControllLayer.cpp
//  TheGodWill
//
//  Created by user on 15/2/5.
//
//

#include "LYBuildingControllLayer.h"

#include "LYBaseFighter.h"

#include "LYFightData.h"

#include "Common.h"
#include "DataManager.h"
#include "guiderManagerLayer.h"
USING_NS_CC;

bool CBuildingControllLayer::init(){
    
    
    if (!Sprite::init()) {

        return false;
    }
    
    m_pTarget = NULL;
    m_iDetailMonsterId = -1;
    
    m_pCircleLayer = Sprite::createWithSpriteFrameName("TD_buildingCircle.png");
    addChild(m_pCircleLayer);
    m_pCircleLayer->setScale(1.5f);
    
    m_pMenu = Menu::create();
    m_pMenu->setPosition(Vec2::ZERO);
    m_pMenu->setAnchorPoint(Vec2::ZERO);
    addChild(m_pMenu);
    
    
    
    //提前初始化3个按钮
    for (int i=0; i<3; i++)
    {
        Sprite* TheButtonBG = Sprite::createWithSpriteFrameName("TD_Icon_BG.png");
        
        Sprite* TheIcon = Sprite::createWithSpriteFrameName("TD_Icon_cannon.png");
        TheButtonBG->addChild(TheIcon,1,CHILD_TAG_BUTTON_ICON);
        
        
        Sprite* TheButtonBGSelect = Sprite::createWithSpriteFrameName("TD_Icon_BG.png");
        
        Sprite* TheIconSelect = Sprite::createWithSpriteFrameName("TD_Icon_cannon.png");
        
        
        TheButtonBGSelect->addChild(TheIconSelect,1,CHILD_TAG_BUTTON_ICON);
        TheButtonBGSelect->setColor(Color3B::GRAY);
        

        LYMenuItemSprite *pMinusItem = LYMenuItemSprite::create(TheButtonBG,TheButtonBGSelect,CC_CALLBACK_1(CBuildingControllLayer::TheBtnDetail, this));
        pMinusItem->setScale(1.5f);
        m_pMenu->addChild(pMinusItem);
        
        
        //价格框
        Sprite* theCostBG = Sprite::createWithSpriteFrameName("TD_CostBG.png");
        theCostBG->setPosition(Vec2(pMinusItem->getBoundingBox().size.width*0.35f,-theCostBG->boundingBox().size.height*0));
        pMinusItem->addChild(theCostBG,10);
    
        //pMinusItem->setPosition(Vec2(0, -m_pCircleLayer->getBoundingBox().size.height*0.47f));
        
        Label* theCostLable = Label::createWithSystemFont("100", "Arial", 30);
        theCostLable->setPosition(theCostBG->getPosition());
        pMinusItem->addChild(theCostLable,11,CHILD_TAG_COST_MONEY);
        
        m_vecButtonUpgrade.push_back(pMinusItem);
    }
    
    
    //初始化卖出按钮
    Sprite * theButtonBG = Sprite::createWithSpriteFrameName("TD_Icon_BG.png");
    Sprite * theSaleIcon = Sprite::createWithSpriteFrameName("TD_saleBuilding.png");
    theSaleIcon->setPosition(Vec2(theButtonBG->boundingBox().size.width*0.5f, theButtonBG->boundingBox().size.height*0.5f));
    theButtonBG->addChild(theSaleIcon,1,CHILD_TAG_BUTTON_ICON);
    
    Sprite * theButtonBGSelect = Sprite::createWithSpriteFrameName("TD_Icon_BG.png");
    Sprite * theSaleIconSelect = Sprite::createWithSpriteFrameName("TD_saleBuilding.png");
    theSaleIconSelect->setPosition(Vec2(theButtonBG->boundingBox().size.width*0.5f, theButtonBG->boundingBox().size.height*0.5f));
    theButtonBGSelect->addChild(theSaleIconSelect,1,CHILD_TAG_BUTTON_ICON);
    theButtonBGSelect->setColor(Color3B::GRAY);
    
    m_pButtonSale = LYMenuItemSprite::create(theButtonBG,theButtonBGSelect,CC_CALLBACK_1(CBuildingControllLayer::onBtnSale, this));
    m_pButtonSale->setScale(1.5f);
    m_pButtonSale->setPosition(Vec2(0, -m_pCircleLayer->getBoundingBox().size.height*0.47f));
    m_pMenu->addChild(m_pButtonSale);

    
    //初始化建筑详情
    m_pDetailBG = Sprite::createWithSpriteFrameName("detail.png");
    m_pDetailBG->setScale(3.3);
    
    addChild(m_pDetailBG);
    m_pDetailBG->setVisible(false);
    
    return true;
}


void CBuildingControllLayer::setTheBuildingInfo(CBaseFighter *target){
    
    //没选中任何建筑的话，则最小化
    if (!target) {
        
        setVisible(false);
        m_pDetailBG->setVisible(false);
        m_iDetailMonsterId = -1;
        
        return;
    }
    
    setVisible(true);
    m_pDetailBG->setVisible(false);
    m_iDetailMonsterId = -1;
    
//    buttonRefresh();
    
    m_pTarget = target;

    //将界面移到相应的塔中心
    CFightScene* temp = CFightData::getInstance()->getTheFightScene();

    //圈的中心应该是塔的中心。而不是脚底下
    Vec3 target3D = Vec3(target->getPositionX(),target->getPositionY()+target->gettheSprite()->getBoundingBox().size.height*0.5f,target->getPositionZ());
    Vec2 targetPos = CommonData::GetSingletonPtr()->getPosOnScreen(temp->getTheCamera(), target3D);
    setPosition(targetPos);
    
    
    //根据塔信息，初始化建筑信息
    fightBuildingInfo* targetInfo = CDataManager::getTheFightBuildingInfoWithId(target->getTheInfo()->monsterId);
    
    
    m_iAllChoices=0;
    for (int i=0; i<m_vecButtonUpgrade.size(); i++) {
        
        LYMenuItemSprite* theButton = m_vecButtonUpgrade.at(i);
        
        if (i>=targetInfo->upgradeMonstersId.size()||targetInfo->upgradeMonstersId.at(i)==0) {
            
            theButton->setVisible(false);
            continue;
        }
        
        m_iAllChoices++;
        
        theButton->setVisible(true);
        
        theButton->setUserData((void*)(targetInfo->upgradeMonstersId.at(i)));
        
        int upGradeId =  targetInfo->upgradeMonstersId.at(i);
        
        fightBuildingInfo* upgradeInfo = CDataManager::getTheFightBuildingInfoWithId(upGradeId);
        
        __String* tempStr = __String::createWithFormat("%s",upgradeInfo->towerIcon.c_str());
        
        Sprite* towerIcon = (Sprite*)theButton->getNormalImage()->getChildByTag(CHILD_TAG_BUTTON_ICON);
        towerIcon->setPosition(Vec2(theButton->getContentSize().width*0.5f,theButton->getContentSize().height*0.5f));
        
        
        towerIcon->setSpriteFrame(tempStr->getCString());
        
        Sprite* towerIcon2 = (Sprite*)theButton->getSelectedImage()->getChildByTag(CHILD_TAG_BUTTON_ICON);
        towerIcon2->setPosition(Vec2(theButton->getContentSize().width*0.5f,theButton->getContentSize().height*0.5f));
        towerIcon2->setSpriteFrame(tempStr->getCString());
        
        
        tempStr->initWithFormat("%d",upgradeInfo->buildCost);
        
        Label* TheCostLabel = (Label*)theButton->getChildByTag(CHILD_TAG_COST_MONEY);//为什么不直接初始化的原因是，貌似label不会跟着移动
        
        Vec2 pos = TheCostLabel->getPosition();
        TheCostLabel->removeFromParent();
        TheCostLabel = Label::createWithSystemFont(tempStr->getCString(), "Arial", 20);
        TheCostLabel->setPosition(pos);
        theButton->addChild(TheCostLabel,12,CHILD_TAG_COST_MONEY);
        
        
        //        TheCostLabel->setString();
        
        Vec2 thePos = TheCostLabel->convertToWorldSpace(Vec2::ZERO);
        
        
        
        //按钮位置变换
        float offsetX=0;
        float startX=0;
        
        float offsetY=0;
        float startY=0;
        
        if (targetInfo->upgradeMonstersId.size()==1) {
            
            startX = m_pCircleLayer->boundingBox().size.width*0;
            startY = m_pCircleLayer->boundingBox().size.height*0.5f;
        }else if (targetInfo->upgradeMonstersId.size()==2){
            
            startX = -m_pCircleLayer->boundingBox().size.width*0.25f;
            offsetX = m_pCircleLayer->boundingBox().size.width*0.5f;
            
            startY = m_pCircleLayer->boundingBox().size.height*0.25f;
        }else if (targetInfo->upgradeMonstersId.size()==3){
            
            startX = -m_pCircleLayer->boundingBox().size.width*0.5f + (i-1)*(-m_pCircleLayer->boundingBox().size.width*0.08f);
            offsetX = m_pCircleLayer->boundingBox().size.width*0.5f;
            
            offsetY = m_pCircleLayer->boundingBox().size.height*0.25f * pow((-1), i+1) + m_pCircleLayer->boundingBox().size.height*0.25f;
        }
        
        
        
        
        float buttonPosX = startX+i*offsetX;
        
        float buttonPosY = startY+i*offsetY;
        
        //        if (buttonPosY>0.5f) {
        //            buttonPosY = 1-buttonPosY;
        //        }
        //
        //        buttonPosY = (buttonPosY)*m_pCircleLayer->getBoundingBox().size.height;
        
        theButton->setPosition(Vec2(buttonPosX, buttonPosY));
        if((targetInfo->upgradeMonstersId.at(i) == 10001) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_TOWER_LEVEL)
           && (guiderManagerLayer::getInstance()->getStepNum() == 7))
        {
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            Vec2 pos = towerIcon->convertToWorldSpaceAR(Vec2::ZERO);
            guiderManagerLayer::getInstance()->m_pos = pos;
            guiderManagerLayer::getInstance()->createLayer(getParent());
        }
        
    }
    
    
    
    //如果只有一个升级项目，则默认为直接显示详细信息
    if (m_iAllChoices==1) {
        //            m_iDetailMonsterId = monsterId;
        
        showDetail((int)targetInfo->upgradeMonstersId.at(0),m_vecButtonUpgrade.at(0));
        
    }
    
    //如果没有升级项目，则默认为直接显示本级的详细信息
    if (m_iAllChoices==0) {
        showDetail(targetInfo->monsterId, NULL);
    }

    
}


void CBuildingControllLayer::onBtnUpgrade(cocos2d::Ref *pSender){
    

    CFightScene* theScene = CFightData::getInstance()->getTheFightScene();
    
    

    
    FighterInfo* monsterInfo=CPersonalData::getInstance()->getBuildingInfoById(m_iDetailMonsterId);
    
    
    
    fightBuildingInfo* buildInfo = CDataManager::getTheFightBuildingInfoWithId(m_iDetailMonsterId);
    
    
    
    if (theScene&&m_pTarget&&monsterInfo&&buildInfo) {
        
        
        m_pTarget->onDied(eDamageType_Upgrade);
        m_pTarget->setVisible(false);
        
        CBaseFighter* temp = theScene->addMonsterWithIdAndGroupAndPosition(monsterInfo, eFightGroup_Freind, Vec2(m_pTarget->getPositionX(), m_pTarget->getPositionZ()),eAttackType_tower);
        
//        CFightData::getInstance()->changeTowerObb(true, temp);
        
        //测试用注释
        CFightData::getInstance()->costStone(buildInfo->buildCost);
        
    }
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_TOWER_LEVEL) && (guiderManagerLayer::getInstance()->getStepNum() == 10))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(getParent());
        CDataManager::saveGuideInfoToDB(GUIDE_TOWER_LEVEL);
    }
    
    setTheBuildingInfo(NULL);
    
}

void CBuildingControllLayer::onBtnSale(cocos2d::Ref *pSender){
    
   
    CFightScene* theScene = CFightData::getInstance()->getTheFightScene();
    
    
    Vec2 thePos =  CFightData::getInstance()->findPathPosByPos(m_pTarget->getFightPosition())->thePoint;
    
    
    CFightData::getInstance()->removePos(thePos);
    
    
    if (theScene&&m_pTarget)
    {
        
        m_pTarget->onDied(eDamageType_Upgrade);
        m_pTarget->setVisible(false);
        
        fightBuildingInfo* theInfo = CDataManager::getTheFightBuildingInfoWithId(m_pTarget->getTheInfo()->monsterId);
        
        theScene->gotTheSton(theInfo->saleGold,m_pTarget->getPosition3D());
        

        CFightData::getInstance()->addStone(theInfo->saleGold);
        
    }
    
    setTheBuildingInfo(NULL);
    
}


void CBuildingControllLayer::TheBtnDetail(cocos2d::Ref *pSender){
    
    Size winsize = Director::getInstance()->getWinSize();
    
    if (!m_pTarget) {
        return;
    }
    
    LYMenuItemSprite* theButton = (LYMenuItemSprite*)pSender;
    long monsterId = (long)(theButton)->getUserData();
//    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_TOWER_LEVEL) && (monsterId != 10001))
//    {
//        return;
//    }
    //钱不够造时，提示
    fightBuildingInfo* buildingInfo = CDataManager::getTheFightBuildingInfoWithId((int)monsterId);
    
    if (buildingInfo->buildCost>CFightData::getInstance()->getStone()) {
        //            theButton->setEnabled(false);
        
        Sprite* temp = CFightManager::getTheFlyWord("您的能量石不足~");
        temp->setPosition(Vec2(winsize.width*0.5f, winsize.height*0.7f));
        CFightData::getInstance()->getTheFightScene()->addChild(temp,11000);
        
        return;
    }
    

    
    if (monsterId==m_iDetailMonsterId) {
        onBtnUpgrade(pSender);
        return;
    }else{
        
        showDetail((int)monsterId,theButton);
        
    }
    
    
    
    
}

void CBuildingControllLayer::showDetail(int monsterId,LYMenuItemSprite* theButton){
    
    Size winsize = Director::getInstance()->getWinSize();
    
    m_iDetailMonsterId = monsterId;
    
    
    fightBuildingInfo* theFightInfo = CDataManager::getTheFightBuildingInfoWithId((int)monsterId);
    monsterInfo* theMonsterInfo = CDataManager::getMonsterInfoByMonsterId((int)monsterId);
    
    skillInfo* theSkillInfo = NULL;
    
    if (theMonsterInfo->allSkill.size()>0)
    {
        theSkillInfo = CDataManager::getTheSkillInfoWithId(theMonsterInfo->allSkill[0]);
    }
    
    
    //根据按钮位置，调整详细框的位置
    
    Vec2 buttonPos = Vec2::ZERO;
    
    float buttonWidth = 0;
    
    float buttonHeight = 0;
    
    Vec2 buttonOriginPos = Vec2::ZERO;
    
    if (theButton) {
        
        buttonPos = theButton->convertToWorldSpace(Point::ZERO);
        
        buttonOriginPos = theButton->getPosition();
        
        buttonWidth = theButton->getBoundingBox().size.width;
        
        buttonHeight = theButton->getBoundingBox().size.height;
    }
    
    
    bool ifLeft = buttonPos.x>(winsize.width*0.5f)? true: false;
    bool ifUp = buttonPos.y>(winsize.height*0.5f)? false: true;
    
    float offsetX =  buttonWidth*0.5f + m_pDetailBG->getBoundingBox().size.width*0.5f;
    float offsetY =  buttonHeight*0.5f + m_pDetailBG->getBoundingBox().size.height*0.5f;
    
    offsetX = ifLeft ? -offsetX: offsetX;
    offsetY = ifUp ? offsetY:-offsetY;
    
    
    
    m_pDetailBG->setVisible(true);
    m_pDetailBG->setPosition(buttonOriginPos+Vec2(offsetX, offsetY));
    
    Size BGSize = m_pDetailBG->getBoundingBox().size;
    
    m_pDetailBG->removeAllChildren();
    
    __String *tempStr = __String::createWithFormat("");
    
    
    Label* TheTitle = Label::createWithSystemFont(theMonsterInfo->monsterName, "Arial", 30);
    TheTitle->setAnchorPoint(Vec2(0, 0.5f));
    TheTitle->setScale(0.3);
    TheTitle->setAlignment(TextHAlignment::CENTER);
    TheTitle->setPosition(Vec2(BGSize.width*0.005f, BGSize.height*0.275f));
    m_pDetailBG->addChild(TheTitle);
    
    
    Label* TheDesc = Label::createWithSystemFont(theMonsterInfo->monsterIntroduce, "Arial", 25,Size(BGSize.width,0));
    TheDesc->setAnchorPoint(Vec2(0, 0.5f));
    //TheDesc->setContentSize(Size(BGSize.width*0.5,0));
    TheDesc->setScale(0.3);
    TheDesc->setAlignment(TextHAlignment::LEFT);
    TheDesc->setPosition(Vec2(0, -TheTitle->boundingBox().size.height*0.5f-TheDesc->boundingBox().size.height*0.5f)+TheTitle->getPosition());
    TheDesc->setColor(Color3B(243, 197, 17));
    m_pDetailBG->addChild(TheDesc);
    
    
    Label* info1 = Label::createWithSystemFont("伤害: ", "Arial", 25);
    info1->setAnchorPoint(Vec2(0, 0.5f));
    info1->setScale(0.3);
    info1->setAlignment(TextHAlignment::CENTER);
    info1->setColor(Color3B(243, 197, 17));
    info1->setPosition(Vec2(0,-TheDesc->boundingBox().size.height*0.5f-info1->getBoundingBox().size.height*1)+TheDesc->getPosition());
    m_pDetailBG->addChild(info1);
    
    
    tempStr->initWithFormat("%d",theMonsterInfo->minAtk);
    Label* damageLable = Label::createWithSystemFont(tempStr->getCString(), "Arial", 25);
    damageLable->setAnchorPoint(Vec2(0, 0.5f));
    damageLable->setScale(0.3);
    damageLable->setAlignment(TextHAlignment::CENTER);
    damageLable->setPosition(Vec2(info1->getBoundingBox().size.width,0)+info1->getPosition());
    m_pDetailBG->addChild(damageLable);
    
    
    Label* info2 = Label::createWithSystemFont("攻击数量: ", "Arial", 25);
    info2->setScale(0.3);
    info2->setColor(Color3B(243, 197, 17));
    info2->setAnchorPoint(Vec2(0, 0.5f));
    info2->setAlignment(TextHAlignment::CENTER);
    info2->setPosition(Vec2(0,-info1->boundingBox().size.height*0.5f-info2->getBoundingBox().size.height*0.5f)+info1->getPosition());
    m_pDetailBG->addChild(info2);
    
    
    tempStr->initWithFormat("%d",1);//此处暂时写死为1
    Label* numberLable = Label::createWithSystemFont(tempStr->getCString(), "Arial", 25);
    numberLable->setScale(0.3);
    numberLable->setAnchorPoint(Vec2(0, 0.5f));
    numberLable->setAlignment(TextHAlignment::CENTER);
    numberLable->setPosition(Vec2(info2->getBoundingBox().size.width,0)+info2->getPosition());
    m_pDetailBG->addChild(numberLable);
    
    
    Label* info3 = Label::createWithSystemFont("攻击速度: ", "Arial", 25);
    info3->setScale(0.3);
    info3->setColor(Color3B(243, 197, 17));
    info3->setAnchorPoint(Vec2(0, 0.5f));
    info3->setAlignment(TextHAlignment::CENTER);
    info3->setPosition(Vec2(0,-info2->boundingBox().size.height*0.5f-info3->getBoundingBox().size.height*0.5f)+info2->getPosition());
    m_pDetailBG->addChild(info3);
    
    
    tempStr->initWithFormat("%0.1fs",theMonsterInfo->attackCD);
    Label* cdLable = Label::createWithSystemFont(tempStr->getCString(), "Arial", 25);
    cdLable->setScale(0.3);
    cdLable->setAnchorPoint(Vec2(0, 0.5f));
    cdLable->setAlignment(TextHAlignment::CENTER);
    cdLable->setPosition(Vec2(info3->getBoundingBox().size.width,0)+info3->getPosition());
    m_pDetailBG->addChild(cdLable);
    
    
    
    Label* info4 = Label::createWithSystemFont("伤害类型: ", "Arial", 25);
    info4->setScale(0.3);
    info4->setColor(Color3B(243, 197, 17));
    info4->setAnchorPoint(Vec2(0, 0.5f));
    info4->setAlignment(TextHAlignment::CENTER);
    info4->setPosition(Vec2(0,-info3->boundingBox().size.height*0.5f-info4->getBoundingBox().size.height*0.5f)+info3->getPosition());
    m_pDetailBG->addChild(info4);
    
    
    if (theSkillInfo&&theSkillInfo->hurtNumbers>1) {
        tempStr->initWithFormat("范围伤害");
    }else{
        tempStr->initWithFormat("单体伤害");
    }
    Label* typeLable = Label::createWithSystemFont(tempStr->getCString(), "Arial", 25);
    typeLable->setScale(0.3);
    typeLable->setAnchorPoint(Vec2(0, 0.5f));
    typeLable->setAlignment(TextHAlignment::CENTER);
    typeLable->setPosition(Vec2(info4->getBoundingBox().size.width,0)+info4->getPosition());
    m_pDetailBG->addChild(typeLable);
    
    buttonRefresh();
    if((monsterId == 10001) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_TOWER_LEVEL)
       && (guiderManagerLayer::getInstance()->getStepNum() == 9))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        Vec2 pos = guiderManagerLayer::getInstance()->m_pos;
        guiderManagerLayer::getInstance()->createLayer(getParent(), GUIDE_SHOW_FINGER, pos);
    }

    
    
    
    
    
}



void CBuildingControllLayer::buttonRefresh(){
    
    
    
    for (int i=0; i<m_vecButtonUpgrade.size(); i++) {
        
        LYMenuItemSprite* theButton = m_vecButtonUpgrade.at(i);
        
        
        if (theButton->getUserData()==nullptr) {
            continue;
        }
        
        long monsterId = (long)theButton->getUserData();
        
        
        
        
        
        
        theButton->removeChildByTag(CHILD_TAG_SELECT);
        
        
        Sprite* towerIcon = (Sprite*)theButton->getNormalImage()->getChildByTag(CHILD_TAG_BUTTON_ICON);
//        theButton->setEnabled(true);
        towerIcon->setColor(Color3B::WHITE);
        
        Sprite* towerIcon2 = (Sprite*)theButton->getSelectedImage()->getChildByTag(CHILD_TAG_BUTTON_ICON);
        
        
        Sprite* upIcon = NULL;
        
        
        Label* TheCostLabel = (Label*)theButton->getChildByTag(CHILD_TAG_COST_MONEY);
        
        
        
        if (monsterId == m_iDetailMonsterId) {
            
            
            towerIcon->setVisible(false);
            towerIcon2->setVisible(false);
            
            upIcon = Sprite::createWithSpriteFrameName("TD_Select.png");
            upIcon->setPosition(Vec2(theButton->getContentSize().width*0.5f,theButton->getContentSize().height*0.5f));
            theButton->addChild(upIcon,1,CHILD_TAG_SELECT);
        }else{
            towerIcon->setVisible(true);
            towerIcon2->setVisible(true);
        }
        
        //钱不够造时，按钮灰掉
        fightBuildingInfo* buildingInfo = CDataManager::getTheFightBuildingInfoWithId((int)monsterId);
        
        if (buildingInfo->buildCost>CFightData::getInstance()->getStone()) {
//            theButton->setEnabled(false);
            towerIcon->setColor(Color3B::GRAY);
            
            if (upIcon) {
                upIcon->setColor(Color3B::GRAY);
            }
            
            TheCostLabel->setColor(Color3B::RED);
        }else{
            TheCostLabel->setColor(Color3B::WHITE);
        }
        
        
        
    }
    

}









//视椎体裁剪
//Effect3DOutline
//createPerspective
//AmbientLight    环境光
//_directionalLight