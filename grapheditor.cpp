#include "grapheditor.h"
#include <QDebug>

GraphEditor::GraphEditor(QWidget *parent) : QWidget(parent)
{

}

void GraphEditor::addvertex()
{
    fabc.push_back(fabc.size());
       ++a;
    qDebug() << a;
}

void GraphEditor::showsize()
{
    qDebug() << a;
}

