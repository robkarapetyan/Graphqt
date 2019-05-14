#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "mywidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void made_reverse();

public slots:

    void on_add_vertex_clicked();
    void on_addedge_clicked();
    void Receiving_objects_counter(int,int);

private:
    Ui::MainWindow *ui;

signals:
    void Add_vertex_signal(int);
    void Add_edge_signal(int);
    void show_blocks_signal(int);
    void delete_signal(int);
private slots:

    void on_show_blocks_clicked();
    void on_delete_2_clicked();
};

#endif // MAINWINDOW_H
