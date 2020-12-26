#include "../../Includes/gui/knn_node.hpp"
#include<QDialog>
#include<QVBoxLayout>
#include<QMessageBox>

knn_node::knn_node(int width, int height) : node(width,height,2){
    header_text.setText("KNN NODE");

    previewBtn.setText("preview");
    k_neighbours.setMinimum(1);

    distance_label.setText("distance function");
    k_neighbours_label.setText("k neighbours");
    previewBtn.setText("preview");
    distance_box.addItem("Euclidean distance");
    distance_box.addItem("Manhattan distance");
    distance_box.addItem("SMC distance");
    distance_box.addItem("Zakard distance");
    distance_box.addItem("Cos distance");

    distance_box.setParent(&body);
    distance_label.setParent(&body);
    k_neighbours.setParent(&body);
    k_neighbours_label.setParent(&body);
    previewBtn.setParent(&body);

    distance_box.setGeometry(geometry().x() + 10,geometry().y() + 10,110,30);
    distance_label.setGeometry(geometry().x() + 130,geometry().y() + 10,110,30);
    k_neighbours.setGeometry(geometry().x() + 10,geometry().y() + 50,110,30);
    k_neighbours_label.setGeometry(geometry().x() + 130,geometry().y() + 50,110,30);
    previewBtn.setGeometry(geometry().x() + 170,geometry().y() + 90,70,20);

    connect(&distance_box,SIGNAL(currentTextChanged(QString )),this,SLOT(combo_changed( QString )));
    connect(&k_neighbours,SIGNAL(valueChanged(int)),this,SLOT(k_changed(int)));
    connect(&previewBtn, SIGNAL(clicked()), this, SLOT(preview_b()));
}

void knn_node::serialize(std::ofstream &os)
{
    os<<"-n knn"<<std::endl;
    os<<" x="<<geometry().topLeft().x()<<std::endl;
    os<<" y="<<geometry().topLeft().y()<<std::endl;
    os<<" metric="<<distance_box.currentText().toStdString()<<std::endl;
    os<<" k="<<k_neighbours.value()<<std::endl;
}

#include<iostream>
bool knn_node::run()
{
     packet msg = inputs[0]->get_packet();
     auto col = msg.packet_columns;
     int col_num = 0;
     for(int i = 0; i < col.size();i++) {
         std::cout<< i << " je = " << col[i].role << std::endl;
         if(col[i].role == TARGET){
             col_num++;
             break;
         }
     }
     if(warning_cheque([&](auto &x){
         if (col_num == 0) {
             std::cout << "BAKA" << std::endl;
             x += "The packet didn't had target column!\n";
             return true;
         } else {
         return false;
         }
     })) {
        return false;
     }
    else {
        table train =inputs[0]->get_table();
        table test = inputs[0]->get_table();
        train.pop("partition");
        test.pop("partition");
        test.pop(train.get_target());
        classifier.fit(train);
        t = classifier.predict(test);
        outputs[0]->send_data(t);
        return true;
    }
}

void knn_node::on_input_changed()
{
     packet msg = inputs[0]->get_packet();
     auto col = msg.packet_columns;
     int col_num = 0;
     for(int i = 0; i < col.size();i++) {
         if(col[i].role == TARGET){
             col_num++;
             break;
         }
     }
     if(warning_cheque([&](auto &x){
         if (col_num == 0) {
             std::cout << "BAKA" << std::endl;
             x += "The packet didn't had target column!\n";
             return true;
         } else {
         return false;
         }
     })) { }
    else {
        msg.add_column("assigned",column_role::INPUT_COLUMN,column_type::CONTINUOUS);
        outputs[0]->send_packet(msg);
        table t = inputs[0]->get_table();
    }
}

packet knn_node::get_msg()
{
    packet msg = inputs[0]->get_packet();
    msg.add_column("assigned",column_role::INPUT_COLUMN,column_type::CONTINUOUS);
    return msg;
}

void knn_node::preview()
{
    QDialog dial;
    QTableWidget tab;
    make_QTable(tab,t);

    QVBoxLayout vb;
    dial.setLayout(&vb);

    vb.addWidget(&tab);

    vb.addSpacing(20);

    table train = inputs[0]->get_table();
    train.pop(train.get_target());

    classifier.fit(inputs[0]->get_table());
    table train_res = classifier.predict(train);
    double train_acc = accuracy_score(inputs[0]->get_table(),train_res,inputs[0]->get_table().get_target());

    QLabel train_lab;
    train_lab.setText("Train accuracy : "+QString::number(train_acc));

    table conf_train = confusion_matrix(inputs[0]->get_table(),train_res,inputs[0]->get_table().get_target());
    QTableWidget q_conf_train;
    make_QTable(q_conf_train,conf_train);

    vb.addWidget(&train_lab);
    vb.addWidget(&q_conf_train);
    dial.exec();
}

void knn_node::preview_b() {
    preview();
}

void knn_node::combo_changed(QString str)
{
    if(str.toStdString()=="Manhattan distance")
        classifier.set_function(manhattan_dist);
    if(str.toStdString()=="Euclidean distance")
        classifier.set_function(euclidean_dist);
    if(str.toStdString()=="SMC distance")
        classifier.set_function(smc_dist);
    if(str.toStdString()=="Zakard distance")
        classifier.set_function(zakard_dist);
    if(str.toStdString()=="Cos distance")
        classifier.set_function(cos_dist);
}

void knn_node::k_changed(int v)
{
    classifier.set_k(v);
}
