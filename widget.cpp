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

QIcon Widget::getIconfromApp(QString app)
{
    QFileInfo file(app);
    QFileIconProvider *fIcon = new QFileIconProvider();
    QIcon icon = fIcon->icon(file);
    return icon;
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->listWidget->setViewMode(QListView::IconMode);
}

void Widget::addToList(QString app)
{

    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setText(cutName(app));
    newItem->setIcon(getIconfromApp(app));
    ui->listWidget->addItem(newItem);

    datos appData;
    appData.dir = appData;
    appsDir.append(app);

}


void Widget::on_addBtn_clicked()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                tr("Open App"),
                                "/home",
                                tr("Exe Files (*.exe)"),
                                &selectedFilter);
//    QString fileName = QFileDialog::getOpenFileName(
//                this,tr("Open Image"), "/home", tr("Exe Files (*.exe *.jpg *.bmp *.png)"));
    addToList(fileName);
}

Widget::~Widget()
{
    delete ui;
}
