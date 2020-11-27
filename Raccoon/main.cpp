#include "Includes/gui/mainwindow.h"
#include <QApplication>
#include <QFile>
#include "Includes/transformations.hpp"
#include "Includes/file_reader.hpp"
#include "Includes/dbscan.hpp"
#include "Includes/agglo_cluster.hpp"
#include "Includes/decision_tree.hpp"
#include<iostream>
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QFile styleFile( ":/res/Resources/style.qss" );
    styleFile.open( QFile::ReadOnly );
    QString style( styleFile.readAll() );
    app.setStyleSheet( style );

    MainWindow window;


    window.show();

    return app.exec();

    return 0;
}
