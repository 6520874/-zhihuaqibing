//
//  CSKillLVUpQueen.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/12/29.
//
//

#include "CSKillLVUpQueen.h"

CSkillLvQueen *CSkillLvQueen::t = NULL;


CSkillLvQueen::CSkillLvQueen()
{
    
}


CSkillLvQueen::~CSkillLvQueen()
{
    std::vector<SkillLvUp_s*>::iterator it = m_pSkillLvUp_Vec.begin();
    
    for(int i=0;i<m_pSkillLvUp_Vec.size();i++,it++)
    {
        delete *it;
        *it=  NULL;
    }
    
    m_pSkillLvUp_Vec.clear();
}


std::vector<CSkillLvQueen::SkillLvUp_s*> CSkillLvQueen::GetASkillUp()
{
    return m_pSkillLvUp_Vec;
}


void CSkillLvQueen::addASkillUp(int index,int iSkillId,int iMonsterId,int iTime)
{
    char strSKillId[100] = {0};
    sprintf(strSKillId,"%d_%d",iSkillId,iMonsterId);
    
    SkillLvUp_s *p = new SkillLvUp_s();
    p->iOnlyId = index;
    p->sSkillId = strSKillId;
    p->iCostTime = iTime;
    m_pSkillLvUp_Vec.push_back(p);
}


void CSkillLvQueen::delASkillUp(std::string strSkillId)
{
    std::vector<SkillLvUp_s*>::iterator it = m_pSkillLvUp_Vec.begin();
    
    for(;it!=m_pSkillLvUp_Vec.end();)
    {
        if((*it)->sSkillId == strSkillId )
        {
            delete *it;
            m_pSkillLvUp_Vec.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void  CSkillLvQueen::delASkillUp(int index)
{
    std::vector<SkillLvUp_s*>::iterator it = m_pSkillLvUp_Vec.begin();
    
    for(;it!=m_pSkillLvUp_Vec.end();)
    {
        if((*it)->iOnlyId == index)
        {
            delete *it;
            m_pSkillLvUp_Vec.erase(it);
        }
        else
        {
            it++;
        }
    }

}
