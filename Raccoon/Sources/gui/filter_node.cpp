#include "../../Includes/gui/filter_node.hpp"

filter_node::filter_node(int width, int height) : node(width, height, 1)
{
    header_text.setText("FILTER COLUMNS");

    columns_lbl.setParent(&body);
    columns_lbl.setGeometry(10, 0, 200, 30);
    columns_lbl.setText("Select columns to remove:");

    columns.setParent(&body);
    columns.setGeometry(10, 30, 230, 140);
    columns.setSelectionMode(QAbstractItemView::NoSelection);

    preview_btn.setParent(&body);
    preview_btn.setGeometry(180, 180, 60, 30);
    preview_btn.setText("preview");

    connect(&preview_btn, SIGNAL(clicked()), this, SLOT(preview_b()));
    connect(&columns,SIGNAL(itemChanged(QListWidgetItem *)),this,SLOT(list_changed(QListWidgetItem *)));
}

void filter_node::on_input_changed()
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
}

void filter_node::serialize(std::ofstream &os)
{
    os<<"-n filter"<<std::endl;
    os<<" x="<<geometry().topLeft().x()<<std::endl;
    os<<" y="<<geometry().topLeft().y()<<std::endl;
    for(int i=0;i<columns.count();i++)
        os<<" "<<columns.item(i)->text().toStdString()<<std::endl;
}

bool filter_node::run()
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
    for(int i=0;i<columns.count();i++)
        if(columns.item(i)->checkState()==Qt::CheckState::Checked)
        {
            t.pop(columns.item(i)->text().toStdString());
        }
    outputs[0]->send_data(t);
    needs_update=false;
    return true;
    }
}

packet filter_node::get_msg()
{
    packet msg = inputs[0]->get_packet();
    for(int i=0;i<columns.count();i++)
        if(columns.item(i)->checkState()==Qt::CheckState::Checked)
        {
            msg.remove_column(columns.item(i)->text().toStdString());
        }
    return msg;
}

void filter_node::preview_b() {
    preview();
}

void filter_node::list_changed(QListWidgetItem *)
{
    packet msg = inputs[0]->get_packet();
    for(int i=0;i<columns.count();i++)
        if(columns.item(i)->checkState()==Qt::CheckState::Checked)
        {
            msg.remove_column(columns.item(i)->text().toStdString());
        }
    outputs[0]->send_packet(msg);
}
