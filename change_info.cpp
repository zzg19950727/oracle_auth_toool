#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "list_search.h"

enum STAT change_state(enum STAT ori, int change)
{
    int n = ori;
    n += change;
    return (enum STAT)n;
}

void MainWindow::with_grant_option_changed(QTreeWidgetItem *item)
{
    QString priv = item->text(Ui::RIGHT);
    if(ctx.db_id == NOT_FIND)
    {
        int n = index_priv(user_list[ctx.user_id].DDL_list, priv);
        if(n == NOT_FIND)return;
        if(item->checkState(Ui::WITH_GRANT_OPTION) == Qt::Checked)
            user_list[ctx.user_id].DDL_list[n].with_grant_option = true;
        else
            user_list[ctx.user_id].DDL_list[n].with_grant_option = false;
    }
    else if(ctx.table_id == NOT_FIND)
    {
        int n = index_priv(user_list[ctx.user_id].db_list[ctx.db_id].priv_list, priv);
        if(n == NOT_FIND)return;
        if(item->checkState(Ui::WITH_GRANT_OPTION) == Qt::Checked)
            user_list[ctx.user_id].db_list[ctx.db_id].priv_list[n].with_grant_option = true;
        else
            user_list[ctx.user_id].db_list[ctx.db_id].priv_list[n].with_grant_option = false;
    }
    else
    {
        int n = index_priv(user_list[ctx.user_id].db_list[ctx.db_id].\
                table_list[ctx.table_id].priv_list, priv);
        if(n == NOT_FIND)return;
        if(item->checkState(Ui::WITH_GRANT_OPTION) == Qt::Checked)
            user_list[ctx.user_id].db_list[ctx.db_id].\
                            table_list[ctx.table_id].priv_list[n].with_grant_option = true;
        else
            user_list[ctx.user_id].db_list[ctx.db_id].\
                            table_list[ctx.table_id].priv_list[n].with_grant_option = false;
    }
}


void MainWindow::DDL_right_changed(QTreeWidgetItem* item, int col)
{
    if( !inited )
        return;
    ctx.db_id = NOT_FIND;
    if(ctx.user_id==NOT_FIND)
    {
        QMessageBox::warning(this, "错误", "请选择一个用户");
        reflesh_DDL_right();
        return;
    }
    if(col == Ui::WITH_GRANT_OPTION)
    {
        with_grant_option_changed(item);
        return;
    }
    if(col!=Ui::STATE)
        return;

    int change;
    if(item->checkState(Ui::STATE) == Qt::Checked)
        change = 1;
    else
        change = -1;
    QString right = item->text(Ui::RIGHT);
    int n = index_priv(user_list[ctx.user_id].DDL_list, right);
    if( n== NOT_FIND )
    {
        assert(change == 1);
        Priv tmp;
        tmp.str = right;
        tmp.state = GRANT;
        tmp.with_grant_option = false;
        user_list[ctx.user_id].DDL_list.push_back(tmp);
        /*ui->treeWidget_right->setColumnCount(4);
        ui->treeWidget_right->setColumnWidth(2, 150);
        item->setCheckState(Ui::WITH_GRANT_OPTION, Qt::Unchecked);*/
    }
    else
    {
        enum STAT ori_state = user_list[ctx.user_id].DDL_list[n].state;
        user_list[ctx.user_id].DDL_list[n].state = change_state(ori_state, change);
    }
    inited = false;
    reflesh_DDL_right();
    inited = true;
}

void MainWindow::get_table_list(vector<int> &table_list)
{
    QTreeWidgetItem* item = ui->treeWidget_table_info->currentItem();
    if( !item )
        return;
    QTreeWidgetItem* parent = item->parent();
    if(parent)
        item = parent;
    ctx.current_db = item->text(Ui::DB);
    ctx.db_id = index_db(user_list[ctx.user_id].db_list, ctx.current_db);
    if( ctx.db_id == NOT_FIND )
    {
        DB tmp;
        tmp.db_name = ctx.current_db;
        user_list[ctx.user_id].db_list.push_back(tmp);
        ctx.db_id = index_db(user_list[ctx.user_id].db_list, ctx.current_db);
    }

    int child_count = item->childCount();
    for(int i=0; i<child_count; i++)
    {
        QTreeWidgetItem* child = item->child(i);
        if(child->checkState(Ui::DB) == Qt::Checked)
        {
            QString table_name = child->text(Ui::TABLE);
            int table_id = index_table(
                        user_list[ctx.user_id].db_list[ctx.db_id].table_list, table_name);
            if( table_id == NOT_FIND )
            {
                Table table;
                table.table_name = table_name;
                user_list[ctx.user_id].db_list[ctx.db_id].table_list.push_back(table);
                table_id = index_table(
                            user_list[ctx.user_id].db_list[ctx.db_id].table_list, table_name);
            }
            table_list.push_back(table_id);
        }

    }
}

void MainWindow::change_priv_for_one_table(QString priv, int change)
{
    int n = index_priv(user_list[ctx.user_id].db_list[ctx.db_id].\
            table_list[ctx.table_id].priv_list, priv);
    if( n== NOT_FIND )
    {
        //assert(change == 1);
        Priv tmp;
        tmp.str = priv;
        tmp.state = GRANT;
        tmp.with_grant_option = false;
        user_list[ctx.user_id].db_list[ctx.db_id].\
                table_list[ctx.table_id].priv_list.push_back(tmp);
    }
    else
    {
        enum STAT ori_state =
                user_list[ctx.user_id].db_list[ctx.db_id].\
                table_list[ctx.table_id].priv_list[n].state;
        user_list[ctx.user_id].db_list[ctx.db_id].\
                table_list[ctx.table_id].priv_list[n].state =
                change_state(ori_state, change);
    }
}

void MainWindow::privileges_changed(QTreeWidgetItem* item, int col)
{
    if(col!=Ui::WITH_GRANT_OPTION && col!=Ui::STATE)
        return;

    if( !inited )
        return;

    vector<int> table_list;
    get_table_list(table_list);
    if(ctx.user_id==NOT_FIND)
    {
        QMessageBox::warning(this, "错误", "请选择一个用户");
        reflesh_privileges();
        return;
    }
    else if(ctx.db_id==NOT_FIND || table_list.empty())
    {
        QMessageBox::warning(this, "错误", "请选择一张表");
        reflesh_privileges();
        return;
    }
    if(col == Ui::WITH_GRANT_OPTION)
    {
        for(int i=0; i<table_list.size(); i++)
        {
            ctx.table_id = table_list[i];
            with_grant_option_changed(item);
        }

        return;
    }

    int change;
    if(item->checkState(Ui::STATE) == Qt::Checked)
        change = 1;
    else
        change = -1;
    QString priv = item->text(Ui::RIGHT);
    /*//change privilege for db.*
    if(ctx.table_id == NOT_FIND)
    {
        return;
        int n = index_priv(user_list[ctx.user_id].db_list[ctx.db_id].priv_list, priv);
        if( n== NOT_FIND )
        {
            assert(change == 1);
            Priv tmp;
            tmp.str = priv;
            tmp.state = GRANT;
            tmp.with_grant_option = false;
            user_list[ctx.user_id].db_list[ctx.db_id].priv_list.push_back(tmp);
        }
        else
        {
            enum STAT ori_state =
                    user_list[ctx.user_id].db_list[ctx.db_id].priv_list[n].state;
            user_list[ctx.user_id].db_list[ctx.db_id].priv_list[n].state =
                    change_state(ori_state, change);
        }
    }
    //change privilege for db.table
    else
    {
        int n = index_priv(user_list[ctx.user_id].db_list[ctx.db_id].\
                table_list[ctx.table_id].priv_list, priv);
        if( n== NOT_FIND )
        {
            assert(change == 1);
            Priv tmp;
            tmp.str = priv;
            tmp.state = GRANT;
            tmp.with_grant_option = false;
            user_list[ctx.user_id].db_list[ctx.db_id].\
                    table_list[ctx.table_id].priv_list.push_back(tmp);
        }
        else
        {
            enum STAT ori_state =
                    user_list[ctx.user_id].db_list[ctx.db_id].\
                    table_list[ctx.table_id].priv_list[n].state;
            user_list[ctx.user_id].db_list[ctx.db_id].\
                    table_list[ctx.table_id].priv_list[n].state =
                    change_state(ori_state, change);
        }
    }*/
    for(int i=0; i<table_list.size(); i++)
    {
        ctx.table_id = table_list[i];
        change_priv_for_one_table(priv, change);
    }
    reflesh_privileges();
}
