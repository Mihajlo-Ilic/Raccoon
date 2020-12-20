#include "../../Includes/gui/categoricaltobinnary_node.hpp"
#include "../../Includes/transformations.hpp"

categoricalToBinnary_node::categoricalToBinnary_node(int width,int height) : node(width,height,1)
{
    header_text.setText("CAT. TO BINNARY NODE");

    selectLabel.setParent(&body);
    listWidget.setParent(&body);
    previewBtn.setParent(&body);

    selectLabel.setText("Select attribute:");
    previewBtn.setText("preview");
    listWidget.setSelectionMode(QAbstractItemView::NoSelection);

    selectLabel.setGeometry(geometry().x() + 10,geometry().y() + 10,120,20);
    listWidget.setGeometry(geometry().x() + 10,geometry().y() + 40,230,150);
    previewBtn.setGeometry(geometry().x() + 190,geometry().y() + 200,50,20);

    connect(&previewBtn,SIGNAL(clicked()),this,SLOT(preview_b()));
    connect(&listWidget,SIGNAL(itemChanged(QListWidgetItem *)),this,SLOT(list_changed(QListWidgetItem *)));

}



void categoricalToBinnary_node::serialize(std::ofstream &os)
{
    os<<"-n cat_to_bin"<<std::endl;
    os<<" x="<<geometry().topLeft().x()<<std::endl;
    os<<" y="<<geometry().topLeft().y()<<std::endl;
    for(int i=0;i<listWidget.count();i++)
        os<<" "<<listWidget.item(i)->text().toStdString()<<std::endl;
}

void categoricalToBinnary_node::on_input_changed()
{
    listWidget.clear();
    packet msg = inputs[0]->get_packet();
    for(const auto& it:msg.packet_columns){

        QListWidgetItem *l_item =new QListWidgetItem();
        l_item->setText(QString::fromStdString(it.name));
        l_item->setFlags(l_item->flags() | Qt::ItemIsUserCheckable);
        l_item->setCheckState(Qt::Unchecked);
        if(it.type!=NOMINAL){
            l_item->setFlags(l_item->flags() & ~Qt::ItemIsEnabled);
        }
        listWidget.addItem(l_item);
    }
    needs_update = true;
}

bool categoricalToBinnary_node::run()
{
    int br = 0;
    for(int i = 0; i < listWidget.count();i++)
        if(listWidget.item(i)->checkState()==Qt::CheckState::Checked)
            br++;
    if(warning_cheque([&](auto &x){
        if (br == 0) {
            x += "You did't select atribute for conversion\n";
            return true;
        } else {
        return false;
        }
    })) { return false; }
    else {
        t=inputs[0]->get_table();
        for(const auto& it:listWidget.selectedItems())
            categorical_to_binary(t,it->text().toStdString());

        outputs[0]->send_data(t);

        needs_update=false;

        t=inputs[0]->get_table();
        for(int i=0;i<listWidget.count();i++)
            if(listWidget.item(i)->checkState()==Qt::CheckState::Checked)
            {
                categorical_to_binary(t,listWidget.item(i)->text().toStdString());
            }

        outputs[0]->send_data(t);

        needs_update=false;
        return true;
    }
}

packet categoricalToBinnary_node::get_msg()
{
    packet msg = inputs[0]->get_packet();
    for(int i=0;i<listWidget.count();i++)
        if(listWidget.item(i)->checkState()==Qt::CheckState::Checked)
        {
            msg.add_column("bin_"+listWidget.item(i)->text().toStdString(),column_role::INPUT_COLUMN,column_type::CONTINUOUS);
        }
    return msg;
}

void categoricalToBinnary_node::list_changed(QListWidgetItem *item)
{
    (void)item;
    packet msg = inputs[0]->get_packet();
    for(int i=0;i<listWidget.count();i++)
        if(listWidget.item(i)->checkState()==Qt::CheckState::Checked)
        {
            msg.add_column("bin_"+listWidget.item(i)->text().toStdString(),column_role::INPUT_COLUMN,column_type::CONTINUOUS);
        }
    outputs[0]->send_packet(msg);
}


void categoricalToBinnary_node::preview_b() {
    int br = 0;
    for(int i = 0; i < listWidget.count();i++)
        if(listWidget.item(i)->checkState()==Qt::CheckState::Checked)
            br++;
    if(warning_cheque([&](auto &x){
        if (br == 0) {
            x += "You did't select atribute for conversion\n";
            return true;
        } else {
        return false;
        }
    })) { }
    else
        preview();
}
