//
//  pvpDataManager.cpp
//  WarOfGod
//
//  Created by user on 15/8/10.
//
//

#include "pvpCommonData.h"
#include "zzbNetControlModule.h"
#include "zzbNewNetStruct.h"

pvpCommonData* pvpCommonData::m_singleton = nullptr;

pvpCommonData::pvpCommonData()
{
    m_allBuildObb.clear();
    m_serverTime = 0;
    m_towerInfo = NULL;
    m_bUpBack = true;
    m_bEvolution = false;
    m_bFollowBuild = true;
    //m_pCircleSprite = NULL;
    m_iProtectedTime = 0;
    
    m_iAllBuildNum = 0;
    m_iDestroyedBuildNum = 0;
    
    m_bRemainTime = 0;
    m_bProductTime = 0;
    m_bProductGetStone = 0;
    for(int i = 0; i < MAP_X_INDEX; i++)
    {
        for(int j = 0; j < MAP_Y_INDEX; j++)
        {
            locationInfo pInfo;
            pInfo.sprite = NULL;
            pInfo.type = 0;
            m_allBlock[i][j] = pInfo;
        }
    }
}


pvpCommonData::~pvpCommonData()
{
    
}

pvpCommonData* pvpCommonData::getInstance(){
    
    //判断m_singleton是否已被实例化， 如果没有，将之实例
    
    if(nullptr == m_singleton)
    {
        m_singleton = new pvpCommonData();
    }
    
    return m_singleton;
}

void pvpCommonData::clearAllBuildInfo()
{
    for(int i = 0; i < m_allBuildObb.size(); i++)
    {
        if(m_allBuildObb[i].towerInfo)
        {
            delete m_allBuildObb[i].towerInfo;
            m_allBuildObb[i].towerInfo = NULL;
        }
    }
    m_allBuildObb.clear();
}

void pvpCommonData::parseUserTeamInfo(std::vector<std::string> paramsStr)
{
    clearAllBuildInfo();
    int index = 0;
    int state = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(paramsStr[index++]);
    CPersonalData::getInstance()->setState(state);
    if(state == 0)
    {
        CPersonalData::getInstance()->setPvpMainLayer(true);
        std::string mapStr = _netNewRecvEventBase::parseStrAfterEqualFromInstruction(paramsStr[index++]);
        std::vector<__String> parameters = CDBMgr::getInstance()->separateInfoWithMark(mapStr, ";");
        for (int i = 0;i < (int)parameters.size(); i++)
        {
            std::vector<__String> tempParam = CDBMgr::getInstance()->separateInfoWithMark(parameters[i], ",");
            towerOBBInfo spInfo;
            spInfo.userBuildingId = tempParam[0].intValue();
            spInfo.buildingId = tempParam[1].intValue();
            pvpBuildInfo* pBuildInfo = CDataManager::getBuildInfo(spInfo.buildingId);
            spInfo.xPos = tempParam[2].intValue();
            spInfo.zPos = tempParam[3].intValue();
            spInfo.state = tempParam[4].intValue();
            spInfo.startUpTime = tempParam[5].intValue();
            spInfo.needTime = tempParam[6].intValue();
            spInfo.validTime = 0;
            if((spInfo.state == 2) && (pBuildInfo->level > 1)){
                pBuildInfo = CDataManager::getBuildInfo(spInfo.buildingId - 1);
            }
            
            if(tempParam.size() > 7)
            {
                int addStone = tempParam[7].intValue();
                if((addStone >= 1) && (pBuildInfo->buildingType == 10) && (spInfo.state == 0))
                {
                    spInfo.validTime = -1;
                }
                else {
                    spInfo.validTime = 0;
                }
            }
            buildInfo *bPinfo = new buildInfo();
            FighterInfo theInfo;
            CDataManager::CalcFighterInfoWith(&theInfo, pBuildInfo->monsterId, pBuildInfo->level, 1, 0, false, 1, 0, 0, 0);
            
            //pvpBuildInfo* pInfo = CDataManager::getBuildInfo(spInfo.buildingId);
            bPinfo->pInfo = pBuildInfo;
            bPinfo->towerBase = NULL;
            bPinfo->shadow = NULL;
            spInfo.towerInfo = bPinfo;
            spInfo.theInfo = theInfo;
            m_allBuildObb.push_back(spInfo);
        }
        int stone = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(paramsStr[index++]);
        CPersonalData::getInstance()->setUserKuangStone(stone);
        
        m_serverTime = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(paramsStr[index++]);
        
        m_iProtectedTime = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(paramsStr[index++]);
         
        std::string  bornTime = _netNewRecvEventBase::parseStrAfterEqualFromInstruction(paramsStr[index++]);
        
        int bEvolution = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(paramsStr[index++]);
        pvpCommonData::getInstance()->setEvolution(bEvolution);
        int iPos = bornTime.find(",");
        
        int iBornTime =  atoi(bornTime.substr(iPos+1,bornTime.length()).c_str());
        
         if(iBornTime ==0)
         {
             iBornTime = 60;
         }
    
        CFightData::getInstance()->setTheBornTime(iBornTime);
    
        
    }
}

void pvpCommonData::parseIsEndProductBack(std::vector<std::string> paramsStr)
{
    int index = 0;
    int state = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(paramsStr[index++]);
    CPersonalData::getInstance()->setState(state);
    if(state == 0)
    {
        m_bRemainTime = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(paramsStr[index++]);
        
        m_bProductGetStone = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(paramsStr[index++]);
        
        m_bProductTime = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(paramsStr[index++]);
    }
}

void pvpCommonData::setAllBack(locationInfo allBlock[][MAP_Y_INDEX])
{
    for(int i = 0;i < MAP_X_INDEX; i++)
    {
        for(int j = 0; j < MAP_Y_INDEX; j++)
        {
            m_allBlock[i][j] = allBlock[i][j];
        }
    }
}

