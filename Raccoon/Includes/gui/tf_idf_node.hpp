#ifndef TF_IDF_NODE_HPP
#define TF_IDF_NODE_HPP

#include "node.hpp"
#include "../transformations.hpp"
#include <QPushButton>
#include <QObject>

class tf_idf_node : public node
{
    Q_OBJECT
public:
    tf_idf_node(int width,int height);
    void on_input_changed() override;
    bool run() override;

    void serialize(std::ofstream& os) override;
private:
    QPushButton previewBtn;
public slots:
    void preview_b();
};


#endif // TF_IDF_NODE_HPP
