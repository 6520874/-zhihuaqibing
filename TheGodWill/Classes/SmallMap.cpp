//
//  SmallMap.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/12/1.
//
//

#include "SmallMap.h"
#include "PaoWuxian.h"
#include "graphicsIdentify.h"
#include "fightScene.h"
#include "LYFightData.h"

CSmallMap *CSmallMap ::t = NULL;

CSmallMap::CSmallMap(CFightScene *pScene)
{
    m_pCurView = NULL;
    m_bMenuViewMove = false;
    m_pMenuViewMove = NULL;
    m_WinSize = Director::getInstance()->getWinSize();
    m_pScene = pScene;
}


std::string  CSmallMap::GetSoldierTypeImg(FighterType t, FightGroup theGroup,AttackType theAttack ,bool ifBoss)
{
    std::string temp = "";
    
    
    if (theAttack == eAttackType_walker)
    {
        temp = "map_protect.png";
    
    }else if (ifBoss)
    {
            temp = "map_boss.png";
            
    }else if (theGroup == eFightGroup_Freind)
    {
         switch (t) {
             case eFighterType_line:
                 temp = "heng.png";
                 break;
             case eFighterType_circle:
                 temp = "circle.png";
                 break;
             case eFighterType_triangle:
                 temp = "threejiao.png";
                 break;
             case eFighterType_square:
                 temp = "square.png";
                 break;
             case eFighterType_v:
                 temp = "z.png";
                 break;
             case eFighterType_Tower:
                 temp = "lan.png";
                 
                 CCLOG("蓝塔图标");
                 break;
             case eFighterType_toushicheluodian:
                 temp = "sightbead.png";
                 break;
             default:
                 break;
         }
     }
     else if (theGroup == eFightGroup_Enemy){
         switch (t) {
             case eFighterType_line:
                 temp = "heng_1.png";
                 break;
             case eFighterType_circle:
                 temp = "circle_1.png";
                 break;
             case eFighterType_triangle:
                 temp = "threejiao_1.png";
                 break;
             case eFighterType_square:
                 temp = "square_1.png";
                 break;
             case eFighterType_v:
                 temp = "z_1.png";
                 break;
             case eFighterType_Tower:
                 temp = "hong.png";
                 CCLOG("红塔图标");
                 break;
             default:
                 break;
         }

     }else if (theGroup == eFightGroup_midFight
               ||theGroup == eFightGroup_midNotFight){
         switch (t) {
             case eFighterType_Tower:
                 temp = "huang.png";
                 CCLOG("黄塔图标");
                 break;
             default:
                 CCLOG("LYError:: 除了塔以外，还有别的中立？");
                 break;
         }
         
     }
    
    return temp;
}



//精灵转换为别的精灵接口

Sprite *  CSmallMap::SetSmallMapPostion(Vec3 vt,FighterType eFightType, FightGroup theGroup,AttackType theAttackType,bool ifBoss)
{
    
    float fx =  vt.x - Start3DPOINT+ONESIDE;
    

    log("fy = %f",vt.y);
    
    float fYbei = 0;
    if(vt.z<0)
    {
        vt.z =  -vt.z+STARTZPOINT;
        fYbei =  vt.z/(STARTZPOINT+-ENDZPOINT);
    }
    else
    {
        fYbei = vt.z/(STARTZPOINT+-ENDZPOINT);
    }
    
    std::string strImg = GetSoldierTypeImg(eFightType, theGroup,theAttackType,ifBoss);
    
    float  pCury = BIGMAPHIGH*(fYbei+0.1f);
    
    Sprite *pSoldier= NULL;
    
    
    if(strImg!="")
    {
       pSoldier =  Sprite::createWithSpriteFrameName(strImg.c_str());
        
        if(eFightType == eFighterType_Tower)
        {
          pSoldier->setScale(0.3);
        }
        else
        {
            pSoldier->setScale(0.5);
        }
        
  
        

        if(vt.x==0&&eFightType>eFighterType_toushicheluodian)
        {
            fx = m_WinSize.width/3.35;
            pCury = BIGMAPHIGH*0.5f;
        }
    
        
        //if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
        {
            pSoldier->setPosition(fx/BIGMAPLEN*SMALLMAPLEN,pCury);
        }
    
        addChild(pSoldier,10);
        
        
        if(theGroup == eFightGroup_Enemy)
        {
            pSoldier->setColor(Color3B::RED);
        }
        
        m_pSpriteMap.pushBack(pSoldier);
    }
    return pSoldier;

}


void CSmallMap::RemoveSmallMapPostion(Sprite *pSprite)
{
    Vector<Sprite *>::iterator it = m_pSpriteMap.begin();
    
    for(;it!=m_pSpriteMap.end();it++)
    {
        if(*it== pSprite)
        {
            m_pSpriteMap.erase(it);
            pSprite->removeFromParent();
            return;
        }
    }
   
}

void  CSmallMap::SetSmallMapPostion(Vec3  vt, Sprite *p)
{
    Vec2 thepos = getMapPosBy3dPos(vt);
    
    if (p)
    {
        p->setPosition(thepos);
    }
}


CSmallMap::~CSmallMap()
{
 
    for(int i=0;i<m_pSpriteMap.size();i++)
    {
        if(m_pSpriteMap.at(i))
        {
            m_pSpriteMap.at(i)->removeFromParent();
            
    
        }
    }

    m_pSpriteMap.clear();
    
    
}



void CSmallMap::OnMenuViewMove(Ref *pSender)
{

    
}

bool CSmallMap::init()
{
    Layer::init();
    
    return true;
}


bool CSmallMap::onTouchBegan(Touch *pTouch, Event *pEvent)
{
   // m_bMenuViewMove = m_pMenuViewMove->onTouchBegan(pTouch,pEvent);
    
    return true;
}

void CSmallMap::onTouchMoved(Touch *pTouch, Event *pEvent)
{
     if(m_bMenuViewMove)
     {
         m_pMenuViewMove->onTouchMoved(pTouch,pEvent);
     }
}

void CSmallMap::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if(m_bMenuViewMove)
    {
        m_pMenuViewMove->onTouchEnded(pTouch,pEvent);
        m_bMenuViewMove = false;
    }
    
}

void CSmallMap::onTouchCancelled(Touch *touch, Event *pEvent)
{
    if(m_bMenuViewMove)
    {
        m_pMenuViewMove->onTouchCancelled(touch,pEvent);
        m_bMenuViewMove = false;
    }
}



Vec2 CSmallMap::getMapPosBy3dPos(cocos2d::Vec3 pos3D){
    
    float fx =  pos3D.x -  Start3DPOINT+ONESIDE;
    
  
    float fBei = 0;
    if(pos3D.z<0)
    {
        pos3D.z = -pos3D.z;
        fBei = (pos3D.z+STARTZPOINT)/(STARTZPOINT+-ENDZPOINT);
    }
    else
    {
        fBei  = pos3D.z/(STARTZPOINT+-ENDZPOINT);
    }
    
    float  pCury = BIGMAPHIGH*(fBei+0.1f);
 

     Vec2   thePos = Vec2(fx/BIGMAPLEN*SMALLMAPLEN,pCury);
    
    
    
    return thePos;
}
