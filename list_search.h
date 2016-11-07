#ifndef LIST_SEARCH
#define LIST_SEARCH
#include "info.h"
#define NOT_FIND -1

int index_priv(PrivList list, QString priv);
int index_table(TableList list, QString table);
int index_db(DBList list, QString db);
int index_user(UserList list, QString name, QString host);

#endif // LIST_SEARCH

