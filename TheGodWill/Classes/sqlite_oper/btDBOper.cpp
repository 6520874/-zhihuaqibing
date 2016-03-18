#include "btDBOper.h"
#include "PublicDefine.h"


sqlite3 *pDB = NULL;//数据库指针
char * errMsg = NULL;//错误信息
std::string sqlstr;//SQL指令
int result;//sqlite3_exec返回值

//获取打开的数据库
sqlite3* CDataBaseOper::getDB()
{
	return pDB;
}
//创建数据库
void CDataBaseOper::initDB(const char *db )
{
	//打开一个数据库，如果该数据库不存在，则创建一个数据库文件
	result = sqlite3_open(db, &pDB);
	if( result != SQLITE_OK )
        log( "打开数据库失败，错误码:%d ，错误原因:%s\n" , result, errMsg );
    std::string pass = "shanghaicerongwangluo";

    
    //sqlite3_rekey(pDB,pass.c_str(), pass.length());
    
   //bool pss = UserDefault::getInstance()->getBoolForKey("cerongpass");
    
    {
      //log("加密打开");
//#ifdef Pro_Release
  //    sqlite3_key(pDB, pass.c_str(), pass.length());
//#endif
    }

    /*#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sqlite3_key(pDB, pass.c_str(), pass.length());
    #endif*/
}


//tableIsExist的回调函数
int isExisted( void * para, int n_column, char ** column_value, char ** column_name )
{
	bool *isExisted_=(bool*)para;
	*isExisted_=(**column_value)!='0';
	return 0;
}
//判断表格是否存在
bool CDataBaseOper::tableIsExist( string name )
{
	if (pDB!=NULL)
	{
		//判断表是否存在
		bool tableIsExisted;
		sqlstr = "select count(type) from sqlite_master where type='table' and name ='"+name+"'";
		result =sqlite3_exec(pDB,sqlstr.c_str(),isExisted,&tableIsExisted,&errMsg);
		return tableIsExisted;
	}
	return false;
}


//在数据库中判断名为name的表示否存在，如果不存在则创建这张表
//@示例语句string sqls = "create table user(id integer,username text,password text)";
void CDataBaseOper::createTable( string sql,string name )
{
	if (!tableIsExist(name))
	{
		//创建表，设置ID为主键，且自动增加
		result = sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
		if( result != SQLITE_OK )
			CCLog( "创建表失败，错误码:%d ，错误原因:%s\n" , result, errMsg );
	}
    
}


//删除表格
//@示例语句sqlstr="drop table name";
void CDataBaseOper::deleteTable( string sql,string name )
{
	if (tableIsExist(name))
	{
		result = sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errMsg);
		if( result != SQLITE_OK )
			CCLog( "创建表失败，错误码:%d ，错误原因:%s\n" , result, errMsg );
	}
}


//插入数据
//@示例语句sqlstr=" insert into MyTable_1( name ) values ( '擎天柱' ) ";
void CDataBaseOper::insertData( string sql ){
	result = sqlite3_exec( pDB, sql.c_str() , NULL, NULL, &errMsg );
	if(result != SQLITE_OK )
		CCLog( "插入记录失败，错误码:%d ，错误原因:%s\n" , result, errMsg );
}


//删除数据
//@示例语句sqlstr="delete from MyTable_1 where ID = 2";
void CDataBaseOper::deleteData( string sql )
{
    CCLog("%s",sql.c_str());
	result=sqlite3_exec( pDB, sql.c_str() , NULL, NULL, &errMsg );
	if(result != SQLITE_OK )
		CCLog( "插入记录失败，错误码:%d ，错误原因:%s\n" , result, errMsg );
}


//修改数据
//@示例语句        sqlstr="update MyTable_1 set name='威震天' where ID = 3";
//也可以 replace into table (name,sex) values("hello","m")
void CDataBaseOper::updateData( string sql )
{
    CCLog("%s",sql.c_str());
	result = sqlite3_exec( pDB, sql.c_str() , NULL, NULL, &errMsg );
	if(result != SQLITE_OK )
		CCLog( "插入记录失败，错误码:%d ，错误原因:%s\n" , result, errMsg );
}

//COC wwj
void CDataBaseOper::startTransaction()
{
	result = sqlite3_exec( pDB, "begin transaction" , NULL, NULL, &errMsg );
	if(result != SQLITE_OK ) {
		CCLOG( "开始事务失败!!错误码:%d ，错误:%s\n" , result, errMsg );
	}
}
void CDataBaseOper::commitTransaction()
{
	result = sqlite3_exec( pDB, "commit transaction" , NULL, NULL, &errMsg );
	if(result != SQLITE_OK ) {
		CCLOG( "提交事务失败!!错误码:%d ，错误:%s\n" , result, errMsg );
	}
}
void CDataBaseOper::rollbackTransaction()
{
	result = sqlite3_exec( pDB, "rollback transaction" , NULL, NULL, &errMsg );
	if(result != SQLITE_OK ) {
		CCLOG( "回滚事务失败!!错误码:%d ，错误:%s\n" , result, errMsg );
	}
}

//getDataCount的回调函数
int loadRecordCount( void * para, int n_column, char ** column_value, char ** column_name )
{
	int *count=(int*)para;
	*count=n_column;
	return 0;
}
//获取记录的条数
//@示例语句string sqlsssss = "select count(*) from user";
//@示例语句  取得表格字段的语句string sqlsssss = "select * from user";
int CDataBaseOper::getDataCount( string sql )
{
	int count=0;
	sqlite3_exec( pDB, sql.c_str() , loadRecordCount, &count, &errMsg );
	return count;
}


//getDataInfo的回调函数
int loadRecord( void * para, int n_column, char ** column_value, char ** column_name )
{
	CCLog("ID=%s,name=%s,password=%s,word=%s",column_value[0],column_value[1],column_value[2],column_value[3]);
	return 0;
}
//获取一条记录的信息 其中的pSend是一个实体类我们以后可以自定义一个继承了CCObject的类来代替他保存数据库中取出来的数据
void CDataBaseOper::getDataInfo( string sql,cocos2d::CCObject *pSend )
{
	sqlite3_exec( pDB, sql.c_str() , loadRecord, NULL, &errMsg );
}


//关闭数据库
void CDataBaseOper::closeDB()
{
	sqlite3_close(pDB);
}