#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>

#include "modalconfig.h"
#include "jobtree.h"
#include "limiter.h"

namespace Ui {
class Widget;
}

class QMenu;

class Widget : public QWidget
{
	Q_OBJECT
private:
	QString cutName(QString s);
	QIcon getIconfromApp(QString app);
	void createMenu(QPoint pos);
	void setActions();
	void setConfigModal();
    void setConfigJobTree();
	void showAppOpt(int idx);
    int currentJobIdx();
public:
	explicit Widget(QWidget *parent = 0);
    void addJob(QString jobName);
    void addToJob(QString app, int jobId);
	void assingValues(int idx);
	~Widget();

private slots:
	void on_addBtn_clicked();
	void on_rmBtn_clicked();
	void runApp();
	void configApp();
    void addAppFile();


private:
	Ui::Widget *ui;

    JobTree *jobsTree;

    QList<QTreeWidgetItem *> items;

	QList<datos> appsOpt;

	QAction *run;
    QAction *addApp;
	QAction *config;
	QAction *remove;
	QMenu *menu;
	modalConfig *mod;
};

#endif // WIDGET_H
