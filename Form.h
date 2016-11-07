#ifndef Form_H
#define Form_H
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QMessageBox>
#include <QCheckBox>
#include <QComboBox>
#include <QList>

class Form : public QWidget
{
    Q_OBJECT
public:
    explicit Form(QWidget *parent = 0);
    void new_user();
    void new_priv();
    void new_db_or_table();

public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_4;

    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_5;

    QLabel *label_logo;

    QLabel *label_name;
    QLineEdit *lineEdit_name;

    QLabel *label_host;
    QLineEdit *lineEdit_host;

    QLabel *label_passwd;
    QLineEdit *lineEdit_passwd;

    QLabel *label_desc;
    QLineEdit *lineEdit_desc;

    QPushButton *ok;
};

#endif // Form_H
