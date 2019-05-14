#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QPoint>
#include <QVector>



class Node : public QObject
{
    Q_OBJECT
public:
    explicit Node(QObject *parent = nullptr);
    QPoint m_point;
    QVector<QPoint *> m_nexts;
    bool selected;
signals:

public slots:

private:
};

#endif // NODE_H
