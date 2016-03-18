//
//  CSKillLVUpQueen.h
//  TheGodWill
//
//  Created by prefer_shan on 14/12/29.
//
//

#ifndef __TheGodWill__CSKillLVUpQueen__
#define __TheGodWill__CSKillLVUpQueen__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "AllDefine.h"

USING_NS_CC;

class CSkillLvQueen:public Node
{
public:
    struct SkillLvUp_s
    {
        int  iOnlyId;
        std::string  sSkillId;
        int  iCostTime;
        int  iSkillToLv;
    };
    
    static CSkillLvQueen* GetSingletonPtr()
    {
        if (!t)
        {
            t = new CSkillLvQueen();
            
        }
        return t;
    }
    
    void addASkillUp(int index,int iSkillId,int iMonsterId =0,int iTime=100);
    void delASkillUp(std::string strSkillId);
    void delASkillUp(int index);
    
    std::vector<SkillLvUp_s*> GetASkillUp();
    
    
    //解析技能升级道具
    //void ResolveLvProp(std::string strNeedItem);
private:
   

    CSkillLvQueen();
    ~CSkillLvQueen();
    
    std::vector<SkillLvUp_s*> m_pSkillLvUp_Vec;
    
    static CSkillLvQueen *t;
};

#endif /* defined(__TheGodWill__CSKillLVUpQueen__) */
