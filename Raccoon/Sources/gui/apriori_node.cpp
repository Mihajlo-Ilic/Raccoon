#include "../../Includes/gui/apriori_node.hpp"
#include<QHBoxLayout>
#include<QFormLayout>
#include<QListWidget>

apriori_node::apriori_node(int width, int height):node(250,250,1,1)
{
    header_text.setText("APRIORI NODE");

    support_spin.setParent(&body);
    support_label.setParent(&body);
    confidence_spin.setParent(&body);
    confidence_label.setParent(&body);

    support_label.setText("min support");
    confidence_label.setText("min confidence");

    support_spin.setGeometry(geometry().x() + 10,geometry().y() + 10,110,30);
    support_label.setGeometry(geometry().x() + 130,geometry().y() + 10,110,30);
    confidence_spin.setGeometry(geometry().x() + 10,geometry().y() + 50,110,30);
    confidence_label.setGeometry(geometry().x() + 130,geometry().y() + 50,110,30);

    previewBtn.setParent(&body);
    previewBtn.setText("preview");
    previewBtn.setGeometry(geometry().x() + 130,geometry().y() + 80,110,30);

    connect(&previewBtn,SIGNAL(clicked()),this,SLOT(preview_b()));

    connect(&support_spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),[&](double d){
        model.set_support(d);
    });

    connect(&confidence_spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),[&](double d){
        model.set_confidence(d);
    });
}

void apriori_node::serialize(std::ofstream &os)
{
    os<<"-n apriori"<<std::endl;
    os<<" x="<<geometry().topLeft().x()<<std::endl;
    os<<" y="<<geometry().topLeft().y()<<std::endl;
    os<<" support="<<support_spin.value()<<std::endl;
    os<<" confidence="<<confidence_spin.value()<<std::endl;
}

void apriori_node::preview_b()
{
    preview();
}

void apriori_node::on_input_changed()
{
    packet msg = inputs[0]->get_packet();
    msg.add_column("support",INPUT_COLUMN,CONTINUOUS);
    msg.add_column("confidence",INPUT_COLUMN,CONTINUOUS);
    outputs[0]->send_packet(msg);
    needs_update = true;
}

bool apriori_node::run()
{
    t = inputs[0]->get_table();
    t = model.fit(t);
    outputs[0]->send_data(t);
    return true;
}

void apriori_node::preview()
{
    QDialog tablePreview;
    tablePreview.setGeometry(400,400,500,500);
    tablePreview.setModal(true);


    QHBoxLayout dialog_layout;
    tablePreview.setLayout(&dialog_layout);

    QTableWidget data_table;
    make_QTable(data_table,t);
    data_table.setSortingEnabled(true);

    dialog_layout.addWidget(&data_table);
    data_table.setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    QFrame frm;
    QFormLayout f_lay;
    frm.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    frm.setLayout(&f_lay);

    dialog_layout.addWidget(&frm);

    QDoubleSpinBox min_conf;
    min_conf.setMinimum(confidence_spin.value());
    min_conf.setSingleStep(0.1);

    f_lay.addRow("minimum confidence:",&min_conf);

    QDoubleSpinBox min_support;
    min_support.setSingleStep(0.1);
    min_support.setMinimum(support_spin.value());

    f_lay.addRow("minimum support:",&min_support);

    QDoubleSpinBox min_lift;
    f_lay.addRow("minimum lift:",&min_lift);
    min_lift.setSingleStep(0.1);

    QDoubleSpinBox max_lift;
    f_lay.addRow("maximum lift:",&max_lift);
    max_lift.setSingleStep(0.1);

    QDoubleSpinBox min_dep;
    f_lay.addRow("minimum deployability:",&min_dep);
    min_dep.setSingleStep(0.1);

    QDoubleSpinBox max_dep;
    f_lay.addRow("maximum deployability:",&max_dep);
    max_dep.setSingleStep(0.1);

    //spinbox events

    int conf_location = 0;
    int support_location = 0;
    int lift_location = 0;
    int deplo_location = 0;
    int item_sets_location = 0;

    for(int i=0;i<data_table.columnCount();i++){
        if(data_table.horizontalHeaderItem(i)->text() == "confidence")
            conf_location = i;
        if(data_table.horizontalHeaderItem(i)->text() == "support")
            support_location = i;
        if(data_table.horizontalHeaderItem(i)->text() == "lift")
            lift_location = i;
        if(data_table.horizontalHeaderItem(i)->text() == "deployability")
            deplo_location = i;
        if(data_table.horizontalHeaderItem(i)->text() == "item_sets")
            item_sets_location = i;
    }


    connect(&min_conf, QOverload<double>::of(&QDoubleSpinBox::valueChanged),[&](double d){

        for(int i=0;i<data_table.rowCount();i++){
            if(data_table.item(i,conf_location)->text().toFloat()<d)
                data_table.hideRow(i);
            else  data_table.showRow(i);
        }
    });

    connect(&min_support, QOverload<double>::of(&QDoubleSpinBox::valueChanged),[&](double d){

        for(int i=0;i<data_table.rowCount();i++){
            if(data_table.item(i,support_location)->text().toFloat()<d)
                data_table.hideRow(i);
            else  data_table.showRow(i);
        }
    });

    connect(&min_lift, QOverload<double>::of(&QDoubleSpinBox::valueChanged),[&](double d){

        for(int i=0;i<data_table.rowCount();i++){
            if(data_table.item(i,lift_location)->text().toFloat()<d)
                data_table.hideRow(i);
            else  data_table.showRow(i);
        }
    });

    connect(&max_lift, QOverload<double>::of(&QDoubleSpinBox::valueChanged),[&](double d){

        for(int i=0;i<data_table.rowCount();i++){
            if(data_table.item(i,lift_location)->text().toFloat()>=d)
                data_table.hideRow(i);
            else  data_table.showRow(i);
        }
    });

    connect(&min_dep, QOverload<double>::of(&QDoubleSpinBox::valueChanged),[&](double d){

        for(int i=0;i<data_table.rowCount();i++){
            if(data_table.item(i,deplo_location)->text().toFloat()<d)
                data_table.hideRow(i);
            else  data_table.showRow(i);
        }
    });

    connect(&max_dep, QOverload<double>::of(&QDoubleSpinBox::valueChanged),[&](double d){

        for(int i=0;i<data_table.rowCount();i++){
            if(data_table.item(i,deplo_location)->text().toFloat()>=d)
                data_table.hideRow(i);
            else  data_table.showRow(i);
        }
    });

    //adding list widgets
    QListWidget ante_list;
    ante_list.setSelectionMode(QAbstractItemView::MultiSelection);
    f_lay.addRow("show only antedecedent",&ante_list);
    auto cols = inputs[0]->get_table().col_names();

    for(auto& it :cols)
        ante_list.addItem(QString::fromStdString(it));

    QListWidget cons_list;
    cons_list.setSelectionMode(QAbstractItemView::MultiSelection);
    f_lay.addRow("show only consequent",&cons_list);

    for(auto& it :cols)
        cons_list.addItem(QString::fromStdString(it));

    connect(&ante_list, &QListWidget::itemSelectionChanged,[&](){

        for(int i=0;i<data_table.rowCount();i++){
           auto vec = data_table.item(i,item_sets_location)->text().split("->");
           bool help = true;
           for(auto& it:cons_list.selectedItems())
               if(vec[1].contains(it->text())==false)
                   help=false;
           for(auto& it:ante_list.selectedItems())
               if(vec[0].contains(it->text())==false)
                   help=false;
           if(help==false)
               data_table.hideRow(i);
           else data_table.showRow(i);
        }
    });

    connect(&cons_list, &QListWidget::itemSelectionChanged,[&](){
        for(int i=0;i<data_table.rowCount();i++){
           auto vec = data_table.item(i,item_sets_location)->text().split("->");
           bool help = true;
           for(auto& it:cons_list.selectedItems())
               if(vec[1].contains(it->text())==false)
                   help=false;
           for(auto& it:ante_list.selectedItems())
               if(vec[0].contains(it->text())==false)
                   help=false;
           if(help==false)
               data_table.hideRow(i);
           else data_table.showRow(i);
        }

    });

    tablePreview.exec();
}
