#include "../../Includes/gui/node.hpp"

#include<QApplication>
#include<QGraphicsSceneMouseEvent>
#include<QDialog>
#include<QHBoxLayout>
#include<QMouseEvent>
#include<QMimeData>
#include<QDrag>
#include<QPen>
#include<QGraphicsScene>

//INCLUDE IS NEEDED TO ACCESS STATIC CONNECTORS NEEDED BY SCENE
//TO MAKE AN EDGE ON DROP EVENTS.PROBABLY BAD DESIGN BUT WE CAN'T THINK OF A BETTER SOLUTION CURRENTLY
#include"../../Includes/gui/raccoon_scene.hpp"

#include<cmath>

void make_QTable(QTableWidget& qTable,const table &t)
{
    auto colomns = t.col_names();
    qTable.setRowCount(t.row_n());
    qTable.setColumnCount(t.col_n());

    for(unsigned i = 0; i < colomns.size(); i++){
        qTable.setHorizontalHeaderItem(i,new QTableWidgetItem(QString::fromStdString(colomns[i])));
        for(int j = 0; j < t.row_n();j++)
             qTable.setItem(j,i,new QTableWidgetItem(QString::fromStdString(t[colomns[i]][j].get_string())));
        }

    for(int j = 0; j < t.row_n();j++)
        qTable.setVerticalHeaderItem(j,new QTableWidgetItem(QString::fromStdString(std::to_string(j) + " " + t[j].r_name())));
}

//NODE METHOD DEFINITONS

node::node(int width,int height,int n_inputs,int n_outputs){
    dragged = false;
    header.setParent(this);
    body.setParent(this);

    //QT Components initialization
    setGeometry(0,0,width,height);
    header.setGeometry(0,0,width,25);
    body.setGeometry(0,25,width,height-25);

    header_text.setParent(&header);
    header_text.setGeometry(0,0,width,25);
    header_text.setAlignment(Qt::AlignCenter);

    //TODO: ADD A SYSTEM TO BE ABLE TO UPDATE WARNING ICON WHEN SOME TEXT IS READ FROM CERR STREAM
    //OR SOMETHING LIKE THAT SO WHEN AN ALGORITHM HAS A PROBLEM NODE CAN NOTIFY USER BY POPPING WARNING ICON
    //A SYSTEM IS NEEDED TO UPDATE PARENT NODE FROM AN ALGORITHM MAYBE READING FROM CERR STREAM CAN WORK
    warning_icon.setParent(&header);
    warning_icon.setGeometry(width-30,0,25,25);
    warning_icon.setAlignment(Qt::AlignCenter);
    warning_icon.setMaximumHeight(25);
    warning_icon.setMaximumWidth(25);
    QPixmap pix(":/res/warning.svg");
    warning_icon.setPixmap(pix);
    warning_icon.setScaledContents(true);
    warning_icon.setToolTip("Desila se greska brale :(\n ee");
    warning_icon.hide();

    exit_btn.setParent(&header);
    exit_btn.setGeometry(5,5,15,15);
    exit_btn.setMaximumHeight(25);
    exit_btn.setMaximumWidth(25);
    QPixmap pix_exit(":/res/Resources/Action_icons/delete.svg");
    exit_btn.setIcon(QIcon(pix_exit));

    header.setObjectName("header");
    needs_update = true;

    //Output and Input socket initialization
    inputs.resize(n_inputs,nullptr);
    outputs.resize(n_outputs,nullptr);

    for(unsigned i=0;i<inputs.size();i++){
        inputs[i] = new input_connector(this);
        inputs[i]->set_position(find_input_position(i));
    }

    for(unsigned i=0;i<outputs.size();i++){
        outputs[i] = new output_connector(this);
        outputs[i]->set_position(find_output_position(i));
    }
}

//NEEDED TO ADD ALL QGRAPHICSITEMS INSIDE NODE TO SCENE
void node::add_to_scene(QGraphicsScene* scene){
    for(auto it:inputs)
        it->add_to_scene(scene);
    for(auto it:outputs)
        it->add_to_scene(scene);
}

void node::mousePressEvent(QMouseEvent *event){
    if(event->button() & Qt::LeftButton) {
        dragStart = event->pos();
        dragged = true;
    }
}
void node::mouseMoveEvent(QMouseEvent *event){
    if(!(event->buttons() & Qt::LeftButton)) {
        dragged = false;
        return;
    }
    //CALCULATES IF THE DISTANCE FROM THE DRAG START IS ENOUGH TO BE CLASSIFIED AS DRAGGING EVENT
    if((event->pos() - dragStart).manhattanLength() < QApplication::startDragDistance())
        return;
    //WHEN NODE IS DRAGGED UPDATE ITS POSITION AND ALL POSITIONS OF ITS CONNECTORS
    if(dragged) {
        QPointF temp = geometry().topLeft() + event->pos() - dragStart;
        set_position(temp);
    }
}
//NEXT 2 METHODS ARE PRIMARILY USED FOR GRAPH ALGORITHMS IN THE SCENE
std::vector<node *> node::get_input_nodes(){
    std::vector<node*> res;
    for(auto it:inputs)
        res.push_back(it->get_output_node());
    return res;
}
std::vector<node *> node::get_output_nodes(){
    std::vector<node*> res;
    for(auto it:outputs){
        for(auto n:it->get_input_nodes())
            res.push_back(n);
    }
    return res;
}
//UPDATES POSITION OF NODE AND ALL OF ITS CONNECTORS
void node::set_position(const QPointF& point){
    setGeometry(point.x(),point.y(),geometry().width(),geometry().height());
    for(unsigned i=0;i<inputs.size();i++)
        inputs[i]->set_position(find_input_position(i));
    for(unsigned i=0;i<outputs.size();i++)
        outputs[i]->set_position(find_output_position(i));
}
#include<iostream>
void node::send_data()
{
    std::cout<<needs_update<<std::endl;
    if(needs_update == false){
        std::cout<<"saljem"<<std::endl;
        for(auto it:outputs)
            it->send_data(t);
    }
}

int node::used_inputs()
{
    int s=0;
    for(auto it:inputs)
        if(it->get_output_node()!=nullptr)
            s++;
    return s;
}

QPointF node::find_input_position(int index){
    int segment = geometry().height()/(inputs.size() + 1);
    return QPointF(geometry().x() - 15, geometry().y() + segment*(index+1));

}

QPointF node::find_output_position(int index){
    int segment = geometry().height()/(outputs.size() + 1);
    return QPointF(geometry().topRight().x()+1, geometry().y() + segment*(index+1));
}
//GENERIC PREVIEW FUNCTION THAT MOST NODES USE.IT CONVERTS INTERNAL Table TO QTableWidget AND PRESENTS IT IN A DIALOG WINDOW
void node::preview(){
    QDialog tablePreview;
    tablePreview.setModal(true);
    QHBoxLayout hBox;
    tablePreview.setLayout(&hBox);
    tablePreview.setGeometry(500,500,500,500);
    QTableWidget qTable{};
    hBox.addWidget(&qTable);
    make_QTable(qTable,t);
    tablePreview.exec();
}

node::~node(){

}


//EDGE METHOD DEFINITIONS

edge::edge(input_connector* input,output_connector *output):in(input),out(output){
    in->add_edge(this);
    out->add_edge(this);

    setLine(in->get_position().x(),in->get_position().y()+10,
                out->get_position().x(),out->get_position().y()+10);
    
    QPen pen{QColor(20,20,20)};
    pen.setWidth(5);
    pen.setCapStyle(Qt::PenCapStyle::RoundCap);
    pen.setStyle(Qt::DashDotLine);

    arrow.setPen(pen);
    arrow.setZValue(-0.5);
    setZValue(-0.5);

    setPen(pen);
    update_arrow();

    out->force_send();
}
void edge::mousePressEvent(QGraphicsSceneMouseEvent *event){
//TODO
    Q_UNUSED(event);
}
//UPDATES ONE BEGINING AND END POINTS OF THE LINE
void edge::set_begin_pos(const QPointF& new_point){
    setLine(new_point.x(),new_point.y()+10,line().p2().x(),line().p2().y());
    update_arrow();
}
void edge::set_end_pos(const QPointF& new_point){
    setLine(line().p1().x(),line().p1().y(),new_point.x(),new_point.y()+10);
    update_arrow();
}
//USED TO PASS THE DATA BETWEEN 2 CONNECTORS
void edge::pass_data(const table& t){
    in->recieve_data(t);
}

void edge::add_to_scene(QGraphicsScene *scene)
{
    scene->addItem(this);
    scene->addItem(&arrow);
}

node *edge::get_input_node()
{
    return out->get_parent();
}

node *edge::get_output_node()
{
    return in->get_parent();
}

//CALCULATES NORMAL VECTOR OF THE LINE THEN NORMALIZES IT AND SCALES IT AND CREATES AN ARROW FROM IT
void edge::update_arrow(){
    QPolygonF pol;
    QPointF begin{line().p1().x(),line().p1().y()};
    QPointF end{line().p2().x(),line().p2().y()};

    QPointF vec=begin-end;
    QPointF vec_n{-vec.y(),vec.x()};

    qreal len= std::sqrt(QPointF::dotProduct(vec,vec));
    qreal scale=20/len;

    qreal len_n= std::sqrt(QPointF::dotProduct(vec_n,vec_n));
    qreal scale_n=10/len_n;

    QPointF s=begin-vec*scale;

    QPointF l=s+vec_n*scale_n;
    QPointF d=s-vec_n*scale_n;
    pol.push_back(begin);
    pol.push_back(l);
    pol.push_back(d);
    arrow.setPolygon(pol);
}
edge::~edge(){
    in->disconnect(this);
    out->disconnect(this);
}

//CONNECTOR METHOD DEFINITIONS

connector::connector(node* p):QGraphicsEllipseItem(0,0,15,15),parent(p){
    setAcceptHoverEvents(true);
    setAcceptDrops(true);
    text.setText("");
}

void connector::add_to_scene(QGraphicsScene* scene){
    scene->addItem(this);
    scene->addItem(&text);
}
//WHEN MOUSE IS PRESSED ON CONNECTOR IT UPDATES GLOBAL VARIABLES OF SCENE TO BE HIM AND STARTS A DRAG EVENT WITH TAGS 'LINE'
//TO INDICATE THAT A LINE SHOULD BE DRAWN BETWEEN HIM AND MOUSE POSITION
void connector::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(type==OUTPUT)
        raccoon_scene::selected_output=this;
    if(type==INPUT)
        raccoon_scene::selected_input=this;
    QDrag *drag = new QDrag(event->widget());
    QMimeData *mime = new QMimeData;
    mime->setText("line");
    drag->setMimeData(mime);
    drag->exec();
}
//UPDATES SCENES GLOBAL INPUT CONNECTOR TO BE THIS SO AN EDGE CAN BE CREATED
void connector::dropEvent(QGraphicsSceneDragDropEvent *event){
    Q_UNUSED(event);
    if(type==INPUT && raccoon_scene::selected_output!=nullptr)
        raccoon_scene::selected_input=this;
    if(type==OUTPUT && raccoon_scene::selected_input!=nullptr)
        raccoon_scene::selected_output=this;
}
void connector::set_text(std::string txt){
     text.setText(QString::fromStdString(txt));
}
//WHEN POSITION OF A CONNECTOR IS UPDATED LINES THAT ARE CONNECTED TO HIM SHOULD ALSO UPDATE ONE OF THEIR POINTS
void connector::set_position(const QPointF& new_point){
    setPos(new_point);
    text.setPos(new_point.x()+2,new_point.y()-20);
    update_edge_position(new_point);
}
QPointF connector::get_position() const{
    return QPointF(pos().x(),pos().y());
}

node* connector::get_parent(){
    return parent;
}

connector::~connector(){

}


//INPUT CONNECTOR EVENTS
input_connector::input_connector(node* p):connector(p){
    QPen pen(QColor(30,30,30));
    pen.setWidth(3);
    setPen(pen);
    setBrush(QBrush(QColor(110,150,110)));
    input_edge=nullptr;
    type=INPUT;
}
void input_connector::add_edge(edge *e){
    if(input_edge!=nullptr)
        delete  input_edge;
    input_edge=e;
}

node* input_connector::get_output_node()
{
    return input_edge->get_output_node();
}
void input_connector::update_edge_position(const QPointF& new_point){
    if(input_edge!=nullptr)
        input_edge->set_begin_pos(new_point);
}
void input_connector::disconnect(edge* e){
    if(input_edge==e)
    input_edge=nullptr;
}
//WHEN DATA IS RECIEVED TROUGH INPUT CONNECTOR PARENT NODE IS NOTIFIED SO IT CAN REACT
void input_connector::recieve_data(const table& t){
    input_table=t;
    parent->on_input_changed();
}
const table& input_connector::get_table() const{
    return input_table;
}

void output_connector::force_send()
{
    parent->send_data();
}

input_connector::~input_connector(){

}

//OUTPUT CONECTOR METHOD DEFINITIONS
output_connector::output_connector(node* p):connector(p){
    QPen pen(QColor(30,30,30));
    pen.setWidth(3);
    setPen(pen);
    setBrush(QBrush(QColor(11, 126, 184)));
    type=OUTPUT;

    text.setBrush(QBrush(QColor(11, 126, 184)));
}
void output_connector::add_edge(edge *e){
    output_edges.push_back(e);
}

std::vector<node *> output_connector::get_input_nodes()
{
    std::vector<node*> res;
    for(auto it:output_edges)
        res.push_back(it->get_input_node());
    return res;
}
void output_connector::update_edge_position(const QPointF& new_point){
    for(auto& it:output_edges)
        if(it!=nullptr)
            it->set_end_pos(new_point);
}
void output_connector::disconnect(edge* e){
    output_edges.erase(std::find(output_edges.begin(),
                                 output_edges.end(),e));
}
void output_connector::send_data(const table& t){
    for(auto it:output_edges)
        if(it!=nullptr)
            it->pass_data(t);
}
output_connector::~output_connector(){

}
