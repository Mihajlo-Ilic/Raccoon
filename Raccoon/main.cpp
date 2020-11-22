#include "Includes/gui/mainwindow.h"
#include <QApplication>
#include <QFile>
#include "Includes/transformations.hpp"
#include "Includes/file_reader.hpp"
#include "Includes/naive_bayes.hpp"
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
    table t;
    t.push("x");
    t.push("y");
    t.push("z");
    t.push_row({{"x",1.6},{"y",2},{"z",0.3}});
    t.push_row({{"x",2.3},{"y",0.2},{"z",4}});
    std::cout<<t<<std::endl;
    std::cout<<euclidean_dist(t[0],t[1])<<std::endl;
    return 0;
}
