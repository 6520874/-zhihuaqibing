//
//  SkillTouchDetail.cpp
//  WarOfGod
//
//  Created by prefer_shan on 15/5/6.
//
//

#include "SkillTouchDetail.h"
#include "DataManager.h"
#include "DBMgr.h"

bool CSKillTouchDetail::init()
{
    
    CBaseLayer::init();
    
    {
        FighterInfo* fighterInfo  =  m_pfighterInfo;
        
        int iTag =  m_iSkillIndex;
        
        int  iMonsterId = fighterInfo->monsterId;
        int  iRare = fighterInfo->rare;
        
        if(iRare>5)
        {
            iRare = 5;
        }
        
        std::vector<skillInfo*>  m_pskill = CDataManager::getAvailableSkillInfo(iMonsterId,5);

        
        std::vector<heroSkill*> pVec = CDBMgr::getInstance()->getHeroSkillByMonsterId(fighterInfo->monsterId);

    
        if(m_pskill.size()>0||pVec.size()>0)
        {
            skillInfo  pInfo;
            
            if(iMonsterId>1000)
            {
                pInfo.skillId = pVec[iTag]->heroSkillId;
                pInfo.skillName = pVec[iTag]->heroSkillName;
                pInfo.skillIcon =  pVec[iTag]->skillIcon;
                pInfo.skillDes = pVec[iTag]->heroSkillDes;
                
            }
            else
            
            {
                pInfo = *m_pskill[iTag];
            }

            Sprite  *pSKillDetil = Sprite::createWithSpriteFrameName("detail.png");
            pSKillDetil->setScale(3);
            pSKillDetil->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
            addChild(pSKillDetil,999,999);
            
            Node * pNode = Node::create();
            pNode->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
            addChild(pNode,999,998);
            
            Sprite *pSkill ;
            
            if(iMonsterId>1000)
            {
                pSkill =  Sprite::createWithSpriteFrameName(pInfo.skillIcon);
            }
            else
            {
                pSkill = CDataManager::getSkillIcoWithId(&pInfo);
            }
            pSkill->setScale(2);
            
            Size tSkill = pSKillDetil->getBoundingBox().size;
            pSkill->setPosition(-tSkill.width*0.475,tSkill.height*0.25);
            pSkill->setAnchorPoint(Vec2(0,0.5));
            pNode->addChild(pSkill);
            
            std::string strName = pInfo.skillName;
            
            Label *pName = Label::createWithTTF(strName.c_str(),"fonts/cuti.ttf",30);
            pName->setColor(Color3B(249,109,2));
            pName->setAnchorPoint(Vec2(0,0.5));
            pName->setPosition(-tSkill.width*0.3,tSkill.height*0.3);
            pNode->addChild(pName);
            
            int iLv =  0;
            
            if(fighterInfo->theFighterSkillInfo.size()>0)
            {
               iLv = fighterInfo->theFighterSkillInfo[iTag].skillLevel;
            }
            else
            {
                iLv = 1;
            }
    
        
            
            char strNum[300] = {0};
            //atk
            
            int iposd = pInfo.skillDes.find("%d");
            int iposf  = pInfo.skillDes.find("%.2f");
            
            if(pInfo.skillId == 9)//巨石皮肤
            {
                strcpy(strNum, pInfo.skillDes.c_str());
            }
            
            float fRatio = 1;
             if(fighterInfo->theFighterSkillInfo.size()>0)
             {
                 fRatio =  m_pfighterInfo->theFighterSkillInfo[iTag].damageRatio;

             }
            
            if(iposd>0)
            {
                int  iAtk =  m_pfighterInfo->attk;
                int  fAtkNum = iAtk*fRatio;
                sprintf(strNum,pInfo.skillDes.c_str(),fAtkNum);
            }
            
            if(iposf>0)
            {
                float fDamger =  CDataManager::getTheDamageRatioWithIdAndLevel(pInfo.skillId,iLv);
                
                if(pInfo.skillId== eAllSkills_reborn||pInfo.skillId == eAllSkills_silinqiyue)
                {
                    fDamger = CDataManager::getTheStartRatioWithIdAndLevel(pInfo.skillId,iLv);
                }
                
                float fRatioPer = fDamger*100;
            
                if(pInfo.skillId == eAllSkills_Qiannenjifa)
                {
                    sprintf(strNum,pInfo.skillDes.c_str(),fRatioPer,fRatioPer);
                }
                else
                {
                    sprintf(strNum,pInfo.skillDes.c_str(),fRatioPer);
                }
            }
            
            
            if(iMonsterId>1000)
            {
                int iHeroId = pInfo.skillId;
                
                iLv  = CDBMgr::getInstance()->GetHeroSkillLvById(iHeroId);
                
                float fHurtNum = CDataManager::getTheDamageWithHeroKillIdAndLevel(iHeroId,iLv);
                
                int iHurtNum = 0;
                
                if(iHeroId == 3)
                {
                    iHurtNum = fHurtNum*m_pfighterInfo->attk;
                }
                else
                {
                    iHurtNum = fHurtNum;
                }
                
                float fTime =  CDataManager::getTheLastTimeWithIdAndLevel(iHeroId,iLv);
                
                char strTime[50] = {0};
                
                sprintf(strTime,"%.2f",fTime);
                
              
                
                if(iHeroId==2)
                {
                    sprintf(strNum,pInfo.skillDes.c_str(),iHurtNum,strTime);
                }
                else if(iHeroId == 3)
                {
                    sprintf(strNum,pInfo.skillDes.c_str(),iHurtNum);
                }
                else
                {
                    sprintf(strNum,pInfo.skillDes.c_str(),strTime);
                }
            
                
            }
          
            char strLv[20] ={0};
            sprintf(strLv,"LV:%d",iLv);
            
            Label *pLv = Label::createWithTTF(strLv,"fonts/cuti.ttf",30);
            pLv->setAlignment(TextHAlignment::LEFT);
            pLv->setColor(Color3B(107,255,232));
            pLv->setPosition(-tSkill.width*0.2,tSkill.height*0.15);
            pNode->addChild(pLv);
            
            Label *pDesc = Label::createWithTTF(strNum,"fonts/cuti.ttf",20,Size(pSKillDetil->getBoundingBox().size.width*0.9,0));
            
            pDesc->setAnchorPoint(Vec2(0,0.5));
            pDesc->setPosition(-tSkill.width*0.45,pLv->getPositionY()-pLv->getBoundingBox().size.width);
            pDesc->setColor(Color3B(107,255,232));
            pNode->addChild(pDesc);
        
        }
        
    }
    
    return true;
}

CSKillTouchDetail::CSKillTouchDetail(FighterInfo* pInfo,int iSkillIndex)
{
    m_pfighterInfo = pInfo;
    m_iSkillIndex = iSkillIndex;
}

CSKillTouchDetail::~CSKillTouchDetail()
{


}


