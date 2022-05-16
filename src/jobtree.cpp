#include "include/jobtree.h"
#include <QMouseEvent>
#include <QDebug>

#define iconSize 32
#define fontSize 12

JobTree::JobTree(QWidget *parent)
    : QTreeWidget(parent)
{
    setFont(QFont("Raleway", fontSize));
    setIconSize(QSize(iconSize,iconSize));
    setColumnCount(1);
    setHeaderHidden(true);

}

JobTree::~JobTree()
{

}

void JobTree::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton){
        emit rightClick(event->pos());
    }
    else {
        QTreeWidget::mousePressEvent(event);
    }
}

void JobTree::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == 16777220){//Enter
        emit enterPress();
    }
    else{
        QTreeWidget::keyPressEvent(event);
    }
}

