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
    auto p = inputs[0]->get_packet();
    p.add_column("cluster",column_role::INPUT_COLUMN,column_type::CONTINUOUS);
    outputs[0]->send_packet(p);
}

packet dbscan_node::get_msg()
{
    packet msg=inputs[0]->get_packet();
    msg.add_column("cluster",column_role::INPUT_COLUMN,column_type::CONTINUOUS);
    return msg;
}

void dbscan_node::run()
{
    table r=inputs[0]->get_table();
    t=clusterizer.fit(r);
    outputs[0]->send_data(t);
}

void dbscan_node::preview_b() {
    preview();
}
#include <QDialog>
#include <QHBoxLayout>
#include <random>
void dbscan_node::preview() {
    QDialog *tablePreview=new QDialog();
    tablePreview->setGeometry(400,400,500,500);
    tablePreview->setModal(true);


    QTableWidget data_table;
    make_QTable(data_table,t);

    cluster_colors.clear();
    auto clusts = t["cluster"].unique();
    srand(time(NULL));
    for(unsigned i = 0; i<clusts.size(); i++) {
        int random_nr = rand();
        cluster_colors[clusts[i].get_string()] = QColor(130+random_nr%125,130+(random_nr/255)%125,130+(random_nr/(255*255))%125);
    }

    for(const auto &it:clusts) {
        for(auto& entry : data_table.findItems(QString::fromStdString(it.get_string()),Qt::MatchExactly)){
                        if(data_table.horizontalHeaderItem(entry->column())->text() =="cluster"){
                            entry->setBackground(cluster_colors[it.get_string()]);
                            entry->setForeground(QColor(0,0,0));
                        }
                }
    }

    QVBoxLayout vbox;
    tablePreview->setLayout(&vbox);

    vbox.addWidget(&data_table);

    double d = siluette_coef(t);

    vbox.addSpacing(20);
    QLabel sil_l;
    make_siluete(sil_l,d);

    QLabel lab;
    lab.setText("Siluete score: "+QString::number(d));
    vbox.addWidget(&lab);
    vbox.addWidget(&sil_l);

    tablePreview->exec();
}

void dbscan_node::metric_change(QString str)
{
   if(str=="Euclidean")
       clusterizer.set_metric(euclidean_dist);
   if(str=="Hamming")
       clusterizer.set_metric(hamming_dist);
   if(str=="Manhattan")
       clusterizer.set_metric(manhattan_dist);
   if(str=="SMC")
       clusterizer.set_metric(smc_dist);
   if(str=="Jaccard")
       clusterizer.set_metric(zakard_dist);
   if(str=="Cosine")
       clusterizer.set_metric(cos_dist);
}

void dbscan_node::eps_change(double eps)
{
    clusterizer.set_eps(eps);
}

void dbscan_node::num_change(int n)
{
    clusterizer.set_n_neighbors(n);
}
