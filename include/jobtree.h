#ifndef JOBTREE_H
#define JOBTREE_H

#include <QTreeWidget>

class JobTree : public QTreeWidget
{
    Q_OBJECT
public:
    JobTree(QWidget *parent = 0);
    ~JobTree();
private:
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
signals:
    void rightClick(QPoint pos);
    void enterPress();
};

#endif // JOBTREE_H
