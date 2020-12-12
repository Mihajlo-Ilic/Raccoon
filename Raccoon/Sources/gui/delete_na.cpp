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
    packet msg = inputs[0]->get_packet();
    needs_update = false;
    outputs[0]->send_packet(msg);
}

bool delete_na::run()
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
    remove_na_rows(t);
    outputs[0]->send_data(t);
    return true;
    }
}
