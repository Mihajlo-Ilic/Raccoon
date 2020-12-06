#ifndef DOC_READER_NODE_HPP
#define DOC_READER_NODE_HPP

#include "node.hpp"
#include "../file_reader.hpp"
#include <QTreeWidget>
#include <QFormLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
class doc_reader_node : public node {
    Q_OBJECT
private:
    std::string current_path;
    std::unordered_set<std::string> stop_words;
    int min_freq;
    int max_freq;
    QTreeWidget * doc_tree;
    QFormLayout * settings;

    QPushButton * stop_word_btn;
    QPushButton * preview_btn;
    QPushButton * add_doc_btn;
    QPushButton * browse_btn;

    QComboBox * set_class;
    QSpinBox * set_min;
    QSpinBox * set_max;
    QCheckBox * binary_chk;
public slots:
    void preview_b();
    void browse();
    void browse_stop();
    void add();
public:
    void on_input_changed() override;
    void run() override;
    doc_reader_node(int width, int height);
    void check_table();
    void loadStopWords(std::string stop_words_path);
    void loadTextFromDir();
    ~doc_reader_node() = default;
};


#endif // DOC_READER_NODE_HPP
