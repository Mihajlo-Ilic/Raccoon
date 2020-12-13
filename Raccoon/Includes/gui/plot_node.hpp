#ifndef PLOT_NODE_H
#define PLOT_NODE_H



#include "../table.hpp"
#include "node.hpp"

#include <QListWidget>
#include <QComboBox>

class plot_node : public node
{
    Q_OBJECT
private:
    QComboBox combo_box;
    QListWidget list_widget;
    QLabel label;
    QLabel label2;
    QPushButton previewBtn;
public:
    plot_node(int width, int height);
    void plot_function(std::vector<std::string> atributes, std::string label);
    void plot_3D(std::vector<std::string> atributes, std::string label,std::vector<std::pair<entry,long>> classInRGB);
    void plot_2D(std::vector<std::string> atributes, std::string label,std::vector<std::pair<entry,long>> classInRGB);
    bool run() override;
    void preview() override;
    void on_input_changed() override;

    void serialize(std::ofstream& os) override;
public slots:
    void preview_b();
};


#endif // PLOT_NODE_H
