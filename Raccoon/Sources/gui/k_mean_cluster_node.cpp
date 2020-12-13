#include "../../Includes/gui/k_mean_cluster_node.hpp"
#include<QDialog>
#include<QVBoxLayout>

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

void k_mean_cluster_node::serialize(std::ofstream &os)
{
    os<<"-n k_mean"<<std::endl;
    os<<" x="<<geometry().topLeft().x()<<std::endl;
    os<<" y="<<geometry().topLeft().y()<<std::endl;
    os<<" distance="<<distance_box.currentText().toStdString()<<std::endl;
    os<<" k="<<k_neighbours.value()<<std::endl;
}

bool k_mean_cluster_node::run()
{
    t=inputs[0]->get_table();
    if(warning_cheque([&](auto &x){
        if (t.col_n() == 0) {
            x += "The packet was empty!\n";
            return true;
        } else {
        return false;
        }
    })) {
       return false;
    }
    else {
        model.fit(t);
        t=model.predict(t);
        outputs[0]->send_data(t);
        return true;
    }
}

void k_mean_cluster_node::on_input_changed()
{
    packet msg = inputs[0]->get_packet();
    msg.add_column("cluster",column_role::INPUT_COLUMN,column_type::CONTINUOUS);
    outputs[0]->send_packet(msg);
}

packet k_mean_cluster_node::get_msg()
{
    packet msg=inputs[0]->get_packet();
    msg.add_column("cluster",column_role::INPUT_COLUMN,column_type::CONTINUOUS);
    return msg;
}

void k_mean_cluster_node::preview()
{
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

    QLabel lab;
    lab.setText("Siluete score: "+QString::number(d));
    vbox.addWidget(&lab);

    QSlider slider;
    slider.setOrientation(Qt::Horizontal);
    slider.setMaximum(100);
    slider.setMinimum(-100);
    slider.setValue(d*100);
    slider.setEnabled(false);
    slider.setStyleSheet("QSlider::groove:horizontal { visibility:hidden; }QSlider::handle:horizontal:disabled{background:rgba(0,0,255,255);} QSlider{background-color: qlineargradient(spread:repeat, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 0, 0, 255),stop:0.5 rgba(255, 183, 0, 255), stop:1 rgba(0, 255, 126, 255));}");

    vbox.addWidget(&slider);

    tablePreview->exec();
}


void k_mean_cluster_node::preview_b() {
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

