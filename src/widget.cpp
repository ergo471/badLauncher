#include <QFile>
#include <QProcess>
#include <QIcon>
#include <QFileIconProvider>
#include <QFileDialog>
#include <QDebug>
#include <QMenu>

#include "../include/widget.h"
#include "ui_widget.h"


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
	run = new QAction(QIcon(":/play.png"),"Run", this);
    addApp = new QAction(QIcon(":/plus-round.png"),"Add New", this);
	config = new QAction(QIcon(":/settings.png"),"Settings", this);
	remove = new QAction(QIcon(":/Trash.png"),"Remove", this);


	//esto conecta las acciones del menu de las apps
	connect(run, &QAction::triggered, this, &Widget::runApp);
    connect(addApp, &QAction::triggered, this, &Widget::addAppFile);
	connect(config, &QAction::triggered, this, &Widget::configApp);
	connect(remove, &QAction::triggered, this, &Widget::on_rmBtn_clicked);

	//esto agrega las acciones al menu
	menu = new QMenu(this);

	menu->addAction(run);
    menu->addAction(addApp);
	menu->addAction(config);
	menu->addAction(remove);

}

//A very useful function :)
int Widget::currentJobIdx()
{
    int cont = 0;
    foreach(QTreeWidgetItem *el, items){
        if(el == jobsTree->currentItem()){
            break;
        }
        cont++;
    }
    return cont;
}

void Widget::setConfigModal()
{
	mod = new modalConfig(this);

	connect(mod, &QDialog::accepted, this, [&](){
        int idx = currentJobIdx();
        assingValues(idx);
    });
}

void Widget::setConfigJobTree()
{
    //activa el boton de eliminar
    connect(jobsTree, &JobTree::clicked, this, [=](){
        ui->rmBtn->setEnabled(true);
    });
    connect(jobsTree, &JobTree::doubleClicked, this, [=](){
        if(jobsTree->currentItem()->parent() == nullptr)
            jobsTree->openPersistentEditor(jobsTree->currentItem(),0);
    });
    connect(jobsTree, &JobTree::currentItemChanged, this, [=](QTreeWidgetItem *curr,QTreeWidgetItem *prev){
        if(jobsTree->isPersistentEditorOpen(prev))
            jobsTree->closePersistentEditor(prev,0);
        (void)curr;
    });

    //muestra el menu de opciones
    connect(jobsTree, &JobTree::rightClick, this, [=](QPoint pos){
        QTreeWidgetItem *el = jobsTree->itemAt(pos);
        if(el!=jobsTree->currentItem()){
            return;
        }
        if(el->parent()==nullptr)
            createMenu(cursor().pos());
    });
}


Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);
    jobsTree = new JobTree(this);

    ui->horizontalLayout->insertWidget(0,jobsTree);

	//prepara el menu y el modal de configuracion
	setActions();
    setConfigModal();
    setConfigJobTree();


    //Testing purposes
    addJob("Job");
    jobsTree->setCurrentItem(items[0]);
//    addToJob("/home/jmlopez/Documents/example.jpg",0);

}


Widget::~Widget()
{
	delete ui;
}

void Widget::addJob(QString jobName)
{
    QTreeWidgetItem *newItem = new QTreeWidgetItem(jobsTree,QStringList(jobName));
    newItem->setIcon(0, QIcon(":/cube.png"));

    items.append(newItem);
    jobsTree->addTopLevelItems(items);
    jobsTree->setCurrentItem(newItem);
    datos appData;
    appsOpt.append(appData);
}

void Widget::addToJob(QString app, int jobId)
{

    QTreeWidgetItem *newItem = new QTreeWidgetItem(items[jobId]);
    newItem->setText(0,cutName(app));
    newItem->setIcon(0,getIconfromApp(app));

    //Agregar a la lista
    int idx = currentJobIdx();
    appsOpt[idx].appDir.append(app);
    appsOpt[idx].workDir.append(app);
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
    addJob("Job");
}

void Widget::on_rmBtn_clicked()
{
    QTreeWidgetItem *item = jobsTree->currentItem();
    if(item->parent() == nullptr){
        int idx = currentJobIdx();
        appsOpt.removeAt(idx);
    }
    items.removeOne(item);
    jobsTree->removeItemWidget(item,0);

	delete item;

}

void Widget::runApp()
{
	//Open the app....
    int idx = currentJobIdx();
	Limiter algo;

    for(auto el : appsOpt[idx].appDir){
        algo.addApp(el.toStdString());
    }


	algo.setProcessMemoryLimit(appsOpt[idx].ProcessMemoryLimit);

	algo.setMaximumWorkingSetSize(appsOpt[idx].MaximumWorkingSetSize);
	algo.setMinimumWorkingSetSize(appsOpt[idx].MinimumWorkingSetSize);

	algo.applyLimits();
//	algo.run();
    showAppOpt(idx);
}


void Widget::configApp()
{
    int idx = currentJobIdx();
	mod->setValues(appsOpt[idx]);
    mod->show();
}

void Widget::addAppFile()
{
    //This code is for the action Add not for Add a job
    QString fileName = QFileDialog::getOpenFileName(this,
                        tr("Open App"),
                        "/home",
                        tr("Exe Files (*.exe *.png *.jpg)"));

    if(fileName != nullptr){
        qDebug() << fileName << '\n';
        addToJob(fileName, currentJobIdx());
    }
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
