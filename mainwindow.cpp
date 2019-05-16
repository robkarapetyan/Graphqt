#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mywidget.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(this,SIGNAL(Add_vertex_signal(int)),ui->xmywidget,SLOT(Receiving_button(int)));
    QObject::connect(this,SIGNAL(Add_edge_signal(int)),ui->xmywidget,SLOT(Receiving_button(int)));
    QObject::connect(this,SIGNAL(delete_signal(int)),ui->xmywidget,SLOT(Receiving_button(int)));
    QObject::connect(ui->xmywidget,SIGNAL(objects_total_signal(size_t,size_t)),this,SLOT(Receiving_objects_counter(size_t,size_t)));
    QObject::connect(this,SIGNAL(show_blocks_signal(int)),ui->xmywidget,SLOT(Receiving_button(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_add_vertex_clicked()
{
    emit Add_vertex_signal(1);
    ui->label->setText("Vertex edit mode");
}

void MainWindow::on_addedge_clicked()
{
    emit Add_edge_signal(2);
    ui->label->setText("Edge edit mode");
}

void MainWindow::Receiving_objects_counter(size_t v, size_t e)
{
    auto printable = QStringLiteral("Vertexes total: %1").arg(v);
    auto printable1 = QStringLiteral("Edges total: %1").arg(e);

    ui->label_2->setText(printable);
    ui->label_3->setText(printable1);
}

void MainWindow::on_show_blocks_clicked()
{
    emit show_blocks_signal(3);
}

void MainWindow::on_delete_2_clicked()
{
    emit delete_signal(4);
}
