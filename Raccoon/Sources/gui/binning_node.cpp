#include "../../Includes/gui/binning_node.hpp"
#include "../../Includes/transformations.hpp"

void binning_node::on_input_changed()
{
    t=inputs[0]->get_table();
    listWidget.clear();
    for(const auto& it:inputs[0]->get_table().col_names())
        listWidget.addItem(QString::fromStdString(it));
    needs_update=true;
}

binning_node::binning_node(int width, int height) : node(width,height,1)
{
    header_text.setText("BINNING NODE");

    listWidget.setParent(&body);
    binningMethod.setParent(&body);
    binningShuffle.setParent(&body);
    binningMethodLabel.setParent(&body);
    binningShuffleLabel.setParent(&body);
    previewBtn.setParent(&body);

    listWidget.setSelectionMode(QAbstractItemView::MultiSelection);
    binningMethodLabel.setText("Method:");
    binningShuffleLabel.setText("Representation:");
    binningMethod.addItem("binning width");
    binningMethod.addItem("binning frequency");
    previewBtn.setText("preview");
    binningShuffle.addItem("By mean");
    binningShuffle.addItem("By border");

    listWidget.setGeometry(geometry().x() + 10,geometry().y() + 10,230,130);
    binningMethodLabel.setGeometry(geometry().x() + 10,geometry().y() + 150,110,20);
    binningShuffleLabel.setGeometry(geometry().x() + 130,geometry().y() + 150,110,20);
    binningMethod.setGeometry(geometry().x() + 10,geometry().y() + 180,110,20);
    binningShuffle.setGeometry(geometry().x() + 130,geometry().y() + 180,110,20);
    previewBtn.setGeometry(geometry().x() + 190,geometry().y() + 205,50,20);

    connect(&previewBtn,SIGNAL(clicked()),this,SLOT(preview_b()));
    connect(&binningShuffle,SIGNAL(currentTextChanged(QString )),this,SLOT(changed( QString )));
    connect(&binningMethod,SIGNAL(currentTextChanged(QString )),this,SLOT(changed( QString )));
    connect(&listWidget,SIGNAL(itemSelectionChanged()),this,SLOT(list_changed()));
}

void binning_node::run()
{

}

void binning_node::list_changed()
{
    t=inputs[0]->get_table();
    for(const auto& it:listWidget.selectedItems()){

        if(binningMethod.currentText()=="binning width")
            binning_width(t,it->text().toStdString(),1);
        if(binningMethod.currentText()=="binning frequency")
            binning_frequency(t,it->text().toStdString(),1);


        if(binningShuffle.currentText()=="By mean")
            binning_mean(t,it->text().toStdString(),1);
        if(binningShuffle.currentText()=="By border")
            binning_boundry(t,it->text().toStdString(),1);
    }

    outputs[0]->send_data(t);

    needs_update=false;
}



void binning_node::changed(const QString &s)
{
    Q_UNUSED(s);
    list_changed();
}

void binning_node::preview_b() {
    preview();
}
