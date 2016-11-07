#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "list_search.h"

void MainWindow::on_add_account_clicked()
{
    form = new Form();
    form->new_user();
    form->show();
    connect(form->ok, SIGNAL(clicked(bool)), this, SLOT(new_account()) );
}

void MainWindow::new_account()
{
    disconnect(form->ok, SIGNAL(clicked(bool)), this, SLOT(new_account()) );
    INFO tmp;
    tmp.name = form->lineEdit_name->text();
    //tmp.host = form->lineEdit_host->text();
    tmp.password = form->lineEdit_passwd->text();
    tmp.create = false;

    if(tmp.name.length() && tmp.password.length() )
    {
        if(index_user(user_list, tmp.name, tmp.host) == NOT_FIND)
        {
            user_list.push_back(tmp);
            QTreeWidgetItem *item = new QTreeWidgetItem;
            item->setText(Ui::NAME, tmp.name);
            ///item->setText(Ui::HOST, tmp.host);
            ui->treeWidget_user_accounts->addTopLevelItem(item);
        }
        else
        {
            QString text = QString("账户 %1 已存在!").arg(tmp.name);
            QMessageBox::warning(this, "错误", text);
        }
    }

    delete form;
    form = NULL;
}

void MainWindow::menu_new_user_clicked()
{
    form = new Form();
    form->new_user();
    form->show();
    connect(form->ok, SIGNAL(clicked(bool)), this, SLOT(new_user()) );
}

void MainWindow::new_user()
{
    disconnect(form->ok, SIGNAL(clicked(bool)), this, SLOT(new_user()) );
    INFO tmp;
    tmp.name = form->lineEdit_name->text();
    //tmp.host = form->lineEdit_host->text();
    tmp.password = form->lineEdit_passwd->text();
    tmp.create = true;

    if(tmp.name.length() && tmp.password.length() )
    {
        if(index_user(user_list, tmp.name, tmp.host) == NOT_FIND)
        {
            user_list.push_back(tmp);
            QTreeWidgetItem *item = new QTreeWidgetItem;
            item->setText(Ui::NAME, tmp.name);
            //item->setText(Ui::HOST, tmp.host);
            ui->treeWidget_user_accounts->addTopLevelItem(item);
        }
        else
        {
            QString text = QString("用户 %1 已存在!").arg(tmp.name);
            QMessageBox::warning(this, "错误", text);
        }
    }

    delete form;
    form = NULL;
}

void MainWindow::on_pushButton_new_right_clicked()
{
    form = new Form();
    form->new_priv();
    form->show();
    connect(form->ok, SIGNAL(clicked(bool)), this, SLOT(new_right()) );
}

void MainWindow::new_right()
{
    disconnect(form->ok, SIGNAL(clicked(bool)), this, SLOT(new_right()) );
    Priv tmp;
    tmp.str = form->lineEdit_name->text();
    tmp.desc = form->lineEdit_desc->text();

    if(tmp.str.length() && tmp.desc.length())
    {
        if(index_priv(DDL_list, tmp.str) == NOT_FIND)
        {
            DDL_list.push_back(tmp);
            QTreeWidgetItem *item = new QTreeWidgetItem;

            item->setText(Ui::RIGHT, tmp.str);
            item->setText(Ui::DESC, tmp.desc);

            item->setCheckState(Ui::STATE, Qt::Unchecked);
            ui->treeWidget_right->addTopLevelItem(item);
        }
        else
        {
            QString text = QString("权限 %1 已存在!").arg(tmp.str);
            QMessageBox::warning(this, "错误", text);
        }
    }

    delete form;
    form = NULL;
}

void MainWindow::on_pushButton_new_db_clicked()
{
    form = new Form();
    form->new_db_or_table();
    form->show();
    connect(form->ok, SIGNAL(clicked(bool)), this, SLOT(new_db()) );
}

void MainWindow::new_db()
{
    disconnect(form->ok, SIGNAL(clicked(bool)), this, SLOT(new_db()) );
    DB tmp;
    tmp.db_name = form->lineEdit_name->text();

    if(tmp.db_name.length())
    {
        if(index_db(db_list, tmp.db_name) == NOT_FIND)
        {
            db_list.push_back(tmp);
            QTreeWidgetItem *item = new QTreeWidgetItem;
            item->setText(Ui::DB, tmp.db_name);
            ui->treeWidget_table_info->addTopLevelItem(item);
        }
        else
        {
            QString text = QString("数据库 %1 已存在!").arg(tmp.db_name);
            QMessageBox::warning(this, "错误", text);
        }
    }

    delete form;
    form = NULL;
}

void MainWindow::on_pushButton_new_table_clicked()
{
    if( !ui->treeWidget_table_info->currentItem() )//没选择数据库
        return;
    else if( !ui->treeWidget_table_info->currentItem()->text(Ui::DB).length() )//选择的是表
        return;
    form = new Form();
    form->new_db_or_table();
    form->show();
    connect(form->ok, SIGNAL(clicked(bool)), this, SLOT(new_table()) );
}

void MainWindow::new_table()
{
    disconnect(form->ok, SIGNAL(clicked(bool)), this, SLOT(new_table()) );
    Table tmp;
    tmp.table_name = form->lineEdit_name->text();

    if(tmp.table_name.length())
    {
        QTreeWidgetItem *item_db = ui->treeWidget_table_info->currentItem();
        int n = index_db(db_list, item_db->text(0));
        if( n != NOT_FIND )
        {
            if(index_table(db_list[n].table_list, tmp.table_name) == NOT_FIND)
            {
                db_list[n].table_list.push_back(tmp);
                QTreeWidgetItem *item_table = new QTreeWidgetItem;
                item_table->setText(Ui::TABLE, tmp.table_name);
                item_table->setCheckState(Ui::DB, Qt::Unchecked);
                item_db->addChild(item_table);
            }
            else
            {
                QString text = QString("表 %1 已存在于 %2!").arg(tmp.table_name).arg(item_db->text(Ui::DB));
                QMessageBox::warning(this, "错误", text);
            }
        }
    }

    delete form;
    form = NULL;
}

void MainWindow::on_pushButton_new_priv_clicked()
{
    form = new Form();
    form->new_priv();
    form->show();
    connect(form->ok, SIGNAL(clicked(bool)), this, SLOT(new_priv()) );
}

void MainWindow::new_priv()
{
    disconnect(form->ok, SIGNAL(clicked(bool)), this, SLOT(new_priv()) );
    Priv tmp;
    tmp.str = form->lineEdit_name->text();
    tmp.desc = form->lineEdit_desc->text();

    if(tmp.str.length() && tmp.desc.length())
    {
        if(index_priv(priv_list, tmp.str) == NOT_FIND)
        {
            priv_list.push_back(tmp);
            QTreeWidgetItem *item = new QTreeWidgetItem;
            item->setCheckState(Ui::STATE, Qt::Unchecked);

            item->setText(Ui::RIGHT, tmp.str);
            item->setText(Ui::DESC, tmp.desc);
            ui->treeWidget_privileges->addTopLevelItem(item);
        }
        else
        {
            QString text = QString("权限 %1 已存在!").arg(tmp.str);
            QMessageBox::warning(this, "错误", text);
        }
    }

    delete form;
    form = NULL;
}

