#include "../../Includes/gui/delete_na.hpp"
#include "../../Includes/transformations.hpp"

delete_na::delete_na(int width, int height) : node(width, height, 1)
{
    header_text.setText("DELETE N/A ROWS");

    n_deleted_lbl.setParent(&body);
    n_deleted_lbl.setGeometry(30, 30, 120, 30);
    n_deleted_lbl.setText("# of rows deleted:");

    n_deleted.setParent(&body);
    n_deleted.setReadOnly(true);
    n_deleted.setGeometry(160, 30, 50, 30);

    n_rows_lbl.setParent(&body);
    n_rows_lbl.setGeometry(30, 80, 120, 30);
    n_rows_lbl.setText("# of rows in table:");

    n_rows.setParent(&body);
    n_rows.setReadOnly(true);
    n_rows.setGeometry(160, 80, 50, 30);

    preview_btn.setParent(&body);
    preview_btn.setGeometry(180, 180, 60, 30);
    preview_btn.setText("preview");

    connect(&preview_btn, SIGNAL(clicked()), this, SLOT(preview_b()));
}

void delete_na::preview_b() {
    preview();
}

void delete_na::on_input_changed(){
    t=inputs[0]->get_table();
    remove_na_rows(t);

    outputs[0]->send_data(t);

    n_deleted.setText(
                QString::fromStdString( std::to_string(inputs[0]->get_table().row_n()
                -t.row_n())));
    n_rows.setText(QString::fromStdString(std::to_string(t.row_n())));

    needs_update=false;
}

void delete_na::run()
{

}
