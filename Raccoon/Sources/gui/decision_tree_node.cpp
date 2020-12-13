#include "../../Includes/gui/decision_tree_node.hpp"
#include <chrono>
#include <QDialog>
#include <QHBoxLayout>
#include <future>
decision_tree_node::decision_tree_node(int width, int height) : node(width, height, 2) {
    header_text.setText("DECISION TREE");

    preview_btn.setText("preview");
    preview_btn.setParent(&body);
    preview_btn.setGeometry(170, 180, 70, 30);

    min_row_lbl.setParent(&body);
    min_row_lbl.setText("Min. # of rows in node:");
    min_row_lbl.setGeometry(5,5,100,30);
    min_row_sb.setParent(&body);
    min_row_sb.setGeometry(140, 5, 70, 30);
    min_row_sb.setMinimum(1);

    max_depth_lbl.setParent(&body);
    max_depth_lbl.setText("Max. depth of tree:");
    max_depth_lbl.setGeometry(5, 40, 120, 30);
    max_depth_sb.setParent(&body);
    max_depth_sb.setGeometry(140, 40, 70, 30);
    max_depth_sb.setMinimum(0);

    min_clean_lbl.setParent(&body);
    min_clean_lbl.setText("Min. node clean:");
    min_clean_lbl.setGeometry(5, 75, 120, 30);
    min_clean_sb.setParent(&body);
    min_clean_sb.setGeometry(140, 75, 70, 30);
    min_clean_sb.setMaximum(1);
    min_clean_sb.setMinimum(0);


    clean_func_lbl.setParent(&body);
    clean_func_lbl.setText("Clean function:");
    clean_func_lbl.setGeometry(5, 110, 100, 30);
    clean_func.setParent(&body);
    clean_func.setGeometry(140, 110, 100, 30);
    clean_func.addItem("Entropy");
    clean_func.addItem("Gini index");

    connect(&preview_btn, SIGNAL(clicked()), this, SLOT(preview_b()));
    connect(&clean_func,SIGNAL(currentTextChanged(QString )),this,SLOT(combo_changed( QString )));
    connect(&min_clean_sb,SIGNAL(valueChanged(double)),this,SLOT(clean_changed(double)));
    connect(&max_depth_sb,SIGNAL(valueChanged(int)),this,SLOT(depth_changed(int)));
    connect(&min_row_sb,SIGNAL(valueChanged(int)),this,SLOT(row_changed(int)));
}

void decision_tree_node::serialize(std::ofstream &os)
{
    os<<"-n dec_tree"<<std::endl;
    os<<" x="<<geometry().topLeft().x()<<std::endl;
    os<<" y="<<geometry().topLeft().y()<<std::endl;
    os<<" min_row="<<min_row_sb.value()<<std::endl;
    os<<" max_depth="<< max_depth_sb.value()<<std::endl;
    os<<" min_clean="<<min_clean_sb.value()<<std::endl;
    os<<" metric="<<clean_func.currentText().toStdString()<<std::endl;

}

#include<iostream>
void decision_tree_node::on_input_changed()
{
    packet msg = inputs[0]->get_packet();
    msg.add_column("assigned",column_role::INPUT_COLUMN,column_type::CONTINUOUS);
    outputs[0]->send_packet(msg);
}

bool decision_tree_node::run()
{
    table training = inputs[0]->get_table();
    table test = inputs[1]->get_table();
    training.pop("partition");
    test.pop("partition");
    test.pop(training.get_target());

    //tree.fit(training);
    tree.fit_concurrent(training);
    std::chrono::steady_clock::time_point curr_time = std::chrono::steady_clock::now();
    t=tree.classify(test);
    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time-curr_time;
    std::cout<<"tree time : "<< elapsed_seconds.count()<<".s"<<std::endl;
    return true;
}

packet decision_tree_node::get_msg()
{
    packet msg = inputs[0]->get_packet();
    msg.add_column("assigned",column_role::INPUT_COLUMN,column_type::CONTINUOUS);
    return msg;
}

void decision_tree_node::preview_b() {
    preview();
}
#include <QFormLayout>
#include <QColorDialog>
void decision_tree_node::preview() {
    tab_gscene.clear();
    QDialog *tablePreview=new QDialog();
    tablePreview->setGeometry(400,400,500,500);
    tablePreview->setModal(true);

    QHBoxLayout *dialog_layout=new QHBoxLayout();
    tablePreview->setLayout(dialog_layout);

    QTabWidget *tabs=new QTabWidget;
    dialog_layout->addWidget(tabs);

    QFrame *tree_frame=new QFrame();
    QHBoxLayout *tree_layout=new QHBoxLayout();
    tree_frame->setLayout(tree_layout);

    tree_layout->addWidget(&tab_gview);
    tab_gscene.setSceneRect(0,0,2000,2000);
    tab_gview.setScene(&tab_gscene);

    tree.draw_tree(&tab_gscene);
    QRectF rect = tab_gscene.itemsBoundingRect();
    tab_gscene.setSceneRect(rect);

    QTableWidget data_table;
    make_QTable(data_table,t);

    QFrame *frame = new QFrame();
    tree_layout->addWidget(frame);
    QFormLayout *forma = new QFormLayout();
    frame->setLayout(forma);

    //X-SCALE
    QSpinBox *x_scale = new QSpinBox();
    x_scale->setMinimum(1);
    x_scale->setMaximum(300);
    x_scale->setValue(100);
    x_scale->setSuffix("%");
    forma->addRow("x_scale:", x_scale);

    //Y-SCALE
    QSpinBox *y_scale = new QSpinBox();
    y_scale->setMinimum(1);
    y_scale->setMaximum(300);
    y_scale->setValue(100);
    y_scale->setSuffix("%");
    forma->addRow("y_scale:", y_scale);

    //LINE SIZE
    QSpinBox *line_size = new QSpinBox();
    line_size->setMinimum(1);
    line_size->setMaximum(100);
    line_size->setValue(1);
    forma->addRow("line size:", line_size);

    //TEXT SCALE
    QSpinBox *text_scale = new QSpinBox();
    text_scale->setMinimum(1);
    text_scale->setMaximum(300);
    text_scale->setValue(100);
    text_scale->setSuffix("%");
    forma->addRow("text_scale:", text_scale);

    //COLOR SETTERS
    QPushButton *text_color = new QPushButton();
    forma->addRow("text color:",text_color);

    QPushButton *background_color = new QPushButton();
    forma->addRow("background color:",background_color);

    QPushButton *line_color = new QPushButton();
    forma->addRow("line color:",line_color);

    connect(x_scale, QOverload<int>::of(&QSpinBox::valueChanged),[=](int i){
        tab_gview.resetTransform();
        tab_gview.scale((double)i/100.0, (double)y_scale->value()/100.0);
    });

    connect(y_scale, QOverload<int>::of(&QSpinBox::valueChanged),[=](int i){
        tab_gview.resetTransform();
        tab_gview.scale((double)x_scale->value()/100.0, (double)i/100.0);
    });

    connect(line_size, QOverload<int>::of(&QSpinBox::valueChanged),[=](int i){
        for(auto it : tab_gscene.items())
        {
            if(it->type() == QGraphicsLineItem::Type) {
                QPen pen = ((QGraphicsLineItem*)(it))->pen();
                pen.setWidth(i);
                ((QGraphicsLineItem*)(it))->setPen(pen);
            }
        }
    });

    connect(text_scale, QOverload<int>::of(&QSpinBox::valueChanged),[=](int i){
        for(auto it : tab_gscene.items())
        {
            if(it->type() == QGraphicsSimpleTextItem::Type) {
                ((QGraphicsSimpleTextItem*)(it))->setScale((double)i/100.0);
            }
        }
    });

    connect(text_color, &QPushButton::clicked, [&]() {
        auto col = QColorDialog::getColor();
        for(auto it : tab_gscene.items())
        {
            if(it->type() == QGraphicsSimpleTextItem::Type) {
                ((QGraphicsSimpleTextItem*)(it))->setBrush(col);
            }
        }
    });

    connect(line_color, &QPushButton::clicked, [&]() {
        auto col = QColorDialog::getColor();
        for(auto it : tab_gscene.items())
        {
            if(it->type() == QGraphicsLineItem::Type) {
                ((QGraphicsLineItem*)(it))->setPen(col);
            }
        }
    });

    connect(background_color, &QPushButton::clicked, [&]() {
        tab_gscene.setBackgroundBrush(QColorDialog::getColor());
    });



    tabs->addTab(&data_table,"Table");
    tabs->addTab(tree_frame, "Tree");

    tablePreview->exec();
}

void decision_tree_node::combo_changed(QString str)
{
    if(str.toStdString()=="Entropy")
        tree.set_func(entropy);
    if(str.toStdString()=="Gini index")
        tree.set_func(gini);
}

void decision_tree_node::clean_changed(double v)
{
    tree.set_clean(v);
}

void decision_tree_node::depth_changed(int v)
{
    tree.set_max_depth(v);
}

void decision_tree_node::row_changed(int v)
{
    tree.set_min_row(v);
}



