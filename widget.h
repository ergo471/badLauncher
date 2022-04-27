#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>

namespace Ui {
class Widget;
}

typedef struct{
    QString appDir, workDir;
    unsigned long long MinimumWorkingSetSize = 256,
                       MaximumWorkingSetSize = 2000,
                       ProcessMemoryLimit;
    unsigned long long CheckMiliSec = 1000, PauseMiliSec = 0;
}datos;

class Widget : public QWidget
{
    Q_OBJECT
private:
    QString cutName(QString s);
    QIcon getIconfromApp(QString app);
    void removeFromList(QString app);
    void createMenu(QPoint pos);
    void setActions();
public:
    explicit Widget(QWidget *parent = 0);
    void addToList(QString app);
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
};

#endif // WIDGET_H
