#include "../../Includes/gui/filter_node.hpp"

filter_node::filter_node(int width, int height) : node(width, height, 1)
{
    header_text.setText("FILTER COLUMNS");

    columns_lbl.setParent(&body);
    columns_lbl.setGeometry(10, 0, 200, 30);
    columns_lbl.setText("Select columns to remove:");

    columns.setParent(&body);
    columns.setGeometry(10, 30, 230, 140);
    columns.setSelectionMode(QAbstractItemView::MultiSelection);

    preview_btn.setParent(&body);
    preview_btn.setGeometry(180, 180, 60, 30);
    preview_btn.setText("preview");

    connect(&preview_btn, SIGNAL(clicked()), this, SLOT(preview_b()));
    connect(&columns,SIGNAL(itemSelectionChanged()),this,SLOT(list_changed()));
}

void filter_node::on_input_changed()
{
    t=inputs[0]->get_table();
    columns.clear();
    for(const auto& it:inputs[0]->get_table().col_names())
        columns.addItem(QString::fromStdString(it));
    needs_update=true;
}

void filter_node::run()
{

}

void filter_node::preview_b() {
    preview();
}

void filter_node::list_changed()
{
    table curr=inputs[0]->get_table();
    for(const auto& it:columns.selectedItems())
        curr.pop(it->text().toStdString());
    t=curr;

    outputs[0]->send_data(t);

    needs_update=false;
}
