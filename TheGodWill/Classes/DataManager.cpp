//
//  DataManager.cpp
//  TheGodWill
//
//  Created by user on 14/12/16.
//
//

#include "DataManager.h"

#include "DBMgr.h"
#include "LYFightData.h"


modelInfo* CDataManager::getModelInfoByModelId(int modelId){
    
    
    std::vector<modelInfo*>* theModleInfo = &CDBMgr::getInstance()->theModleInfo;
    
    for (std::vector<modelInfo*>::iterator ite = theModleInfo->begin(); ite!=theModleInfo->end(); ite++) {
        
        modelInfo* temp = *ite;
        
        if (temp->modelId==modelId) {
            return temp;
        }
        
    }
    
    return NULL;
}

MonsterEvolutionInfo* CDataManager::getMonsterEvolutionById(int rare)
{
    std::vector<MonsterEvolutionInfo>* theModleInfo = &CDBMgr::getInstance()->m_pMonsterEvoltionInfo;
    for (int i = 0; i < theModleInfo->size(); i++) {
        if (theModleInfo->at(i).m_star == rare) {
            return &theModleInfo->at(i);
        }
        
    }
    return NULL;
}

MonsterAllEvolutionInfo CDataManager::getAllEvolutionInfoByRare(int rare){
    
    std::vector<MonsterEvolutionInfo>* theModleInfo = &CDBMgr::getInstance()->m_pMonsterEvoltionInfo;
    
    MonsterAllEvolutionInfo target;
    
    for (int i=0; i<theModleInfo->size(); i++) {
        
        if (theModleInfo->at(i).m_star<=rare) {
            
            target.m_allAddAtk+=theModleInfo->at(i).m_addAtk;
            target.m_allAddHP+=theModleInfo->at(i).m_addHp;
            
        }
        
    }
    
    
    return target;
    
}






std::vector<monsterInfo*> CDataManager::getAllMonsterInfo()
{
    return CDBMgr::getInstance()->theMonsterInfo;
}



Sprite *CDataManager::getMonsterInfodebris(int monsterId)
{
    //std::string headIcon[5]={"white_bg.png","green_bg.png","blue_bg.png", "purple_bg.png","gold_bg.png"};
    
    Sprite* headSp = Sprite::createWithSpriteFrameName("debrisBg.png");

    LayerColor *pColor = LayerColor::create();
    pColor->setColor(Color3B::BLACK);
    pColor->setOpacity(255);
    pColor->setContentSize(headSp->getContentSize());
    
    headSp->addChild(pColor,-2);
    
    pColor->setPosition(Vec2(0, 0));

    
    monsterInfo* pInfo= getMonsterInfoByMonsterId(monsterId);
    Sprite* pIcon = Sprite::createWithSpriteFrameName(pInfo->monsterIcon.c_str());
    pIcon->setScale(0.3);
    
    headSp->addChild(pIcon,-1);
    
    pIcon->setPosition(Vec2(headSp->getContentSize().width / 2, headSp->getContentSize().height / 2));
    
    
    return  headSp;
}

Sprite* CDataManager::getMonsterIconAndBg(int monsterId, bool isShowStar,float fMonsterScale)
{
    char buf[128] =  {0};
    std::string headIcon[5]={"white_bg.png","green_bg.png","blue_bg.png", "purple_bg.png","gold_bg.png"};
    int rare = getMonsterStarById(monsterId);
    
    if(rare > 0)
    {
        rare -= 1;
    }
    
    Sprite* headSp = Sprite::createWithSpriteFrameName(headIcon[rare].c_str());

    Size tHead = headSp->getContentSize();
    
    monsterInfo* pInfo= getMonsterInfoByMonsterId(monsterId);
    Sprite* pIcon = Sprite::createWithSpriteFrameName(pInfo->monsterIcon.c_str());
    pIcon->setScale(fMonsterScale);
    
    headSp->addChild(pIcon);
    
    pIcon->setPosition(Vec2(headSp->getContentSize().width / 2, headSp->getContentSize().height / 2));
    
    if(isShowStar)
    {
        int rare = CDataManager::getMonsterStarById(monsterId);
        for(int i = 0;i < rare; i++)
        {
            if(rare == 1){
                Sprite* star = Sprite::createWithSpriteFrameName("advance_star.png");
                pIcon->addChild(star, 5);
                star->setPosition(Vec2(pIcon->getContentSize().width / 2, pIcon->getContentSize().height /*- star->getContentSize().height * 0.32*/));
            }
            else if(rare == 2){
                Sprite* star = Sprite::createWithSpriteFrameName("advance_star.png");
                pIcon->addChild(star, 5);
                star->setPosition(Vec2(pIcon->getContentSize().width *0.4 + i*30 , pIcon->getContentSize().height));
            }
            else if(rare == 3){
                Sprite* star = Sprite::createWithSpriteFrameName("advance_star.png");
                pIcon->addChild(star, 5);
                star->setPosition(Vec2(pIcon->getContentSize().width *0.31 + i*25 , pIcon->getContentSize().height));
            }
            else if(rare == 4){
                Sprite* star = Sprite::createWithSpriteFrameName("advance_star.png");
                pIcon->addChild(star, 5);
                star->setPosition(Vec2(pIcon->getContentSize().width *0.268 + i*20 , pIcon->getContentSize().height));
            }
            else if(rare == 5){
                Sprite* star = Sprite::createWithSpriteFrameName("advance_star.png");
                pIcon->addChild(star, 5);
                star->setPosition(Vec2(pIcon->getContentSize().width *0.225 + i*18 , pIcon->getContentSize().height));
            }
        }
    }
    
    
    return headSp;
}

monsterInfo* CDataManager::getMonsterInfoByMonsterId(int monsterId){
    
    std::vector<monsterInfo*>* theModleInfo = &CDBMgr::getInstance()->theMonsterInfo;
    
    for (std::vector<monsterInfo*>::iterator ite = theModleInfo->begin(); ite!=theModleInfo->end(); ite++) {
        
        monsterInfo* temp = *ite;
        
        if (temp->monsterId==monsterId) {
            return temp;
        }
        
    }
    
    return NULL;
}



//计算相应的士兵属性
void CDataManager::CalcFighterInfoWith(FighterInfo *targetInfo, int monsterId,int monsterLevel, int rare,int theType, int ifFight, int skillLevel1,int skillLevel2,int skillLevel3,int skillLevel4){
    
    
    //获得士兵模板信息
    monsterInfo* templetInfo = getMonsterInfoByMonsterId(monsterId);
    targetInfo->theMonsterInfo = templetInfo;
    
    targetInfo->monsterId = monsterId;
    targetInfo->monsterType = templetInfo->monsterType;
    
    targetInfo->theModelInfo = getModelInfoByModelId(templetInfo->modelId);
    
    targetInfo->walkSpeed = templetInfo->walkSpeed;
    targetInfo->attackCD = templetInfo->attackCD;
    targetInfo->attackRang = templetInfo->attackRange;
    targetInfo->energyCost = templetInfo->energyCost;
    
    targetInfo->ifFight = ifFight;
    
    targetInfo->theType = theType;
    targetInfo->rare = rare;
    
    targetInfo->attk = getTheNumWithMinMax(1,templetInfo->maxLevel,templetInfo->minAtk,templetInfo->maxAtk,monsterLevel);
    targetInfo->maxHP = getTheNumWithMinMax(1,templetInfo->maxLevel,templetInfo->minHp,templetInfo->maxHp,monsterLevel);
    
    //加入星级影响
    MonsterAllEvolutionInfo temp= CDataManager::getAllEvolutionInfoByRare(targetInfo->rare);
    targetInfo->attk+=temp.m_allAddAtk;
    targetInfo->maxHP+=temp.m_allAddHP;
    
    
    targetInfo->level = monsterLevel;
    
    
    int tempSkillLevel[4] = {skillLevel1,skillLevel2,skillLevel3,skillLevel4};
    
    
    //测试用
    //    tempSkillLevel[0]=20;
    //    tempSkillLevel[1]=20;
    //    tempSkillLevel[2]=20;
    //    tempSkillLevel[3]=20;
    
    
    //初始化技能信息
    targetInfo->theFighterSkillInfo.clear();
    for (int i=0; i<templetInfo->allSkill.size(); i++) {
        
        FighterSkillInfo temp =CalcFightSkillInfoWithIdAndLevel(templetInfo->allSkill[i],tempSkillLevel[i]) ;
        
//        temp.theSkillInfo = CDataManager::getTheSkillInfoWithId(templetInfo->allSkill[i]);
        
        if (temp.theSkillInfo) {
            
//            temp.skillLevel = tempSkillLevel[i];
//            
//            temp.theSkillEffectInfo = CDataManager::getTheSkillEffectWithId(temp.theSkillInfo->skillEffectId);
//            
//            temp.damageRatio = getTheNumWithMinMax(1, temp.theSkillInfo->maxLevel, temp.theSkillInfo->damageRatioMin, temp.theSkillInfo->damageRatioMax, tempSkillLevel[i]);
//            
//            temp.startRatio = getTheNumWithMinMax(1, temp.theSkillInfo->maxLevel, temp.theSkillInfo->startRatioMin, temp.theSkillInfo->startRatioMax, tempSkillLevel[i]);
            
            targetInfo->theFighterSkillInfo.push_back(temp);
            
            //加入被动技能影响。
            if(temp.theSkillInfo->startCondition==-1)
            {
                
                    
                    if(temp.theSkillInfo->skillId == eAllSkills_shechenlong)
                    {
                        targetInfo->attackRang += targetInfo->attackRang*temp.damageRatio;
                    }
                    else if(temp.theSkillInfo->skillId == eAllSkills_yuansuqiyue)
                    {
                      targetInfo->attk += targetInfo->attk*temp.damageRatio;
                    }
                    else
                    {
                        if(temp.theSkillInfo->skillId == eAllSkills_Qiannenjifa)
                        {
                            targetInfo->attk += targetInfo->attk*temp.damageRatio;
                            targetInfo->maxHP += targetInfo->maxHP*temp.damageRatio;
                        }
                    }
              
                
            }
            
        }
        
        
    }
    
    
    
    
    
    
    
    //获得士兵子弹信息
    targetInfo->theBulletInfo =  getBulletInfoByBulletId(templetInfo->bulletId);
    
    
    
}

FighterSkillInfo CDataManager::CalcFightSkillInfoWithIdAndLevel(int monsterId,int skillLevel){
    
    FighterSkillInfo temp;
    temp.theSkillInfo = CDataManager::getTheSkillInfoWithId(monsterId);
    
    if (temp.theSkillInfo) {
        
        temp.skillLevel = skillLevel;
        
        temp.theSkillEffectInfo = CDataManager::getTheSkillEffectWithId(temp.theSkillInfo->skillEffectId);
        
        temp.damageRatio = getTheNumWithMinMax(1, temp.theSkillInfo->maxLevel, temp.theSkillInfo->damageRatioMin, temp.theSkillInfo->damageRatioMax, skillLevel);
        
        temp.startRatio = getTheNumWithMinMax(1, temp.theSkillInfo->maxLevel, temp.theSkillInfo->startRatioMin, temp.theSkillInfo->startRatioMax, skillLevel);
        
        
    }

    return temp;
    
}




void CDataManager::getTheAddAtrWith(monsterInfo *targetInfo, int *addHP, int *addAtk){
    
    if (targetInfo&&addHP&&addAtk) {
        *addAtk = (targetInfo->maxAtk-targetInfo->minAtk)/(targetInfo->maxLevel-1);
        
        *addHP = (targetInfo->maxHp-targetInfo->minHp)/(targetInfo->maxLevel-1);
    }
    
    
}





bulletInfoDB* CDataManager::getBulletInfoByBulletId(int bulletId){
    
    
    std::vector<bulletInfoDB*>* theModleInfo = &CDBMgr::getInstance()->theBulletInfo;
    
    for (std::vector<bulletInfoDB*>::iterator ite = theModleInfo->begin(); ite!=theModleInfo->end(); ite++) {
        
        bulletInfoDB* temp = *ite;
        
        if (temp->bulletId==bulletId) {
            return temp;
        }
        
    }
    
    return NULL;
    
}







float CDataManager::getTheNumWithMinMax(int minLevel, int maxLevel, float minNum, float maxNum, int nowLevel){
    
    float targetValue=0;
    

    if (nowLevel==minLevel) {
        return minNum;
    }
    
    if(maxLevel==minLevel){
        return nowLevel*minNum;
    }
    
    
    targetValue = (maxNum-minNum)/(maxLevel-minLevel)*(nowLevel-minLevel)+minNum;
    
    return targetValue;
}

std::vector<mapInfo*> CDataManager::getTheMapInfoWithId(int mapId){
    
    std::vector<mapInfo*> target;
    
    std::vector<mapInfo*>* theVec = &CDBMgr::getInstance()->theMapInfo;
    
    for (std::vector<mapInfo*>::iterator ite = theVec->begin(); ite!=theVec->end(); ite++) {
        
        mapInfo* temp = *ite;
        
        if (temp->mapId==mapId) {
            target.push_back(temp);
        }
        
    }
    
    return target;
}

levelInfo* CDataManager::getTheLevelInfoWithId(int levelId){
    
    std::vector<levelInfo*>* theVec = &CDBMgr::getInstance()->theLevelInfo;
    
    for (std::vector<levelInfo*>::iterator ite = theVec->begin(); ite!=theVec->end(); ite++) {
        
        levelInfo* temp = *ite;
        
        if (temp->levelId==levelId) {
            return temp;
        }
        
    }
    
    return NULL;
    
}


skillInfo* CDataManager::getTheSkillInfoWithId(int skillId)
{
    std::vector<skillInfo*>* theVec = &CDBMgr::getInstance()->theSkillInfo;
    
    for (std::vector<skillInfo*>::iterator ite = theVec->begin(); ite!=theVec->end(); ite++) {
        
        skillInfo* temp = *ite;
        
        if (temp->skillId==skillId)
        {
            return temp;
        }
        
    }
    
    return NULL;
}


void  CDataManager::InitWithSkillId(Sprite*pBg,skillInfo *pInfo)
{
    pBg->removeAllChildren();
    //pBg->initWithFile("soldierUp/sBg.png");
    
    Size tBg = pBg->getContentSize();
    
    char strSkillBuf[50] = {0};
    sprintf(strSkillBuf,"%s",pInfo->skillIcon.c_str());
    
    Sprite *pSkillBuf = Sprite::createWithSpriteFrameName(strSkillBuf);
    pSkillBuf->setPosition(tBg.width*0.5,tBg.width*0.5);
    pBg->addChild(pSkillBuf,0,1);
    
}

fightBuildingInfo* CDataManager::getTheFightBuildingInfoWithId(int monsterId){
    
    std::vector<fightBuildingInfo*>* theVec = &CDBMgr::getInstance()->m_vecFightBuildingInfo;
    
    for (std::vector<fightBuildingInfo*>::iterator ite = theVec->begin(); ite!=theVec->end(); ite++) {
        
        fightBuildingInfo* temp = *ite;
        
        if (temp->monsterId==monsterId)
        {
            return temp;
        }
        
    }
    
    return NULL;
    
}

std::vector<fightBuildingInfo*>* CDataManager::getAllFightBuildingInfo(){
    
    return &CDBMgr::getInstance()->m_vecFightBuildingInfo;
    
}


fightTalkingInfo* CDataManager::getTheFightTalkingInfoWithId(int id){
    
    std::vector<fightTalkingInfo*>* theVec = &CDBMgr::getInstance()->m_vecFightTalkingInfo;
    
    for (std::vector<fightTalkingInfo*>::iterator ite = theVec->begin(); ite!=theVec->end(); ite++) {
        
        fightTalkingInfo* temp = *ite;
        
        if (temp->talkId==id)
        {
            return temp;
        }
        
    }
    
    return NULL;
    
    
}

std::vector<fightTalkingInfo*> CDataManager::getFightTalkingInfoByType(int type){
    
    std::vector<fightTalkingInfo*> skillVec;
    
    std::vector<fightTalkingInfo*>* theVec = &CDBMgr::getInstance()->m_vecFightTalkingInfo;
    
    for (std::vector<fightTalkingInfo*>::iterator ite = theVec->begin(); ite!=theVec->end(); ite++) {
        
        fightTalkingInfo* temp = *ite;
        
        if (temp->type==type)
        {
            skillVec.push_back(temp);
        }
        
    }
    return skillVec;
    
}

fightTalkingInfo* CDataManager::getTheRandomFightTalkingInfoByType(int type){
    
    std::vector<fightTalkingInfo*> skillVec = getFightTalkingInfoByType(type);
    
    
    fightTalkingInfo* theInfo = NULL;
    
    if (skillVec.size()>0) {
        int theNum = rand()%skillVec.size();
        theInfo = skillVec.at(theNum);
    }

    return theInfo;
}




cocos2d::Sprite * CDataManager::getSkillIcoWithId(std::string strImg,float fScale, const Color3B& color)
{
    Sprite *pBg = Sprite::createWithSpriteFrameName("jnbg.png");
    
    Size tBg = pBg->getBoundingBox().size;
    
    Size s = Director::getInstance()->getWinSize();
    //创建裁剪node
    ClippingNode *clipper = ClippingNode::create();
    
    //设置alpha值
    clipper->setAlphaThreshold(1);
    
    DrawNode *stencil = DrawNode::create();
    stencil->setAnchorPoint(Point(0.5,0.5));
    //多边形定点个数;
    Vec2 points[720];
    //圆心，相对坐标;
    Vec2 center(0,0);
    //半径;
    
    float R = tBg.height*0.42;
    
    /**普及下求圆坐标的方法
     * 已经半径r、圆心坐标(cX,cY)、圆心偏移角度a，求出其相应偏移角度的坐标(X1,Y1);
     * X1 = cX + cos(a*3.14/180)*r;
     * Y1 = cY + sin(a*3.14/180)*r;
     */
    // 画空心圆->使用多边形画,分成720等分;
    // 锯齿蛮明显，不知道怎么消除锯齿;
    
    for( int i=0; i < 720; i++)
    {
        float x = center.x + cos(0.5*i*3.14/180) * R;
        float y = center.y + sin(0.5*i*3.14/180) * R;
        points[i] = Vec2(x,y);
        //log("%f,%f", x, y);
    }
    
    //画多边形;
    stencil->drawPolygon(points, sizeof(points)/sizeof(points[0]), Color4F(0,1,0,0), 1, Color4F(1,0,0,1));
    Size tClipSz = clipper->getBoundingBox().size;
    stencil->setPosition(tClipSz.width*0.5,tClipSz.height*0.5);
    clipper->setStencil(stencil);
    
    char strSkillBuf[50] = {0};
    sprintf(strSkillBuf,"%s",strImg.c_str());

    Sprite *pSkillBuf = Sprite::createWithSpriteFrameName(strSkillBuf);
    pSkillBuf->setScale(fScale);

    pSkillBuf->setColor(color);

    pSkillBuf->setPosition(tClipSz.width*0.5,tClipSz.height*0.5);
    clipper->addChild(pSkillBuf,0,1);
    clipper->setPosition(tBg.width*0.5,tBg.height*0.51);
    pBg->addChild(clipper,0,1);
    

    return  pBg;


}


cocos2d::Sprite * CDataManager::getSkillIcoWithId(skillInfo *pInfo,float fScale,bool bDisable)
{
    //soldierUp/sBg.png
    //common/jnbg.png
    
    Sprite *pBg = Sprite::createWithSpriteFrameName("jnbg.png");
    
    Size tBg = pBg->getBoundingBox().size;
    
    Size s = Director::getInstance()->getWinSize();
    //创建裁剪node
    ClippingNode *clipper = ClippingNode::create();
    
    //设置alpha值
    clipper->setAlphaThreshold(1);
    
    DrawNode *stencil = DrawNode::create();
    stencil->setAnchorPoint(Point(0.5,0.5));
    //多边形定点个数;
    Vec2 points[720];
    //圆心，相对坐标;
    Vec2 center(0,0);
    //半径;
    
    float R = tBg.height*0.42;
    
    /**普及下求圆坐标的方法
     * 已经半径r、圆心坐标(cX,cY)、圆心偏移角度a，求出其相应偏移角度的坐标(X1,Y1);
     * X1 = cX + cos(a*3.14/180)*r;
     * Y1 = cY + sin(a*3.14/180)*r;
     */
    // 画空心圆->使用多边形画,分成720等分;
    // 锯齿蛮明显，不知道怎么消除锯齿;
    
    for( int i=0; i < 720; i++)
    {
        float x = center.x + cos(0.5*i*3.14/180) * R;
        float y = center.y + sin(0.5*i*3.14/180) * R;
        points[i] = Vec2(x,y);
        //log("%f,%f", x, y);
    }
    
    //画多边形;
    stencil->drawPolygon(points, sizeof(points)/sizeof(points[0]), Color4F(0,1,0,0), 1, Color4F(1,0,0,1));
    Size tClipSz = clipper->getBoundingBox().size;
    stencil->setPosition(tClipSz.width*0.5,tClipSz.height*0.5);
    clipper->setStencil(stencil);
    
    char strSkillBuf[50] = {0};
    sprintf(strSkillBuf,"%s",pInfo->skillIcon.c_str());
    Sprite *pSkillBuf = Sprite::createWithSpriteFrameName(strSkillBuf);
    pSkillBuf->setScale(0.6);
    pSkillBuf->setPosition(tClipSz.width*0.5,tClipSz.height*0.5);
    clipper->addChild(pSkillBuf,0,1);
    
    if(bDisable)
    {
        pSkillBuf->setColor(Color3B::GRAY);
    }
    
    clipper->setPosition(tBg.width*0.5,tBg.height*0.51);
    pBg->addChild(clipper,0,1);
    return  pBg;
}

skillEffect* CDataManager::getTheSkillEffectWithId(int skillEffectId)
{
    
    std::vector<skillEffect*>* theVec = &CDBMgr::getInstance()->theSkillEffectInfo;
    
    for (std::vector<skillEffect*>::iterator ite = theVec->begin(); ite!=theVec->end(); ite++) {
        
        skillEffect* temp = *ite;
        
        if (temp->skillEffectId==skillEffectId) {
            return temp;
        }
        
    }
    
    
    return NULL;
}

std::vector<skillInfo*> CDataManager::getAvailableSkillInfo(int monsterId, int rare)
{
    std::vector<skillInfo*> skillVec;
    if(rare != 0){
        //获取英雄的基本信息
        monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(monsterId);
        //获取当前选中英雄的进阶信息
        MonsterEvolutionInfo* pEvoInfoBefore = CDataManager::getMonsterEvolutionById(rare);//获取进阶前的怪物技能数
        if(pMonsterInfo->allSkill.size() < pEvoInfoBefore->m_skillNum)//如果怪物本身技能数不足时
        {
            for (int i = 0; i < pMonsterInfo->allSkill.size(); i++) {
                skillInfo* pInfo = CDataManager::getTheSkillInfoWithId(pMonsterInfo->allSkill.at(i));
                skillVec.push_back(pInfo);
            }
        }
        else{
            for (int i = 0; i < pEvoInfoBefore->m_skillNum; i++) {
                skillInfo* pInfo = CDataManager::getTheSkillInfoWithId(pMonsterInfo->allSkill.at(i));
                skillVec.push_back(pInfo);
            }
        }
    }
    return skillVec;
}

std::vector<skillInfo*> CDataManager::getUnavailableSkillInfo(int monsterId, int rare)
{
    std::vector<skillInfo*> skillVec;
    //获取英雄的基本信息
    //判断当前用户是否已有该英雄
    bool isHave = false;
    monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(monsterId);
    if(rare > 0)
    {
        std::vector<FighterInfo*> userMonterVec = CPersonalData::getInstance()->m_vecMyFighterInfo;
        MonsterEvolutionInfo* pEvoInfoBefore = CDataManager::getMonsterEvolutionById(rare);//获取进阶前的怪物技能数
        for(int i = 0;i < userMonterVec.size(); i++)
        {
            if(userMonterVec.at(i)->monsterId == monsterId)
            {
                isHave = true;
                int index = 0;
                if(pMonsterInfo->allSkill.size() > pEvoInfoBefore->m_skillNum)//如果怪物本身技能数不足时
                {
                    index = pEvoInfoBefore->m_skillNum;
                }
                for(; index < pMonsterInfo->allSkill.size(); index++)
                {
                    int skillId = pMonsterInfo->allSkill.at(index);
                    skillVec.push_back(CDataManager::getTheSkillInfoWithId(skillId));
                }
                break;
            }
        }
        
    }
    if(!isHave){
        for (int j = 0; j < pMonsterInfo->allSkill.size(); j++)
        {
            int skillId = pMonsterInfo->allSkill.at(j);
            skillVec.push_back(CDataManager::getTheSkillInfoWithId(skillId));
        }
    }
    return skillVec;
}


SkillUpgradeInfo* CDataManager::getSkillUpgradeInfowithId(FighterType  eMonsterType)
{
    std::vector<SkillUpgradeInfo*> pskillInfo = CDBMgr::getInstance()->GetSkillUpgradeInfo();
    
    for(int i=0;i<pskillInfo.size();i++)
    {
        if(pskillInfo[i]->m_eMonsterType == eMonsterType)
        {
            return  pskillInfo[i];
        }
    }
    
    return NULL;
}


SkillUpgradeInfo * CDataManager::getSkillUpgradeInfowithId(FighterType eMonsterType,int iSkillLevel)
{
    std::vector<SkillUpgradeInfo*> pskillInfo = CDBMgr::getInstance()->GetSkillUpgradeInfo();

    for(int i=0;i<pskillInfo.size();i++)
    {
        if(pskillInfo[i]->m_eMonsterType == eMonsterType && pskillInfo[i]->iSkillLevel == iSkillLevel)
        {
            return pskillInfo[i];
        }
    }
    return  NULL;
}


heroSkill* CDataManager::getTheHeroSkillWithId(int heroSkillId){
    
    std::vector<heroSkill*>* theVec = &CDBMgr::getInstance()->theHeroSkillInfo;
    
    for (std::vector<heroSkill*>::iterator ite = theVec->begin(); ite!=theVec->end(); ite++) {
        
        heroSkill* temp = *ite;
        
        if (temp->heroSkillId==heroSkillId) {
            return temp;
        }
        
    }
    
    
    return NULL;
    
    
    
    
    
}

int CDataManager::getMidLevelAwardById(int midLevel, bool ifEqlite)
{
    return CDBMgr::getInstance()->getMidLevelAwardById(midLevel, ifEqlite);
}

int CDataManager::getAllLevelStarById(int midLevelId, bool ifEqlite)
{
    return CDBMgr::getInstance()->getAllLevelStarById(midLevelId, ifEqlite);
}

int CDataManager::getAllLevelById(int midLevelId)
{
    return CDBMgr::getInstance()->getAllLevelById(midLevelId);
}

std::string CDataManager::getImgByLevelId(int levelId)
{
    std::string levelIcon[6] = {"jidi.png","hus.png","shuliang.png","fangs.png","sug.png","boss.png"};
    levelInfo* pSmallLevelInfo = CDataManager::getTheLevelInfoWithId(levelId);
    if(pSmallLevelInfo)
    {
        return levelIcon[pSmallLevelInfo->levelCondition];
    }
    return "";
}

int CDataManager::getStarBySmallLevelId(int smallLevelId, bool ifEqlite)
{
    return CDBMgr::getInstance()->getStarBySmallLevelId(smallLevelId, ifEqlite);
}

void CDataManager::insertIntoMonsterInfo(int monsterId, int num, int rare, int ifFight)
{
    CDBMgr::getInstance()->insertIntoMonsterInfo(monsterId, num, rare, ifFight);
}

void CDataManager::setMonsterStarById(int monsterId)
{
    bool isExisted = false;
    std::vector<FighterInfo*>::iterator it = CPersonalData::getInstance()->m_vecMyFighterInfo.begin();
    for(; it != CPersonalData::getInstance()->m_vecMyFighterInfo.end(); it++)
    {
        if ((*it)->monsterId == monsterId) {
            
            (*it)->rare += 1;
            isExisted = true;
            break;
        }
    }
}

int CDataManager::getMonsterStarById(int monsterId)
{
    int rare = 0;
    std::vector<FighterInfo*> userMonterVec = CPersonalData::getInstance()->m_vecMyFighterInfo;
    for(int i = 0; i < userMonterVec.size(); i++)
    {
        if(userMonterVec.at(i)->monsterId == monsterId)
        {
            rare = userMonterVec.at(i)->rare;
            break;
        }
    }
    return rare;
}

Sprite* CDataManager::getSpriteIconAndKuangById(int monsterId)
{
    std::string icon;
    std::vector<monsterInfo*> allMonster = getAllMonsterInfo();
    for (int i = 0; i < allMonster.size(); i++) {
        if (allMonster.at(i)->monsterId == monsterId) {
            icon = allMonster.at(i)->monsterIcon;
        }
    }
    Sprite* bg = Sprite::createWithSpriteFrameName("smallkuang.png");
    Sprite* heroIcon = Sprite::createWithSpriteFrameName(icon.c_str());
    bg->addChild(heroIcon);
    return bg;
}

void CDataManager::judgeUserIfUpgrade(Node * pTarget, int exp,int *resultAddStamina)
{
    CDBMgr::getInstance()->judgeUserIfUpgrade(pTarget, exp,resultAddStamina);
}

upgradeInfo* CDataManager::getUpgradeInfoByLevel(int level)
{
    return CDBMgr::getInstance()->getUpgradeInfoByLevel(level);
}

std::vector<myMonster*> CDataManager::getuserMonsterInfo()
{
    return CDBMgr::getInstance()->UserMonsterInfo;
}

myMonster * CDataManager::getuserMonsterById(int iMonsterId)
{
    for(int i=0; CDBMgr::getInstance()->UserMonsterInfo.size();i++)
    {
        if(iMonsterId == CDBMgr::getInstance()->UserMonsterInfo[i]->monsterId)
        {
            return CDBMgr::getInstance()->UserMonsterInfo[i];
            
        }
    }
    return NULL;
}

std::vector<FighterSkillInfo>* getFighterSkillInfoById(int monsterId)
{
    std::vector<FighterInfo*> m_pSoldierVec = CPersonalData::getInstance()->getAllSoldierInfo();
    for(int i = 0; i < m_pSoldierVec.size(); i++){
        if (m_pSoldierVec.at(i)->monsterId == monsterId) {
            return &m_pSoldierVec.at(i)->theFighterSkillInfo;
        }
    }
    return NULL;
}


propInfo* CDataManager::getPropInfoById(int propId)
{
    return CDBMgr::getInstance()->getPropInfoById(propId);
}

std::vector<int> CDataManager::getItemPathById(int propId, bool eQlite)
{
    return CDBMgr::getInstance()->getItemPathById(propId, eQlite);
}

void CDataManager::SaveMonsterInfoById(myMonster* pMonster)
{
    CDBMgr::getInstance()->SaveUserMonsterInfo(pMonster->monsterId,pMonster->monsterLevel,pMonster->skillLevel1,pMonster->skillLevel2,pMonster->skillLevel3,pMonster->skillLevel4,pMonster->ifFight,pMonster->theType,pMonster->rare);
}


Sprite * CDataManager::getPropSpriteById(int propId, bool isNeedBg)
{
    std::string iconBg[5]={"whiteBg.png","greenBg.png","blueBg.png","purpleBg.png", "orangeBg.png"};
    propInfo* pInfo =  CDBMgr::getInstance()->getPropInfoById(propId);
    if(pInfo)
    {
        __String * temp = __String::createWithFormat("%s",pInfo->m_sItemIcon.c_str());
        
        Sprite * pIcon = Sprite::createWithSpriteFrameName(temp->getCString());
        
        pIcon->setAnchorPoint(Vec2(0,0));
        if(isNeedBg)
        {
            Sprite* propBg = Sprite::createWithSpriteFrameName(iconBg[pInfo->m_iItemLevel - 1].c_str());
            propBg->addChild(pIcon);
            return propBg;
        }
        else
        {
            return pIcon;
        }
    }
    else
    {
        return NULL;
    }
}



PropOpenInfo *CDataManager::getPropOpenInfoById(int iPropId)
{
    //std::vector<PropOpenInfo*> pInfo = CDBMgr::getInstance()->GetPropOpenInfo();
    
    
    //概率
    int iRandNum = rand()%100;
    
    log("iRandNum = %d\n",iRandNum);
    
    std::vector<PropOpenInfo*> pInfo = CDBMgr::getInstance()->getPropOpenInfoById(iPropId);
    
    int fNum = 0;
    PropOpenInfo* pLast = NULL;
    
    
    for(int i=0;i<pInfo.size();i++)
    {
        fNum += (int)(atof(pInfo[i]->m_sRatio.c_str())*100);
        
        if(fNum>=iRandNum)
        {
            pLast = pInfo[i];
            break;
        }
    }
    
    return pLast;
    
}


std::vector<midLevelInfo> CDataManager::getMidLevelInfoFromDB()
{
    return CDBMgr::getInstance()->midLevelInfoVec;
}

std::vector<starInfo> CDataManager::getAllStarInfo()
{
    return CDBMgr::getInstance()->m_levelStarInfo;
}

midLevelInfo* CDataManager::getMidLevelInfoById(int midLevelId)
{
    
    return CDBMgr::getInstance()->getMidLevelInfoById(midLevelId);
}

void CDataManager::saveUserTeamToDB(int monsterId, int iFight)
{
    CDBMgr::getInstance()->saveUserTeamToDB(monsterId, iFight);
}

void CDataManager::saveUserAwardInfo(int midLevelId, int num, bool isEqlite)
{
    CDBMgr::getInstance()->saveUserAwardInfo(midLevelId, num, isEqlite);
}


void CDataManager::saveUserDebrisToDB(int debrisId, int num, int type)
{
    CDBMgr::getInstance()->saveUserDebrisToDB(debrisId, num, type);
}

int CDataManager::getGuideInfoSum()
{
    std::vector<guideInfo> guideInfo = CDBMgr::getInstance()->thsGuiderInfo;
    std::vector<int> typeVec;
    for(int i = 0; i < guideInfo.size(); i++)
    {
        bool bExisted = false;
        for(int j = 0; j < typeVec.size(); j++)
        {
            if(typeVec[j] == guideInfo[i].type)
            {
                bExisted = true;
                break;
            }
        }
        if(!bExisted)
        {
            typeVec.push_back(guideInfo[i].type);
        }
    }
    return typeVec.size();
}

std::vector<guidePro> CDataManager::getTheGuideProgress()
{
    return CDBMgr::getInstance()->thsGuideProInfo;
}

std::vector<gameStory> CDataManager::getStoryInfo()
{
    return CDBMgr::getInstance()->theGameStoryInfo;
}

std::vector<guideInfo> CDataManager::getGuideInfo()
{
    return CDBMgr::getInstance()->thsGuiderInfo;
}

std::vector<guideInfo> CDataManager::getGuideInfoByType(int type)
{
    if(type == 0)
    {
        type += 1;
    }
    std::vector<guideInfo> allVec = getGuideInfo();
    std::vector<guideInfo> tempVec;
    for(int i = 0; i < allVec.size(); i++)
    {
        if(allVec.at(i).type == type)
        {
            tempVec.push_back(allVec.at(i));
        }
    }
    return tempVec;
}

bool CDataManager::getIsExistedByType(GuideState type)
{
    std::vector<guideInfo> tempVec = getGuideInfo();
    for(int i = 0; i < tempVec.size(); i++)
    {
        if(type == tempVec.at(i).type)
        {
            return true;
        }
    }
    return false;
}

std::vector<gameStory> CDataManager::getStoryInfoByLevelId(storyState type)
{
    std::vector<gameStory> tempInfo = getStoryInfo();
    std::vector<gameStory> tempVec;
    for(int i =0 ;i < tempInfo.size(); i++)
    {
        if(type == tempInfo[i].storyType)
        {
            tempVec.push_back(tempInfo[i]);
        }
    }
    return tempVec;
}

int CDataManager::getStoryTypeByLevelId(int Id, int startTime)
{
    std::vector<gameStory> allStoryVec = getStoryInfo();
    std::vector<int> userStoryVec = CDBMgr::getInstance()->theUserInfo->storyInfo;
    //先找出该关卡的所有剧情类型
    int storyType = 0;
    for(int i = 0; i < allStoryVec.size(); i++)
    {
        if(startTime == 1)
        {
            if((allStoryVec[i].levelId == Id) && (allStoryVec[i].startTime >= startTime))
            {
                storyType = allStoryVec[i].storyType;
                break;
            }
        }
        else
        {
            if((allStoryVec[i].levelId == Id) && (allStoryVec[i].startTime == startTime))
            {
                storyType = allStoryVec[i].storyType;
                break;
            }
        }
    }
    for(int j = 0; j < userStoryVec.size(); j++)
    {
        if(userStoryVec.at(j) == storyType)
        {
            return 0;
        }
    }
    return storyType;
}

bool CDataManager::getEndedBy(GuideState type)
{
  return true;
    
    std::vector<guidePro> tempVec = getTheGuideProgress();
    for(int i = 0; i < tempVec.size(); i++)
    {
        if(type == tempVec.at(i).typeId)
        {
            return true;
        }
    }
    return false;
   // return true;
}

int CDataManager::getGuideStepNum()
{
    std::vector<guidePro> tempVec = getTheGuideProgress();
    int maxIndex = 1;
    int curValue = int(tempVec.size());
    for(int i = 0; i < tempVec.size(); i++)
    {
        if(tempVec.at(i).typeId > maxIndex)
        {
            maxIndex = tempVec.at(i).typeId;
        }
    }
    for(int i = 1; i < maxIndex; i++)
    {
        bool isExiste = false;
        for (int j = 0; j < tempVec.size(); j++) {
            if(tempVec.at(j).typeId == i)
            {
                isExiste = true;
                break;
            }
        }
        if(!isExiste)
        {
            if(curValue > i)
            {
                curValue = i;
            }
        }
    }
    
    //return 100000;//测试用
    return curValue;
}

void CDataManager::setUserIdAndAuthCode()
{
    CDBMgr::getInstance()->setUserIdAndAuthCode();
}

std::vector<TaskBase*> CDataManager::GetAllTaskInfo()
{
    return CDBMgr::getInstance()->GetAllTaskInfo();
}

TaskBase* CDataManager::getTaskInfoById(int taskId)
{
    return CDBMgr::getInstance()->getTaskInfoById(taskId);
}

void CDataManager::eraseTaskById(int taskId)
{
    CDBMgr::getInstance()->eraseTaskById(taskId);
}

void CDataManager::saveTaskState(int taskType, TaskState state, bool bTaskId)
{
    CDBMgr::getInstance()->saveTaskState(taskType, state,bTaskId);
}

userNum* CDataManager::getUserInfoNum()
{
    return &CDBMgr::getInstance()->m_userInfoNum;
}

void CDataManager::refreshUserInfoNum()
{
    CDBMgr::getInstance()->refreshUserInfoNum();
}

void CDataManager::refreshDailyTask()
{
    CDBMgr::getInstance()->refreshDailyTask();
}

void CDataManager::saveUpdateExchangeNum(int type)
{
    CDBMgr::getInstance()->saveUpdateExchangeNum(type);
}

void CDataManager::saveUpdateDailyaTaskTime(std::string time)
{
    CDBMgr::getInstance()->saveUpdateDailyaTaskTime(time);
}

void CDataManager::saveUpdateExchangeNumTime(std::string time)
{
    CDBMgr::getInstance()->saveUpdateExchangeNumTime(time);
}


void CDataManager::saveUserLoginTime(std::string time)
{
    CDBMgr::getInstance()->saveUserLoginTime(time);
}

int CDataManager::getTaskProByType(int taskType)
{
    return CDBMgr::getInstance()->getTaskProByType(taskType);
}

void CDataManager::saveTaskInfoToDB(int taskType, int num, bool ibMax)
{
    CDBMgr::getInstance()->saveTaskInfoToDB(taskType, num, ibMax);
}

void CDataManager::saveGameStoryInfoToDB(int type)
{
    CDBMgr::getInstance()->saveGameStoryInfoToDB(type);
}

void CDataManager::saveGuideInfoToDB(int type)
{
    CDBMgr::getInstance()->saveGuideInfoToDB(type);
    if(!getIsExistedByType(GuideState(type + 1)))
    {
        CDBMgr::getInstance()->saveGuideInfoToDB(type + 1);
    }
    //talkingdata
    //talkingdata
    
    __String* tempStr = __String::createWithFormat("guide-%d",type);
    TDCCMission::onBegin(tempStr->getCString());
    TDCCMission::onCompleted(tempStr->getCString());
    
}

void CDataManager::SaveUserPropToDB(int iPropId,int iPropNum)
{
    CDBMgr::getInstance()->SaveUserPropDb(iPropId, iPropNum);
}

void CDataManager::saveMonsterStarToDB(int monsterId,int rare)
{
    CDBMgr::getInstance()->saveMonsterStarToDB(monsterId, rare);
}

void CDataManager::saveUserStoneAndCoinToDB(int num, int type, bool ifAdd)
{
    CDBMgr::getInstance()->saveUserStoneAndCoinToDB(num, type, ifAdd);
}

userInfoInDB* CDataManager::getTheUSerInfDB()
{
    return CDBMgr::getInstance()->theUserInfo;
}

int CDataManager::getDebrisNumById(int monsterId)
{
    int num = 0;
    std::vector<myMonster*> userMonterVec = CDBMgr::getInstance()->UserMonsterInfo;
    for(int i = 0; i < userMonterVec.size(); i++)
    {
        if(userMonterVec.at(i)->monsterId == monsterId)
        {
            num = userMonterVec.at(i)->num;
            break;
        }
    }
    return num;
}

void CDataManager::saveUserTowerLevelInfoToDB(int level)
{
    CDBMgr::getInstance()->saveUserTowerLevelInfoToDB(level);
}

void CDataManager::saveUserLevelInfoToDB(int level, int rare, bool isEqlite)
{
    CDBMgr::getInstance()->saveUserInfoToDB(level, rare, isEqlite);
}

void CDataManager::saveUserInfo(int stamina, int level, int exp)
{
    CDBMgr::getInstance()->saveUserInfo(stamina, level, exp);
}

float CDataManager::getExpInfoByLevel(int level)
{
    return CDBMgr::getInstance()->getExpInfoFromDB(level);
}

starInfo* CDataManager::getStarInfoById(int starId)
{
    return CDBMgr::getInstance()->getStarInfoById(starId);
}

std::vector<int> CDataManager::getStarConditionById(int levelId)
{
    return CDBMgr::getInstance()->getStarConditionById(levelId);
}

float CDataManager::getTheDamageRatioWithIdAndLevel(int skillId, int skillLevel){
    
    
    skillInfo* targetInfo = getTheSkillInfoWithId(skillId);
    
    
    float theNumber =  getTheNumWithMinMax(1, targetInfo->maxLevel, targetInfo->damageRatioMin, targetInfo->damageRatioMax, skillLevel);
    
    
    return theNumber;
}

float CDataManager::getTheStartRatioWithIdAndLevel(int skillId, int skillLevel){
    
    skillInfo* targetInfo = getTheSkillInfoWithId(skillId);
    
    
    float theNumber =  getTheNumWithMinMax(1, targetInfo->maxLevel, targetInfo->startRatioMin, targetInfo->startRatioMax, skillLevel);
    
    return theNumber;
    
}

float CDataManager::getTheLastTimeWithIdAndLevel(int skillId, int skillLevel){
    
    
    heroSkill* targetInfo = getTheHeroSkillWithId(skillId);
    
    
    float theNumber =  getTheNumWithMinMax(1, targetInfo->levelMax, targetInfo->lastTimeMin, targetInfo->lastTimeMax, skillLevel);
    
    
    return theNumber;
    
}


float CDataManager::getTheDamageWithHeroKillIdAndLevel(int skillId, int skillLevel){
    
    heroSkill* targetInfo = getTheHeroSkillWithId(skillId);
    
    float theNumber = getTheNumWithMinMax(1, targetInfo->levelMax, targetInfo->damageMin, targetInfo->damageMax, skillLevel);
    
    return theNumber;
    
}


std::vector<reinforcementInfo*> CDataManager::getReinforceMentInfoByLevel(int level){
    
    
    std::vector<reinforcementInfo*> target;
    
    std::vector<reinforcementInfo*>* theVec = &CDBMgr::getInstance()->m_vecFightReinforcementInfo;
    
    for (std::vector<reinforcementInfo*>::iterator ite = theVec->begin(); ite!=theVec->end(); ite++) {
        
        reinforcementInfo* temp = *ite;
        
        if (temp->levelId==level) {
            target.push_back(temp);
        }
        
    }
    
    return target;
}


std::string CDataManager::getRandomTips(){
    
    
    std::vector<tipsInfo*>* theVec = &CDBMgr::getInstance()->m_vecTipsInfo;
    
    if (theVec->size()<=0) {
        return "";
    }
    
    
    int offset =  rand()%theVec->size();
    
    return  (*theVec->at(offset)).content;
    
    
}

void CDataManager::saveUserAllInfo()
{
    CDBMgr::getInstance()->saveUserAllInfo();
}

void CDataManager::saveUserGuideInfo()
{
    CDBMgr::getInstance()->saveUserGuideInfo();
}

void CDataManager::saveUserTaskListInfo()
{
    CDBMgr::getInstance()->saveUserTaskListInfo();
}

void CDataManager::saveUserTaskInfo()
{
    CDBMgr::getInstance()->saveUserTaskInfo();
}

void CDataManager::saveUserMonsterInfo()
{
    CDBMgr::getInstance()->saveUserMonsterInfo();
}

void CDataManager::saveUserItemsInfo()
{
    CDBMgr::getInstance()->saveUserItemsInfo();
}

std::vector<chargeInfo> CDataManager::getChageInfo()
{
    return CDBMgr::getInstance()->theChargeInfo;
}

chargeInfo* CDataManager::getChargeInfoById(int chargeId)
{
    return CDBMgr::getInstance()->getChargeInfoById(chargeId);
}

std::vector<string> CDataManager::getMessageInfo()
{
    return CDBMgr::getInstance()->m_requestInfo;
}

void CDataManager::insertMessageInfo(std::string meesgaeInfo)
{
    CDBMgr::getInstance()->insertMessageInfo(meesgaeInfo);
}

void CDataManager::delMeeageInfo(std::string meesgaeInfo)
{
    CDBMgr::getInstance()->delMeeageInfo(meesgaeInfo);
}

pvpBuildInfo* CDataManager::getBuildInfo(int buildId)
{
    return CDBMgr::getInstance()->getBuildInfo(buildId);
}

int CDataManager::getBuildTypeNum()
{
    return CDBMgr::getInstance()->getBuildTypeNum();
}

int CDataManager::getBuildIdByType(int buildType)
{
    return CDBMgr::getInstance()->getBuildIdByType(buildType);
}

void CDataManager::saveVideoInfo(std::vector<videoListInfo> tempVec)
{
    CDBMgr::getInstance()->saveVideoInfo(tempVec);
}

void CDataManager::saveVideoById(int videoId)
{
    CDBMgr::getInstance()->saveVideoById(videoId);
}

void CDataManager::saveRevenueFlag(int videoId)
{
    CDBMgr::getInstance()->saveRevenueFlag(videoId);
}

void CDataManager::saveNewVideoInfo()
{
    CDBMgr::getInstance()->saveNewVideoInfo();
}

void CDataManager::saveRewardInfo(int videoId)
{
    CDBMgr::getInstance()->saveRewardInfo(videoId);
}

std::vector<videoListInfo> CDataManager::getVideoListInfo()
{
    return CDBMgr::getInstance()->getVideoListInfo();
}

std::string CDataManager::getDigitFromBackStr(std::string str)
{
    string::size_type pos = str.find('=', 0);
    std::string temp = "";
    if(pos != std::string::npos)
    {
        temp = str.substr(pos + 1, -1);
    }
    return temp;
}

std::string CDataManager::getStrFromBackStr(std::string str)
{
    string::size_type pos = str.find('=', 0);
    std::string temp = "";
    if(pos != std::string::npos)
    {
        temp = str.substr(0, pos);
    }
    return temp;
}


