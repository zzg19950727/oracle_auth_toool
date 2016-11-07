#include "list_search.h"

int index_priv(PrivList list, QString priv)
{
    for(unsigned int i=0; i<list.size(); i++)
    {
        if(priv == list[i].str)
            return i;
    }
    return -1;
}

int index_table(TableList list, QString table)
{
    for(unsigned int i=0; i<list.size(); i++)
    {
        if(table == list[i].table_name)
            return i;
    }
    return -1;
}

int index_db(DBList list, QString db)
{
    for(unsigned int i=0; i<list.size(); i++)
    {
        if(db == list[i].db_name)
            return i;
    }
    return -1;
}

int index_user(UserList list, QString name, QString host)
{
    for(unsigned int i=0; i<list.size(); i++)
    {
        if(name==list[i].name)
            return i;
    }
    return -1;
}
