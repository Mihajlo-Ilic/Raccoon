#include "../../Includes/gui/plot_node.hpp"
#include <unordered_map>
#include<iostream>
#include<numeric>
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
    for(auto it:classes){
        classInRGB.push_back(std::make_pair(it,rand()));
    }

    // ISCRTAVANJE 3D -> Q3DSCATTER
    if(attributes.size() == 3) {

        Q3DScatter *scatter = new Q3DScatter();
        QWidget *container = QWidget::createWindowContainer(scatter);

        QWidget *widget = new QWidget;
        QHBoxLayout *hLayout = new QHBoxLayout(widget);
        QVBoxLayout *vLayout = new QVBoxLayout();
        hLayout->addWidget(container, 1);
        hLayout->addLayout(vLayout);

        QFont font = scatter->activeTheme()->font();
        font.setPointSize(3);
        scatter->activeTheme()->setFont(font);
        scatter->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
        scatter->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);

        QScatterDataProxy *proxy = new QScatterDataProxy;
        QScatter3DSeries *series = new QScatter3DSeries(proxy);
        series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
        series->setMeshSmooth(true);
        scatter->addSeries(series);



        scatter->axisX()->setTitle("X");
        scatter->axisY()->setTitle("Y");
        scatter->axisZ()->setTitle("Z");

        vLayout->addWidget(new QLabel(QStringLiteral("Change point size: ")),0,Qt::AlignTop);
        QSpinBox *size_of_points_SpinBox2 = new QSpinBox();
        vLayout->addWidget(size_of_points_SpinBox2,1,Qt::AlignTop);

        std::unordered_map<std::string,double> uniqueToInt;
        for(auto it : attributes) {
            double br = 1;
            if(t[it].type == NOMINAL) {
                for(auto un : t[it].unique()) {
                    uniqueToInt[un.get_string()] = br++;
                }
            }
        }
        scatter->setFlags(scatter->flags() ^ Qt::FramelessWindowHint);
        int br=0;
        for(auto it : classInRGB) {
            std::cout << "Element: "<< it.first << " - " << it.second  << std::endl;
            QScatter3DSeries *series = new QScatter3DSeries;
            QScatterDataArray data;
            for(int j = 0; j < t.row_n(); j++) {
                if(it.first.get_string() == t[label][j].get_string()) {
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
            float B = it.second/(255*255)%255;
            scatter->seriesList().at(br++)->setBaseColor(QColor::fromRgb(R,G,B));
            scatter->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);
            scatter->setShadowQuality(scatter->ShadowQualityNone);
        }

        widget->resize(800,600);
        widget->show();

    }

    // 2D -> QCHART
    else {
        QDialog *dialog = new QDialog;
        QHBoxLayout *hBox = new QHBoxLayout;
        dialog->setLayout(hBox);

        QFormLayout *frameLayout = new QFormLayout;
        QFrame *frame = new QFrame();
        frame->setLayout(frameLayout);
        QChartView *c = new QChartView();
        hBox->addWidget(c);
        hBox->addWidget(frame);


        QSpinBox *size_of_points_SpinBox = new QSpinBox();
        frameLayout->addRow("Marker size: ", size_of_points_SpinBox);

        QSpinBox *size_of_axis_SpinBox = new QSpinBox();
        frameLayout->addRow("Axis size: ",size_of_axis_SpinBox);

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

        double maxX = std::numeric_limits<double>::min();
        double maxY = std::numeric_limits<double>::min();;
        double minX = std::numeric_limits<double>::max();;
        double minY = std::numeric_limits<double>::max();;
        for(auto it : classInRGB) {
            QScatterSeries *scatter = new QScatterSeries;
            std::string labelName = it.first.get_string();
            scatter->setName(QString::fromStdString(labelName));

            for(int j = 0; j < t.row_n(); j++) {
                if(it.first.get_string() == t[label][j].get_string()) {
                    QPointF point(50,50);

                    if(t[attributes[0]].type == NOMINAL) {
                        point.setX((double)uniqueToInt[t[attributes[0]][j].get_string()]);
                        if((double)uniqueToInt[t[attributes[0]][j].get_string()] > maxX)
                            maxX = (double)uniqueToInt[t[attributes[0]][j].get_string()];
                        if((double)uniqueToInt[t[attributes[0]][j].get_string()] < minX)
                            minX = (double)uniqueToInt[t[attributes[0]][j].get_string()];
                    }
                    else {
                        point.setX(t[attributes[0]][j].get_double());
                        if(t[attributes[0]][j].get_double() > maxX)
                            maxX = t[attributes[0]][j].get_double();
                        if(t[attributes[0]][j].get_double() < minX)
                            minX = t[attributes[0]][j].get_double();
                    }
                    if(t[attributes[1]].type == NOMINAL) {
                        point.setY((double)uniqueToInt[t[attributes[1]][j].get_string()]);
                        if((double)uniqueToInt[t[attributes[1]][j].get_string()] > maxY)
                            maxY = (double)uniqueToInt[t[attributes[1]][j].get_string()];
                        if((double)uniqueToInt[t[attributes[1]][j].get_string()] < minY)
                            minY = (double)uniqueToInt[t[attributes[1]][j].get_string()];
                    }
                    else {
                        point.setY(t[attributes[1]][j].get_double());
                        if(t[attributes[1]][j].get_double() > maxY)
                            maxY = t[attributes[1]][j].get_double();
                        if(t[attributes[1]][j].get_double() < minY)
                            minY = t[attributes[1]][j].get_double();
                    }
                    *scatter << point;
                }
            }
            float R = it.second%255;
            float G = (it.second/255)%255;
            float B = it.second/(255*255)%255;
            scatter->setColor(QColor::fromRgb(R,G,B));
            c->chart()->addSeries(scatter);

        }
        c->chart()->createDefaultAxes();
        c->chart()->axes(Qt::Horizontal).first()->setRange(minX-0.3, maxX + 0.3);
        c->chart()->axes(Qt::Vertical).first()->setRange(minY - 0.3, maxY + 0.3);

        connect(size_of_points_SpinBox, QOverload<int>::of(&QSpinBox::valueChanged),[&](int i){
            for(auto it:c->chart()->series())
                ((QScatterSeries *)it)->setMarkerSize(i);
        });
        connect(size_of_axis_SpinBox, QOverload<int>::of(&QSpinBox::valueChanged),[&](int i){
            QFont labelsFont;
            labelsFont.setPixelSize(i+0.0);
            c->chart()->axes(Qt::Horizontal).first()->setLabelsFont(labelsFont);
            c->chart()->axes(Qt::Vertical).first()->setLabelsFont(labelsFont);
        });
        dialog->resize(800,600);
        dialog->exec();


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