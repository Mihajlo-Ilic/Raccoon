#include "Includes/gui/mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDialog>
#include <QScreen>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QObject>
#include <QFileDialog>

#include "Includes/transformations.hpp"
#include "Includes/file_reader.hpp"
#include "Includes/dbscan.hpp"
#include "Includes/agglo_cluster.hpp"
#include "Includes/decision_tree.hpp"
#include<iostream>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    std::string init_scene = "";
    std::string theme = "Light";

    QRect rec = QApplication::primaryScreen()->geometry();

    QDialog init_dial;
    init_dial.setStyleSheet(" background-color: qlineargradient(spread:pad, x1:0.524092, y1:1, x2:0.518, y2:0, stop:0 rgba(40, 40, 40, 255), stop:1 rgba(93, 93, 93, 255));  color:white;font:13pt Consolas;");
    init_dial.setGeometry(rec.width()/2-250,rec.height()/2-150,500,300);
    init_dial.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    QLabel raccon_image(&init_dial);
    raccon_image.setGeometry(0,0,300,300);
    raccon_image.setMaximumHeight(300);
    raccon_image.setMaximumWidth(300);

    QPixmap img(":/res/Resources/Node_icons/trash_panda.svg");
    img = img.scaled(raccon_image.size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    raccon_image.setPixmap(img);
    init_dial.setWindowIcon(img);

    QLabel welcome_label(&init_dial);
    welcome_label.setStyleSheet("background-color:rgba(255,0,0,255)");
    welcome_label.setText("Welcome to Raccoon!\n\ndevelopers:\n Mihajlo Ilic\n Gavrilo Jovanovic\n Katarina Nikolic");
    welcome_label.setGeometry(300,10,200,150);
    welcome_label.setStyleSheet("QLabel{ background-color:rgba(0,0,0,0);font:15pt Arial; }");


    QLabel theme_label(&init_dial);
    theme_label.setText("Theme:");
    theme_label.setGeometry(340,170,80,30);
    theme_label.setStyleSheet("background-color:rgba(0,0,0,0)");

    QComboBox theme_combo(&init_dial);
    theme_combo.setGeometry(400,170,80,30);
    theme_combo.addItem("Light");
    theme_combo.addItem("Dark");
    theme_combo.setStyleSheet("border:1px solid black;background-color:rgba(0,0,0,0)");

    QPushButton load_scene_btn(&init_dial);
    load_scene_btn.setGeometry(360,210,120,30);
    load_scene_btn.setText("load scene");
    load_scene_btn.setIcon(QIcon(":/res/Resources/Action_icons/open_file.png"));
    load_scene_btn.setStyleSheet("QPushButton::hover{border:1px solid orange;background-color:rgba(0,0,0,0)} QPushButton{border:1px solid black;background-color:rgba(0,0,0,0)}");

    QPushButton new_scene_btn(&init_dial);
    new_scene_btn.setGeometry(360,250,120,30);
    new_scene_btn.setText(" new scene");
    new_scene_btn.setIcon(QIcon(":/res/Resources/Action_icons/new_icon.png"));
    new_scene_btn.setStyleSheet("QPushButton::hover{border:1px solid orange;background-color:rgba(0,0,0,0)} QPushButton{border:1px solid black;background-color:rgba(0,0,0,0)}");


    QPushButton exit_btn(&init_dial);
    exit_btn.setGeometry(10,260,40,30);
    exit_btn.setText("exit");
    exit_btn.setStyleSheet("QPushButton::hover{color:orange;} QPushButton{background-color:rgba(0,0,0,0)}");


    QObject::connect(&exit_btn, &QPushButton::clicked, []() {
        exit(0);
    });

    QObject::connect(&load_scene_btn, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(nullptr, "Open File",
                                                            "",
                                                            "RQN Files (*.rqn)");
        init_scene = fileName.toStdString();
        init_dial.close();
    });

    QObject::connect(&new_scene_btn, &QPushButton::clicked, [&]() {
        init_dial.close();
    });

    QObject::connect(&theme_combo, QOverload<const QString&>::of(&QComboBox::currentTextChanged),[&](const QString& curr){

        theme =curr.toStdString();
    });

    init_dial.exec();


    if(theme=="Dark"){
        QFile styleFile( ":/res/Resources/style.qss" );
        styleFile.open( QFile::ReadOnly );
        QString style( styleFile.readAll() );
        app.setStyleSheet( style );
    } else {
        QFile styleFile( ":/res/Resources/style2.qss" );
        styleFile.open( QFile::ReadOnly );
        QString style( styleFile.readAll() );
        app.setStyleSheet( style );
    }


    MainWindow window;


    window.show();
    window.init(theme,init_scene);
    return app.exec();

    return 0;
}
