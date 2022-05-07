#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include "modalconfig.h"

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
    void showAppOpt(int idx);
public:
    explicit Widget(QWidget *parent = 0);
    void addToList(QString app);
    void assingValues(int idx);

    ~Widget();

private slots:
    void on_addBtn_clicked();
    void on_rmBtn_clicked();
    void runApp();
    void configApp();

private:
    Ui::Widget *ui;
    QList<datos> appsOpt;
    QAction *run;
    QAction *config;
    QAction *remove;
    QMenu *menu;
    modalConfig *mod;
};

#endif // WIDGET_H
