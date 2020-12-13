#include "../../Includes/gui/partition_node.hpp"
#include "../../Includes/transformations.hpp"
#include <QDialog>
#include <QHBoxLayout>
#include <QTableWidget>

partition_node::partition_node(int width, int height) : node(width, height, 1,2)
{
    header_text.setText("PARTITION NODE");
    preview_btn.setParent(&body);
    preview_btn.setText("preview");
    preview_btn.setGeometry(5, 75, 60, 30);

    lbl_percent.setParent(&body);
    lbl_percent.setText("Train/test ratio:");
    lbl_percent.setGeometry(5,5,100,30);
    sb_percent.setParent(&body);
    sb_percent.setGeometry(105, 5, 40, 30);

    lbl_random.setParent(&body);
    lbl_random.setText("Randomness seed:");
    lbl_random.setGeometry(5, 40, 120, 30);
    sb_random.setParent(&body);
    sb_random.setGeometry(125, 40, 40, 30);
    sb_random.setMinimum(0);

    sb_percent.setMinimum(0);
    sb_percent.setMaximum(100);

    outputs[0]->set_text("train");
    outputs[1]->set_text("test");

    connect(&preview_btn,SIGNAL(clicked()),this,SLOT(preview_b()));
    connect(&sb_percent,SIGNAL(valueChanged(int)),this,SLOT(changed(int)));
    connect(&sb_random,SIGNAL(valueChanged(int)),this,SLOT(changed(int)));
}

void partition_node::serialize(std::ofstream &os)
{
    os<<"-n partition"<<std::endl;
    os<<" x="<<geometry().topLeft().x()<<std::endl;
    os<<" y="<<geometry().topLeft().y()<<std::endl;
    os<<" percent="<<sb_percent.text().toStdString()<<std::endl;
    os<<" random="<<sb_random.text().toStdString()<<std::endl;

}

void partition_node::preview_b() {
    preview();
}

void partition_node::on_input_changed(){
    packet msg = inputs[0]->get_packet();
    msg.add_column("partition",column_role::PARTITION,column_type::NOMINAL);
    outputs[0]->send_packet(msg);
    outputs[1]->send_packet(msg);
    needs_update = true;
}

bool partition_node::run()
{
    t=inputs[0]->get_table();
    if(warning_cheque([&](auto &x){
        if (t.col_n() == 0) {
            x += "The packet didn't had the table!\n";
            return true;
        } else {
        return false;
        }
    })) { return true; }
    else {
    partition(t,(double)sb_percent.value()/100.0,sb_random.value());

    outputs[0]->send_data(t[t.where("partition",[](auto x){return x==entry("training");})]);
    outputs[1]->send_data(t[t.where("partition",[](auto x){return x==entry("test");})]);
    needs_update=false;
    return true;
    }
}

packet partition_node::get_msg()
{
    packet msg = inputs[0]->get_packet();
    msg.add_column("partition",column_role::PARTITION,column_type::NOMINAL);
    return msg;
}

void partition_node::changed(int i){
    Q_UNUSED(i);

}

