#include "../../Includes/gui/raccoon_scene.hpp"
#include<iostream>
#include<QGraphicsSceneDragDropEvent>
#include<QMimeData>
#include<queue>
#include<set>

#include"../../Includes/gui/csv_node.hpp"
#include"../../Includes/gui/aproximation_node.hpp"
#include"../../Includes/gui/binning_node.hpp"
#include"../../Includes/gui/categoricaltobinnary_node.hpp"
#include"../../Includes/gui/delete_na.hpp"
#include"../../Includes/gui/filter_node.hpp"
#include"../../Includes/gui/knn_node.hpp"
#include"../../Includes/gui/normalization_node.hpp"
#include"../../Includes/gui/partition_node.hpp"
#include"../../Includes/gui/standardization_node.hpp"
#include"../../Includes/gui/naive_bayes_node.hpp"
#include"../../Includes/gui/outputtable_node.hpp"
#include"../../Includes/gui/stats_node.hpp"
#include"../../Includes/gui/k_mean_cluster_node.hpp"
#include"../../Includes/gui/decision_tree_node.hpp"
#include"../../Includes/gui/dbscan_node.hpp"
#include"../../Includes/gui/aglo_node.hpp"
#include"../../Includes/gui/plot_node.hpp"

//instance for singleton pattern
raccoon_scene *raccoon_scene::instance = nullptr;
//used to communicate with node.cpp for creating edges between 2 selected connectors kinda ugly :(
connector* raccoon_scene::selected_input = nullptr;
connector* raccoon_scene::selected_output = nullptr;

raccoon_scene::raccoon_scene(int width,int height):QGraphicsScene(){
    selected_input = nullptr;
    selected_output = nullptr;
    addItem(&connector_line);
    QPen pen{QColor(20,20,20)};
    pen.setWidth(5);
    pen.setCapStyle(Qt::PenCapStyle::RoundCap);
    pen.setStyle(Qt::DashDotLine);

    connector_line.setPen(pen);
    setSceneRect(QRect(0,0,width,height));
}
//IF DRAG N DROP WITH TAG LINE IS ACTIVE THEN UPDATE ONE END OF THE LINE TO PRESENT WHAT CONNECTORS ARE BEING PAIRED
void raccoon_scene::dragMoveEvent(QGraphicsSceneDragDropEvent *event){

    if(event->mimeData()->text()=="line"){
        connector_line.setLine(connector_line.line().p1().x(),connector_line.line().p1().y()
                               ,event->scenePos().x(),event->scenePos().y());
        QGraphicsScene::dragMoveEvent(event);
    } else event->accept();
}
void raccoon_scene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event){
    QGraphicsScene::dragLeaveEvent(event);
}
void raccoon_scene::dragEnterEvent(QGraphicsSceneDragDropEvent *event){

    if(event->mimeData()->text()=="line"){
        connector_line.setLine(event->scenePos().x(),event->scenePos().y()
                               ,event->scenePos().x(),event->scenePos().y());
        QGraphicsScene::dragEnterEvent(event);
    } else event->accept();

}
//LONG AND UGLY FUNCTION TLDR; DEPENDING ON THE TAG OF DRAG N DROP EVENT CREATES APROPRIATE NODE :)
void raccoon_scene::dropEvent(QGraphicsSceneDragDropEvent *event){
    connector_line.setLine(0,0,0,0);
    if(event->mimeData()->text()=="csv_button"){
        csv_node* n=new csv_node(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    if(event->mimeData()->text()=="approximate_button"){
        aproximation_node* n=new aproximation_node(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    if(event->mimeData()->text()=="to_numeric_button"){
        categoricalToBinnary_node* n=new categoricalToBinnary_node(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    if(event->mimeData()->text()=="remove_na_button"){
        delete_na* n=new delete_na(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    if(event->mimeData()->text()=="filter_button"){
        filter_node* n=new filter_node(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    if(event->mimeData()->text()=="knn_button"){
        knn_node* n=new knn_node(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    if(event->mimeData()->text()=="normalization_button"){
        normalization_node* n=new normalization_node(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    if(event->mimeData()->text()=="partition_button"){
        partition_node* n=new partition_node(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    if(event->mimeData()->text()=="standardize_button"){
        standardization_node* n=new standardization_node(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    if(event->mimeData()->text()=="binning_button"){
        binning_node* n=new binning_node(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    if(event->mimeData()->text()=="bayes_button"){
        nb_node* n=new nb_node(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    if(event->mimeData()->text()=="plot_button"){
        plot_node* n=new plot_node(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    if(event->mimeData()->text()=="stats_button"){
        stats_node* n=new stats_node(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    if(event->mimeData()->text()=="k_means_button"){
        k_mean_cluster_node* n=new k_mean_cluster_node(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    if(event->mimeData()->text()=="dec_tree_button"){
        decision_tree_node* n=new decision_tree_node(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    if(event->mimeData()->text()=="dbscan_button"){
        dbscan_node* n=new dbscan_node(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    if(event->mimeData()->text()=="aglo_button"){
        aglo_node* n=new aglo_node(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    QGraphicsScene::dropEvent(event);
    if(selected_input && selected_output){
        edge * e = new edge((input_connector*)selected_input,
                            (output_connector*)selected_output);
        if(has_cycle(scene_nodes)==false)
            e->add_to_scene(this);
        else
            delete e;
    }

    selected_input = nullptr;
    selected_output = nullptr;
}
//SINGLETON PATTERN IF OBJECT OF CLASS R_SCENE EXISTS DONT CREATE ANOTHER
raccoon_scene* raccoon_scene::get_instance(int width,int height){
    if(instance==nullptr){
        instance=new raccoon_scene(width,height);
        return instance;
    }
    return nullptr;
}
#include<iostream>
//TOP SORT FOR RUNNING THE SCENE
void raccoon_scene::run_graph(){
    std::map<node*, int> helper;
        std::for_each(scene_nodes.begin(), scene_nodes.end(), [&helper](auto it) {
            helper.insert(std::make_pair(it,it->used_inputs()));
        });
        std::queue<node*> q;
        for(auto &it:helper) {
            if(it.second == 0) {
                q.push(it.first);
            }
        }
        while(!q.empty()) {
            auto curr = q.front();
            q.pop();
            curr->run();
            if(curr->used_outputs() ==0)
                curr->preview();
            for(auto& it:curr->get_output_nodes()) {
                helper[it]--;
                if(helper[it] == 0)
                    q.push(it);
            }
        }
}
//USED TO IDENTIFY IF THERE IS A CYCLE IN GRAPH.IF THERE IS WE DON'T ALLOW THAT BECAUSE GRAPH NEEDS TO STAY ACYCLIC

bool findCycle(std::set<node*> parents,node * n) {
    parents.insert(n);
    for(auto out:n->get_output_nodes()){
        if(parents.find(out)!=parents.end())
            return true;
        else {
            bool b=findCycle(parents,out);
            if(b== true)
                return true;
        }
    }
    return false;
}


bool raccoon_scene::has_cycle(std::vector<node*> graph){
    std::set<node*> visited;
        for(auto it: graph) {
            if(visited.find(it)==visited.end())
            {
                bool b = findCycle(visited,it);
                if(b == true)
                    return true;
            }
        }
        return false;
}
//TODO BETTER DESTRUCTOR NOT SURE IF WORKS CURRENTLY. PLACEHOLDER
raccoon_scene::~raccoon_scene(){
    for(auto it:scene_nodes)
        delete it;
}
