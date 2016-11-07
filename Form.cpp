#include "Form.h"

Form::Form(QWidget *parent) :
QWidget(parent)
{
    this->setFocus();
    this->setStyleSheet("background-image:url(:background.jpg)");
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":background.jpg")));
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    label_logo = new QLabel();
    label_logo->setMinimumHeight(65);
    label_logo->setStyleSheet("background-image:url(:oracle.png)");
    verticalLayout = new QVBoxLayout();
    verticalLayout->addWidget(label_logo);

    this->setMaximumWidth(230);
    this->setMinimumWidth(230);
}

void Form::new_user()
{
    horizontalLayout = new QHBoxLayout();
    label_name = new QLabel("用户名:");
    horizontalLayout->addWidget(label_name);
    label_name->setMinimumWidth(60);
    lineEdit_name = new QLineEdit();
    horizontalLayout->addWidget(lineEdit_name);
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);
    verticalLayout->addLayout(horizontalLayout);
/*
    horizontalLayout_2 = new QHBoxLayout();
    label_host = new QLabel("主机:");
    horizontalLayout_2->addWidget(label_host);
    label_host->setMinimumWidth(60);
    lineEdit_host = new QLineEdit();
    horizontalLayout_2->addWidget(lineEdit_host);
    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_2->addItem(horizontalSpacer_2);
    verticalLayout->addLayout(horizontalLayout_2);
*/
    horizontalLayout_3 = new QHBoxLayout();
    label_passwd = new QLabel("密码:");
    horizontalLayout_3->addWidget(label_passwd);
    label_passwd->setMinimumWidth(60);
    lineEdit_passwd = new QLineEdit();
    horizontalLayout_3->addWidget(lineEdit_passwd);
    horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_3->addItem(horizontalSpacer_3);
    verticalLayout->addLayout(horizontalLayout_3);

    horizontalLayout_4 = new QHBoxLayout();
    horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_4->addItem(horizontalSpacer_4);
    ok = new QPushButton("确定");
    horizontalLayout_4->addWidget(ok);
    horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_4->addItem(horizontalSpacer_5);
    verticalLayout->addLayout(horizontalLayout_4);

    this->setLayout( verticalLayout );
}

void Form::new_priv()
{
    horizontalLayout = new QHBoxLayout();
    label_name = new QLabel("权限名:");
    horizontalLayout->addWidget(label_name);
    label_name->setMinimumWidth(60);
    lineEdit_name = new QLineEdit();
    horizontalLayout->addWidget(lineEdit_name);
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);
    verticalLayout->addLayout(horizontalLayout);

    horizontalLayout_2 = new QHBoxLayout();
    label_desc = new QLabel("描述:");
    horizontalLayout_2->addWidget(label_desc);
    label_desc->setMinimumWidth(60);
    lineEdit_desc = new QLineEdit();
    horizontalLayout_2->addWidget(lineEdit_desc);
    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_2->addItem(horizontalSpacer_2);
    verticalLayout->addLayout(horizontalLayout_2);

    horizontalLayout_4 = new QHBoxLayout();
    horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_4->addItem(horizontalSpacer_4);
    ok = new QPushButton("确定");
    horizontalLayout_4->addWidget(ok);
    horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_4->addItem(horizontalSpacer_5);
    verticalLayout->addLayout(horizontalLayout_4);

    this->setLayout( verticalLayout );
}

void Form::new_db_or_table()
{
    horizontalLayout = new QHBoxLayout();
    label_name = new QLabel("名称:");
    horizontalLayout->addWidget(label_name);
    label_name->setMinimumWidth(60);
    lineEdit_name = new QLineEdit();
    horizontalLayout->addWidget(lineEdit_name);
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);
    verticalLayout->addLayout(horizontalLayout);

    horizontalLayout_4 = new QHBoxLayout();
    horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_4->addItem(horizontalSpacer_4);
    ok = new QPushButton("确定");
    horizontalLayout_4->addWidget(ok);
    horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_4->addItem(horizontalSpacer_5);
    verticalLayout->addLayout(horizontalLayout_4);

    this->setLayout( verticalLayout );
}
