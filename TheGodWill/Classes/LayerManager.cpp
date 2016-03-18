#include "LayerManager.h"

#include "BaseLayer.h"

#include "HelloWorldScene.h"

#include "fightScene.h"
#include "mainMenuLayer.h"
#include "LoginLayer.h"
#include "levelInfoLayer.h"
#include "BaseScene.h"
#include "heroCompoundLayer.h"
#include "chargeInfoLayer.h"
#include "LYLoadingLayer.h"
#include "LvUpLayer.h"
#include "SkillUpMenuLayer.h"
#include "PropLayer.h"
#include "TaskLayer.h"
#include "FightPrepareLayer.h"
#include "SoliderSelectLayer.h"
#include "systemSetLayer.h"
#include "FightWinLayer.h"
#include "FightFailLayer.h"
#include "LoginIntro.h"
#include "LoginLayer.h"
#include "Shop.h"
#include "DrawCard.h"

#include "pvpMainLayer.h"
//#include "pvpEnemyLayer.h"

#include "LYFightScenePVP.h"

CLayerManager* CLayerManager::t = NULL;

USING_NS_CC;

CLayerManager::CLayerManager()
{
    //CPlistManager::plistInit();
    
    m_pCurrentScene = NULL;
    m_pCurrentLayer = NULL;
    
    m_pLoadingLayer = NULL;
    
    
    
    
    //相关layer的plist 。  如果没有使用loading界面，则不用在此加入
    {
        layerPlist fightPlist;
        
        fightPlist.type=eLayer_Fight;
        
        fightPlist.thePlist.push_back("plist/shape.plist");
        fightPlist.thePlist.push_back("plist/towerDefense.plist");
        fightPlist.thePlist.push_back("plist/guide_1.plist");
        fightPlist.thePlist.push_back("plist/guide_2.plist");
        fightPlist.thePlist.push_back("plist/fight.plist");
        fightPlist.thePlist.push_back("plist/fightAnimation.plist");
        fightPlist.thePlist.push_back("plist/effect.plist");
        fightPlist.thePlist.push_back("plist/skill.plist");
        
        fightPlist.thePlist.push_back("plist/skillEffect.plist");
        
        fightPlist.thePlist.push_back("plist/fightTalkIcon.plist");
        
        m_VecAllPlist.push_back(fightPlist);
    }
    
    {
        layerPlist pvpMainPlist;
        
        pvpMainPlist.type=eLayer_PVPLayer;
        pvpMainPlist.thePlist.push_back("plist/pvpMain.plist");
        pvpMainPlist.thePlist.push_back("plist/pvpBuild.plist");
        pvpMainPlist.thePlist.push_back("plist/pvpCommon.plist");
        pvpMainPlist.thePlist.push_back("plist/pvpEffect.plist");
        pvpMainPlist.thePlist.push_back("plist/pvpIcon.plist");
        pvpMainPlist.thePlist.push_back("plist/fight.plist");
        m_VecAllPlist.push_back(pvpMainPlist);
    }


}

CLayerManager::~CLayerManager()
{
}


CLayerManager* CLayerManager::GetSingletonPtr()
{
    if (!t)
    {
        t = new CLayerManager();
    }
    return t;
}

Node* CLayerManager::createLayer(layerType type, Node* parentNode,bool ifChangeScene,bool ifAddAutomatic,std::vector<std::string>* parameters)
{
    SimpleAudioEngine::getInstance()->playEffect("music/newUI.mp3");
    CBaseLayer* pBaseLayer = NULL;
    switch (type)
    {
        case eLayer_Fight:
        {
//            //测试用
//            pBaseLayer = CFightScenePVP::create(EPVPMode_PLAY,10,"");
//            pBaseLayer->setlayerType(type);
            
//            ((CFightScenePVP*)pBaseLayer)->setPVPMode(EPVPMode_PLAY,"");
            
            pBaseLayer = CFightScene::create();
            pBaseLayer->setlayerType(type);
        }
            break;
        case eLayer_Login:
            pBaseLayer = CLoginLayer::create();
            pBaseLayer->setlayerType(type);
        case eLayer_MainMenu:
            pBaseLayer = CMainMenuLayer::create();
            pBaseLayer->setlayerType(type);
            break;
        case eLayer_LevelInfo:
            pBaseLayer = CLevelInfoLayer::create();
            pBaseLayer->setlayerType(type);
            break;
        case eLayer_Compound:
            pBaseLayer = CHeroCompoundLayer::create();
            pBaseLayer->setlayerType(type);
            break;
        case eLayer_LvUp:
            pBaseLayer = CLvUpLayer::create();
            pBaseLayer->setlayerType(type);
            break;
        case eLayer_LvSkill:
            pBaseLayer = CSKillMenuLayer::create();
            pBaseLayer->setlayerType(type);
            break;
        case eLayer_PropLayer:
            pBaseLayer =  CPropLayer::create();
            pBaseLayer->setlayerType(type);
            break;
        case eLayer_TaskLayer:
            pBaseLayer = CTaskLayer::create(1);
            pBaseLayer->setlayerType(type);
            break;
        case eLayer_DailyTaskLayer:
            pBaseLayer = CTaskLayer::create(0);
            pBaseLayer->setlayerType(type);
            break;
        case eLayer_TeamLayer:
            //pBaseLayer =  CFightFailLayer::create();
            //pBaseLayer = CFightWinLayer::create();
            pBaseLayer = FightPrepareLayer::create();
            pBaseLayer->setlayerType(type);
            break;
        case eLayer_DrawCardLayer:
            pBaseLayer = CDrawCard::create();
            pBaseLayer->setlayerType(type);
            break;
        case eLayer_SoliderSelectLayer:
            pBaseLayer = SoliderSelectLayer::create();
            pBaseLayer->setlayerType(type);
            break;
        case eLayer_SetLayer:
            pBaseLayer = CSystemSetLayer::create();
            pBaseLayer->setlayerType(type);
            break;
        case eLayer_ShopLayer:
            pBaseLayer = CShopLayer::create();
            pBaseLayer->setlayerType(type);
            break;
        case eLayer_PVPLayer:
            pBaseLayer = pvpMainLayer::create();
            pBaseLayer->setlayerType(type);
            break;
        case eLayer_LoginFirst:
            pBaseLayer=  CLoginLayer::create();
            pBaseLayer->setlayerType(type);
            break;
        case eLayer_LoginSec:
            pBaseLayer = CLoginIntro::create();
            pBaseLayer->setlayerType(type);
            break;
        case eLayer_Charge:
            pBaseLayer = chargeInfoLayer::create();
            pBaseLayer->setlayerType(type);
            break;
        case eLayer_Loading:
            pBaseLayer = CLoadingLayer::create();
            pBaseLayer->setlayerType(type);
            break;
            
        case eLayer_FightPVP:
        {
            
            if (parameters->size()<3) {
                CCLOG("LYASSERT::fightPVP parameters error");
            }
            
            __String temp = parameters->at(0);
            PVPMode mode = (PVPMode)temp.intValue();
            
            temp = parameters->at(1);
            int stone = temp.intValue();
            
            temp = parameters->at(2);
            std::string mapInfo = temp.getCString();
            
            std::string enemyInfo = "";
            std::string videoInfo = "";
            
            if (parameters->size()>=4) {
                temp = parameters->at(3);
                enemyInfo = temp.getCString();
            }
            if (parameters->size()>=5) {
                temp = parameters->at(4);
                videoInfo = temp.getCString();
            }
            
            pBaseLayer = CFightScenePVP::create(mode,stone,mapInfo,enemyInfo,videoInfo);
            pBaseLayer->setlayerType(type);
        }
            
    }
    
    if (pBaseLayer && type != eLayer_error)
    {
        m_iLayerType = type;
//        pBaseLayer->init(type, pBaseLayer);
//        pBaseLayer->autorelease();
        pBaseLayer->setAnchorPoint(Point());
        pBaseLayer->setPosition(Point());
        
        
        //如果不想换场景的话
        if (!ifChangeScene) {
            
            if (NULL != parentNode)//如果有父节点，就加在父节点上
            {
                m_pCurrentLayer = pBaseLayer;
                parentNode->addChild(pBaseLayer, 999);
                
                
            }else{ //否则，删除当前layer，并且替换
                
                m_pCurrentScene->removeAllLayer();
                
                if (ifAddAutomatic) { //如果是自动加上，则直接加上该layer，否则由玩家手动加上
                    m_pCurrentScene->addLayer(pBaseLayer);
                    
                    m_pCurrentLayer = pBaseLayer;
                }

            }
            
        }
//        
//        
//        
//        else if(ifChangeScene == false)//如果不指定父节点，并且不想换场景。那就加在默认场景上
//        {
//            
//        }
        //add to stack
//        pushLayer(pBaseLayer);
    }
    else
    {
//        CC_SAFE_DELETE(pBaseLayer);     //此处大坑。不可贸然使用delete，哪怕你有个safe！！！
    }
    
    if (ifChangeScene) {
        
        CBaseScene* theScene = CBaseScene::create();

        theScene->addLayer(pBaseLayer);
        
        Director::getInstance()->replaceScene(theScene);
        
        m_pCurrentScene = theScene;
        
        return theScene;
    }

    return pBaseLayer;
}

Node* CLayerManager::createLayerWithLoading(layerType type, cocos2d::Node *parentNode,bool ifchangeScene,std::vector<std::string>parameters){
    
    m_pLoadingLayer = (CLoadingLayer*)CLayerManager::createLayer(eLayer_Loading,NULL,false,false);
    
    m_pLoadingLayer->setTheChanging(type, parentNode, ifchangeScene,parameters);
    
    m_pCurrentScene->addLayer(m_pLoadingLayer);
    m_pCurrentLayer = m_pLoadingLayer;
    
    return m_pLoadingLayer;
    
}




bool CLayerManager::findPlist(const char* plistFile)
{
    std::map<std::string,int>::iterator it = m_mapPlist.find(plistFile);
    if(it == m_mapPlist.end()){
        return false;
    }
    return true;
}

bool CLayerManager::erasePlist(const char* plistFile)
{
    std::map<std::string,int>::iterator it = m_mapPlist.find(plistFile);
    if(it != m_mapPlist.end()){
        it->second -= 1;
        if(it->second <=0){
            m_mapPlist.erase(plistFile);
            CCSpriteFrameCache::getInstance()->removeSpriteFramesFromFile(plistFile);
            
            
            
            return true;
        }
    }
    return false;
}

void CLayerManager::pushPlist(const char* plistFile)
{
    std::map<std::string,int>::iterator it = m_mapPlist.find(plistFile);
    if(it == m_mapPlist.end()){
        m_mapPlist.insert(std::make_pair(plistFile,1));
        CCSpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistFile);
    }else{
        it->second += 1;
    }
}


void CLayerManager::addLoadingLayer(){
    
//    m_pLoadingLayer =  CBaseLayer::create();
//    
//    m_pLoadingLayer->setColor(Color3B::BLACK);
//    
//    Label* temp = Label::createWithSystemFont("laoding", "Arial", 50);
//    
//    temp->runAction(RepeatForever::create(RotateBy::create(0.5f, 300)));
//    
//    m_pCurrentScene->addChild(m_pLoadingLayer,1000);
    
    
}

void CLayerManager::removeLoadingLayer(){
    
    
    if (m_pLoadingLayer) {
        m_pLoadingLayer->removeFromParentAndCleanup(true);
        m_pLoadingLayer = NULL;
    }
    
    
    
    
}





CBaseLayer* CLayerManager::getCurrentLayer()
{
    return m_pCurrentLayer;
    
}


#pragma mark -

int CLayerManager::getCurLayerType()
{
    return m_iLayerType;
}


//MARK: loading界面的plist管理
std::vector<std::string>* CLayerManager::getPlistsByLayerType(layerType type){
    
    for (int i=0; i<m_VecAllPlist.size(); i++) {
        
        layerPlist* temp = &m_VecAllPlist.at(i);
        
        
        if (temp->type == type) {
            return &(temp->thePlist);
        }
        
        
        
    }
    
    return NULL;
}




