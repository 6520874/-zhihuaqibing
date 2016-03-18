//
//  thePartolManLayer.cpp
//  WarOfGod
//
//  Created by user on 15/8/12.
//
//

#include "thePartolFight.h"
#include "LYFightManager.h"
#include "LYFightData.h"

CPartolFight::CPartolFight()
{
    m_iIndex = 0;
    m_iAddHeight = 0;
    m_iRadius = 0;
    m_iMonsterId = 0;
    
    m_bStartStoped = false;
    m_fTime = 0;
    m_fStopTime = 0;
    m_tagetPos = Vec2::ZERO;
}

CPartolFight::~CPartolFight()
{
    
}

CPartolFight* CPartolFight::create(FighterInfo *theInfo, FightGroup theGroup)
{
    CPartolFight* temp = new CPartolFight();
    temp->m_iAttackType = eAttackType_Partol;
    if (temp && temp->initWithInfo(theInfo, theGroup))
    {
        
        temp->autorelease();
        return temp;
    }else{
        
        delete temp;
        temp = NULL;
        return NULL;
    }
}

bool CPartolFight::initWithInfo(FighterInfo *theInfo,FightGroup theGroup)
{
    if(!CBaseFighter::initWithInfo(theInfo, theGroup))
    {
        return false;
    }

    
    
    if(CPersonalData::getInstance()->getPvpMainLayer())
    {
        playAniamteAction(m_pAction_Walk, true);
    }
    else
    {
        buildInfo* pTowerInfo = pvpCommonData::getInstance()->getTowerInfo();//取出祭坛周围的四个点，矩形
        Vec3 tempPos = pTowerInfo->theTower->getPosition3D();
        m_iRadius = 62 * pTowerInfo->theTower->m_fScale;
        //int blockNum = maxDistance/LENGTH_BLOCK_PER;//计算攻击半径所占方格数
        //左上角
        float xPos = tempPos.x - 3 * LENGTH_BLOCK_PER;
        float zPos = tempPos.z - 3 * LENGTH_BLOCK_PER;
        m_persionPos.push_back(Vec3(xPos, 0, zPos));
        
        xPos = tempPos.x + 3 * LENGTH_BLOCK_PER;
        zPos = tempPos.z - 3 * LENGTH_BLOCK_PER;
        m_persionPos.push_back(Vec3(xPos, 0, zPos));
        
        xPos = tempPos.x + 3 * LENGTH_BLOCK_PER;
        zPos = tempPos.z + 3 * LENGTH_BLOCK_PER;
        m_persionPos.push_back(Vec3(xPos, 0, zPos));
        
        xPos = tempPos.x - 3 * LENGTH_BLOCK_PER;
        zPos = tempPos.z + 3 * LENGTH_BLOCK_PER;
        m_persionPos.push_back(Vec3(xPos, 0, zPos));
        
        xPos = tempPos.x - 3 * LENGTH_BLOCK_PER;
        zPos = tempPos.z - 3 * LENGTH_BLOCK_PER;
        m_persionPos.push_back(Vec3(xPos, 0, zPos));
        
        //判断路径是否越界
        for(int i = 0; i < (int)m_persionPos.size(); i++)
        {
            if(m_persionPos[i].x  < MAP_START_XPOS)
            {
                m_persionPos[i].x = MAP_START_XPOS;
            }
            else if(m_persionPos[i].x > MAP_START_XPOS + 23 * LENGTH_BLOCK_PER)
            {
                m_persionPos[i].x = MAP_START_XPOS + 23 * LENGTH_BLOCK_PER;
            }
            if(m_persionPos[i].z < ENDZPOINT)
            {
                m_persionPos[i].z = -8 * LENGTH_BLOCK_PER + STARTZPOINT;
            }
            else if(m_persionPos[i].z > STARTZPOINT)
            {
                m_persionPos[i].z =  STARTZPOINT;
            }
        }
    }
    return true;
}

void CPartolFight::update(float dt)
{
    if(CPersonalData::getInstance()->getPvpMainLayer())
    {
        
        if(m_bStop)
        {
            if(m_fStopTime > 3)
            {
                m_fStopTime -= 3;
                m_bStop = false;
                m_bStartStoped = false;
            }
            else{
                m_fStopTime += dt;
            }
        }
        else{
            if(m_fTime > 5)
            {
                m_bStop = true;
                m_fTime -= 5;
            }
            else{
                m_fTime += dt;
            }
        }
        if(!m_bStartStoped)
        {
            positionUpdate(dt);
        }
       
        roatationUpdate(dt);
    }
    else{
        CBaseFighter::update(dt);
    }
}

void CPartolFight::roatationUpdate(float time)
{
    CBaseFighter::roatationUpdate(time);
}

float CPartolFight::getFightDistance(CBaseFighter* target)
{
    return m_tagetPos.getDistance(target->getFightPosition())-target->getNowRadius();//-getNowRadius();//要减去此人和自身的radius
    
}

void CPartolFight::positionUpdate(float time)
{
    //buildInfo* pTowerInfo = pvpCommonData::getInstance()->getTowerInfo();
//    if(pTowerInfo)
//    {
//        
//    }
    cocos2d::Vector<CBaseFighter*> enemyVec = CFightData::getInstance()->getFightMonstersByGroup(m_iFightGroup);
    bool bExisted = false;
    for(int i = 0; i < enemyVec.size(); i++)
    {
        float distance = getFightDistance(enemyVec.at(i));
        if(fabs(distance) < m_iRadius)//在攻击范围之内
        {
            bExisted = true;
            break;
        }
    }
    if(bExisted)
    {
        m_bIfSearchMode = false;
    }
    else{
        float dist = m_v3SearchPos.distance(getPosition3D());
        
        m_bIfSearchMode=true;
        
        //判断距离，如果到达目标点，则开始下个目标点的寻路
        if (dist<getNowSpeedWithOutFrame(time))
        {
            if(m_bStop)
            {
                stateChange(eFighteState_Idle);
                m_bStartStoped = true;
                return;
            }
            stateChange(eFighteState_Walk);
            Vec3 pos = getValidPosition();
            //                pos = Vec3(pos.x, pos.y + m_iAddHeight, pos.z);
            if(pos != m_v3SearchPos)
            {
                m_v3SearchPos = pos;
            }
        }
    }
    //setAddMoveSpeed(-5);
    CBaseFighter::positionUpdate(time);
}

Vec3 CPartolFight::getValidPosition()
{
    Vec3 destPos;
    if(CPersonalData::getInstance()->getPvpMainLayer())
    {
        buildInfo* pTowerInfo = pvpCommonData::getInstance()->getTowerInfo();
        Vec3 tempPos = pTowerInfo->theTower->getPosition3D();
        float xValue = 0;
        float zValue = 0;
        int index_x;//找出的位置x索引
        int index_y;
        //先算出祭坛所在位置索引
        float index = (pTowerInfo->theTower->getPositionX() - MAP_START_XPOS)/LENGTH_BLOCK_PER;
        index = ((int)(index/0.5f))*0.5;
        float indey = (pTowerInfo->theTower->getPositionZ() - STARTZPOINT)/LENGTH_BLOCK_PER + 9;
        indey = ((int)(indey/0.5f))*0.5;
        m_iRadius = 62 * pTowerInfo->theTower->m_fScale;
        int blockNum = m_iRadius/LENGTH_BLOCK_PER;//计算攻击半径所占方格数
        bool bValid;
        do
        {
            bValid = true;
            index_x = 0;
            index_y = 0;
            //随机获取攻击圈内的位置
            float xPos = (rand()%(2*blockNum) - blockNum);
            if(xPos > 0)
            {
                xPos -= 0.5f;
            }
            else{
                xPos += 0.5f;
            }
            int tempNum = sqrt(blockNum*blockNum - fabs(xPos) * fabs(xPos));
            if(tempNum > 0)
            {
                float zPos = (rand()%(2*tempNum) - tempNum);
                if(zPos > 0)
                {
                    zPos -= 0.5f;
                }
                else{
                    zPos += 0.5f;
                }
                //判断当前放置的位置是否被占用
                index_x = (int)(index + xPos);
                index_y = (int)(indey + zPos);
                if(index_x < 0 || index_y < 1 || index_x >= MAP_X_INDEX || index_y >= MAP_Y_INDEX)
                {
                    //这里以publicDefine.h中定义的z轴最远距离来判断
                    bValid = false;
                }
//                if(pvpCommonData::getInstance()->m_allBlock[index_x][index_y].type == 1)
//                {
//                    bValid = false;
//                }
            }
            
        }while(!bValid);
        xValue = MAP_START_XPOS + index_x * LENGTH_BLOCK_PER;
        zValue = (index_y - 9) * LENGTH_BLOCK_PER + STARTZPOINT;
        log("x坐标suoyin  = %d, y坐标suoyin = %d", index_x, index_y);
        log("x坐标 = %f, y坐标 = %f", xValue, zValue);
        destPos = Vec3(xValue, 1, zValue);
    }
    else{
        if(pvpCommonData::getInstance()->getUpBack())
        {
            if((m_iIndex) >= (int)m_persionPos.size())
            {
                pvpCommonData::getInstance()->setUpBack(false);
                m_iIndex -= 2;
                destPos = m_persionPos[m_iIndex];
            }
            else{
                destPos = m_persionPos[m_iIndex];
                m_iIndex++;
            }
        }
        else{
            if(m_iIndex < 0)
            {
                pvpCommonData::getInstance()->setUpBack(true);
                m_iIndex += 2;
                destPos = m_persionPos[m_iIndex];
            }
            else{
                destPos = m_persionPos[m_iIndex];
                m_iIndex--;
            }
        }

        
    }
    
    return destPos;
}



//void CPartolFight::updatePartolTower(buildInfo* tower)
//{
//    m_towerInfo = tower;
//}

void CPartolFight::setDestationPos(Vec3 pos)
{
    m_v3SearchPos = pos;
}

