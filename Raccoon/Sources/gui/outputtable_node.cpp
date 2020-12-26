#include "../../Includes/gui/outputtable_node.hpp"
#include<fstream>
#include<QFileDialog>

outputTable_node::outputTable_node(int width,int height) : node(width,height,1,0)
{
    header_text.setText("OUTPUT NODE");

    file_browser_button.setIcon(QPixmap(":/res/Resources/Action_icons/save_file.png"));

    path_line.setParent(&body);
    path_line.setGeometry(geometry().x() + 10,geometry().y() + 20,180,30);

    file_browser_button.setParent(&body);
    file_browser_button.setGeometry(geometry().x() + 200,geometry().y() + 20,30,30);

    connect(&file_browser_button, &QPushButton::clicked, [&]() {
       QString file= QFileDialog::getSaveFileName(this, tr("Save Table"));
       path_line.setText(file+".csv");
    });
}

void outputTable_node::on_input_changed()
{

}

bool outputTable_node::run()
{
    if(path_line.text()!=""){
        table t=inputs[0]->get_table();
        std::ofstream file(path_line.text().toStdString());

        auto cols = t.col_names();
        if(cols.size()>0){
            for(int i=0;i<cols.size()-1;i++)
                file<<cols[i]<<",";
            file<<cols[cols.size()-1]<<std::endl;

            for(int i=0;i<t.row_n();i++){
                for(int j=0;j<cols.size()-1;j++)
                    file<<t[cols[j]][i].get_string()<<",";
                file<<t[cols[cols.size()-1]][i].get_string()<<std::endl;
            }
        }
        file.close();
    }
    return true;
}

void outputTable_node::preview()
{
    QMessageBox msg;
    msg.setText("Table saved to "+path_line.text()+"csv");
    msg.exec();
}

void outputTable_node::preview_b() {
    QMessageBox msg;
    msg.setText("Table has been saved");
    msg.exec();
}

void outputTable_node::serialize(std::ofstream &os)
{
    os<<"-n output_node"<<std::endl;
    os<<" x="<<geometry().topLeft().x()<<std::endl;
    os<<" y="<<geometry().topLeft().y()<<std::endl;
}
