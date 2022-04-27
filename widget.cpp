#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QIcon>
#include <QFileIconProvider>
#include <QFileDialog>
#include <QDebug>
#include <QMenu>

#define iconSize 64

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

void Widget::removeFromList(QString app)
{
    //si coinciden en nombre lo elimino....puede mejorar
    for(int i = 0; i < appsOpt.size(); i++)
    {
        auto el = appsOpt[i];
        if(cutName(el.appDir) == app){
            appsOpt.removeAt(i);
            return;
        }
    }
}

void Widget::createMenu(QPoint pos)
{
    QMenu menu(this);
    menu.addAction(run);
    menu.addAction(config);
    menu.addAction(remove);
    menu.exec(pos);
}

void Widget::setActions()
{
    run = new QAction("Run", this);
    config = new QAction("Config", this);
    remove = new QAction("Remove", this);

    connect(run, &QAction::triggered, this, &Widget::runApp);
    connect(config, &QAction::triggered, this, &Widget::configApp);
    connect(remove, &QAction::triggered, this, &Widget::on_rmBtn_clicked);
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setIconSize(QSize(iconSize,iconSize));
    setActions();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::addToList(QString app)
{

    //Agregar al listWidget
    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setText(cutName(app));
    newItem->setIcon(getIconfromApp(app));
    ui->listWidget->addItem(newItem);

    //activa el boton de eliminar
    connect(ui->listWidget, &QListWidget::itemSelectionChanged, this, [&](){
        ui->rmBtn->setEnabled(true);
    });

    //muestra el menu de opciones
    connect(ui->listWidget, &QListWidget::itemClicked, this, [&](){
        createMenu(cursor().pos());
    });

    //Agregar a la lista
    datos appData;
    appData.appDir = app;
    appData.workDir = app;
    appsOpt.append(appData);

}


void Widget::on_addBtn_clicked()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                tr("Open App"),
                                "/home",
                                tr("Exe Files (*.exe *.png *.jpg)"),
                                &selectedFilter);
    addToList(fileName);
}

void Widget::on_rmBtn_clicked()
{
    //Eliminar item del listWidget
    QList<QListWidgetItem*> lista = ui->listWidget->selectedItems();
    for(auto el : lista){
        removeFromList(el->text());
        ui->listWidget->removeItemWidget(el);
        delete el;
    }

}

void Widget::runApp()
{
    //Open the app....
    qDebug() << "YEAH, it runs\n";
}

void Widget::configApp()
{
    //open widget to config
}
