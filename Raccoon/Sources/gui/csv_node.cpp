#include "../../Includes/gui/csv_node.hpp"
#include "../../Includes/file_reader.hpp"
#include <QFileDialog>
#include <iostream>
#include <QComboBox>

csv_node::csv_node(int width, int height) : node(width,height,0){
    header_text.setText("CSV NODE");
    edit.setParent(&body);
    edit.setGeometry(geometry().x() + 10,geometry().y() + 10,150,30);
    loadBtn.setParent(&body);
    browseBtn.setParent(&body);
    browseBtn.setGeometry(geometry().x() + 165,geometry().y() + 10,30,30);
    loadBtn.setGeometry(geometry().x() + 200,geometry().y() + 10,30,30);
    loadBtn.setText("load");
    colomns.setParent(&body);
    colomns.setGeometry(geometry().x() + 10, geometry().y() + 50, 100, 150);
    previewBtn.setParent(&body);
    previewBtn.setGeometry(geometry().x() + 170,geometry().y() + 185, 70,30);
    runBtn.setGeometry(geometry().x() + 200,geometry().y() + 205, 30,20);
    previewBtn.setText("preview");

    role_box.setParent(&body);
    type_box.setParent(&body);
    role_l.setParent(&body);
    type_l.setParent(&body);

    role_l.setText("Role");
    type_l.setText("Type");

    role_l.setGeometry(geometry().x() + 125, geometry().y() + 50, 100, 30);
    role_box.setGeometry(geometry().x() + 120, geometry().y() + 70, 100, 30);

    type_l.setGeometry(geometry().x() + 125, geometry().y() + 100, 100, 30);
    type_box.setGeometry(geometry().x() + 120, geometry().y() + 130, 100, 30);


    connect(&browseBtn,SIGNAL(clicked()),this,SLOT(browse()));
    connect(&previewBtn,SIGNAL(clicked()),this,SLOT(preview_slot()));
    connect(&loadBtn,SIGNAL(clicked()),this,SLOT(load()));
    connect(&colomns,SIGNAL(itemSelectionChanged()),this,SLOT(list_changed()));

    connect(&role_box,SIGNAL(currentTextChanged(QString )),this,SLOT(role_changed( QString )));
    connect(&type_box,SIGNAL(currentTextChanged(QString )),this,SLOT(type_changed( QString )));

}

void csv_node::load() {
    if(needs_update){
    std::string path = edit.text().toStdString();
    t = loadFromFile(path);
    type_box.clear();
    role_box.clear();


    type_box.addItem("Nominal");
    type_box.addItem("Ordinal");
    type_box.addItem("Continuous");

    role_box.addItem("Input");
    role_box.addItem("Target");
    role_box.addItem("Partition");


    colomns.clear();
    for(auto it:t.col_names())
        colomns.addItem(QString::fromStdString(it));


    outputs[0]->send_packet(packet(t));

    }
    needs_update = false;
}

void csv_node::preview_slot() {
    preview();
}
packet csv_node::get_msg() {
    return packet(t);
}

void csv_node::list_changed()
{
   if(colomns.selectedItems().size()>0){
        auto sel=colomns.selectedItems()[0];
        type_box.setCurrentIndex(t[sel->text().toStdString()].type);
        role_box.setCurrentIndex(t[sel->text().toStdString()].role);
   }
}

void csv_node::type_changed(QString ty)
{
    if(colomns.selectedItems().size()>0){
        std::string col = colomns.selectedItems()[0]->text().toStdString();
        if(ty=="Continuous")
            t[col].type=CONTINUOUS;
        if(ty=="Nominal")
            t[col].type=NOMINAL;
        if(ty=="Ordinal")
            t[col].type=ORDINAL;
        outputs[0]->send_packet(packet(t));
    }
}

void csv_node::role_changed(QString rol)
{
    if(colomns.selectedItems().size()>0){
        std::string col = colomns.selectedItems()[0]->text().toStdString();
        if(rol=="Input")
            t[col].role=INPUT_COLUMN;
        if(rol=="Partition")
            t[col].role=PARTITION;
        if(rol=="Target")
            t[col].role=TARGET;
        outputs[0]->send_packet(packet(t));
    }
}


void csv_node::browse() {

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("CSV files (*.csv)"));
    edit.setText(fileName);
    needs_update = true;
}

void csv_node::run()  {
    outputs[0]->send_data(t);
}

void csv_node::on_input_changed()
{

}
