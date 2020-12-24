#ifndef SCENE_HPP
#define SCENE_HPP

#include"node.hpp"

#include<QGraphicsScene>
#include<QGraphicsItem>
#include <stack>

class action {
public:
    virtual void undo() = 0;
    virtual void redo() = 0;
};

class create_action : public action {
private:
    node * n;
    QPointF scene_pos;
public:
    create_action(node * n, QPointF scene_pos);
    void undo() override;
    void redo() override;


};

class move_action : public action {
private:
    node * n;
    QPoint start_point;
    QPoint end_point;
public:
    move_action(node *n, QPoint start_point, QPoint end_point);
    void set_end_pt(QPoint pt);
    void undo() override;
    void redo() override;

};


class delete_action : public action {
private:
    node * n;
public:
    delete_action(node * n);
    void undo() override;
    void redo() override;
    node * get_node() { return n; }
};

class connect_action : public action {
private:
    node * n1;
    node * n2;
    std::pair<int, int> idxes;
public:
    connect_action(edge * e);
    void undo() override;
    void redo() override;
};

class disconnect_action : public action {
private:
    node * n1;
    node * n2;
    std::pair<int, int> idxes;
public:
    disconnect_action(edge * e);
    void undo() override;
    void redo() override;
};

class raccoon_scene : public QGraphicsScene {
public:
    raccoon_scene(const raccoon_scene& rhs) = delete;
    raccoon_scene& operator=(const raccoon_scene& rhs) = delete;

    //QT EVENTS USED FOR DRAGING NODES AND EDGES , AND ADDING TO SCENE
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);

    static raccoon_scene* get_instance(int width,int height);
    static raccoon_scene *get_ptr();
    void undo_action();
    void redo_action();
    //Graph algorithms used for checking cycles and top sort
    void run_graph();

    static connector* selected_input;
    static connector* selected_output;

    void delete_graph();
    void delete_edge();

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void unmerge_nodes();

    void save_scene(const std::string &path);
    void load_scene(const std::string &path);


    ~raccoon_scene();

private:
    raccoon_scene(int width,int height);
    bool has_cycle(std::vector<node*> graph);

    QGraphicsLineItem connector_line;

    static raccoon_scene* instance;
};


#endif
