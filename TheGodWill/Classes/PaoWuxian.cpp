#include "PaoWuxian.h"
#include "PublicDefine.h"
#include "fightScene.h"
#include "Common.h"
#include  "LYBaseAttack.h"
#include "LYFightData.h"
#include "SmallMap.h"
#include "DataManager.h"
#include "guiderManagerLayer.h"
#include "LYBaseHero.h"
#include "LYFightScenePVP.h"

#include "DBMgr.h"


USING_NS_CC;
#define  Len -120

/*
 1.小地图最右边没对齐
 3.鼠标中间
 4.投石车往外抖
 */

Scene* CPaoWuxian::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    //CPaoWuxian *layer = CPaoWuxian::create();

    // add layer as a child to scene
    //scene->addChild(layer);
    
    // return the scene
    return scene;
}

CPaoWuxian::CPaoWuxian(Vec3 VCurPos)
{
    
    m_startBall=NULL;
    
    m_theBall = NULL;
    
    m_pFightScene = NULL;
    
    m_VCurPos = VCurPos;
    m_WinSize = Director::getInstance()->getWinSize();
    m_pMiaozhunJing = NULL;
    m_pMiaozhunJing2 = NULL;
    m_pTarget  = NULL;
    m_pHandler = NULL;
    m_bPaoActive =false;
    m_pToushicheSmallMap = NULL;
    m_iShakeTime = 0;
    m_theBall = NULL;
    m_bShake = false;
    m_iShakeRange = 20;
    m_iShakeSec = 0;
    m_iPastY = 0;
    
    m_pListener = NULL;
    
    m_iLineUpdateIndex=0;
    
    m_stSkillInfo = CDataManager::CalcFightSkillInfoWithIdAndLevel(102,1);//暂时定为1级
    
    m_iTheDamage=0;
    
    m_pTheDragBall = NULL;
    
}

CPaoWuxian::~CPaoWuxian()
{
    
    CCLOG("aaa");
}


void CPaoWuxian::OnTouchMoveBall(Ref  *pSender)
{
    log("ontouch MoveBall\n");
}


void CPaoWuxian::OnTouchStarBall(Ref *pSender)
{
    log("ontouch star Ball\n");
}


void CPaoWuxian::StartToushiche(cocos2d::Ref * pTarget,
                                cocos2d::SEL_MenuHandler pHandler,CFightScene *pScene)
{
//    m_pTarget = pTarget;
//    m_pHandler= pHandler;
//    m_pFightScene = pScene;
//
//    bool ifInitTouch = true;
//    
//    if (m_pFightScene&&m_pFightScene->gettheFightSceneType()!=eFightSceneType_PVP) { //pvp模式不需要监听
//        
//        //如果是录像模式，则记录
//        CFightScenePVP* target = ((CFightScenePVP*)m_pFightScene);
//        
//        if (target->getthePVPMode()==EPVPMode_REC) {
//            ifInitTouch=false;
//        }
//    }
//    
//    if(ifInitTouch){
//        m_pListener = EventListenerTouchOneByOne::create();//创建一个触摸监听
//        m_pListener->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false
//        m_pListener->onTouchBegan = CC_CALLBACK_2(CPaoWuxian::onTouchBegan, this);
//        m_pListener->onTouchMoved = CC_CALLBACK_2(CPaoWuxian::onTouchMoved, this);
//        m_pListener->onTouchEnded = CC_CALLBACK_2(CPaoWuxian::onTouchEnded, this);
//        _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener, this);
//    }
    
  }

void CPaoWuxian::EndTouuShiche()
{

}


// on "init" you need to initialize your instance
bool CPaoWuxian::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    
    //是否录像模式判断
    m_bIfRecMode = false;
    if (m_pFightScene&&m_pFightScene->gettheFightSceneType()==eFightSceneType_PVP) {
        
        //如果是录像模式，则记录
        CFightScenePVP* target = ((CFightScenePVP*)m_pFightScene);
        
        if (target->getthePVPMode()==EPVPMode_REC) {
            m_bIfRecMode=true;
        }
    }
    
    
    //pvp模式不需要监听
    if(!m_bIfRecMode){
        m_pListener = EventListenerTouchOneByOne::create();//创建一个触摸监听
        m_pListener->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false
        m_pListener->onTouchBegan = CC_CALLBACK_2(CPaoWuxian::onTouchBegan, this);
        m_pListener->onTouchMoved = CC_CALLBACK_2(CPaoWuxian::onTouchMoved, this);
        m_pListener->onTouchEnded = CC_CALLBACK_2(CPaoWuxian::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithFixedPriority(m_pListener,m_pFightScene->getZOrder()+1);
    }
    
    
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    SpeedMax = 300;//每秒最大速度100
    pullMax = 50;//最大拉扯距离
    
    addSpeed = -300;//重力加速度 像素每秒
    
    //wangzi.c3b
    //models/toushiche/paodan.c3b
    //paodan.c3b
    
    //bulletInfoDB* pInfo = CDataManager::getBulletInfoByBulletId(3);
    //萝卜
    
    //初始化炮弹
    m_theBall = Sprite3D::create();
    
    Sprite3D *pNode =  Sprite3D::create("models/toushiche/paodan.c3b");
    
    //测试用
//    Sprite3D *pNode = CFightManager::getTheModelWithPath("bulletModels/ball/mofad.png");
//    ParticleSystem *particle = ParticleSystemQuad::create("particleSystem/poisonBall.plist");
    
    pNode->setScale(0.5);
    pNode->runAction(RepeatForever::create(RotateBy::create(0.5f, Vec3(0, 0, 90))));
    pNode->setPosition3D(Vec3(0, 0, 0));
    m_theBall->addChild(pNode);
    
    
    ParticleSystem *particle = ParticleSystemQuad::create("particleSystem/fireAttack.plist");
    CFightManager::addCamera(particle);
    particle->setScale(1);
    particle->setPosition3D(Vec3(0, 0, pNode->getBoundingBox().size.height*0.5f));
    m_theBall->addChild(particle,10);
//    particle->setGlobalZOrder(1001);
    
    
    m_theBall->setCameraMask((unsigned short)CameraFlag::USER1);
    

    m_BallStartVec3 = Vec3(Start3DPOINT-600*0.10f,150,-450);
    m_CarOffsetVec3 = Vec3(600*0.1+20,-80,0);
    m_startPosVec3 = m_BallStartVec3;
    addChild(m_theBall,11);
    m_theBall->setPosition3D(m_BallStartVec3);
    
    m_theBall->setVisible(false);
    
//    CBaseAttack *pAttkLayer = CBaseAttack::create();
//    pAttkLayer->setBulletSprite(pNode);
//    m_theBall = pAttkLayer->getBulletSprite();
//    m_theBall->setScale(1);
//    m_theBall->setCameraMask((unsigned short)CameraFlag::USER1);
    
    
    
    
    
//    m_theBall->runAction(RepeatForever::create(RotateBy::create(0.5f, Vec3(0, 0, 90))));
    
    

    
    //wangzi.c3b
    //models/toushiche/TSC.c3b
    //toushijiquan.c3b
    //paodan.c3b
    
    modelInfo* pModelInfo = CDataManager::getModelInfoByModelId(1016);
    m_startBall = Sprite3D::create(pModelInfo->modelName.getCString());
    m_startBall->setVisible(false);
    m_startBall->setRotation3D(Vec3(0,90,0));
    m_startBall->setScale(0.1);
  
    
    Sprite3D *pLeft3D  = CFightManager::getTheModelWithPath("pvpArrow.png");

    pLeft3D->setRotation3D(Vec3(90, 0, 0));

    pLeft3D->setPosition3D(Vec3(1000,100,0));
    
    pLeft3D ->setScale(10);

    m_startBall->addChild(pLeft3D,0,201);
    m_startBall->setCameraMask((unsigned short )CameraFlag::USER1);

    Sprite3D *pRight3D  = CFightManager::getTheModelWithPath("pvpArrow.png");
    
    pRight3D->setRotation3D(Vec3(90,0,180));
    
    pRight3D->setPosition3D(Vec3(-1000,100,0));
    
    pRight3D->setScale(10);
    
    m_startBall->addChild(pRight3D,0,202);
    m_startBall->setCameraMask((unsigned short )CameraFlag::USER1);
    
    Vec3 tVec3 = m_BallStartVec3+m_CarOffsetVec3;
    
    tVec3.y  = 0;
    
    m_startBall->setPosition3D(tVec3);
    
    addChild(m_startBall,10);
    
    setGlobalZOrder(10);
    
    
    m_pTheDragBall = Sprite::createWithSpriteFrameName("toushiMove.png");
    
    m_pTheDragBall->setPosition(m_pFightScene->getTheToushiActive()->getPosition());
    
    m_pTheDragBall->setVisible(false);
    
    addChild(m_pTheDragBall);
    

    
    return true;
}



void CPaoWuxian::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

bool CPaoWuxian::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_USE_MANGONEL) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
    {
        return false;
    }
    m_vecTouchStartPoint = pTouch->getLocation();
    
    unscheduleUpdate();
//    Vec2  startPointVec2 = CCDirector::getInstance()->convertToGL(pTouch->getLocationInView());
//    startPoint = Vec3(startPointVec2.x,startPointVec2.y,Len);
    
    startPoint = Vec3(m_pFightScene->getTheToushiActive()->getPositionX(), m_pFightScene->getTheToushiActive()->getPositionY(), Len);
    
    m_pFightScene->m_bMoveToushiche = false;
    
    
    //小地图
    if(m_VCurPos.z<=-1)
    {
        startPoint.x =  startPoint.x*BIGMAPLEN/m_WinSize.width;
    }
    
    modelInfo* pModelInfo = CDataManager::getModelInfoByModelId(1016);
    auto animation = Animation3D::create(pModelInfo->modelName.getCString());
    auto animate = Animate3D::create(animation, pModelInfo->idleFrame.start/60, pModelInfo->idleFrame.duration/60);
    
    
    m_startBall->stopAllActions();
    m_startBall->runAction(animate);
    
    m_startBall->runAction(Sequence::create(DelayTime::create(animate->getDuration()),CallFunc::create([=](){
       
    m_theBall->setPosition3D(m_startBall->getPosition3D()-m_CarOffsetVec3);
    m_theBall->setVisible(true);
    }) , NULL));
    
    
    m_bPaoActive = false;
    
    schedule(schedule_selector(CPaoWuxian::lineUpdate), 0.1f, -1, 0);
    
    m_pTheDragBall->setVisible(true);

    return true;
}

void CPaoWuxian::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    
    Vec3 pos = m_theBall->getPosition3D();
    Point tempPoint = CCDirector::getInstance()->convertToGL(pTouch->getLocationInView());
    
    m_bPaoActive = true;

    endedPoint =  Vec3(tempPoint.x,tempPoint.y,Len);
    

   //float fx = tempPoint.x/BIGMAPLEN*m_WinSize.width;
    
    //小地图
    if(m_VCurPos.z<=-1)
    {
        endedPoint.x = endedPoint.x*BIGMAPLEN/m_WinSize.width;
    }
    
    refreshPath();
    if(CPersonalData::getInstance()->getCatapult() == false)
    {
        CPersonalData::getInstance()->setCatapult(true);
        NotificationCenter::getInstance()->postNotification("removeGuide");
    }
}

void CPaoWuxian::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    

    Vec2 touchEnd = pTouch->getLocation();

   
    

    
    if (m_vecTouchStartPoint.distance(touchEnd)<20) { //移动距离太小，则不做任何操作
        return;
    }
    if(!((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_USE_MANGONEL) && (guiderManagerLayer::getInstance()->getStepNum() == 5)))
    {
        m_pFightScene->StartToushiCD();
    }
    
    Sprite3D * pLeft =  (Sprite3D*)m_theBall->getChildByTag(201);
    
    Sprite3D * pRight = (Sprite3D*)m_theBall->getChildByTag(202);
    
    if(pLeft)
    {
        pLeft->setVisible(false);
    }
    
    if(pRight)
    {
        pRight->setVisible(false);
    }
    

    
    
    if(m_bPaoActive)
    {
        CFightScene  *pScene =  CFightData::getInstance()->getTheFightScene();
        
        if(pScene->getTheToushiActive())
        {
            pScene->getTheToushiActive()->setVisible(false);
        }
        
        StartAttack();
    
    }

    m_pTheDragBall->setVisible(false);
    
   
    
    //m_theBall->setVisible(false);
    
    
    unschedule(schedule_selector(CPaoWuxian::lineUpdate));
    
}

void CPaoWuxian::StartAttack(){
    
    flyTime = 0;
    m_iShakeTime=0;
    
    //m_theBall->setVisible(true);
    m_theBall->setPosition3D(m_BallStartVec3);
    
    m_TmpCameraVec3 = m_pFightScene->getCameraPos();

    
        
    
    scheduleUpdate();
    
    modelInfo* pModelInfo = CDataManager::getModelInfoByModelId(1016);
    auto animation = Animation3D::create(pModelInfo->modelName.getCString());
    auto animate = Animate3D::create(animation, pModelInfo->walkFrame.start/60, pModelInfo->walkFrame.duration/60);
    
    //投石速度降低，从而配合投石的速度
    float scale = targetSpeedX/900;
    animate->setSpeed(scale);
    
    m_startBall->stopAllActions();
    m_startBall->runAction(animate);

    
    //pvp记录投石数据
    if (!m_bIfRecMode&&CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
    {
        CFightScenePVP* target = ((CFightScenePVP*)m_pFightScene);
        
        CBaseFighter* hero = CFightData::getInstance()->getTheFriendHero();
        
        //获取投石机能造成的伤害值
        m_iTheDamage = CDBMgr::getInstance()->getAffectNum(CDBMgr::eAffectType::eEnergyType, 4);
        
        
        target->addRecordStringWithCatapult(m_BallStartVec3, Vec2(targetSpeedX, targetSpeedY),m_iTheDamage);
    }
    
//    if (m_pFightScene&&m_pFightScene->gettheFightSceneType()==eFightSceneType_PVP) {
//        
//        //如果是录像模式，则记录
//        CFightScenePVP* target = ((CFightScenePVP*)m_pFightScene);
//        
//        if (target->getthePVPMode()==EPVPMode_PLAY) {
//            target->addRecordStringWithCatapult(m_BallStartVec3, Vec2(targetSpeedX, targetSpeedY));
//        }
//        
//
//    }
    
}




void CPaoWuxian::StartAttackWith(cocos2d::Vec3 ballPos, cocos2d::Vec2 speed,int damage){
    
    
    m_BallStartVec3 = ballPos;
    
    targetSpeedX = speed.x;
    targetSpeedY = speed.y;
    
    m_iTheDamage = damage;

    StartAttack();
}






void CPaoWuxian::lineUpdate(float time){
    
    if (m_pathSpr.size()==0) {
        return;
    }
    
    
    m_iLineUpdateIndex++;
    
    
    if (m_iLineUpdateIndex>=m_pathSpr.size()) {
        
        m_iLineUpdateIndex=0;
    }
    
    
    int offset = 1;
    if (m_iLineUpdateIndex%2==0) { //判断是奇数亮还是偶数亮
        offset=0;
    }
    
    
    for (int i=0; i<m_pathSpr.size(); i++) {
        
        if (i%2==offset) {
            m_pathSpr.at(i)->setOpacity(200);
        }else{
            m_pathSpr.at(i)->setOpacity(125);
        }
        
        
    }
    
}



void CPaoWuxian::onExit()
{
    _eventDispatcher->removeEventListener(m_pListener);
    Layer::onExit();
}

void CPaoWuxian::RemoveTouch()
{
    
    
}

void CPaoWuxian::onTouchCancelled(Touch *touch, Event *pEvent)
{

}


void CPaoWuxian::refreshPath(){
    
    //先清除所有路径
    
    m_pTheDragBall->setVisible(true);
    
    for (int i=0; i<m_pathSpr.size(); i++)
    {
        m_pathSpr[i]->removeFromParentAndCleanup(true);
    }
    
    m_pathSpr.clear();
    

    //m_pToushiActive
    
    
   
    
    
    Vec3  BallStartVec3 =  m_BallStartVec3; //m_startBall->getPosition3D();

    //固定起始点的y坐标，只能拖动x坐标
    endedPoint.y = 425;

    //抛出点的坐标限制
    
    float dragDistance = 100;  //拖动最大的距离，此距离可以最终打到基地
    
    float sensity = 0.5f;//灵敏度
    
    endedPoint.x = MIN(endedPoint.x, m_pFightScene->getTheToushiActive()->getPositionX());
    endedPoint.x = MAX(endedPoint.x, m_pFightScene->getTheToushiActive()->getPositionX()-dragDistance/sensity);
    
    m_pTheDragBall->setPositionX(endedPoint.x);

    
    //真实效果修正
    endedPoint.x = m_pFightScene->getTheToushiActive()->getPositionX() - (m_pFightScene->getTheToushiActive()->getPositionX()-endedPoint.x)*sensity;
    
    
    
    
    float pullStrength = startPoint.distance(endedPoint);
    
    
    CCLOG("拖动的结束点为%f",endedPoint.x);
    
//    CCLOG("pullStrength:%f",pullStrength);
    
    float cos = (endedPoint.y-startPoint.y)/pullStrength;
    
    float sin = (endedPoint.x-startPoint.x)/pullStrength;
    
    float speedX = -pullStrength/pullMax*SpeedMax *sin;
    float speedY = -pullStrength/pullMax*SpeedMax *cos;
    
//    CCLOG("speedX:%f",speedX);
//    CCLOG("speedY:%f",speedY);
    
    targetSpeedX = speedX;
    targetSpeedY = speedY;

    
    
    float pathOffset = 30;//每个路径节点的x间隔
    
    float timeOffset = pathOffset/speedX;//每个路径节点的间隔时间
    
//    CCLOG("timeOffset:%f",timeOffset);

    float allOffsetX = BallStartVec3.x;
    float allOffsetY = BallStartVec3.y;
    float allTime = 0;
    
    float toushX=0;
    float toushY=999;
    float preOffsetY=allOffsetY;
    
    while (allOffsetX<=END3DPOINT+ONESIDE)//判断显示曲线的区域
    {
        
        
        float tempX = speedX*allTime;
        float tempY = speedY*allTime + 0.5* addSpeed * pow(allTime,2);
        
        allOffsetX = BallStartVec3 .x+tempX;
        allOffsetY = BallStartVec3 .y+tempY;
        
        
        //显示曲线的范围
        if (allOffsetX>=BallStartVec3.x&&allOffsetX<=(BallStartVec3.x+1000)&&allOffsetY>=0&&allOffsetY<=500) {
            
            auto temptarget = Sprite3D::create("models/toushiche/paodan.c3b");
            temptarget->setCameraMask((unsigned short )CameraFlag::USER1);
            temptarget->setScale(0.2f);
//            temptarget->setColor( Color3B::GREEN);
//            temptarget->setOpacity(180);
            addChild(temptarget,1001);
            temptarget->setPosition3D(Vec3(BallStartVec3.x+tempX,BallStartVec3.y+tempY,BallStartVec3.z));
            
            if(BallStartVec3.y+tempY<-10)
            {
                temptarget->setVisible(false);
            }
            
            m_pathSpr.push_back(temptarget);
        }
        
        
        //记录最靠近y=0的坐标点，用于放置瞄准镜(并且之前的y值必须比现在的要大，这才证明是下降)
        if (abs(allOffsetY)<=toushY&&preOffsetY>allOffsetY) {
            toushY = abs(allOffsetY);
            toushX=allOffsetX;
        }
        
        preOffsetY = allOffsetY;
        
//        CCLOG("tempX:%f",tempX);
//        CCLOG("tempY:%f",tempY);
        
//        auto temptarget = BillBoard::create("CloseNormal.png");
        
        
//        temptarget->setMode(BillBoard::Mode::VIEW_PLANE_ORIENTED);
        
//        temptarget->setGlobalZOrder(1000);
        
        //temptarget->setVisible(true);
        
        
//        CFightData::getInstance()->getTheFightScene()->getSpriteBG()->addChild(temptarget,1000);
//        m_pFightScene->gettheLayer3D()->addChild(temptarget,1000);
//        CFightData::getInstance()->getTheFightScene()->addChild(temptarget,10000);
        

        allTime+=timeOffset;
        
        
    }
    
    
    //投石机落点位置修正
    if(m_pMiaozhunJing)
    {
        m_pMiaozhunJing->removeFromParent();
    }
    if(m_pMiaozhunJing2)
    {
        m_pMiaozhunJing2->removeFromParent();
    }
    
   
    
    //正向旋转一个圈
//    m_pMiaozhunJing = Sprite3D::create("models/toushiche/toushijiquan.c3b");
    m_pMiaozhunJing = Sprite3D::create();
    
    Sprite* temp = Sprite::createWithSpriteFrameName("fight_aim.png");
    m_pMiaozhunJing->addChild(temp);
    
    m_pMiaozhunJing->setScale(2.5f);
    m_pMiaozhunJing->setCameraMask((unsigned short )CameraFlag::USER1);
    m_pMiaozhunJing->setPosition3D(Vec3(toushX,2,BallStartVec3.z));
    m_pMiaozhunJing->setRotation3D(Vec3(90,0,0));
    temp->setOpacity(170);
    addChild(m_pMiaozhunJing,10,100);
    m_pMiaozhunJing->runAction(RepeatForever::create(RotateBy::create(2, Vec3(0, 135, 0))));
    
    //反向旋转一个圈
    m_pMiaozhunJing2 = Sprite3D::create();
    
    Sprite* temp2 = Sprite::createWithSpriteFrameName("fight_aim2.png");
    m_pMiaozhunJing2->addChild(temp2);
    
    m_pMiaozhunJing2->setScale(2.5f);
    m_pMiaozhunJing2->setCameraMask((unsigned short )CameraFlag::USER1);
    m_pMiaozhunJing2->setPosition3D(Vec3(toushX,2,BallStartVec3.z));
    m_pMiaozhunJing2->setRotation3D(Vec3(90,0,0));
    temp2->setOpacity(170);
    addChild(m_pMiaozhunJing2,10,100);
    m_pMiaozhunJing2->runAction(RepeatForever::create(RotateBy::create(2, Vec3(0, -90, 0))));
//    Sprite3D* fightAim = Sprite3D::create();
//    Sprite* temp2 = Sprite::create("fight/fight_aim.png");
//    fightAim->addChild(temp2);
//    fightAim->setRotation3D(Vec3(90,0,0));
//    m_pMiaozhunJing->addChild(fightAim);
//    fightAim->runAction(RepeatForever::create(RotateBy::create(2, Vec3(0, -90, 0))));
    
    
    //小地图显示落点
    if(m_pToushicheSmallMap)
    {
        m_pToushicheSmallMap->removeFromParent();
    }
    
    m_pToushicheSmallMap  = CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(Vec3(toushX,toushY,BallStartVec3.z),eFighterType_toushicheluodian);

    
}

void CPaoWuxian::update(float time)
{
    if(m_bShake)
    {
        if (m_iShakeTime==0) {
            m_iShakeTimes=0;
            m_iOffsetY=3;
            m_iShakeRange=5;
            m_iPastY = m_pFightScene->getCameraPos().y;
        }
        
        m_iShakeTime++;
        
        log("ShakeTime = %d\n",m_iShakeTime);

        
        
        int nowY = m_pFightScene->getCameraPos().y;
        
        int newY = m_iOffsetY*time*60+nowY;
        
        
        
//        m_pFightScene->_camera->setPositionY(newY);
        m_pFightScene->moveCameraPos(m_pFightScene->getCameraPos().x,newY);
        
        int iy = newY - m_iPastY;
        
        if (abs(iy)>=m_iShakeRange) {
            m_iOffsetY=-m_iOffsetY;
            m_iShakeTimes++;
//            m_iShakeRange-=2;
//            m_iOffsetY = m_iOffsetY*2/3;
        }
        
        
        
        if (m_iShakeTimes>5) {
            unscheduleUpdate();
            m_iShakeTime  = 0;
    //        m_iShakeRange = 5;
            m_bShake = false;
            if(!((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_USE_MANGONEL) && (guiderManagerLayer::getInstance()->getStepNum() == 5)))
            {
               m_pFightScene->StartToushiCD();
            }
//            m_pFightScene->_camera->setPositionY(m_TmpCameraVec3.y);
      
            m_pFightScene->moveCameraPos(m_pFightScene->getCameraPos().x,m_TmpCameraVec3.y);
            m_pFightScene->m_bToushiActive = false;
        }
    

        
        return;
    }
    
    Size winsize = Director::getInstance()->getWinSize();
    
    flyTime += time;
    
    Vec3 BallStartVec3 = m_BallStartVec3;
    
    float tempX = targetSpeedX*flyTime+ BallStartVec3.x;
    float tempY = targetSpeedY*flyTime + 0.5* addSpeed * pow(flyTime, 2)+ BallStartVec3.y;
    
//    CCLOG("theballPosX:%f,theballPosY:%f",tempX,tempY);
    
    float nextTempY = targetSpeedY*(flyTime+time) + 0.5* addSpeed * pow((flyTime+time), 2)+ BallStartVec3.y;
    
    {
        {
            m_theBall->setPosition3D(Vec3(tempX, tempY,m_theBall->getPositionZ()));
            
            //Vec2  tx = Director::getInstance()->convertToUI(Vec2(tempX,tempY));
            //跟随摄像机变化
            float  fTheBallx = m_theBall->getPositionX();
            
            if(fTheBallx<END3DPOINT&&tempY>0&&fTheBallx>Start3DPOINT&&!m_bIfRecMode)//只有非录像模式时，才会强制镜头跟随
            {
                
                
            
                m_pFightScene->CameraRunWithSmallMap(fTheBallx);
//                m_pFightScene->_camera->setPositionX(fTheBallx);
                m_pFightScene->moveCameraPos(fTheBallx);
//                log("fTheBallx= %f",fTheBallx);
            }
        }
        
        if(tempY<=0||nextTempY<=0)  //ly 防止速度太快导致的y轴测试直接降到太深
        {
            if( m_pMiaozhunJing)
            {
                m_pMiaozhunJing->setVisible(false);
            }
            if( m_pMiaozhunJing2)
            {
                m_pMiaozhunJing2->setVisible(false);
            }
            
            
            if(m_pToushicheSmallMap)
            {
                m_pToushicheSmallMap->removeFromParent();
            }
            
            
            if(m_iShakeTime == 0)
            {
                m_bShake = true;
                
                unscheduleUpdate();
                scheduleUpdate();
                m_theBall->setVisible(false);
                onDamage();
                return ;
            }
            

            //砸到怪物
            log("tempX = %f tempY= %f\n",tempX,tempY);
            
            
            unscheduleUpdate();
        }
        else
        {
            m_theBall->setVisible(true);
        }
        
        if(m_pToushicheSmallMap)
        {
            m_pToushicheSmallMap->setVisible(true);
        }
        
        if(tempX>endedPoint.x ||tempY> endedPoint.y)
        {
            for (int i=0; i<m_pathSpr.size(); i++)
            {
                m_pathSpr[i]->removeFromParentAndCleanup(true);
            }
            m_pathSpr.clear();
        }
    }
}


void CPaoWuxian::UpdateShake(float ft)
{
}


void CPaoWuxian::CameraRunWithSmallMap(float x)
{
    float fAddx =  x -  m_pFightScene->getCameraPos().x;

    float fSmallx = fAddx/BIGMAPLEN*m_WinSize.width;
    
    float fx  = m_pFightScene->m_pCurView->getPositionX();
    
    if((fx+fSmallx)>m_WinSize.width- m_pFightScene->m_pCurView->boundingBox().size.width*0.5)
    {
        m_pFightScene->m_pCurView->setPositionX(m_WinSize.width- m_pFightScene->m_pCurView->boundingBox().size.width*0.5);
    }
    else if((fx+fSmallx)<m_pFightScene->m_pCurView->boundingBox().size.width*0.5)
    {
        m_pFightScene->m_pCurView->setPositionX(m_pFightScene->m_pCurView->boundingBox().size.width*0.5);
    }
    else
    {
        m_pFightScene->m_pCurView->setPositionX(fx+fSmallx);
    }
}

void CPaoWuxian::onDamage(){
    
    
    CBaseAttack* theAttack = CBaseAttack::create();
    theAttack->setAttackerGroup(eFightGroup_Freind);
    
    theAttack->setCameraMask((unsigned short )CameraFlag::USER1);
    
    
    //CBaseFighter* target = CFightData::getInstance()->getTheFriendHero();
    
    
    //获取投石机能造成的伤害值
    int theNum = CDBMgr::getInstance()->getAffectNum(CDBMgr::eAffectType::eEnergyType, 4);
    
    
    
    
    //如果是pvp模式，则直接传伤害值。
    if(m_bIfRecMode){
        
        theAttack->setSkillIdAndStart(&m_stSkillInfo,NULL,NULL,m_theBall->getPosition3D(),m_iTheDamage);

    }else{
        
        m_iTheDamage = theNum;
        
        theAttack->setSkillIdAndStart(&m_stSkillInfo,NULL,NULL,m_theBall->getPosition3D(),m_iTheDamage);
    }
    
    
    m_pFightScene->addChild(theAttack);

    if(!((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_USE_MANGONEL) && (guiderManagerLayer::getInstance()->getStepNum() == 5)))
    {
        m_startBall->setVisible(false);
    }
}



