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
    for(int i = s.size()-1; i >= 0 && s[i] != '/'; i--){
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

void Widget::createMenu(QPoint pos)
{
    menu->popup(pos);

}

void Widget::setActions()
{
    run = new QAction(QIcon(":/images/play.png"),"Run", this);
    config = new QAction(QIcon(":/images/settings.png"),"Settings", this);
    remove = new QAction(QIcon(":/images/Trash.png"),"Remove", this);


    //esto conecta las acciones del menu de las apps
    connect(run, &QAction::triggered, this, &Widget::runApp);
    connect(config, &QAction::triggered, this, &Widget::configApp);
    connect(remove, &QAction::triggered, this, &Widget::on_rmBtn_clicked);

    //esto agrega las acciones al menu
    menu = new QMenu(this);
//    QString style = "background-color: rgb(255,0,0); border-radius: 6px; border-width: 4px;"
//                    "border-style:inset; border-color: rgb(0,255,255); background-color: rgb(0,255,0)";
//    menu->setStyleSheet(style);

    menu->addAction(run);
    menu->addAction(config);
    menu->addAction(remove);

}

void Widget::setConfigModal()
{
    mod = new modalConfig(this);

    connect(mod, &QDialog::accepted, this, [&](){
        int idx = ui->listWidget->currentRow();
        assingValues(idx);
    });
}


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setIconSize(QSize(iconSize,iconSize));

    //prepara el menu y el modal de configuracion
    setActions();
    setConfigModal();

    //testing purposes...
    addToList("/home/jmlopez/Documentsas/nasasdasda.jpg");
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

//Asigna las opciones a la app en la posicion idx...
void Widget::assingValues(int idx)
{
    datos currentApp = mod->getValues();
    currentApp.appDir =  appsOpt[idx].appDir;
    currentApp.workDir =  appsOpt[idx].workDir;
    appsOpt[idx] = currentApp;
}

//Agregar una nueva app
void Widget::on_addBtn_clicked()
{
//    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                tr("Open App"),
                                "/home",
                                tr("Exe Files (*.exe *.png *.jpg)"));

    if(fileName != nullptr){
        qDebug() << fileName << '\n';
        addToList(fileName);
    }
}

void Widget::on_rmBtn_clicked()
{
    //Eliminar item del listWidget
    /*
    Como solo se puede seleccionar uno a la vez no es necesario una lista,
    basta con seleccionar el actual (el currentItem...)

    QList<QListWidgetItem*> lista = ui->listWidget->selectedItems();
    for(auto el : lista){
        removeFromList(el->text());
        ui->listWidget->removeItemWidget(el);
        delete el;
    }
    */
    //Asi queda mucho mas limpio
    QListWidgetItem *item = ui->listWidget->currentItem();
    appsOpt.removeAt(ui->listWidget->row(item));
    ui->listWidget->removeItemWidget(item);
    delete item;

}

void Widget::runApp()
{
    //Open the app....
    int idx = ui->listWidget->currentRow();
    showAppOpt(idx);
}


void Widget::configApp()
{
    int idx = ui->listWidget->currentRow();
    mod->setValues(appsOpt[idx]);
    mod->show();
}

//testing purposes...
void Widget::showAppOpt(int idx)
{
    qDebug() << appsOpt[idx].MinimumWorkingSetSize
             << appsOpt[idx].MaximumWorkingSetSize
             << appsOpt[idx].ProcessMemoryLimit
             << appsOpt[idx].CheckMiliSec
             << appsOpt[idx].PauseMiliSec;
}
