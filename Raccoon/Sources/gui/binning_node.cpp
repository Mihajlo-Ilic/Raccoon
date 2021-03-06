#include "../../Includes/gui/binning_node.hpp"
#include "../../Includes/transformations.hpp"
#include<iostream>
void binning_node::on_input_changed()
{
    listWidget.clear();
    packet msg = inputs[0]->get_packet();
    for(const auto& it:msg.packet_columns){
        QListWidgetItem *l_item =new QListWidgetItem();
        l_item->setText(QString::fromStdString(it.name));
        l_item->setFlags(l_item->flags() | Qt::ItemIsUserCheckable);
        l_item->setCheckState(Qt::Unchecked);
        listWidget.addItem(l_item);
    }
    needs_update = true;
}

void binning_node::serialize(std::ofstream &os)
{
    os<<"-n bin"<<std::endl;
    os<<" x="<<geometry().topLeft().x()<<std::endl;
    os<<" y="<<geometry().topLeft().y()<<std::endl;
    os<<" shuffle="<<binningShuffle.currentText().toStdString()<<std::endl;
    os<<" method="<<binningMethod.currentText().toStdString()<<std::endl;
    for(int i=0;i<listWidget.count();i++)
        os<<" "<<listWidget.item(i)->text().toStdString()<<std::endl;
}

binning_node::binning_node(int width, int height) : node(width,height,1)
{
    header_text.setText("BINNING NODE");

    listWidget.setParent(&body);
    binningMethod.setParent(&body);
    binningShuffle.setParent(&body);
    binningMethodLabel.setParent(&body);
    binningShuffleLabel.setParent(&body);
    sets_spin.setParent(&body);
    set_label.setParent(&body);
    previewBtn.setParent(&body);

    listWidget.setSelectionMode(QAbstractItemView::NoSelection);
    binningMethodLabel.setText("Method:");
    binningShuffleLabel.setText("Representation:");
    binningMethod.addItem("binning width");
    binningMethod.addItem("binning frequency");
    previewBtn.setText("preview");
    binningShuffle.addItem("By mean");
    binningShuffle.addItem("By border");

    sets_spin.setMinimum(1);

    sets_spin.setGeometry(geometry().x() + 60,geometry().y() + 120,50,30);
    set_label.setGeometry(geometry().x() + 10,geometry().y() + 120,40,30);
    set_label.setText("# bins:");

    listWidget.setGeometry(geometry().x() + 10,geometry().y() + 10,230,110);
    binningMethodLabel.setGeometry(geometry().x() + 10,geometry().y() + 150,110,20);
    binningShuffleLabel.setGeometry(geometry().x() + 130,geometry().y() + 150,110,20);
    binningMethod.setGeometry(geometry().x() + 10,geometry().y() + 180,110,20);
    binningShuffle.setGeometry(geometry().x() + 130,geometry().y() + 180,110,20);
    previewBtn.setGeometry(geometry().x() + 170,geometry().y() + 205,70,20);

    connect(&previewBtn,SIGNAL(clicked()),this,SLOT(preview_b()));
    connect(&binningShuffle,SIGNAL(currentTextChanged(QString )),this,SLOT(changed( QString )));
    connect(&binningMethod,SIGNAL(currentTextChanged(QString )),this,SLOT(changed( QString )));
    connect(&listWidget,SIGNAL(itemChanged(QListWidgetItem *)),this,SLOT(list_changed(QListWidgetItem *)));
}

#include<iostream>
bool binning_node::run()
{
    int br = 0;
    for(int i = 0; i < listWidget.count();i++)
        if(listWidget.item(i)->checkState()==Qt::CheckState::Checked)
            br++;
    if(warning_cheque([&](auto &x){
        if (br == 0) {
            x += "You did't select atribute for binning\n";
            return true;
        } else {
        return false;
        }
    })) { return false; }
    else {
        t=inputs[0]->get_table();
        for(int i=0;i<listWidget.count();i++){
            if(listWidget.item(i)->checkState()==Qt::CheckState::Checked)
            {
            if(binningMethod.currentText()=="binning width")
                binning_width(t,listWidget.item(i)->text().toStdString(),sets_spin.value());
            if(binningMethod.currentText()=="binning frequency")
                binning_frequency(t,listWidget.item(i)->text().toStdString(),sets_spin.value());


            if(binningShuffle.currentText()=="By mean")
                binning_mean(t,listWidget.item(i)->text().toStdString(),sets_spin.value());
            if(binningShuffle.currentText()=="By border")
                binning_boundry(t,listWidget.item(i)->text().toStdString(),sets_spin.value());
            }
        }

        outputs[0]->send_data(t);

        needs_update=false;
        return true;
    }
}

packet binning_node::get_msg()
{
    packet msg = inputs[0]->get_packet();
    for(int i=0;i<listWidget.count();i++)
        if(listWidget.item(i)->checkState()==Qt::CheckState::Checked)
        {
            msg.add_column("bin_"+listWidget.item(i)->text().toStdString(),column_role::INPUT_COLUMN,column_type::CONTINUOUS);
        }
    return msg;
}
#include<iostream>
void binning_node::list_changed(QListWidgetItem *item)
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



void binning_node::changed(const QString &s)
{
    Q_UNUSED(s);

}

void binning_node::preview_b() {
    int br = 0;
    for(int i = 0; i < listWidget.count();i++)
        if(listWidget.item(i)->checkState()==Qt::CheckState::Checked)
            br++;
    if(warning_cheque([&](auto &x){
        if (br == 0) {
            x += "You did't select atribute for binning\n";
            return true;
        } else {
        return false;
        }
    })) {  }
    else {
        preview();
    }
}
