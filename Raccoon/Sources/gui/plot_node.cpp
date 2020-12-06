#include "../../Includes/gui/plot_node.hpp"


#include <Q3DScatter>
#include <QtCharts>
#include <Q3DScatter>

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
    if(attributes.size() == 3)
        plot_3D(attributes,label,classInRGB);
    else
        plot_2D(attributes,label,classInRGB);
}

void plot_node::plot_3D(std::vector<std::string> attributes, std::string label,std::vector<std::pair<entry,long>> classInRGB) {
    Q3DScatter *scatter = new Q3DScatter();
    QWidget *container = QWidget::createWindowContainer(scatter);

    QSize screenSize = scatter->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.5));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QFormLayout *formLayout = new QFormLayout;
    // QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(formLayout);

    QFont font = scatter->activeTheme()->font();
    font.setPointSize(3);
    scatter->activeTheme()->setFont(font);
    scatter->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
    scatter->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);

    formLayout->addRow(new QLabel(QStringLiteral("Geometry: ")));
    QComboBox *geometryComboBox = new QComboBox();
    geometryComboBox->addItem("Sphere");
    geometryComboBox->addItem("Point");
    formLayout->addRow(geometryComboBox);

    QComboBox *themeList = new QComboBox(widget);
    themeList->addItem(QStringLiteral("Qt"));
    themeList->addItem(QStringLiteral("Primary Colors"));
    themeList->addItem(QStringLiteral("Digia"));
    themeList->addItem(QStringLiteral("Stone Moss"));
    themeList->addItem(QStringLiteral("Army Blue"));
    themeList->addItem(QStringLiteral("Retro"));
    themeList->addItem(QStringLiteral("Ebony"));
    themeList->addItem(QStringLiteral("Isabelle"));
    themeList->setCurrentIndex(0);
    formLayout->addRow(new QLabel(QStringLiteral("Graph theme: ")));
    formLayout->addRow(themeList);

    std::unordered_map<std::string,double> uniqueToInt;
    for(const auto &it : attributes) {
        double br = 1;
        if(t[it].type == NOMINAL) {
            for(const auto &un : t[it].unique()) {
                uniqueToInt[un.get_string()] = br++;
            }
        }
    }
    scatter->setFlags(scatter->flags() ^ Qt::FramelessWindowHint);
    int br=0;

        QScatterDataProxy *proxy = new QScatterDataProxy;
        QScatter3DSeries *series = new QScatter3DSeries(proxy);
        series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
        scatter->addSeries(series);

    scatter->activeTheme()->setBackgroundEnabled(true);
    scatter->activeTheme()->setLabelBackgroundEnabled(true);
    scatter->axisX()->setTitleVisible(true);
    scatter->axisY()->setTitleVisible(true);
    scatter->axisZ()->setTitleVisible(true);
    scatter->axisX()->setTitle(QString::fromStdString(attributes[0]));
    scatter->axisY()->setTitle(QString::fromStdString(attributes[1]));
    scatter->axisZ()->setTitle(QString::fromStdString(attributes[2]));

    for(const auto &it : classInRGB) {
        QScatter3DSeries *series = new QScatter3DSeries;
        QScatterDataArray data;
        for(int j = 0; j < t.row_n(); j++) {
            if(it.first.get_string() == t[label][j].get_string()) {
                QVector3D *point = new QVector3D;

                if(t[attributes[0]].type == NOMINAL) {
                    point->setX((double)uniqueToInt[t[attributes[0]][j].get_string()]);
                }
                else {
                    point->setX(t[attributes[0]][j].get_double());
                }
                if(t[attributes[1]].type == NOMINAL) {
                    point->setY((double)uniqueToInt[t[attributes[1]][j].get_string()]);
                }
                else {
                    point->setY(t[attributes[1]][j].get_double());
                }
                if(t[attributes[2]].type == NOMINAL) {
                    point->setZ((double)uniqueToInt[t[attributes[2]][j].get_string()]);
                }
                else {
                    point->setZ(t[attributes[2]][j].get_double());
                }

                data << *point;
            }
        }
        series->dataProxy()->addItems(data);
        scatter->addSeries(series);
        float R = it.second%255;
        float G = (it.second/255)%255 + 10;
        float B = it.second/(255*255)%255 + 10;
        scatter->seriesList().at(br++)->setBaseColor(QColor::fromRgb(R,G,B));
        scatter->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);
        scatter->setShadowQuality(scatter->ShadowQualityNone);
    }

    connect(geometryComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](int index){
        for(int j = 0; j < scatter->seriesList().count(); j++) {
            if(index == 0) {
                QScatter3DSeries *ser = scatter->seriesList().at(j);
                QAbstract3DSeries::Mesh m_style = QAbstract3DSeries::Mesh(QAbstract3DSeries::MeshSphere);
                ser->setMesh(m_style);
            }
            else {
                QScatter3DSeries *ser = scatter->seriesList().at(j);
                QAbstract3DSeries::Mesh m_style = QAbstract3DSeries::Mesh(QAbstract3DSeries::MeshPoint);
                ser->setMesh(m_style);
            }
        }
    });

    connect(themeList, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](int index){
        Q3DTheme *currentTheme = scatter->activeTheme();
        currentTheme->setType(Q3DTheme::Theme(index));
    });

    for(int i = 0; i < scatter->seriesList().count();i++) {
        QScatter3DSeries *ser = scatter->seriesList().at(i);
        ser->setMeshSmooth(true);
    }

    widget->resize(800,600);
    widget->show();
}
void plot_node::plot_2D(std::vector<std::string> attributes, std::string label,std::vector<std::pair<entry,long>> classInRGB) {
    QDialog *dialog = new QDialog;
    QHBoxLayout *hBox = new QHBoxLayout;
    dialog->setLayout(hBox);

    QFormLayout *frameLayout = new QFormLayout;
    QFrame *frame = new QFrame();
    QVBoxLayout *vBox = new QVBoxLayout;

    frame->setLayout(vBox);

    QListWidget *listWidget = new QListWidget();

    vBox->addLayout(frameLayout);
    vBox->addWidget(listWidget);
    listWidget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    for(const auto &it : t[label].unique()) {
        QListWidgetItem *l_item =new QListWidgetItem();
        l_item->setText(QString::fromStdString(it.get_string()));
        l_item->setFlags(l_item->flags() | Qt::ItemIsUserCheckable);
        l_item->setCheckState(Qt::Checked);
        listWidget->addItem(l_item);
    }
    QChartView *c = new QChartView();
    hBox->addWidget(c);
    hBox->addWidget(frame);


    QSpinBox *size_of_points_SpinBox = new QSpinBox();
    frameLayout->addRow("Marker size: ", size_of_points_SpinBox);

    QSpinBox *size_of_axis_SpinBox = new QSpinBox();
    frameLayout->addRow("Axis size: ",size_of_axis_SpinBox);

    QPushButton *backgroundColor = new QPushButton();
    frameLayout->addRow("Background color: ", backgroundColor);
    backgroundColor->setStyleSheet("background-color : " + c->chart()->backgroundBrush().color().name());





    c->chart();
    c->setRenderHint(QPainter::Antialiasing);

    std::unordered_map<std::string,double> uniqueToInt;
    for(const auto &it : attributes) {
        double br = 1;
        if(t[it].type == NOMINAL) {
            for(const auto &un : t[it].unique()) {
                uniqueToInt[un.get_string()] = br++;
            }
        }
    }

    double maxX = std::numeric_limits<double>::min();
    double maxY = std::numeric_limits<double>::min();;
    double minX = std::numeric_limits<double>::max();;
    double minY = std::numeric_limits<double>::max();;
    for(const auto &it : classInRGB) {
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
        float G = (it.second/255)%255 + 10;
        float B = it.second/(255*255)%255 + 10;
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
    connect(backgroundColor, (&QPushButton::clicked),[&](){
        QColor col = QColorDialog::getColor();
        c->chart()->setBackgroundBrush(col);
        backgroundColor->setStyleSheet("background-color : " + c->chart()->backgroundBrush().color().name());
    });
    connect(listWidget,QOverload<QListWidgetItem *>::of(&QListWidget::itemChanged),[&](QListWidgetItem *item){
         (void)item;
        for(int j =0 ; j < listWidget->count();j++) {
             if(listWidget->item(j)->checkState() == Qt::Checked)
                 c->chart()->series().at(j)->setVisible(true);
             else
                 c->chart()->series().at(j)->setVisible(false);
         }
    });
    dialog->resize(800,600);
    dialog->exec();
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
