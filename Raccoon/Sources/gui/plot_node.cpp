#include "../../Includes/gui/plot_node.hpp"
#include <unordered_map>
#include<iostream>
using namespace QtDataVisualization;

plot_node::plot_node(int width, int height) : node(width,height,1) {
    header_text.setText("PLOT NODE");

    previewBtn.setText("preview");
    label.setText("Choose and atribute for label");
    label2.setText("Choose axis:");


    combo_box.setParent(&body);
    list_widget.setParent(&body);
    label.setParent(&body);
    label2.setParent(&body);
    previewBtn.setParent(&body);

    label.setGeometry(geometry().x() + 10,geometry().y() + 10,230,20);
    combo_box.setGeometry(geometry().x() + 10,geometry().y() + 40,230,20);
    label2.setGeometry(geometry().x() + 10,geometry().y() + 70,230,20);
    list_widget.setGeometry(geometry().x() + 10,geometry().y() + 100,230,100);
    list_widget.setSelectionMode(QAbstractItemView::MultiSelection);
    previewBtn.setGeometry(geometry().x() + 190,geometry().y() + 200,50,20);

    connect(&previewBtn, SIGNAL(clicked()), this, SLOT(preview_b()));
}

void plot_node::plot_function(std::vector<std::string> attributes, std::string label) {
    std::vector<entry> classes = t[label].unique();
    std::vector<std::pair<entry,long>> classInRGB;
    srand(time(NULL));
    for(auto it:t[label].unique()){
        classInRGB.push_back(std::make_pair(it,rand()));
    }

    // ISCRTAVANJE 3D
    if(attributes.size() == 3) {

        std::unordered_map<std::string,double> uniqueToInt;
        for(auto it : attributes) {
            double br = 1;
            if(t[it].type == NOMINAL) {
                for(auto un : t[it].unique()) {
                    uniqueToInt[un.get_string()] = br++;
                }
            }
        }
        Q3DScatter *scatter = new Q3DScatter();
        scatter->setFlags(scatter->flags() ^ Qt::FramelessWindowHint);
        int b=0;
        for(auto it : classInRGB) {

            QScatter3DSeries *series = new QScatter3DSeries;
            QScatterDataArray data;
            for(int j = 0; j < t.row_n(); j++) {
                if(it.first == t[label][j]) {
                    QVector3D point;

                    if(t[attributes[0]].type == NOMINAL) {
                        point.setX((double)uniqueToInt[t[attributes[0]][j].get_string()]);
                    }
                    else {
                        point.setX(t[attributes[0]][j].get_double());
                    }
                    if(t[attributes[1]].type == NOMINAL) {
                        point.setY((double)uniqueToInt[t[attributes[1]][j].get_string()]);
                    }
                    else {
                        point.setY(t[attributes[1]][j].get_double());
                    }
                    if(t[attributes[2]].type == NOMINAL) {
                        point.setY((double)uniqueToInt[t[attributes[2]][j].get_string()]);
                    }
                    else {
                        point.setY(t[attributes[2]][j].get_double());
                    }

                    data << point;
                }
            }
            series->dataProxy()->addItems(data);
            scatter->addSeries(series);
            float R = it.second%255;
            float G = (it.second/255)%255;
            float B = it.second/(255*255);
            scatter->seriesList().at(0)->setBaseColor(QColor::fromRgb(R,G,B));
            scatter->setShadowQuality(scatter->ShadowQualityNone);
        }
        scatter->resize(800,600);
        scatter->show();

    }
    else {
        QChartView *c = new QChartView();
        c->chart();
        c->setRenderHint(QPainter::Antialiasing);

        std::unordered_map<std::string,double> uniqueToInt;
        for(auto it : attributes) {
            double br = 1;
            if(t[it].type == NOMINAL) {
                for(auto un : t[it].unique()) {
                    uniqueToInt[un.get_string()] = br++;
                }
            }
        }

        for(auto it : classInRGB) {
            QScatterSeries *scatter = new QScatterSeries();
            std::string labelName = it.first.get_string();
            scatter->setName(QString::fromStdString(labelName));
            for(int j = 0; j < t.row_n(); j++) {
                if(it.first == t[label][j]) {

                    QPointF point(50,50);

                    if(t[attributes[0]].type == NOMINAL) {
                        point.setX((double)uniqueToInt[t[attributes[0]][j].get_string()]);
                    }
                    else {
                        point.setX(t[attributes[0]][j].get_double());
                    }
                    if(t[attributes[1]].type == NOMINAL) {
                        point.setY((double)uniqueToInt[t[attributes[1]][j].get_string()]);
                    }
                    else {
                        point.setY(t[attributes[1]][j].get_double());
                    }
                    *scatter << point;
                }
            }
            float R = it.second%255;
            float G = (it.second/255)%255;
            float B = it.second/(255*255);
            scatter->setColor(QColor::fromRgb(R,G,B));
            c->chart()->addSeries(scatter);
            c->chart()->createDefaultAxes();
        }
        c->resize(800,600);
        c->show();
    }
}
void plot_node::run()
{
    t = inputs[0]->get_table();
}

void plot_node::preview()
{
    std::vector<std::string> fromListWidget;
    for(auto it : list_widget.selectedItems())
        if(fromListWidget.size()<=3)
            fromListWidget.push_back(it->text().toStdString());
    std::string label = combo_box.currentText().toStdString();
    plot_function(fromListWidget,label);
}

void plot_node::on_input_changed()
{
    list_widget.clear();
    combo_box.clear();
    packet msg = inputs[0]->get_packet();
    for(const auto& it:msg.packet_columns){
        list_widget.addItem(QString::fromStdString(it.name));
        combo_box.addItem(QString::fromStdString(it.name));
    }
    needs_update = true;
}

void plot_node::preview_b() {
    preview();
}
