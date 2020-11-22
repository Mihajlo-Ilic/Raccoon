#ifndef NODE_HPP
#define NODE_HPP

#include<QWidget>
#include<QFrame>
#include<QLabel>
#include<QPushButton>
#include<QTableWidget>

#include<QGraphicsLineItem>
#include<QGraphicsEllipseItem>
#include<QGraphicsPolygonItem>

#include<vector>

#include "../table.hpp"


class input_connector;
class output_connector;

//helper function makes QTableWidget out of table
void make_QTable(QTableWidget& qTable,const table &t);

//INTERFACE USED TO REPRESENT NODE IN SCENE

class node : public QWidget{
    public:

    //QT EVENT METHODS USED FOR DRAGGING AND SELECTIND NODES
        virtual void mousePressEvent(QMouseEvent *event);
        virtual void mouseMoveEvent(QMouseEvent *event);

    //USED FOR GETTING CONNECTED INPUTS/OUTPUTS
        std::vector<node *> get_input_nodes();
        std::vector<node *> get_output_nodes();
    
    //VIRTUAL METHODS PREVIEW IS USED FOR PREVIEW BUTTON EVENT
    //INPUT CHANGED IS USED TO RESPOND TO A CHANGE WHEN CONNECTOR RECIEVES DATA
    //RUN IS FOR TOP SORT OF THE SCENE
        virtual void run() = 0;
        virtual void on_input_changed() = 0;
        virtual void preview();

        void add_to_scene(QGraphicsScene* scene);
        void set_position(const QPointF& point);

        void send_data();

    //NEEDED FOR TOPS SORT TO KNOW IN DEGREE
        int used_inputs();

        virtual ~node();
    protected:
        node(int width,int height,int n_inputs =1,int n_outputs =1);

        QFrame header;
        QFrame body;
        QLabel header_text;
        QLabel warning_icon;
        QPushButton exit_btn;
        
        std::vector<input_connector *> inputs;
        std::vector<output_connector *> outputs;
        bool needs_update;

        table t;

    private:
        bool dragged;
        QPointF dragStart;

        QPointF find_input_position(int index);
        QPointF find_output_position(int index);
};


class edge : public QGraphicsLineItem{
    public:
        edge(input_connector* input,output_connector *out);
        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

        void set_begin_pos(const QPointF& new_point);
        void set_end_pos(const QPointF& new_point);

        void pass_data(const table& t);
        void add_to_scene(QGraphicsScene *scene);

        node* get_input_node();
        node* get_output_node();

        ~edge();
    private:
        void update_arrow();

        input_connector* in;
        output_connector* out;
        QGraphicsPolygonItem arrow;
};

//CONNECTORS USED TO REPRESENT SOCKETS BETWEEN EDGES

class connector : public QGraphicsEllipseItem{
    public:
    enum conn_type{INPUT,OUTPUT};

        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
        virtual void dropEvent(QGraphicsSceneDragDropEvent *event);
        virtual void disconnect(edge* e) = 0;

        void set_text(std::string txt);
        void set_position(const QPointF& new_point);
        void add_to_scene(QGraphicsScene* scene);

        virtual void add_edge(edge *e) = 0;
        virtual void update_edge_position(const QPointF& new_point) = 0;
        
        QPointF get_position() const;
        node* get_parent();

        virtual ~connector();
    protected:
        connector(node* p);

        node* parent;
        QGraphicsSimpleTextItem text;
        conn_type type;
};


class input_connector : public connector{
    public:
        void update_edge_position(const QPointF& new_point) override;
        void disconnect(edge* e) override;

        void recieve_data(const table& t);
        const table& get_table() const;

        void add_edge(edge *e) override;
        node* get_output_node();

        ~input_connector();
    private:
        input_connector(node *p);
        edge* input_edge;
        table input_table;
    friend class node;
};


class output_connector : public connector {
    public:
        void update_edge_position(const QPointF& new_point) override;
        void disconnect(edge* e) override;

        void send_data(const table& t);

        void force_send();

        void add_edge(edge *e) override;
        std::vector<node *> get_input_nodes();

        ~output_connector();
    private:
        output_connector(node* p);
        std::vector<edge*> output_edges;
    friend class node;
};

#endif
