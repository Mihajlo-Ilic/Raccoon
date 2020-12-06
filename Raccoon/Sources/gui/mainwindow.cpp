#include "../../Includes/gui/mainwindow.h"
#include "ui_mainwindow.h"
#include <QDrag>
#include <QMimeData>
#include <iostream>
#include <QDragEnterEvent>

#include"../../Includes/gui/raccoon_scene.hpp"
#include"../../Includes/transformations.hpp"
#include<iostream>

raccoon_scene *globalScene;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    globalScene = raccoon_scene::get_instance(1920,1080);
    ui->agraphicsView->setAcceptDrops(true);
    ui->agraphicsView->setScene(globalScene);
    /*
    table t;
    std::vector<std::string> term = {"peking","kina","dzeki"};
    t.push(term);
    std::vector<entry> red1 = {4,5,0};
    t.push_row(red1);
    red1 = {1,2,3};
    t.push_row(red1);
    red1 = {7,0,9};
    t.push_row(red1);

    tf_idf(t);

    std::cout << t << std::endl;
    */


    connect(ui->csv_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->approximate_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->binning_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->filter_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->knn_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->normalization_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->partition_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->remove_na_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->standardize_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->to_numeric_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->bayes_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->k_means_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->stats_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->table_output_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->dec_tree_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->dbscan_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->aglo_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->plot_button,SIGNAL(released()),this,SLOT(drop_action()));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    Q_UNUSED(event);

}


void MainWindow::drop_action(){
    QDrag *drag = new QDrag(globalScene);
    QMimeData *mime = new QMimeData;

    QObject* obj = sender();
    mime->setText(obj->objectName());
    drag->setMimeData(mime);
    drag->exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event){
    QMainWindow::dragEnterEvent(event);
    event->accept();
}
void MainWindow::dragMoveEvent(QDragMoveEvent *event){
    QMainWindow::dragMoveEvent(event);
    event->accept();
}
void MainWindow::dropEvent(QDropEvent *event){
    QMainWindow::dropEvent(event);
    event->accept();
}


void MainWindow::on_action_run_triggered()
{
    globalScene->run_graph();
}
