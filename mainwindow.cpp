#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "list_search.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    inited = false;
    have_dervied = false;
    ctx.user_id = NOT_FIND;
    ctx.db_id = NOT_FIND;
    init_UI();
    connect_signal();
    get_all_time(list);
    sort_time(list);
    read_data();
    fill_data();
    check_message();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    if(!have_dervied)
    {
        if( QMessageBox::information(this, "通知", "是否导出脚本?",
                   QMessageBox::Yes, QMessageBox::No)  == QMessageBox::Yes)
        {
            derive_out();
        }
    }
}

void MainWindow::init_UI()
{

    init_menuBar();
    init_ui_user_info();
    ui->treeWidget_user_accounts->setMaximumWidth(210);
    ui->tabWidget_user_info->setMinimumWidth(500);
    ui->treeWidget_right->setColumnWidth(0, 40);
    ui->treeWidget_privileges->setColumnWidth(0, 40);
}

void MainWindow::init_menuBar()
{
    ui->menuBar->clear();
    QMenu* file = new QMenu("文件(F)");
    ui->menuBar->addMenu(file);
    QAction* create_user = new QAction("创建用户", this);
    connect(create_user, SIGNAL(triggered(bool)), this, SLOT(menu_new_user_clicked()));
    QAction* derive = new QAction("载入", this);
    connect(derive, SIGNAL(triggered(bool)), this, SLOT(load()));
    QAction* quit = new QAction("退出", this);
    connect(quit, SIGNAL(triggered(bool)), this, SLOT(quit()));

    file->addAction(create_user);
    file->addAction(derive);
    file->addAction(quit);
}

void MainWindow::init_ui_user_info()
{
    ui->label_user_name->setMinimumWidth(85);
    //ui->label_host->setMinimumWidth(85);
    ui->label_password->setMinimumWidth(85);
    ui->lineEdit_user_name->setMaximumWidth(250);
    ui->lineEdit_user_name->setMinimumWidth(250);
    //ui->lineEdit_host->setMaximumWidth(250);
    //ui->lineEdit_host->setMinimumWidth(250);
    ui->lineEdit_password->setMaximumWidth(250);
    ui->lineEdit_password->setMinimumWidth(250);

}

void MainWindow::connect_signal()
{
    connect(ui->treeWidget_table_info, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(change_table()));
    connect(ui->treeWidget_user_accounts, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(change_account()));
    connect(ui->treeWidget_right, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(DDL_right_changed(QTreeWidgetItem*,int)));
    connect(ui->treeWidget_privileges, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(privileges_changed(QTreeWidgetItem*,int)));
    connect(ui->lineEdit_password, SIGNAL(textEdited(QString)), this, SLOT(change_password(QString)));
}

void MainWindow::fill_data()
{
    QTreeWidgetItem* item;
    ui->treeWidget_user_accounts->clear();
    for(unsigned int i=0; i<user_list.size(); i++)
    {
        item = new QTreeWidgetItem;
        item->setText(Ui::NAME, user_list[i].name);
        //item->setText(Ui::HOST, user_list[i].host);
        ui->treeWidget_user_accounts->addTopLevelItem(item);
    }

    ui->treeWidget_right->clear();
    for(unsigned int i=0; i<DDL_list.size(); i++)
    {
        item = new QTreeWidgetItem;
        item->setCheckState(Ui::STATE, Qt::Unchecked);
        item->setText(Ui::RIGHT, DDL_list[i].str);
        item->setText(Ui::DESC, DDL_list[i].desc);
        ui->treeWidget_right->addTopLevelItem(item);
    }

    ui->treeWidget_privileges->clear();
    for(unsigned int i=0; i<priv_list.size(); i++)
    {
        item = new QTreeWidgetItem;
        item->setCheckState(Ui::STATE, Qt::Unchecked);
        item->setText(Ui::RIGHT, priv_list[i].str);
        item->setText(Ui::DESC, priv_list[i].desc);
        ui->treeWidget_privileges->addTopLevelItem(item);
    }

    ui->treeWidget_table_info->clear();
    for(unsigned int i=0; i<db_list.size(); i++)
    {
        item = new QTreeWidgetItem;
        item->setText(Ui::DB, db_list[i].db_name);
        ui->treeWidget_table_info->addTopLevelItem(item);
        QTreeWidgetItem *child;

        for(unsigned int j=0; j<db_list[i].table_list.size(); j++)
        {
            child = new QTreeWidgetItem;
            child->setText(Ui::TABLE, db_list[i].table_list[j].table_name);
            child->setCheckState(Ui::DB, Qt::Unchecked);
            item->addChild(child);
        }
    }
}

void MainWindow::derive_out()
{
    QString filename = QFileDialog::getSaveFileName(this, "导出", " ", "*.sql");
    filename += ".sql";
    create_scripe(filename);
    have_dervied = true;
    save_data();
}

void MainWindow::load()
{
    QString filename = QFileDialog::getOpenFileName(this, "载入", "data\\", "state (*.dat)");
    if(filename.length() == 0)
        return;
    load_file(filename);
    fill_data();
}


void MainWindow::quit()
{
    this->close();
}

void MainWindow::remove_account(QString user)
{
    UserList::iterator iter = user_list.begin();
    for(int i=0; iter!=user_list.end(); iter++,i++)
    {
        if(user_list[i].name==user /*&& user_list[i].host==host*/)
        {
            user_list.erase(iter);
            return;
        }
    }
}

void MainWindow::on_delete_account_clicked()
{
    QTreeWidgetItem* item;
    item = ui->treeWidget_user_accounts->currentItem();
    if( !item )
        return;

    QString user = item->text(Ui::NAME);
    //QString host = item->text(Ui::HOST);
    remove_account(user);
    delete item;
}

void MainWindow::on_pushButton_delete_DDL_clicked()
{
    QTreeWidgetItem* item;
    item = ui->treeWidget_right->currentItem();
    if( !item )
        return;

    QString ddl_right = item->text(Ui::RIGHT);
    DDLRightList::iterator iter = DDL_list.begin();
    for(int i=0; iter!=DDL_list.end(); iter++,i++)
    {
        if(DDL_list[i].str==ddl_right)
        {
            DDL_list.erase(iter);
            break;
        }
    }
    delete item;
}

void MainWindow::on_pushButton_delete_db_clicked()
{
    QTreeWidgetItem* item;
    item = ui->treeWidget_table_info->currentItem();
    if( !item )
        return;
    QTreeWidgetItem* parent = item->parent();
    //delete table
    if(parent)
    {
        QString DB = parent->text(Ui::DB);
        QString table = item->text(Ui::TABLE);
        int db_n = index_db(db_list, DB);
        if(db_n == NOT_FIND)
            return;
        TableList::iterator iter = db_list[db_n].table_list.begin();
        for(int i=0; iter!=db_list[db_n].table_list.end(); i++,iter++)
        {
            if(db_list[db_n].table_list[i].table_name == table)
            {
                db_list[db_n].table_list.erase(iter);
                break;
            }
        }
    }
    else
    {
        QString DB = item->text(Ui::DB);
        DBList::iterator iter = db_list.begin();
        for(int i=0; iter!=db_list.end(); i++,iter++)
        {
            if(db_list[i].db_name == DB)
            {
                db_list.erase(iter);
                break;
            }
        }
    }
    delete item;
}

void MainWindow::on_pushButton_delete_priv_clicked()
{
    QTreeWidgetItem* item;
    item = ui->treeWidget_privileges->currentItem();
    if( !item )
        return;

    QString priv = item->text(Ui::RIGHT);
    PrivList::iterator iter = priv_list.begin();
    for(int i=0; iter!=priv_list.end(); iter++,i++)
    {
        if(priv_list[i].str==priv)
        {
            priv_list.erase(iter);
            break;
        }
    }
    delete item;
}

void MainWindow::change_password(QString new_password)
{
    if(ctx.user_id == NOT_FIND)
        return;
    user_list[ctx.user_id].new_password = new_password;
}

void MainWindow::on_pushButton_derive_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "导出", " ", "*.sql");
    if(filename.length() == 0)
        return;
    filename += ".sql";
    create_scripe(filename);
    have_dervied = true;
    save_data();
}

void MainWindow::check_message()
{
    QString file = get_lastest_time(list);
    if(QMessageBox::information(this, "message", file+"的操作是否入库?", QMessageBox::Yes, QMessageBox::No)
            == QMessageBox::No )
    {
        QString new_file = get_last_file(list);
        if(QMessageBox::information(this, "message", "是否恢复程序到"+new_file+"?", QMessageBox::Yes, QMessageBox::No)
                    == QMessageBox::Yes)
        {
            load_file(new_file);
            fill_data();
        }
    }
}
