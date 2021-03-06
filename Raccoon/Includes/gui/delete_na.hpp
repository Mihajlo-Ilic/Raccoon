#ifndef DELETE_NA_HPP
#define DELETE_NA_HPP

#include "node.hpp"
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
class delete_na : public node
{
    Q_OBJECT
public:
    delete_na(int width, int height);
    void on_input_changed() override;
    bool run() override;

    void serialize(std::ofstream& os) override;
public slots:
    void preview_b();
private:
    QPushButton preview_btn;
    QLabel n_deleted_lbl;
    QLabel n_rows_lbl;
    QLineEdit n_deleted;
    QLineEdit n_rows;
};

#endif // DELETE_NA_HPP
