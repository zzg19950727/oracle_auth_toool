#ifndef INFO_H
#define INFO_H
#include <assert.h>
#include <vector>
#include <QString>
using std::vector;

/*********************************
 * 0表示无目标权限
 * 1表示授予目标权限
 * 2表示撤销目标权限
 * 3表示用户原来已拥有的权限，不作修改
 * ********************************/
enum STAT {NONE=0, GRANT, REVOKE, OWN};

/*权限集合*/
struct priv
{
    QString str;//权限名称
    QString desc;//权限描述
    enum STAT state;//用户拥有状态
    bool with_grant_option;
};
typedef struct priv Priv;
typedef vector<Priv> PrivList;

/*表集合，一般存放同一个数据库中的所有表*/
struct table
{
    QString table_name;
    PrivList priv_list;//对表的权限
};
typedef struct table Table;
typedef vector<Table> TableList;

/*数据库集合*/
struct db
{
    QString db_name;
    TableList table_list;//拥有的表
    PrivList priv_list;//privileges for db.*
};
typedef struct db DB;
typedef vector<DB> DBList;

typedef vector<Priv> DDLRightList;

struct info
{
    QString name;
    QString host;
    QString password;
    bool create;    //是否是新建用户
    DDLRightList DDL_list;
    DBList db_list;
    //修改后的信息
    QString new_name;
    QString new_host;
    QString new_password;
};
typedef struct info INFO;
typedef vector<INFO> UserList;

/***************************
 * 上下文，表示当前正在修改的权限
 * 记录
 ***************************/
struct my_ctx
{
    QString current_user;
    int user_id;
    QString current_db;
    int db_id;
    QString current_table;
    int table_id;
};
typedef struct my_ctx MyCtx;

#endif // INFO_H

