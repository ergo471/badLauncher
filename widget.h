#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>

namespace Ui {
class Widget;
}

typedef struct{
    QString dir;
    int memory;
}datos;

class Widget : public QWidget
{
    Q_OBJECT
private:
    QString cutName(QString s);
    QIcon getIconfromApp(QString app);
public:
    explicit Widget(QWidget *parent = 0);
    void addToList(QString app);
    ~Widget();

private slots:
    void on_addBtn_clicked();

private:
    Ui::Widget *ui;
    QList<QString> appsDir;
    QList<datos> pap;
};

#endif // WIDGET_H
