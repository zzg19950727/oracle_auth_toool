#include "log.h"
#include <algorithm>

void get_all_time(vector<QString> &list)
{
    list.clear();
    QString filename;
    QDir dir("data");
    foreach(QFileInfo mfi ,dir.entryInfoList())
    {
        if(mfi.isFile())
        {
            filename = mfi.fileName();
            if(filename.contains(".sql"))
                continue;
            list.push_back(filename);
        }
    }
}

void sort_time(vector<QString> &list)
{
    vector<QString>::iterator iter_begin, iter_end;
    iter_begin = list.begin();
    iter_end = list.end();
    sort(iter_begin, iter_end);
}

QString get_lastest_time(vector<QString> &list)
{
    if(list.size()<1)
        return QString("");
    else
        return list[list.size()-1];
}

QString get_last_file(vector<QString> &list)
{
    if(list.size()<2)
        return QString("");
    else
        return list[list.size()-2];
}
