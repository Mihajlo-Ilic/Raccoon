#include "../../Includes/gui/aproximation_node.hpp"
#include "../../Includes/transformations.hpp"

aproximation_node::aproximation_node(int width,int height) : node(width,height)
{
    header_text.setText("APROXIMATION NODE");

    aproximationCombo.setParent(&body);
    aproximationLabel.setParent(&body);
    columns.setParent(&body);
    previewBtn.setParent(&body);
    editAprox.setParent(&body);

    columns.setSelectionMode(QAbstractItemView::NoSelection);
    aproximationLabel.setText("Choose method and attribute:");
    aproximationCombo.addItem("By mean");
    aproximationCombo.addItem("By user adding");
    editAprox.setText("0");
    editAprox.hide();
    previewBtn.setText("preview");

    aproximationLabel.setGeometry(geometry().x() + 10,geometry().y() + 10,230,20);
    aproximationCombo.setGeometry(geometry().x() + 10,geometry().y() + 40,110,20);
    editAprox.setGeometry(geometry().x() + 130,geometry().y() + 40,110,20);
    columns.setGeometry(geometry().x() + 10,geometry().y() + 70,230,120);
    previewBtn.setGeometry(geometry().x() + 190,geometry().y() + 200,50,20);

    connect(&previewBtn,SIGNAL(clicked()),this,SLOT(preview_b()));
    connect(&columns,SIGNAL(itemChanged(QListWidgetItem *)),this,SLOT(list_changed(QListWidgetItem *)));
    connect(&aproximationCombo,SIGNAL(currentTextChanged(QString )),this,SLOT(help_func( QString )));
    connect(&editAprox,SIGNAL(textChanged(QString)),this,SLOT(help_func(QString)));
}

void aproximation_node::preview_b() {
    preview();
}

void aproximation_node::changed()
{
    if(aproximationCombo.currentText()=="By user adding")
        editAprox.show();
    else
        editAprox.hide();
}

void aproximation_node::on_input_changed()
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

void aproximation_node::run()
{

    t=inputs[0]->get_table();
    for(int i=0;i<columns.count();i++){
        if(columns.item(i)->checkState()==Qt::CheckState::Checked)
        {
          if(aproximationCombo.currentText()=="By user adding")
              aproximation_val(t,columns.item(i)->text().toStdString(),editAprox.text().toDouble());
            else
          if(aproximationCombo.currentText()=="By mean")
              aproximation_mean(t,columns.item(i)->text().toStdString());
        }
    }
    outputs[0]->send_data(t);
    needs_update=false;
}

void aproximation_node::help_func(const QString &text){
    Q_UNUSED(text);
    changed();
}

void aproximation_node::list_changed(QListWidgetItem *item)
{
    (void)item;
    packet msg = inputs[0]->get_packet();
    outputs[0]->send_packet(msg);
}

