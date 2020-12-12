#include "../../Includes/gui/naive_bayes_node.hpp"

nb_node::nb_node(int width, int height) : node(width,height,2){
    header_text.setText("Naive Bayes NODE");

    alpha_sbox.setMinimum(0);
    alpha_sbox.setMaximum(1);
    alpha_sbox.setParent(&body);
    alpha_sbox.setGeometry(geometry().x()+130,geometry().y()+10,50,30);

    alpha_label.setText("alpha:");
    alpha_label.setParent(&body);
    alpha_label.setGeometry(geometry().x()+10,geometry().y()+10,110,30);
    alpha_label.setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    table_label.setText("show bayes table");
    table_label.setParent(&body);
    table_label.setGeometry(geometry().x()+10,geometry().y()+40,110,30);
    table_label.setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    table_checkbox.setChecked(true);
    table_checkbox.setParent(&body);
    table_checkbox.setGeometry(geometry().x()+130,geometry().y()+45,20,20);
    table_checkbox.setStyleSheet("QCheckBox::indicator { width:20; height: 20px;}");

    previewBtn.setText("preview");
    previewBtn.setParent(&body);
    previewBtn.setGeometry(geometry().x() + 190,geometry().y() + 200,50,20);

    connect(&previewBtn, SIGNAL(clicked()), this, SLOT(preview_b()));
    connect(&alpha_sbox,SIGNAL(valueChanged(double)),this,SLOT(alpha_changed(double)));
}

bool nb_node::run()
{
    table train = inputs[0]->get_table();
    table test = inputs[1]->get_table();
    classifier.fit(train);
    test.pop(train.get_target());
    t=classifier.predict(test);
    return true;
}

void nb_node::on_input_changed()
{
    packet msg = inputs[0]->get_packet();
    msg.add_column("assigned",column_role::INPUT_COLUMN,column_type::CONTINUOUS);
    outputs[0]->send_packet(msg);
}

packet nb_node::get_msg()
{
    packet msg = inputs[0]->get_packet();
    msg.add_column("assigned",column_role::INPUT_COLUMN,column_type::CONTINUOUS);
    return msg;
}

void nb_node::preview_b() {
    preview();
}
