#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    virtual void keyPressEvent(QKeyEvent *event);

    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);

public slots:
    void drop_action();
private slots:
    void on_action_run_triggered();

    void on_actionSave_As_triggered();

    void on_actionSave_2_triggered();

    void on_actionOpen_Scene_triggered();

    void on_actionNew_Scene_triggered();

    void on_action_unmerge_triggered();

    void on_action_delete_triggered();



private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
