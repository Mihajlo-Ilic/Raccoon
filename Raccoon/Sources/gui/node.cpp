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
#include<QErrorMessage>
#include<iostream>

//INCLUDE IS NEEDED TO ACCESS STATIC CONNECTORS NEEDED BY SCENE
//TO MAKE AN EDGE ON DROP EVENTS.PROBABLY BAD DESIGN BUT WE CAN'T THINK OF A BETTER SOLUTION CURRENTLY
#include"../../Includes/gui/raccoon_scene.hpp"

#include<cmath>

extern std::vector<node*> scene_nodes;


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
    warning_icon.setMaximumHeight(25);
    warning_icon.setMaximumWidth(25);
    warning_icon.setStyleSheet("QLabel{background-color:rgba(0,0,0,0);}");
    QPixmap img(":/res/Resources/Action_icons/warning.svg");
    warning_icon.setIcon(img);
    warning_icon.setToolTip("Desila se greska brale :(\n ee");
    warning_icon.setAttribute(Qt::WA_Hover);
    warning_icon.hide();

    error_msg.setMaximumSize(300,300);

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
        inputs[i] = new input_connector(this,i);
        inputs[i]->set_position(find_input_position(i));
    }

    for(unsigned i=0;i<outputs.size();i++){
        outputs[i] = new output_connector(this,i);
        outputs[i]->set_position(find_output_position(i));
    }

    connect(&exit_btn, &QPushButton::clicked, [&]() {
        delete this;
    });
}

bool node::warning_cheque(std::function<bool(std::string&)> func) {
    std::string error_text;
    if (func(error_text)) {
        std::string str = error_text;
        warning_icon.show();
        connect(&warning_icon,(&QPushButton::clicked),[&,str](){
            error_msg.setText(QString::fromStdString(str));
            error_msg.exec();
        });
        return true;
    }
    warning_icon.hide();
    return false;
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
    for(auto it:inputs){
        auto p = it->get_output_node();
        if(p!=nullptr)
        res.push_back(p);
    }
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

std::vector<edge *> node::get_input_edges()
{
    std::vector<edge*> ret;
    for(auto it:inputs)
        if(it->input_edge!=nullptr)
            ret.push_back(it->input_edge);
    return ret;
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
        for(auto it:outputs)
            it->send_packet(get_msg());
}

int node::used_inputs()
{
    int s=0;
    for(auto it:inputs)
        if(it->get_output_node()!=nullptr)
            s++;
    return s;
}

int node::used_outputs()
{
    int s=0;
    for(auto it:outputs)
        s+=it->used_outputs();
    return s;
}

packet node::get_msg()
{
    if(inputs.size()>1)
        return inputs[0]->get_packet();
    else return packet();
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


    auto it=std::find(scene_nodes.begin(),scene_nodes.end(),this);
    if(it!=scene_nodes.end()){
        scene_nodes.erase(it);
        packet p;
        for(auto i:outputs)
            i->send_packet(p);
    }
    for(auto i:inputs)
        delete i;
    for(auto i:outputs)
        delete i;
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

void edge::pass_packet(const packet &msg)
{
    in->recieve_packet(msg);
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

std::pair<int, int> edge::get_indexes() const
{
    return std::make_pair(in->get_index(),out->get_index());
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

int connector::get_index() const
{
    return index;
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
input_connector::input_connector(node* p,int ind):connector(p){
    index = ind;
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
    if(input_edge!=nullptr)
    return input_edge->get_output_node();
    return nullptr;
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
}

void input_connector::recieve_packet(const packet &msg)
{
    if(recieved_packet!=msg){
        recieved_packet=msg;
        parent->on_input_changed();
    }
    recieved_packet=msg;
}
const table& input_connector::get_table() const{
    return input_table;
}

const packet &input_connector::get_packet() const
{
    return recieved_packet;
}

void output_connector::force_send()
{
    parent->send_data();
}

input_connector::~input_connector(){
    delete input_edge;
}

//OUTPUT CONECTOR METHOD DEFINITIONS
output_connector::output_connector(node* p,int ind):connector(p){
    index=ind;
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
        res.push_back(it->get_output_node());
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

void output_connector::send_packet(const packet &msg)
{
    for(auto it:output_edges)
        if(it!=nullptr)
            it->pass_packet(msg);
}
output_connector::~output_connector(){
    for(auto i:output_edges)
        delete i;
}

packet::packet(){
    packet_rows = 0;
}

packet::packet(const table &t){
    auto vec = t.col_names();
    for(const auto& it:vec)
        add_column(t[it],it);
    packet_rows = t.row_n();
}

bool packet::operator!=(const packet &rhs) const{
    if(packet_columns.size()!=rhs.packet_columns.size())
        return true;
    for(unsigned i=0;i<packet_columns.size();i++){
        if(packet_columns[i]!=rhs.packet_columns[i])
            return true;
    }
    return false;
}

bool packet::operator==(const packet &rhs) const{
    if(packet_columns.size()==rhs.packet_columns.size())
        return true;
    for(unsigned i=0;i<packet_columns.size();i++){
        if(packet_columns[i]==rhs.packet_columns[i])
            return true;
    }
    return false;
}

void packet::add_column(const collumn &col, std::string name){
    column_data new_data;
    new_data.name=name;
    new_data.role=col.role;
    new_data.type=col.type;
    packet_columns.push_back(new_data);
}

void packet::add_column(std::string name, column_role r, column_type t){
    column_data cd;
    cd.name=name;
    cd.role=r;
    cd.type=t;
    packet_columns.push_back(cd);
}

void packet::remove_column(std::string item){
    for(unsigned i=0;i<packet_columns.size();i++){
        if(packet_columns[i].name==item){
            packet_columns.erase(packet_columns.begin()+i);
            return;
        }
    }
}

bool column_data::operator!=(const column_data &rhs) const{
    return name!=rhs.name && role!=rhs.role && type!=rhs.type;
}

bool column_data::operator==(const column_data &rhs) const{
    return name==rhs.name && role==rhs.role && type==rhs.type;
}

void make_siluete(QLabel &lab, double d)
{
    QString left = "stop:"+QString::number(d-0.01) +" rgba(0, 0, 0, 0),";
    QString center = "stop:"+QString::number(d) +" rgba(0, 0, 170, 255),";
    QString right = "stop:"+QString::number(d+0.01) +" rgba(0, 0, 0, 0),";
    lab.setStyleSheet("background-color: qlineargradient(spread:repeat, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 0, 0, 255),"+left+center+right+ "stop:0.5 rgba(255, 183, 0, 255), stop:1 rgba(0, 255, 126, 255));");
}
