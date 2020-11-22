#include "../../Includes/gui/k_mean_cluster_node.hpp"

k_mean_cluster_node::k_mean_cluster_node(int width,int height) : node(width,height,1)
{
    header_text.setText("K-MEAN CLUSTER NODE");

    previewBtn.setText("preview");
    k_neighbours.setMinimum(1);

    distance_label.setText("distance function");
    k_neighbours_label.setText("k means");
    previewBtn.setText("preview");
    distance_box.addItem("Manhattan distance");
    distance_box.addItem("Euclidean distance");
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

    connect(&previewBtn, SIGNAL(clicked()), this, SLOT(preview_b()));
    connect(&distance_box,SIGNAL(currentTextChanged(QString )),this,SLOT(combo_changed( QString )));
    connect(&k_neighbours,SIGNAL(valueChanged(int)),this,SLOT(sb_changed(int)));
}

void k_mean_cluster_node::run()
{

}

void k_mean_cluster_node::on_input_changed()
{
    t=inputs[0]->get_table();
    model.fit(t);
}

void k_mean_cluster_node::preview_b() {
    t=model.predict(t,0);
    preview();
}

void k_mean_cluster_node::combo_changed(QString str)
{
    if(str.toStdString()=="Euclidean distance")
        model.set_metric(euclidean_dist);
    if(str.toStdString()=="Manhattan distance")
        model.set_metric(manhattan_dist);
    if(str.toStdString()=="SMC distance")
        model.set_metric(smc_dist);
    if(str.toStdString()=="Zakard distance")
        model.set_metric(zakard_dist);
    if(str.toStdString()=="Cos distance")
        model.set_metric(cos_dist);
    needs_update=false;
}

void k_mean_cluster_node::sb_changed(int val)
{
     model.set_k(val);
     needs_update=false;
}

