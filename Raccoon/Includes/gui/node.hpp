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
#include <QMessageBox>

#include<vector>

#include "../table.hpp"


class input_connector;
class output_connector;

//helper function makes QTableWidget out of table
void make_QTable(QTableWidget& qTable,const table &t);

//structs used for representing packets being sent between nodes

struct column_data{
    bool operator!=(const column_data& rhs) const;

    bool operator==(const column_data& rhs) const;

    std::string name;
    column_role role;
    column_type type;
};

struct packet{

    packet();

    packet(const table& t);

    bool operator!=(const packet& rhs) const;

    bool operator==(const packet& rhs) const;

    void add_column(const collumn& col,std::string name);

    void add_column(std::string name,column_role r,column_type t);

    void remove_column(std::string item);

    std::vector<column_data> packet_columns;
    int packet_rows;

};


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
        virtual bool run() = 0;
        virtual void on_input_changed() = 0;
        virtual void preview();

        void add_to_scene(QGraphicsScene* scene);
        void set_position(const QPointF& point);

        void send_data();

    //NEEDED FOR TOPS SORT TO KNOW IN DEGREE AND SHOULD IT BE PREVIEWED
        int used_inputs();
        int used_outputs();

        bool warning_cheque(std::function<bool(std::string&)> func);
        virtual packet get_msg();

        virtual ~node();
    protected:
        node(int width,int height,int n_inputs =1,int n_outputs =1);

        QFrame header;
        QFrame body;
        QLabel header_text;

        QPushButton warning_icon;
        QPushButton exit_btn2;

        QPushButton exit_btn;
        QMessageBox error_msg;

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
        void pass_packet(const packet& msg);
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
        void recieve_packet(const packet& msg);

        const table& get_table() const;
        const packet& get_packet() const;

        void add_edge(edge *e) override;
        node* get_output_node();

        ~input_connector();
    private:
        input_connector(node *p);
        edge* input_edge;
        table input_table;
        packet recieved_packet;
    friend class node;
};


class output_connector : public connector {
    public:
        void update_edge_position(const QPointF& new_point) override;
        void disconnect(edge* e) override;

        void send_data(const table& t);
        void send_packet(const packet& msg);

        void force_send();

        void add_edge(edge *e) override;
        std::vector<node *> get_input_nodes();
        int used_outputs() const{
            int s=0;
            for(auto it:output_edges)
                if(it!=nullptr)
                    s++;
            return s;
        }

        ~output_connector();
    private:
        output_connector(node* p);
        std::vector<edge*> output_edges;
    friend class node;
};

void make_siluete(QLabel& lab,double d);

#endif
