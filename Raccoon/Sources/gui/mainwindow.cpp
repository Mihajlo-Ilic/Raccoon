#include "../../Includes/gui/mainwindow.h"
#include "ui_mainwindow.h"
#include <QDrag>
#include <QMimeData>
#include <iostream>
#include <QDragEnterEvent>

#include"../../Includes/gui/raccoon_scene.hpp"
#include"../../Includes/transformations.hpp"
#include<iostream>
#include<QFileDialog>
#include <QShortcut>

raccoon_scene *globalScene;

bool saved_before=false;
std::string save_path;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    globalScene = raccoon_scene::get_instance(1920,1080);
    ui->agraphicsView->setAcceptDrops(true);
    ui->agraphicsView->setScene(globalScene);

    QShortcut * undo_sct = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), this, [] () {globalScene->undo_action();});
    QShortcut * redo_sct = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_R), this, [] () {globalScene->redo_action();});

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
    connect(ui->doc_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->tf_idf_button,SIGNAL(released()),this,SLOT(drop_action()));
    connect(ui->apriori_button,SIGNAL(released()),this,SLOT(drop_action()));

    ui->menuFile->setStyleSheet("QMenu::Item{color:white;}");
    ui->menuEdit->setStyleSheet("QMenu::Item{color:white;}");
    ui->menuHelp->setStyleSheet("QMenu::Item{color:white;}");
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

void MainWindow::init(std::string theme, std::string path)
{
    if(theme=="Light"){
        ui->menuFile->setStyleSheet("QMenu::Item{color:black;}");
        ui->menuEdit->setStyleSheet("QMenu::Item{color:black;}");
        ui->menuHelp->setStyleSheet("QMenu::Item{color:black;}");

        ui->aglo_button->setIcon(QPixmap(":/res/Resources/Node_icons_light/aglo.png"));
        ui->apriori_button->setIcon(QPixmap(":/res/Resources/Node_icons_light/apriori.png"));
        ui->approximate_button->setIcon(QPixmap(":/res/Resources/Node_icons_light/aproximation.png"));
        ui->binning_button->setIcon(QPixmap(":/res/Resources/Node_icons_light/binning.png"));
        ui->to_numeric_button->setIcon(QPixmap(":/res/Resources/Node_icons_light/cat_to_num.png"));
        ui->dbscan_button->setIcon(QPixmap(":/res/Resources/Node_icons_light/dbscan.png"));
        ui->filter_button->setIcon(QPixmap(":/res/Resources/Node_icons_light/filter.png"));

        ui->k_means_button->setIcon(QPixmap(":/res/Resources/Node_icons_light/k_means.png"));
        ui->knn_button->setIcon(QPixmap(":/res/Resources/Node_icons_light/knn.png"));
        ui->normalization_button->setIcon(QPixmap(":/res/Resources/Node_icons_light/normalization.png"));
        ui->partition_button->setIcon(QPixmap(":/res/Resources/Node_icons_light/partition.png"));
        ui->plot_button->setIcon(QPixmap(":/res/Resources/Node_icons_light/plot.png"));
        ui->remove_na_button->setIcon(QPixmap(":/res/Resources/Node_icons_light/remove_na.png"));
        ui->standardize_button->setIcon(QPixmap(":/res/Resources/Node_icons_light/standardization.png"));
        ui->stats_button->setIcon(QPixmap(":/res/Resources/Node_icons_light/stats.png"));
        ui->tf_idf_button->setIcon(QPixmap(":/res/Resources/Node_icons_light/tf_idf.png"));
        ui->dec_tree_button->setIcon(QPixmap(":/res/Resources/Node_icons_light/tree.png"));
    }

    if(path!=""){
        globalScene->load_scene(path);
    }
}


void MainWindow::on_action_run_triggered()
{
    globalScene->run_graph();
}

void MainWindow::on_actionSave_As_triggered()
{
    QString file= QFileDialog::getSaveFileName(this, tr("Save File"));
    globalScene->save_scene(file.toStdString());
    saved_before=true;
    save_path = file.toStdString();
}

void MainWindow::on_actionSave_2_triggered()
{
    if(saved_before)
            globalScene->save_scene(save_path);
        else {
            QString file= QFileDialog::getSaveFileName(this, tr("Save File"));
            globalScene->save_scene(file.toStdString());
            saved_before=true;
            save_path = file.toStdString();
    }
}

void MainWindow::on_actionOpen_Scene_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        "",
                                                        tr("RQN Files (*.rqn)"));
    globalScene->load_scene(fileName.toStdString());
}

void MainWindow::on_actionNew_Scene_triggered()
{
    globalScene->delete_graph();
}

void MainWindow::on_action_delete_triggered() {
    globalScene->delete_edge();
}

void MainWindow::on_action_unmerge_triggered() {
    globalScene->unmerge_nodes();
}


void MainWindow::on_actionUndo_triggered()
{
    globalScene->undo_action();
}

void MainWindow::on_actionRedo_triggered()
{
    globalScene->redo_action();
}
