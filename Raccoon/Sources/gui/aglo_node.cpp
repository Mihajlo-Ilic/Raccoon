#include"../../Includes/gui/aglo_node.hpp"
#include"../../Includes/stats.hpp"


aglo_node::aglo_node(int width, int height):node(width,height,1)
{
    header_text.setText("AGLO CLUSTERING");

    combo_func.setParent(&body);
    label_func.setParent(&body);
    combo_cfunc.setParent(&body);
    label_cfunc.setParent(&body);
    spin_dist.setParent(&body);
    label_dist.setParent(&body);
    spin_num.setParent(&body);
    label_num.setParent(&body);
    preview_button.setParent(&body);


    label_func.setText("instance metric:");
    label_cfunc.setText("cluster metric:");
    label_dist.setText("maximum dist:");
    label_num.setText("number of clusters:");
    preview_button.setText("preview");

    spin_dist.setMinimum(0);
    spin_num.setMinimum(1);

    combo_cfunc.addItem("Best link");
    combo_cfunc.addItem("Worst link");
    combo_cfunc.addItem("Average link");

    combo_func.addItem("Euclidean");
    combo_func.addItem("Manhattan");
    combo_func.addItem("Smc");
    combo_func.addItem("Zakard");
    combo_func.addItem("Hamming");

    label_cfunc.setGeometry(5,10,110,30);
    label_func.setGeometry(5,50,110,30);
    label_dist.setGeometry(5,90,110,30);
    label_num.setGeometry(5,130,110,30);

    combo_cfunc.setGeometry(120,10,90,30);
    combo_func.setGeometry(120,50,90,30);
    spin_dist.setGeometry(120,90,90,30);
    spin_num.setGeometry(120,130,90,30);

    preview_button.setGeometry(geometry().x() + 190,geometry().y() + 200,50,20);

    connect(&preview_button, SIGNAL(clicked()), this, SLOT(preview_b()));
    connect(&combo_cfunc,SIGNAL(currentTextChanged(QString )),this,SLOT(combo_clust_changed( QString )));
    connect(&combo_func,SIGNAL(currentTextChanged(QString )),this,SLOT(combo_changed( QString )));
    connect(&spin_dist,SIGNAL(valueChanged(double)),this,SLOT(dist_changed(double)));
    connect(&spin_num,SIGNAL(valueChanged(int)),this,SLOT(num_changed(int)));

}

void aglo_node::on_input_changed()
{
    t=inputs[0]->get_table();
    t.pop("partition");
    t.pop(t.get_target());
}

void aglo_node::run()
{

}

void aglo_node::preview_b()
{
    t.pop("partition");
    t.pop("cluster");
    t.pop(t.get_target());
    t= model.fit(t);
    preview();
}


void aglo_node::combo_changed(QString str)
{
    if(str.toStdString() == "Euclidean")
        model.set_instance_metric(euclidean_dist);
    if(str.toStdString() == "Manhattan")
        model.set_instance_metric(manhattan_dist);
    if(str.toStdString() == "Smc")
        model.set_instance_metric(smc_dist);
    if(str.toStdString() == "Zakard")
        model.set_instance_metric(zakard_dist);
    if(str.toStdString() == "Hamming")
        model.set_instance_metric(hamming_dist);
}

void aglo_node::combo_clust_changed(QString str)
{
    if(str.toStdString() == "Worst")
        model.set_cluster_metric(max_linkage);
    if(str.toStdString() == "Best")
        model.set_cluster_metric(min_linkage);
    if(str.toStdString() == "Average")
        model.set_cluster_metric(avg_linkage);
}

void aglo_node::dist_changed(double v)
{
    model.set_distance(v);
}

void aglo_node::num_changed(int v)
{
    model.set_num_clusters(v);
}
