#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "list_search.h"

void MainWindow::reflesh_account_info()
{
    ui->lineEdit_user_name->setText(user_list[ctx.user_id].name);

    if(user_list[ctx.user_id].new_password.length())
        ui->lineEdit_password->setText(user_list[ctx.user_id].new_password);
    else
        ui->lineEdit_password->setText(user_list[ctx.user_id].password);
}

void MainWindow::clear_account_info()
{
    ui->lineEdit_user_name->clear();
    ui->lineEdit_password->clear();
}

void MainWindow::clear_DDL_right()
{
    QTreeWidgetItem *item;

    unsigned int row = ui->treeWidget_right->topLevelItemCount();
    for(unsigned int i=0; i<row; i++)
    {
        item = ui->treeWidget_right->topLevelItem(i);
        if( item )
        {
            item->setCheckState(Ui::STATE, Qt::Unchecked);
            item->setCheckState(Ui::WITH_GRANT_OPTION, Qt::Unchecked);
        }
    }
    ui->treeWidget_right->setColumnCount(3);
}

void MainWindow::reflesh_DDL_right()
{

    clear_DDL_right();

    QList<QTreeWidgetItem *> item_list;
    for(unsigned int i=0; i<user_list[ctx.user_id].DDL_list.size(); i++)
    {
        item_list = ui->treeWidget_right->findItems(
                    user_list[ctx.user_id].DDL_list[i].str,
                    Qt::MatchExactly, 1);
        if( item_list.size() != 1 )
            continue;
        enum STAT state = user_list[ctx.user_id].DDL_list[i].state;
        if(state==GRANT || state==OWN)
        {
            item_list[0]->setCheckState(Ui::STATE, Qt::Checked);
            if(state == GRANT)
            {
               ui->treeWidget_right->setColumnCount(4);
               ui->treeWidget_right->setColumnWidth(2, 150);
               if(user_list[ctx.user_id].DDL_list[i].with_grant_option)
                   item_list[0]->setCheckState(Ui::WITH_GRANT_OPTION, Qt::Checked);
               else
                   item_list[0]->setCheckState(Ui::WITH_GRANT_OPTION, Qt::Unchecked);
            }
        }
    }

}

void MainWindow::clear_priv_widget()
{
    QTreeWidgetItem *item;
    ui->treeWidget_privileges->setColumnCount(3);
    unsigned int row = ui->treeWidget_privileges->topLevelItemCount();
    for(unsigned int i=0; i<row; i++)
    {
        item = ui->treeWidget_privileges->topLevelItem(i);
        if( item )
            item->setCheckState(Ui::STATE, Qt::Unchecked);
    }
}

void MainWindow::reflesh_privileges()
{
    inited = false;

    clear_priv_widget();
    QList<QTreeWidgetItem *> item_list;
    //show privileges for db.*
    if(ctx.table_id == NOT_FIND)
    {
        ;/*
        for(unsigned int i=0; i<user_list[ctx.user_id].db_list[ctx.db_id].priv_list.size(); i++)
        {
            item_list = ui->treeWidget_privileges->findItems(
                        user_list[ctx.user_id].db_list[ctx.db_id].priv_list[i].str,
                        Qt::MatchExactly, 1);
            if( item_list.size() != 1 )
                continue;
            enum STAT state = user_list[ctx.user_id].db_list[ctx.db_id].priv_list[i].state;
            if(state==GRANT || state==OWN)
            {
                item_list[0]->setCheckState(Ui::STATE, Qt::Checked);
                if(state == GRANT)
                {
                    ui->treeWidget_privileges->setColumnCount(4);
                    ui->treeWidget_privileges->setColumnWidth(2, 150);

                    if(user_list[ctx.user_id].db_list[ctx.db_id].priv_list[i].with_grant_option)
                        item_list[0]->setCheckState(Ui::WITH_GRANT_OPTION, Qt::Checked);
                    else
                        item_list[0]->setCheckState(Ui::WITH_GRANT_OPTION, Qt::Unchecked);
                }
            }
        }*/
    }
    //show privileges for db.table
    else
    {
        for(unsigned int i=0;
            i<user_list[ctx.user_id].db_list[ctx.db_id].table_list[ctx.table_id].priv_list.size();
            i++)
        {
            item_list = ui->treeWidget_privileges->findItems(
                  user_list[ctx.user_id].db_list[ctx.db_id].table_list[ctx.table_id].priv_list[i].str,
                  Qt::MatchExactly, 1);
            if( item_list.size() != 1 )
                continue;
            enum STAT state =
                    user_list[ctx.user_id].db_list[ctx.db_id].table_list[ctx.table_id].priv_list[i].state;
            if(state==GRANT || state==OWN)
            {
                item_list[0]->setCheckState(Ui::STATE, Qt::Checked);
                if(state == GRANT)
                {
                    ui->treeWidget_privileges->setColumnCount(4);
                    ui->treeWidget_privileges->setColumnWidth(2, 150);
                    if(user_list[ctx.user_id].db_list[ctx.db_id].table_list[ctx.table_id].priv_list[i].with_grant_option)
                        item_list[0]->setCheckState(Ui::WITH_GRANT_OPTION, Qt::Checked);
                    else
                        item_list[0]->setCheckState(Ui::WITH_GRANT_OPTION, Qt::Unchecked);
                }
            }
        }
    }
    inited = true;
}

void MainWindow::clear_table_widget()
{
    QTreeWidgetItem *item;
    unsigned int row = ui->treeWidget_table_info->topLevelItemCount();
    for(unsigned int i=0; i<row; i++)
    {
        item = ui->treeWidget_table_info->topLevelItem(i);
        if(item)
        {
            unsigned int n = item->childCount();
            for(unsigned int j=0; j<n; j++)
            {
                QTreeWidgetItem *child = item->child(j);
                if( child )
                child->setCheckState(Ui::DB, Qt::Unchecked);
            }
        }
    }
}

void MainWindow::change_account()
{
    inited = false;

    QTreeWidgetItem* item = ui->treeWidget_user_accounts->currentItem();
    if( !item )
    {
        ctx.user_id = NOT_FIND;
        clear_account_info();
        clear_DDL_right();
        clear_priv_widget();
        inited = true;
        return;
    }
    QString name = item->text(Ui::NAME);
    //QString host = item->text(Ui::HOST);
    ctx.current_user = name;
    ctx.user_id = index_user(user_list, name, name);
    if(ctx.user_id == NOT_FIND)
        return;
    reflesh_account_info();
    reflesh_DDL_right();
    clear_priv_widget();
    clear_table_widget();
    ctx.db_id = NOT_FIND;
    QString text = QString("用户 %1 的信息").arg(name);
    ui->label_user->setText(text);

    if(ui->treeWidget_table_info->topLevelItemCount())
    {
        ui->treeWidget_table_info->setCurrentItem(ui->treeWidget_table_info->topLevelItem(0));
    }
    inited = true;
}

void MainWindow::change_table()
{
    inited = false;

    if( ctx.user_id == NOT_FIND )
        return;
    QTreeWidgetItem* item = ui->treeWidget_table_info->currentItem();
    if( !item )
    {
        ctx.db_id = NOT_FIND;
        clear_priv_widget();
        inited = true;
        return;
    }
    QTreeWidgetItem* parent = item->parent();
    //clicked table
    if( parent )
    {
        ctx.current_db = parent->text(Ui::DB);
        ctx.db_id = index_db(user_list[ctx.user_id].db_list, ctx.current_db);
        if( ctx.db_id == NOT_FIND )
        {
            DB tmp;
            tmp.db_name = ctx.current_db;
            user_list[ctx.user_id].db_list.push_back(tmp);
            ctx.db_id = index_db(user_list[ctx.user_id].db_list, ctx.current_db);
        }

        ctx.current_table = item->text(Ui::TABLE);
        ctx.table_id = index_table(
                    user_list[ctx.user_id].db_list[ctx.db_id].table_list, ctx.current_table);
        if( ctx.table_id == NOT_FIND )
        {
            Table table;
            table.table_name = ctx.current_table;
            user_list[ctx.user_id].db_list[ctx.db_id].table_list.push_back(table);
            ctx.table_id = index_table(
                        user_list[ctx.user_id].db_list[ctx.db_id].table_list, ctx.current_table);
            clear_priv_widget();
            inited = true;
            return;
        }
        reflesh_privileges();
    }
    //click db
    else
    {
        ctx.current_db = item->text(Ui::DB);
        ctx.db_id = index_db(user_list[ctx.user_id].db_list, ctx.current_db);
        if( ctx.db_id == NOT_FIND )
        {
            DB tmp;
            tmp.db_name = ctx.current_db;
            user_list[ctx.user_id].db_list.push_back(tmp);
            ctx.db_id = index_db(user_list[ctx.user_id].db_list, ctx.current_db);
            ctx.table_id = NOT_FIND;
            clear_priv_widget();
            inited = true;
            return;
        }
        ctx.table_id = NOT_FIND;
        reflesh_privileges();
    }
    inited = true;
}
