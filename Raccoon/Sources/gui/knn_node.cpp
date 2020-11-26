#include "../../Includes/gui/knn_node.hpp"

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
    previewBtn.setGeometry(geometry().x() + 190,geometry().y() + 200,50,20);

    connect(&distance_box,SIGNAL(currentTextChanged(QString )),this,SLOT(combo_changed( QString )));
    connect(&k_neighbours,SIGNAL(valueChanged(int)),this,SLOT(k_changed(int)));
    connect(&previewBtn, SIGNAL(clicked()), this, SLOT(preview_b()));
}

void knn_node::run()
{
    table train =inputs[0]->get_table();
    table test = inputs[0]->get_table();
    train.pop("partition");
    test.pop("partition");
    test.pop(train.get_target());
    classifier.fit(train);
    t = classifier.predict(test);
    outputs[0]->send_data(t);
}

void knn_node::on_input_changed()
{
    packet msg = inputs[0]->get_packet();
    msg.add_column("assigned",column_role::INPUT_COLUMN,column_type::CONTINUOUS);
    outputs[0]->send_packet(msg);
}

packet knn_node::get_msg()
{
    packet msg = inputs[0]->get_packet();
    msg.add_column("assigned",column_role::INPUT_COLUMN,column_type::CONTINUOUS);
    return msg;
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
