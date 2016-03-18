//
//  gameStoryLayer.cpp
//  WarOfGod
//
//  Created by user on 15/4/13.
//
//

#include "gameStoryLayer.h"
#include "storyManagerLayer.h"
#include "CSharedPlist.h"
#include "LYPersonalData.h"
#include "fightScene.h"

static int storyLayerNum=0;

gameStoryLayer::gameStoryLayer()
{
    listener = NULL;
    m_dialogue = NULL;
    m_name = NULL;
    m_bRomoveFlag = false;
    CSharedPlist::getInstance()->pushPlist("plist/story_1.plist");
    CSharedPlist::getInstance()->pushPlist("plist/story_2.plist");
    
    storyLayerNum++;
    
    CCLOG("storyLayerNumd:%d",storyLayerNum);
}

gameStoryLayer::~gameStoryLayer()
{
    if(storyManagerLayer::getInstance()->m_layNum == 1)
    {
        storyManagerLayer::getInstance()->removeGameStoryLayer();
    }
    CSharedPlist::getInstance()->erasePlist("plist/story_1.plist");
    CSharedPlist::getInstance()->erasePlist("plist/story_2.plist");
    
    storyLayerNum--;
    
    CCLOG("storyLayerNumd:%d",storyLayerNum);
}

gameStoryLayer* gameStoryLayer::create()
{
    gameStoryLayer* pRet = new gameStoryLayer();
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else{
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool gameStoryLayer::init()
{
    CBaseLayer::init();
    m_winSize = Director::getInstance()->getWinSize();
    m_type = storyManagerLayer::getInstance()->getstoryType();
    listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(gameStoryLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(gameStoryLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    listener->setSwallowTouches(true);
    initUI();
    return true;
}

void gameStoryLayer::initUI()
{
    m_back = LayerColor::create(Color4B(0, 0, 0, 180), m_winSize.width, m_winSize.height * 0.3f);
    addChild(m_back);
    m_back->setPosition(Vec2(0, 0));
    
    m_storyInfo = CDataManager::getStoryInfoByLevelId(m_type);
    int step = storyManagerLayer::getInstance()->getStepNum();
    m_girl = Sprite::createWithSpriteFrameName(m_storyInfo[step - 1].icon.c_str());
    addChild(m_girl);
    std::string name = m_storyInfo[step - 1].name;
    if(name == "userName")
    {
        name = CPersonalData::getInstance()->getUserInfo()->userName;
    }
    m_name = Label::createWithTTF(name.c_str(), "fonts/cuti.ttf", 30);
    m_name->setColor(Color3B(234, 255, 10));
    addChild(m_name);
    m_name->enableOutline(Color4B(15,200,50, 200));
    
    std::string content = m_storyInfo[step - 1].content;
    std::string::size_type startPos = -1;
    startPos = content.find("userName");
    if(startPos != std::string::npos)
    {
        content.replace(startPos, 8, CPersonalData::getInstance()->getUserInfo()->userName.c_str());
    }

    m_dialogue = Label::createWithTTF(content.c_str(), "fonts/cuti.ttf", 30, Size(m_winSize.width * 0.485f, 200));
    m_dialogue->setColor(Color3B(234, 215, 162));
    addChild(m_dialogue);
    
    if(startPos != std::string::npos)
    {
        std::string tempStr = content.substr(0, startPos);
        auto tempStrLabel = Label::createWithTTF(tempStr.c_str(), "fonts/cuti.ttf", 30);
        int npos = tempStrLabel->getStringLength();
        auto temp = Label::createWithTTF(CPersonalData::getInstance()->getUserInfo()->userName.c_str(), "fonts/cuti.ttf", 30);
        int length = npos + temp->getStringLength();
        
        //计算userName所在行数  目前设置每行显示21字符 长度63
        
        int line = 0;
        int tempNum = (int)tempStr.length();
        int remain = tempNum % 63;
       if(remain + CPersonalData::getInstance()->getUserInfo()->userName.length() > 63)
        {
            line = tempNum / 63 + 1;
        }
        else{
            line = tempNum / 63;
        }
        
        length += line;
        
        for(int i = npos;i < length; i++)
        {
            auto BChar = (Sprite*) m_dialogue->getLetter(i);
            if(BChar)
            {
                BChar->setColor(Color3B::RED);
            }
        }
    }
    
    if(m_storyInfo[step - 1].posType == 0)
    {
        m_dialogue->setAnchorPoint(Vec2(0, 1));
        m_name->setAnchorPoint(Vec2(0, 0.5f));
        m_name->setPosition(Vec2(m_winSize.width * 0.32f, m_back->getContentSize().height * 0.85f));
        m_girl->setPosition(Vec2(m_winSize.width * 0.12f, m_winSize.height * 0.225f));
        m_dialogue->setPosition(Vec2(m_winSize.width * 0.32f, m_back->getContentSize().height * 0.69f));
    }
    else{
        m_dialogue->setAnchorPoint(Vec2(1, 1));
        m_name->setAnchorPoint(Vec2(1, 0.5f));
        m_name->setPosition(Vec2(m_winSize.width * 0.7f, m_back->getContentSize().height * 0.85f));
        if(name == "导师")
        {
            m_name->setPositionX(m_winSize.width * 0.655f);
        }
        m_girl->setFlippedX(true);
        m_girl->setPosition(Vec2(m_winSize.width * 0.88f, m_winSize.height * 0.225f));
        m_dialogue->setPosition(Vec2(m_winSize.width * 0.7f, m_back->getContentSize().height * 0.69f));
    }
}

bool gameStoryLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(m_back && m_back->getBoundingBox().containsPoint(touch->getLocation()));
    {
        if(storyManagerLayer::getInstance()->getStepNum() < m_storyInfo.size())
        {
            storyManagerLayer::getInstance()->setStepNum(storyManagerLayer::getInstance()->getStepNum() + 1);
            int step = storyManagerLayer::getInstance()->getStepNum();
            m_girl->setSpriteFrame(m_storyInfo[step - 1].icon);
            std::string name = m_storyInfo[step - 1].name;
            if(name == "userName")
            {
                name = CPersonalData::getInstance()->getUserInfo()->userName;
            }
            
            std::string content = m_storyInfo[step - 1].content;
            std::string::size_type startPos = content.find("userName");
            if(startPos != std::string::npos)
            {
                content.replace(startPos, 8, CPersonalData::getInstance()->getUserInfo()->userName.c_str());
            }
            m_name->setString(name);
            if(m_dialogue)
            {
                m_dialogue->removeFromParent();
                m_dialogue = NULL;
            }
            
            m_dialogue = Label::createWithTTF(content.c_str(), "fonts/cuti.ttf", 30, Size(m_winSize.width * 0.48f, 200));
            m_dialogue->setColor(Color3B(234, 215, 162));
            addChild(m_dialogue);
            
            if(startPos != std::string::npos)
            {
                std::string tempStr = content.substr(0, startPos);
                auto tempStrLabel = Label::createWithTTF(tempStr.c_str(), "fonts/cuti.ttf", 30);
                int npos = tempStrLabel->getStringLength();
                
                auto temp = Label::createWithTTF(CPersonalData::getInstance()->getUserInfo()->userName.c_str(), "fonts/cuti.ttf", 30);
                int length = npos + temp->getStringLength();
                
                for(int i = npos;i < length; i++)
                {
                    auto BChar = (Sprite*) m_dialogue->getLetter(i);
                    if(BChar)
                    {
                        BChar->setColor(Color3B::RED);
                    }
                }

            }
            
            if(m_storyInfo[step - 1].posType == 0)
            {
                m_dialogue->setAnchorPoint(Vec2(0, 1));
                m_name->setAnchorPoint(Vec2(0, 0.5f));
                m_name->setPosition(Vec2(m_winSize.width * 0.32f, m_back->getContentSize().height * 0.85f));
                m_girl->setPosition(Vec2(m_winSize.width * 0.12f, m_winSize.height * 0.225f));
                m_girl->setFlippedX(false);
                m_dialogue->setPosition(Vec2(m_winSize.width * 0.32f, m_back->getContentSize().height * 0.69f));
            }
            else{
                m_dialogue->setAnchorPoint(Vec2(1, 1));
                m_name->setAnchorPoint(Vec2(1, 0.5f));
                m_name->setPosition(Vec2(m_winSize.width * 0.7f, m_back->getContentSize().height * 0.85f));
                if(name == "导师")
                {
                    m_name->setPositionX(m_winSize.width * 0.655f);
                }
                
                m_girl->setFlippedX(true);
                m_girl->setPosition(Vec2(m_winSize.width * 0.88f, m_winSize.height * 0.225f));
                m_dialogue->setPosition(Vec2(m_winSize.width * 0.7f, m_back->getContentSize().height * 0.69f));
            }
        }
        else
        {
            m_bRomoveFlag = true;
            m_storyInfo = CDataManager::getStoryInfoByLevelId(m_type);
            int step = storyManagerLayer::getInstance()->getStepNum();
            
            if(m_storyInfo[step - 1].startTime==-1)
            {
                NotificationCenter::getInstance()->postNotification("fightResult");
            }
            else {
                if(m_storyInfo[step - 1].startTime==0)
                {
                    NotificationCenter::getInstance()->postNotification("gameData");
                }
            }
            storyManagerLayer::getInstance()->setStepNum(0);
            CDataManager::saveGameStoryInfoToDB(storyManagerLayer::getInstance()->getstoryType());
            Director::getInstance()->resume();
        }
    }
    
    return true;
}

void gameStoryLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(m_bRomoveFlag)
    {
        m_bRomoveFlag = false;
        if(storyManagerLayer::getInstance()->getstoryType() ==4)
        {
            CFightScene* parent = (CFightScene*)getParent();
            parent->m_bifNorAddSolider = true;
        }
        storyManagerLayer::getInstance()->removeGameStoryLayer();
    }
}

