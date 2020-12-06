#include "../../Includes/gui/doc_reader_node.hpp"
#include "../../Includes/file_reader.hpp"
#include <iostream>
#include <fstream>
#include <QHeaderView>
#include <QLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFormLayout>
#include <QFileSystemModel>
#include <stack>
doc_reader_node::doc_reader_node(int width, int height) : node(width, height, 0) {
    header_text.setText("TEXT DOCUMENT READER");

    doc_tree = new QTreeWidget;
    doc_tree->setParent(&body);
    doc_tree->setGeometry(10, 30, 120,180);
    doc_tree->setColumnCount(1);
    QHeaderView * doc_tree_header = new QHeaderView(Qt::Horizontal);
    doc_tree->setHeader(doc_tree_header);
    doc_tree_header->setVisible(false);

    QFrame * btn_header = new QFrame;
    btn_header->setParent(&body);
    btn_header->setGeometry(10, 0, 120, 25);


    add_doc_btn = new QPushButton();
    add_doc_btn->setParent(btn_header);
    add_doc_btn->setGeometry(5,5,15,15);
    add_doc_btn->setMaximumHeight(25);
    add_doc_btn->setMaximumWidth(25);
    QPixmap pix_addfile(":/res/Resources/Action_icons/add_file.svg");
    add_doc_btn->setIcon(QIcon(pix_addfile));

    connect(add_doc_btn,SIGNAL(clicked()),this,SLOT(add()));

    stop_word_btn = new QPushButton();
    stop_word_btn->setParent(btn_header);
    stop_word_btn->setGeometry(25, 5, 15, 15);
    QPixmap pix_stop(":/res/Resources/Action_icons/stop_sign.svg");
    stop_word_btn->setIcon(pix_stop);
    //HACK: find better/more flexible solution for loading stop words
    connect(stop_word_btn, SIGNAL(clicked()), this, SLOT(browse_stop()));

    browse_btn = new QPushButton();
    browse_btn->setParent(&body);
    browse_btn->setGeometry(150, 160, 75, 25);
    browse_btn->setText("browse");

    connect(browse_btn, SIGNAL(clicked()), this, SLOT(browse()));

    preview_btn = new QPushButton();
    preview_btn->setParent(&body);
    preview_btn->setGeometry(150, 190, 75, 25);
    preview_btn->setText("preview");

    connect(preview_btn, SIGNAL(clicked()), this, SLOT(preview_b()));


    settings = new QFormLayout;
    QFrame *settings_frame=new QFrame();
    settings_frame->setParent(&body);
    settings_frame->setGeometry(140, 10, 100, 140);
    settings_frame->setLayout(settings);

    set_class = new QComboBox;
    set_class->addItem("Dir");
    set_class->addItem("File");
    settings->addRow("class:", set_class);

    set_min = new QSpinBox;
    set_min->setMinimum(0);
    settings->addRow("min. freq:", set_min);

    set_max = new QSpinBox;
    set_max->setMinimum(0);
    settings->addRow("max. freq:", set_max);

    binary_chk = new QCheckBox;
    settings->addRow("binary:", binary_chk);

}

void doc_reader_node::check_table()
{
    min_freq = set_min->value();
    max_freq = set_max->value();
    //eliminate words with less than min_freq and more than max_freq appearances
    for(auto col : t.col_names()) {
        entry col_sum = std::accumulate(t[col].begin(), t[col].end(), entry(0.0));
        if(col_sum.get_double() < min_freq || col_sum.get_double() > max_freq)
            t.pop(col);
    }
}

void doc_reader_node::preview_b() {
    preview();
}

void doc_reader_node::browse() {

    if(set_class->currentText() == "Dir") {
        QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
        current_path = dirName.toStdString();

        //loadTextFromDir(path, stop_words, t, true, binary_chk->isChecked());
    }

    else {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Text files (*.txt)"));

        current_path = fileName.toStdString();
        //loadTextFromFile(path, stop_words, t, true, binary_chk->isChecked());
    }

}

void doc_reader_node::browse_stop() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Text files (*.txt)"));
    std::string path = fileName.toStdString();
    loadStopWords(path);
}

void doc_reader_node::on_input_changed(){

}

void doc_reader_node::add() {
    QFileInfo * finfo = new QFileInfo(QString::fromStdString(current_path));
    if(finfo->isDir())
        loadTextFromDir();

    else if(finfo->isFile()) {
        loadTextFromFile(current_path, stop_words, t, true, binary_chk->isChecked());
        QTreeWidgetItem *new_itm = new QTreeWidgetItem();
        new_itm->setText(0, finfo->fileName());
        doc_tree->addTopLevelItem(new_itm);

    }

    else
        std::cerr << "bad path" << std::endl;

    current_path = "";
}

void doc_reader_node::run()
{
    check_table();
    outputs[0]->send_data(t);
}

void doc_reader_node::loadStopWords(std::string stop_words_path) {
    std::ifstream stopFile;
    stopFile.open(stop_words_path);
    //Load stop words (words in the file to ignore...)
    if(!stopFile.is_open())
        std::cerr << "Fajl sa stop recima ne postoji" << std::endl;
    else {
        std::string line;
        while(std::getline(stopFile, line)) {
            std::vector<std::string> words = words_from_line(line);
            stop_words.insert(words.begin(), words.end());
        }
    }

}

void doc_reader_node::loadTextFromDir() {
    t.push_row(std::vector<entry>(t.col_n(), entry(0.0)));
    QFileInfo * finfo = new QFileInfo(QString::fromStdString(current_path));
    t[t.row_n() - 1].set_name(finfo->fileName().toStdString());
    std::unordered_set<std::string> words_in_table;
    for(unsigned i = 0; i<t.col_names().size(); i++) {
        words_in_table.insert(t.col_names()[i]);
    }
    QFileSystemModel *fsmodel = new QFileSystemModel;
    fsmodel->setRootPath(QString::fromStdString(current_path));


    //GUI
    QTreeWidgetItem * root_it = new QTreeWidgetItem();
    root_it->setText(0, finfo->fileName());
    doc_tree->addTopLevelItem(root_it);

    QFileInfoList filetree = fsmodel->rootDirectory().entryInfoList();
    delete fsmodel;
    std::stack<QFileInfo> st;
    for(auto it:filetree)
        st.push(it);

    //begin file tree walk
    while(!st.empty()) {
        auto cur = st.top();
        st.pop();
        //add contents of directory to DFS stack
        if(cur.isDir()) {
            for(auto e : QDir(cur.filePath()).entryInfoList())
                if(e.fileName() != QString(".") && e.fileName() != QString(".."))
                    st.push(e);
        }
        if(cur.isFile()) {
            //check extension
            if(cur.suffix().toStdString() == "txt")
            {
                //GUI stuff
                QTreeWidgetItem * file_item = new QTreeWidgetItem();
                file_item->setText(0, cur.fileName());
                root_it->addChild(file_item);
                //input this file's text
                std::ifstream txtFile;
                txtFile.open(cur.filePath().toStdString());
                if(!txtFile.is_open()) {
                    std::cerr << "Could not open file: " << cur.filePath().toStdString() << std:: endl;
                    continue;
                }

                std::string line;
                //reading file line by line
                while(std::getline(txtFile, line)) {
                    std::vector<std::string> words = words_from_line(line);
                    //if(tolower)
                    std::transform(words.begin(), words.end(), words.begin(), [] (auto w) {return strtolower(w);});
                    for(unsigned i = 0; i<words.size(); i++) {
                        //we only process words that aren't stop words
                        if(stop_words.find(words[i]) == stop_words.end()){
                            //word isn't in the table yet
                            if(words_in_table.find(words[i]) == words_in_table.end())
                            {
                                t.push(words[i]);
                                words_in_table.insert(words[i]);
                                std::for_each(t[words[i]].begin(), t[words[i]].end(), [](entry &e){e = entry(0.0);});
                                t[words[i]][t.row_n() - 1]++;
                            }
                            else
                                if(!binary_chk->isChecked())
                                    t[words[i]][t.row_n() - 1]++;

                        }

                    }
                }

            }
        }
    }
}
