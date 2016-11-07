#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>


void MainWindow::read_data()
{
    QString filename = get_lastest_time(list);

    load_file("data\\"+filename);
}

void MainWindow::load_file(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream in(&file);
    QString text;

    //read DDL right data
    unsigned int DDL_num;
    text = in.readLine();
    in>>DDL_num;
    //qDebug()<<text<<DDL_num;
    //skip enter
    text = in.readLine();
    DDL_list.clear();
    for(unsigned int i=0; i<DDL_num; i++)
    {
        Priv right;
        right.str = in.readLine();
        right.desc = in.readLine();
        DDL_list.push_back(right);
    }

    //read db info
    unsigned int db_num;
    unsigned int table_num;
    text = in.readLine();
    in>>db_num;
    //qDebug()<<text<<db_num;
    //skip enter
    text = in.readLine();
    db_list.clear();
    for(unsigned int i=0; i<db_num; i++)
    {
        DB db;
        db.db_name = in.readLine();

        //read table info
        text = in.readLine();
        in>>table_num;
        //qDebug()<<text<<table_num;
        //skip enter
        text = in.readLine();
        for(unsigned int j=0; j<table_num; j++)
        {
            Table table;
            table.table_name = in.readLine();
            db.table_list.push_back(table);
        }
        db_list.push_back(db);
    }

    //read privileges data
    unsigned int priv_num;
    text = in.readLine();
    in>>priv_num;
    //qDebug()<<text<<priv_num;
    //skip enter
    text = in.readLine();
    priv_list.clear();
    for(unsigned int i=0; i<priv_num; i++)
    {
        Priv priv;
        priv.str = in.readLine();
        priv.desc = in.readLine();
        priv_list.push_back(priv);
    }

    //read user info and privileges
    unsigned int user_num;
    text = in.readLine();
    in>>user_num;
    //qDebug()<<text<<user_num;
    //skip enter
    text = in.readLine();
    user_list.clear();
    for(unsigned int i=0; i<user_num; i++)
    {
        INFO user;
        user.name = in.readLine();
        //user.host = in.readLine();
        user.password = in.readLine();
        user.create = false;

        //read user`s DDL right
        text = in.readLine();
        in>>DDL_num;
        //qDebug()<<text<<DDL_num;
        //skip enter
        text = in.readLine();
        for(unsigned int j=0; j<DDL_num; j++)
        {
            Priv right;
            right.str = in.readLine();
            right.state = OWN;
            user.DDL_list.push_back(right);
        }

        //read user`s db privileges
        text = in.readLine();
        in>>db_num;
        //qDebug()<<text<<db_num;
        //skip enter
        text = in.readLine();
        for(unsigned int j=0; j<db_num; j++)
        {
            DB db;
            db.db_name = in.readLine();

            /*//read db.* privileges
            text = in.readLine();
            in>>priv_num;
            //qDebug()<<text<<priv_num;
            //skip enter
            text = in.readLine();
            for(unsigned int k=0; k<priv_num; k++)
            {
                Priv priv;
                priv.str = in.readLine();
                priv.state = OWN;
                db.priv_list.push_back(priv);
            }*/

            //read db.table privileges
            text = in.readLine();
            in>>table_num;
            //qDebug()<<text<<table_num;
            //skip enter
            text = in.readLine();
            for(unsigned int k=0; k<table_num; k++)
            {
                Table table;
                table.table_name = in.readLine();

                //read privileges
                text = in.readLine();
                in>>priv_num;
                //qDebug()<<text<<priv_num;
                //skip enter
                text = in.readLine();
                for(unsigned int n=0; n<priv_num; n++)
                {
                    Priv priv;
                    priv.str = in.readLine();
                    priv.state = OWN;
                    table.priv_list.push_back(priv);
                }
                db.table_list.push_back(table);
            }
            user.db_list.push_back(db);
        }
        user_list.push_back(user);
    }
    file.close();
}

void MainWindow::save_data()
{
    QDateTime time = QDateTime::currentDateTime();
    QFile file("data/"+time.toString("yyyy-MM-dd-hh-mm")+".dat");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream out(&file);

    //save DDL right data
    out<<"DDL list:\n"<<DDL_list.size()<<"\n";
    for(unsigned int i=0; i<DDL_list.size(); i++)
    {
        out<<DDL_list[i].str<<"\n";
        out<<DDL_list[i].desc<<"\n";
    }

    //save db info
    out<<"db list:\n"<<db_list.size()<<"\n";
    for(unsigned int i=0; i<db_list.size(); i++)
    {
        out<<db_list[i].db_name<<"\n";
        out<<"table num:\n"<<db_list[i].table_list.size()<<"\n";
        for(unsigned int j=0; j<db_list[i].table_list.size(); j++)
        {
            out<<db_list[i].table_list[j].table_name<<"\n";
        }
    }

    //save privileges data
    out<<"privilege list:\n"<<priv_list.size()<<"\n";
    for(unsigned int i=0; i<priv_list.size(); i++)
    {
        out<<priv_list[i].str<<"\n";
        out<<priv_list[i].desc<<"\n";
    }

    //save per user`s info and privileges
    out<<"user list:\n"<<user_list.size()<<"\n";
    for(unsigned int i=0; i<user_list.size(); i++)
    {
        //save user`s info
        out<<user_list[i].name<<"\n";
        //out<<user_list[i].host<<"\n";
        if(user_list[i].new_password.length())
            out<<user_list[i].new_password<<"\n";
        else
            out<<user_list[i].password<<"\n";
        DDLRightList::iterator iter = user_list[i].DDL_list.begin();

        //remove none DDL rights
        for(unsigned int j=0; iter!=user_list[i].DDL_list.end(); )
        {
            enum STAT state = user_list[i].DDL_list[j].state;
            if(state==NONE || state==REVOKE)
            {
                user_list[i].DDL_list.erase(iter);
                continue;
            }
            j++;
            iter++;

        }

        //save user`s DDL rights
        out<<"DDL right:\n"<<user_list[i].DDL_list.size()<<"\n";
        for(unsigned int j=0; j<user_list[i].DDL_list.size(); j++)
        {
            out<<user_list[i].DDL_list[j].str<<"\n";
        }

        //save user`s db privileges
        out<<"db:\n"<<user_list[i].db_list.size()<<"\n";
        for(unsigned int j=0; j<user_list[i].db_list.size(); j++)
        {
            out<<user_list[i].db_list[j].db_name<<"\n";

            PrivList::iterator iter = user_list[i].db_list[j].priv_list.begin();
            //remove none db.* privileges
            for(unsigned int k=0; iter!=user_list[i].db_list[j].priv_list.end(); )
            {
                enum STAT state = user_list[i].db_list[j].priv_list[k].state;
                if(state==NONE || state==REVOKE)
                {
                    user_list[i].db_list[j].priv_list.erase(iter);
                    continue;
                }
                k++;
                iter++;

            }

            /*//save user`s db.* privileges
            out<<"db.* privileges:\n"<<user_list[i].db_list[j].priv_list.size()<<"\n";
            for(unsigned int k=0; k<user_list[i].db_list[j].priv_list.size(); k++)
            {
                out<<user_list[i].db_list[j].priv_list[k].str<<"\n";
            }*/

            //save user`s per table privileges
            out<<"table:\n"<<user_list[i].db_list[j].table_list.size()<<"\n";
            for(unsigned int k=0; k<user_list[i].db_list[j].table_list.size(); k++)
            {
                out<<user_list[i].db_list[j].table_list[k].table_name<<"\n";

                //remove none db.table privileges
                PrivList::iterator iter = user_list[i].db_list[j].table_list[k].priv_list.begin();
                for(unsigned int n=0; iter!=user_list[i].db_list[j].table_list[k].priv_list.end(); )
                {
                    enum STAT state = user_list[i].db_list[j].table_list[k].priv_list[n].state;
                    if(state==NONE || state==REVOKE)
                    {
                        user_list[i].db_list[j].table_list[k].priv_list.erase(iter);
                        continue;
                    }
                    n++;
                    iter++;

                }

                //save user`s db.table privileges
                out<<"db.table privileges:\n"<<user_list[i].db_list[j].table_list[k].priv_list.size()<<"\n";
                for(unsigned int n=0; n<user_list[i].db_list[j].table_list[k].priv_list.size(); n++)
                {
                    out<<user_list[i].db_list[j].table_list[k].priv_list[n].str<<"\n";
                }
            }
        }
    }
    file.close();
}

void create_user(INFO &info, QTextStream &out)
{
    QString command = "CREATE USER ";
    command += info.name;
    command += " IDENTIFIED BY ";
    command += info.password;
    command += ";";
    out<<"--create new user"<<"\n";
    out<<command<<"\n";
}

void alter_password(QString user, QString host,
                    QString password, QTextStream &out)
{
    QString command = "ALTER USER ";
    command += user;
    command += " IDENTIFIED BY ";
    command += password;
    command += ";";
    out<<"--alter user`s password"<<"\n";
    out<<command<<"\n";
}

void revoke_sys_privilege(INFO &info,QString priv, QTextStream &out)
{
    QString command = "REVOKE ";
    command += priv;
    command += " FROM ";
    command += info.name;
    command += ";";
    out<<command<<"\n";
}

void revoke_object_privilege(INFO &info, QString db,QString table,
                          QString priv, QTextStream &out)
{
    QString command = "REVOKE ";
    command += priv;
    command += " ON ";
    command += db;
    command += ".";
    command += table;
    command += " FROM ";
    command += info.name;
    command += ";";
    out<<command<<"\n";
}

void grant_sys_privilege(INFO &info,QString priv,
                         QTextStream &out, bool with_admin_optin)
{
   QString command = "GRANT ";
   command += priv;
   command += " TO ";
   command += info.name;
   if( with_admin_optin )
       command += " WITH ADMIN OPTION";
   command += ";";
   out<<command<<"\n";
}

void grant_object_privilege(INFO &info, QString db,QString table,QString priv,
                         QTextStream &out, bool with_grant_optin)
{
   QString command = "GRANT ";
   command += priv;
   command += " ON ";
   command += db;
   command += ".";
   command += table;
   command += " TO ";
   command += info.name;
   if( with_grant_optin )
       command += " WITH GRANT OPTION";
   command += ";";
   out<<command<<"\n";
}

void MainWindow::create_scripe(QString filename)
{
    QDateTime time = QDateTime::currentDateTime();
    QString backup = "data/"+time.toString("yyyy-MM-dd-hh-mm")+"-backup.sql";
    QFile sql("data/"+time.toString("yyyy-MM-dd-hh-mm")+"-rollback.sql");
    if (!sql.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream out(&file);
    QTextStream sqlout(&sql);

    QString text;
    QDateTime now = QDateTime::currentDateTime();
    out<<"--file created at "<<now.toString("yyyy/MM/dd hh:mm")<<"\n";
    sqlout<<"--backup file for rollback\n";

    for(unsigned int i=0; i<user_list.size(); i++)
    {
        //if need,create new user
        if(user_list[i].create)
            create_user(user_list[i], out);

        //change password
        if(user_list[i].new_password.length())
            alter_password(user_list[i].name, user_list[i].host,
                           user_list[i].new_password, out);

        text = QString("\n--grant or revoke system privileges for %1@%2\n")
                .arg(user_list[i].name).arg(user_list[i].host);

        //deal DDL rights
        unsigned int DDL_num = user_list[i].DDL_list.size();
        if(DDL_num)
            out<<text;
        for(unsigned int j=0; j<DDL_num; j++)
        {
            if(user_list[i].DDL_list[j].state == REVOKE)
            {
                revoke_sys_privilege(user_list[i], user_list[i].DDL_list[j].str, out);
                grant_sys_privilege(user_list[i], user_list[i].DDL_list[j].str, sqlout,
                                    user_list[i].DDL_list[j].with_grant_option);
            }
            else if(user_list[i].DDL_list[j].state == GRANT)
            {
                grant_sys_privilege(user_list[i], user_list[i].DDL_list[j].str, out,
                                    user_list[i].DDL_list[j].with_grant_option);
                revoke_sys_privilege(user_list[i], user_list[i].DDL_list[j].str, sqlout);
            }
        }

        //deal table privileges
        text = QString("\n--grant or revoke object privileges for%1@%2\n")
                .arg(user_list[i].name).arg(user_list[i].host);

        unsigned int db_num = user_list[i].db_list.size();
        if(db_num)
            out<<text;
        for(unsigned int j=0; j<db_num; j++)
        {
            unsigned int priv_num;
            /*//deal db.* privileges
            unsigned int priv_num = user_list[i].db_list[j].priv_list.size();
            for(unsigned int k=0; k<priv_num; k++)
            {
                if(user_list[i].db_list[j].priv_list[k].state == REVOKE)
                    revoke_object_privilege(user_list[i], user_list[i].db_list[j].db_name,
                                     "*", user_list[i].db_list[j].priv_list[k].str, out);
                else if(user_list[i].db_list[j].priv_list[k].state == GRANT)
                    grant_object_privilege(user_list[i], user_list[i].db_list[j].db_name,
                                    "*", user_list[i].db_list[j].priv_list[k].str, out,
                                           user_list[i].db_list[j].priv_list[k].with_grant_option);
            }*/

            //deal db.table privileges
            unsigned int table_num = user_list[i].db_list[j].table_list.size();
            for(unsigned int k=0; k<table_num; k++)
            {
                priv_num = user_list[i].db_list[j].table_list[k].priv_list.size();
                for(unsigned int n=0; n<priv_num; n++)
                {
                    if(user_list[i].db_list[j].table_list[k].priv_list[n].state == REVOKE)
                    {
                        revoke_object_privilege(user_list[i], user_list[i].db_list[j].db_name,
                                                user_list[i].db_list[j].table_list[k].table_name,
                                                user_list[i].db_list[j].table_list[k].priv_list[n].str, out);
                        grant_object_privilege(user_list[i], user_list[i].db_list[j].db_name,
                                               user_list[i].db_list[j].table_list[k].table_name,
                                               user_list[i].db_list[j].table_list[k].priv_list[n].str, sqlout,
                                               user_list[i].db_list[j].table_list[k].priv_list[n].with_grant_option);
                    }
                    else if(user_list[i].db_list[j].table_list[k].priv_list[n].state == GRANT)
                    {
                        grant_object_privilege(user_list[i], user_list[i].db_list[j].db_name,
                                               user_list[i].db_list[j].table_list[k].table_name,
                                               user_list[i].db_list[j].table_list[k].priv_list[n].str, out,
                                               user_list[i].db_list[j].table_list[k].priv_list[n].with_grant_option);
                        revoke_object_privilege(user_list[i], user_list[i].db_list[j].db_name,
                                                user_list[i].db_list[j].table_list[k].table_name,
                                                user_list[i].db_list[j].table_list[k].priv_list[n].str, sqlout);
                    }
                }
            }
        }
    }
    sql.close();
    file.close();
    file.copy(backup);
}

