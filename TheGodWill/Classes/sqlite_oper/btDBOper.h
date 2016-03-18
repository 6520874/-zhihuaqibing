#ifndef _DB_OPERATION_H_
#define _DB_OPERATION_H_

/******************************************************************
 //fileName:f:\workNew\mobile\theycoming\src\mygame1\Classes\dboper.h
 
 //date:2013/1/25 14:20
 
 //desc:数据库的操作
 
 usage:
 // Test Database
 string fullDBPath = CCFileUtils::sharedFileUtils()->getWriteablePath() + "save.db";
 CCLOG("fullPath : %s",fullDBPath.c_str());
 //打开数据库
 DBUtil::initDB(fullDBPath.c_str());
 //创建表
 string createTableSql = "create table zuma (id integer primary key autoincrement,level integer,lastscore integer,bestscore integer,star integer);";
 DBUtil::createTable(createTableSql.c_str(),"zuma");
 //向表格中插入数据
 string sqlss = "insert into zuma (level,lastscore,bestscore,star) values (100,100,500,1)";
 // 插入一条数据
 DBUtil::insertData(sqlss);
 //删除表
 
 // 更新
 string updateString = "update zuma set star = 5;";
 DBUtil::updateData(updateString);
 // 查询数据
 string selectStr = "select * from zuma";
 //    CCObject* pp = new CCObject();
 //    DBUtil::getDataInfo(selectStr, pp);
 TestVO* testVO = new TestVO();
 testVO->mId = 1111;
 DBUtil::getDataInfo(selectStr,testVO);
 // 显示结果
 CCLOG("id:%d",testVO->mId);
 CCLOG("level:%d",testVO->level);
 CCLOG("lastscore:%d",testVO->lastscore);
 CCLOG("bestscore:%d",testVO->bestscore);
 CCLOG("star:%d",testVO->star);
 delete testVO;
 // 不能忘记关闭数据库
 DBUtil::closeDB();
 
 //author:butcher
 ******************************************************************/
#include <string>
#include "sqlite3.h"
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class CDataBaseOper
{
public:
	//用来创建一个db数据库 db为数据库的名字
	//如果不存在,会自动创建一个
	static void initDB(const char *db);
	static sqlite3* getDB();
	//用来判断表格是否存在
	static bool tableIsExist(string name);
	//用来创建一个表名为name的表格，创建时会先匹配时否有该表的存在如果存在则不创建
	static void createTable(string sql,string name);
	//用来删除一张表名为name的表格，删除时会先匹配是否有该表的存在如果不存在则不执行删除操作
	static void deleteTable(string sql,string name);
	//用来向表中插入一条数据
	static void insertData(string sql);
	//用来向表中删除一条数据
	static void deleteData(string sql);
	//用来向表中修改一条数据
	static void updateData(string sql);
	//获取一个记录的条数
	static int getDataCount(string sql);
	//读取一条记录的信息
	static void getDataInfo(string sql,cocos2d::CCObject *pSend);
    
	//关闭打开的数据库
	static void closeDB();
    //事务 COC wwj
	static void startTransaction();
	static void commitTransaction();
	static void rollbackTransaction();
protected:
	//int isExisted( void * para, int n_column, char ** column_value, char ** column_name );
private:
};

#endif