#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QIcon>
#include <QFileIconProvider>
#include <QFileDialog>
#include <QDebug>

QString Widget::cutName(QString s)
{
    QString ans = "";
    for(int i = s.size()-1; i >= 0 && s[i] != '\/'; i--){
        ans = s[i] + ans;
    }
    return ans;
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
//    ui->listWidget->addItem("pepe");
    ui->listWidget->setViewMode(QListView::IconMode);
//    QListWidgetItem *newItem = new QListWidgetItem(ui->listWidget);
//    newItem->setText("itemText");
}

void Widget::addToList(QString app)
{
    QFileInfo f(app);
    QFileIconProvider *fi = new QFileIconProvider();
    QIcon ic = fi->icon(f);

    appsDir.append(app);

    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setText(cutName(app));
    newItem->setIcon(ic);

    ui->listWidget->addItem(newItem);
}


void Widget::on_addBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,tr("Open Image"), "/home", tr("Exe Files (*.exe *.jpg *.bmp *.png)"));
    addToList(fileName);
}

Widget::~Widget()
{
    delete ui;
}
