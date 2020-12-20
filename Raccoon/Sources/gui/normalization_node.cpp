#include "../../Includes/gui/normalization_node.hpp"
#include "../../Includes/transformations.hpp"

normalization_node::normalization_node(int width, int height) : node(width, height, 1)
{
    header_text.setText("NORMALIZE");
    columns.setParent(&body);
    columns.setGeometry(10, 10, 230, 150);
    columns.setSelectionMode(QAbstractItemView::NoSelection);

    preview_btn.setParent(&body);
    preview_btn.setGeometry(180, 180, 60, 30);
    preview_btn.setText("preview");

    connect(&preview_btn, SIGNAL(clicked()), this, SLOT(preview_b()));
    connect(&columns, SIGNAL(itemSelectionChanged()), this, SLOT(list_changed()));
}

void normalization_node::serialize(std::ofstream &os)
{
    os<<"-n normalization"<<std::endl;
    os<<" x="<<geometry().topLeft().x()<<std::endl;
    os<<" y="<<geometry().topLeft().y()<<std::endl;
    for(int i=0;i<columns.count();i++)
        os<<" "<<columns.item(i)<<std::endl;
}

void normalization_node::on_input_changed()
{
    columns.clear();
    packet msg = inputs[0]->get_packet();
    for(const auto& it:msg.packet_columns){
        QListWidgetItem *l_item =new QListWidgetItem();
        l_item->setText(QString::fromStdString(it.name));
        l_item->setFlags(l_item->flags() | Qt::ItemIsUserCheckable);
        l_item->setCheckState(Qt::Unchecked);
        columns.addItem(l_item);
    }
    needs_update = true;
    outputs[0]->send_packet(msg);
}

bool normalization_node::run()
{
    t=inputs[0]->get_table();
    for(int i=0;i<columns.count();i++){
        if(columns.item(i)->checkState()==Qt::CheckState::Checked)
        {
            normalize(t,columns.item(i)->text().toStdString());
        }
    }

    outputs[0]->send_data(t);

    needs_update=false;
    return true;
}

void normalization_node::preview_b() {
    outputs[0]->send_data(t);
    preview();
}

void normalization_node::list_changed(){

}
