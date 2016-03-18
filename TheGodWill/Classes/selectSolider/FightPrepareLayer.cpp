//
//  FightPapreLayer.cpp
//  TheGodWill
//
//  Created by user on 14/12/1.
//
//

#include "FightPrepareLayer.h"
#include "SoliderSelectLayer.h"
#include "LYPersonalData.h"
#include "DataManager.h"
#include "CSharedPlist.h"
#include "LYFightManager.h"
#include "Common.h"
#include "guiderManagerLayer.h"

FightPrepareLayer::FightPrepareLayer()
{
    m_curSelectedSolider = 0;
    m_pMenu = NULL;
    m_pBg = NULL;
    listen = NULL;
    CSharedPlist::getInstance()->pushPlist("plist/selectSolider.plist");
}

FightPrepareLayer::~FightPrepareLayer()
{
    CSharedPlist::getInstance()->erasePlist("plist/selectSolider.plist");
}

bool FightPrepareLayer::init()
{
    if (!CBaseLayer::initWithColor(Color4B(0, 0, 0, 200))) {
        return false;
    }
    m_winSize = Director::getInstance()->getWinSize();
    listen = EventListenerTouchOneByOne::create();
    listen->setSwallowTouches(true);
    listen->onTouchBegan = CC_CALLBACK_2(FightPrepareLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listen,this);
    initUI();
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_TEAM) && !(CDataManager::getEndedBy(GUIDE_SOLIDER_TEAM)))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this);
        if(guiderManagerLayer::getInstance()->getStepNum() == 5)
        {
            CDataManager::saveGuideInfoToDB(GUIDE_SOLIDER_TEAM);
        }
        
    }
    return true;
}


void FightPrepareLayer::addGuideInfo()
{
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_TEAM) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
    {
        Vec2 guidePos = Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.5f);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, guidePos);
    }
}

void FightPrepareLayer::onExit()
{
    _eventDispatcher->removeEventListener(listen);
    CBaseLayer::onExit();
}

bool FightPrepareLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}

void FightPrepareLayer::initUI()
{
    m_pBg = Sprite::createWithSpriteFrameName("team_bg.png");
    addChild(m_pBg);
    //m_pBg->setContentSize(Size(m_pBg->getContentSize().width * 2.0, m_pBg->getContentSize().height * 2.0));
    m_pBg->setScale(2.0f);
    m_pBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.5f));
    std::string name[5] = {"LineDownDiagonal.png", "Triangle.png", "Circle.png","Rectangle.png", "V.png"};
//    TTFConfig  ttf("fonts/Marker Felt.ttf",30);
    
    Size bgSize = m_pBg->getBoundingBox().size;
    LayerColor* pColor = LayerColor::create(Color4B(255, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(pColor);
    pColor->setPosition(Vec2(m_pBg->getPositionX() - bgSize.width * 0.5f, m_pBg->getPositionY() - bgSize.height * 0.5f));
    
    Label* pTitle = Label::createWithSystemFont("队伍编辑", "Arial", 30);
    pColor->addChild(pTitle);
    pTitle->setPosition(Point(bgSize.width / 2, bgSize.height * 0.933f));
    
    //测试
    std::vector<FighterInfo*> tempVec = CPersonalData::getInstance()->m_vecMyTeamInfo;
    
    m_pMenu = Menu::create();
    pColor->addChild(m_pMenu);
    m_pMenu->setPosition(Vec2::ZERO);
    for(int i = 0;i < 5; i++)
    {
        Point pos = Point(bgSize.width * 0.12f + 196*i, bgSize.height * 0.5f);
        addBtn("gray_bg.png", "gray_bg.png", name[i].c_str(), pos, m_pMenu, i + 1);
    }
    
    Sprite* spNormal = Sprite::createWithSpriteFrameName("start.png");
    Sprite* spSelect = Sprite::createWithSpriteFrameName("start.png");
    spSelect->setColor(Color3B::GRAY);
    LYMenuItemSprite* spItem = LYMenuItemSprite::create(spNormal, spSelect, CC_CALLBACK_1(FightPrepareLayer::menuCallBack, this));
    spItem->setPosition(Vec2(bgSize.width / 2, 25));
    spItem->setTag(FIGHT_SURE_TAG);
    m_pMenu->addChild(spItem);
    Label* start = Label::createWithSystemFont("确定", "Arial", 34);
    spItem->addChild(start);
    start->setPosition(Vec2(spNormal->getContentSize().width / 2, spNormal->getContentSize().height / 2));
    
    spNormal = Sprite::createWithSpriteFrameName("closeBtn.png");
    spSelect = Sprite::createWithSpriteFrameName("closeBtn.png");
    spSelect->setColor(Color3B::GRAY);
    LYMenuItemSprite* pItemExit = LYMenuItemSprite::create(spNormal, spSelect, CC_CALLBACK_1(FightPrepareLayer::menuCallBack, this));
    CommonData::enlargeItemSprite(pItemExit, 2);
    pItemExit->setTag(FIGHT_EXIT_TAG);
    pItemExit->setPosition(Point(bgSize.width * 0.945, bgSize.height * 0.936));
    m_pMenu->addChild(pItemExit);
}

void FightPrepareLayer::addBtn(const char* normal, const char* selected, const char* child, Point &pos, Node* parent, int tag)
{
    //Sprite* btnBg = Sprite::createWithSpriteFrameName("gray_bg.png");
    Sprite* spNormal = Sprite::createWithSpriteFrameName(normal);
    Sprite* spSelect = Sprite::createWithSpriteFrameName(selected);
    spSelect->setColor(Color3B::GRAY);
    LYMenuItemSprite* spItem = LYMenuItemSprite::create(spNormal, spSelect, CC_CALLBACK_1(FightPrepareLayer::menuCallBack, this));
    spItem->setPosition(pos);
    spItem->setScale(3.6f);
    spItem->setTag(tag);
    parent->addChild(spItem, 1, tag);
    
    FighterInfo* pInfo = CPersonalData::getInstance()->getTeamSolderByType(tag - 1);
    if(pInfo && (pInfo->rare > 0)){
        Sprite* pChild = Sprite::createWithSpriteFrameName(child);
        spItem->addChild(pChild, 1, 5);
        pChild->setScale(0.58f);
        pChild->setPosition(Point(spNormal->getContentSize().width * spNormal->getScale() / 2, spNormal->getBoundingBox().size.height * 0.165f));
        
        monsterInfo* info = CDataManager::getMonsterInfoByMonsterId(pInfo->monsterId);
        Label* pLabel = Label::createWithSystemFont(info->monsterName.c_str(), "Arial", 32);
        spItem->addChild(pLabel, 1, 6);
        pLabel->setScale(0.25f);
        pLabel->setColor(Color3B(200, 200, 0));
        pLabel->setPosition(Point(spNormal->getContentSize().width * spNormal->getScale() / 2, pChild->getPositionY() +
                                  pChild->getBoundingBox().size.height / 2 + 5));
        
//        Sprite* fazhen = Sprite::createWithSpriteFrameName("fazhen.png");
//        spItem->addChild(fazhen);
//        fazhen->setScaleX(0.65);
//        fazhen->setScaleY(0.2f);
//        fazhen->setPosition(Vec2(spNormal->getContentSize().width * spNormal->getScale() / 2, pLabel->getPositionY() +
//                                 pLabel->getBoundingBox().size.height / 2 + 48));
        //英雄
        FighterInfo* pInfo = CPersonalData::getInstance()->getTeamSolderByType(tag - 1);
        modelInfo* pModelInfo = CDataManager::getModelInfoByModelId(pInfo->theModelInfo->modelId);
        Sprite3D* hero = Sprite3D::create(pModelInfo->modelName.getCString());
        //spItem->addChild(hero);
        Size modeSize = hero->getBoundingBox().size;
        float scale;
        
        if(modeSize.width * modeSize.height < 30)
        {
            scale = 830/(modeSize.width * modeSize.height);
            hero->setScale(scale);
        }
        else if(modeSize.width * modeSize.height < 50)
        {
            hero->setScale(1250/(modeSize.width * modeSize.height));
        }
        else if(modeSize.width * modeSize.height < 100)
        {
            hero->setScale(1450/(modeSize.width * modeSize.height));
        }
        else if(modeSize.width * modeSize.height < 150){
            hero->setScale(1650/(modeSize.width * modeSize.height));
        }
        
        else{
            hero->setScale(28*8.5/modeSize.width);;
        }
        //计算模型的位置
        float tempPos = pLabel->getPositionY() + (spNormal->getContentSize().height - pLabel->getPositionY()) * spItem->getScale() * 0.5f;
        hero->setPosition(Vec2(spNormal->getContentSize().width * spItem->getScale() / 2, tempPos));
        
        if(tag == 1)
        {
            hero->setRotation3D(Vec3(0, (hero->getRotation3D().y + 30), 0));
        }
        else if(tag == 2){
            hero->setRotation3D(Vec3(0, (hero->getRotation3D().y + 20), 0));
        }
        if(tag == 4)
        {
            hero->setRotation3D(Vec3(0, (hero->getRotation3D().y - 10), 0));
        }
        else if(tag == 5){
            hero->setRotation3D(Vec3(0, (hero->getRotation3D().y - 20), 0));
        }
        auto animation = Animation3D::create(pModelInfo->modelName.getCString());
        if (animation)
        {
            auto animate = Animate3D::create(animation, pModelInfo->idleFrame.start/60, pModelInfo->idleFrame.duration/60);
            
            if(pInfo->monsterId ==  14|| pInfo->monsterId ==  15)
            {
                animate->setSpeed(0.3f);//测试用，换模型了就删掉
            }

            
            cocos2d::Action* _swim = RepeatForever::create(animate);
            hero->runAction(_swim);
        }

        
        ClippingNode *clipper = ClippingNode::create();
        spItem->addChild(clipper,1);
        clipper->setScale(0.27f);
        clipper->setAnchorPoint(Vec2(0.5, 0.5));
        clipper->addChild(hero);
        DrawNode *stencil = DrawNode::create();
        Point rectangle[4];
        rectangle[0] = Point(5, 10);
        rectangle[1] = Point(spNormal->getBoundingBox().size.width - 8, 10);
        rectangle[2] = Point(spNormal->getBoundingBox().size.width - 8, spNormal->getBoundingBox().size.height - 10);
        rectangle[3] = Point(5, spNormal->getBoundingBox().size.height - 10);
        Color4F white = {1, 1, 1, 1};
        stencil->drawPolygon(rectangle, 4, white, 1, white);
        clipper->setStencil(stencil);
        
    }
    else
    {
        Sprite* pChild = Sprite::createWithSpriteFrameName(child);
        spItem->addChild(pChild, 1, 5);
        pChild->setScale(0.6f);
        pChild->setPosition(Point(spNormal->getContentSize().width * spNormal->getScale() / 2, spNormal->getBoundingBox().size.height * 0.55f));
        FighterInfo* pInfo = CPersonalData::getInstance()->getTeamSolderByType(tag - 1);
        
        Label* pLabel = Label::createWithSystemFont("士兵选择", "Arial", 32);
        spItem->addChild(pLabel, 1, 6);
        pLabel->setScale(0.25f);
        pLabel->setColor(Color3B(50, 200,80));
        pLabel->setPosition(Point(spNormal->getContentSize().width * spNormal->getScale() / 2, spNormal->getBoundingBox().size.height * 0.3f));
    }
    
    
    
}

void FightPrepareLayer::showSolider(int curPage,int soliderIndex)
{
    //根据选中的兵种显示兵种图片
    char buf[32];
    sprintf(buf, "midlevel_%d.png", (curPage - 1)*PAGE_NUM + soliderIndex);
    LYMenuItemSprite* pItem = (LYMenuItemSprite*)(m_pMenu->getChildByTag(m_curSelectedSolider));
    if (pItem)
    {
        Point pos;
        Label* pLabel = (Label*)(pItem->getChildByTag(6));
        if (pLabel)
        {
            pos = pLabel->getPosition();
            pLabel->removeFromParent();
        }
        else
        {
            Sprite* temp = (Sprite*)(pItem->getChildByTag(10));
            if (temp)
            {
                pos = temp->getPosition();
                temp->removeFromParent();
            }
        }
        
        Sprite* sp = Sprite::create(buf);
        pItem->addChild(sp,1,10);
        sp->setPosition(pos);
    }
}

void FightPrepareLayer::menuCallBack(Ref* pSender)
{
    int tag = ((Node*)pSender)->getTag();
    if((tag == FIGHT_EXIT_TAG) || (tag == FIGHT_SURE_TAG)){
        CPersonalData::getInstance()->setrestartPlayMusic(false);
        removeFromParent();
    }
    else{
        m_curSelectedSolider = tag;
        std::vector<FighterInfo>m_myFighterInfo = CPersonalData::getInstance()->getSoliderInfoByType(tag - 1);
        if (m_myFighterInfo.empty())
        {
            
            Sprite * thetarget = CFightManager::getTheFlyWord("您还没有该类型士兵","fonts/cuti.ttf");
            thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
            addChild(thetarget, 10);
            
            
            /*Label* pLabel = Label::createWithSystemFont("你还没有该类型士兵", "Arial", 32);
            addChild(pLabel, 10);
            pLabel->setOpacity(0);
            pLabel->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
            pLabel->runAction(Sequence::create(FadeIn::create(0), MoveBy::create(1.5f, Vec2(0, 200)), NULL));
            pLabel->runAction(Sequence::create(DelayTime::create(1.3f), FadeOut::create(1.0f), NULL));*/
            
        }
        else{
            CPersonalData::getInstance()->setSoliderType(tag - 1);
            CLayerManager::GetSingletonPtr()->createLayer(eLayer_SoliderSelectLayer,this->getParent(),false);
            removeFromParent();
        }
    }
}
