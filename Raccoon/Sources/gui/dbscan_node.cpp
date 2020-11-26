#include "../../Includes/gui/dbscan_node.hpp"
#include"../../Includes/stats.hpp"

dbscan_node::dbscan_node(int width, int height) : node(width, height, 1)
{
    header_text.setText("DBSCAN CLUSTERING");

    preview_btn.setText("preview");
    preview_btn.setParent(&body);
    preview_btn.setGeometry(170, 180, 70, 30);

    min_neighbor_lbl.setParent(&body);
    min_neighbor_lbl.setText("Min. # of neighbors:");
    min_neighbor_lbl.setGeometry(5,5,150,30);
    min_neighbor_sb.setParent(&body);
    min_neighbor_sb.setGeometry(160, 5, 70, 30);

    eps_lbl.setParent(&body);
    eps_lbl.setText("Epsilon distance:");
    eps_lbl.setGeometry(5, 40, 150, 30);
    eps_sb.setParent(&body);
    eps_sb.setGeometry(160, 40, 70, 30);
    eps_sb.setMinimum(0);

    metric_lbl.setParent(&body);
    metric_lbl.setText("Distance function:");
    metric_lbl.setGeometry(5, 110, 100, 30);
    metric_cb.setParent(&body);
    metric_cb.setGeometry(125, 110, 100, 30);

    metric_cb.addItem("Euclidean");
    metric_cb.addItem("Hamming");
    metric_cb.addItem("Manhattan");
    metric_cb.addItem("SMC");
    metric_cb.addItem("Jaccard");
    metric_cb.addItem("Cosine");

    connect(&preview_btn, SIGNAL(clicked()), this, SLOT(preview_b()));

    connect(&metric_cb,SIGNAL(currentTextChanged(QString )),this,SLOT(metric_change( QString )));
    connect(&eps_sb,SIGNAL(valueChanged(double)),this,SLOT(eps_change(double)));
    connect(&min_neighbor_sb,SIGNAL(valueChanged(int)),this,SLOT(num_change(int)));
}

void dbscan_node::on_input_changed()
{
    t=inputs[0]->get_table();
    needs_update=false;
}

packet dbscan_node::get_msg()
{
    packet msg=inputs[0]->get_packet();
    msg.add_column("cluster",column_role::INPUT_COLUMN,column_type::CONTINUOUS);
    return msg;
}

void dbscan_node::run()
{
    table t=inputs[0]->get_table();
    t=clasterizer.fit(t);
    outputs[0]->send_data(t);
}

void dbscan_node::preview_b() {
    preview();
}

void dbscan_node::metric_change(QString str)
{
   if(str=="Euclidean")
       clasterizer.set_metric(euclidean_dist);
   if(str=="Hamming")
       clasterizer.set_metric(hamming_dist);
   if(str=="Manhattan")
       clasterizer.set_metric(manhattan_dist);
   if(str=="SMC")
       clasterizer.set_metric(smc_dist);
   if(str=="Jaccard")
       clasterizer.set_metric(zakard_dist);
   if(str=="Cosine")
       clasterizer.set_metric(cos_dist);
}

void dbscan_node::eps_change(double eps)
{
    clasterizer.set_eps(eps);
}

void dbscan_node::num_change(int n)
{
    clasterizer.set_n_neighbors(n);
}
