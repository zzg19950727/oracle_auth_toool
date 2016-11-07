#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QList>
#include <QFile>
#include <stdio.h>
#include <QDebug>
#include "Form.h"
#include "info.h"
#include "log.h"

namespace Ui {
class MainWindow;
enum {NAME=0, HOST};
enum {STATE=0, RIGHT, DESC, WITH_GRANT_OPTION};
enum {DB=0, TABLE};
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
/*******************************************/
private:
    void closeEvent(QCloseEvent *event);
    void init_UI();
    void init_menuBar();
    void init_ui_user_info();
    void connect_signal();
    void fill_data();
    void read_data();
    void load_file(QString filename);
    void save_data();
    void create_scripe(QString filename);
    void check_message();

private slots:
    void derive_out();

    void load();

    void quit();
/********************************************/
private slots:
    void on_add_account_clicked();

    void new_account();

    void menu_new_user_clicked();

    void new_user();

    void on_pushButton_new_right_clicked();

    void new_right();

    void on_pushButton_new_db_clicked();

    void new_db();

    void on_pushButton_new_table_clicked();

    void new_table();

    void on_pushButton_new_priv_clicked();

    void new_priv();

/********************************************/
private:
    void reflesh_account_info();

    void clear_account_info();

    void clear_DDL_right();

    void reflesh_DDL_right();

    void clear_priv_widget();

    void reflesh_privileges();

    void clear_table_widget();

private slots:
    void change_account();

    void change_table();
/*********************************************/
private:

private slots:
    void with_grant_option_changed(QTreeWidgetItem* item);

    void DDL_right_changed(QTreeWidgetItem*, int);

    void get_table_list(vector<int>& table_list);

    void change_priv_for_one_table(QString priv, int change);

    void privileges_changed(QTreeWidgetItem*, int);
/**********************************************/

    void remove_account(QString user);

    void on_delete_account_clicked();

    void on_pushButton_delete_DDL_clicked();

    void on_pushButton_delete_db_clicked();

    void on_pushButton_delete_priv_clicked();

    void change_password(QString);

    void on_pushButton_derive_clicked();

private:
    Ui::MainWindow *ui;
    Form* form;
    UserList user_list;     //all users privileges and info
    DDLRightList DDL_list;  //all DDL rights
    DBList db_list;         //all databases info
    PrivList priv_list;     //all privileges
    MyCtx ctx;              //current user info
    bool inited;
    bool have_dervied;
    vector<QString> list;
};

#endif // MAINWINDOW_H
