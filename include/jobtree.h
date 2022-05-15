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
signals:
    void rightClick(QPoint pos);
};

#endif // JOBTREE_H
