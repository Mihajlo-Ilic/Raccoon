#ifndef SCENE_HPP
#define SCENE_HPP

#include"node.hpp"

#include<QGraphicsScene>
#include<QGraphicsItem>

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

    //Graph algorithms used for checking cycles and top sort
    void run_graph();

    static connector* selected_input;
    static connector* selected_output;

    ~raccoon_scene();
private:
    raccoon_scene(int width,int height);
    bool has_cycle(std::vector<node*> graph);

    QGraphicsLineItem connector_line;

    std::vector<node*> scene_nodes;
    static raccoon_scene* instance;
};


#endif
