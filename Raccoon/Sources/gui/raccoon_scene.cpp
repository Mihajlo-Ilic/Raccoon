#include "../../Includes/gui/raccoon_scene.hpp"
#include<iostream>
#include<QGraphicsSceneDragDropEvent>
#include<QMimeData>
#include<queue>
#include<set>
#include<QMessageBox>
#include<fstream>

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
#include "../../Includes/gui/doc_reader_node.hpp"
#include "../../Includes/gui/tf_idf_node.hpp"
#include "../../Includes/gui/apriori_node.hpp"

//instance for singleton pattern
raccoon_scene *raccoon_scene::instance = nullptr;
//used to communicate with node.cpp for creating edges between 2 selected connectors kinda ugly :(
connector* raccoon_scene::selected_input = nullptr;
connector* raccoon_scene::selected_output = nullptr;

std::vector<node*> scene_nodes;



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
    if(event->mimeData()->text()=="table_output_button"){
        outputTable_node* n=new outputTable_node(250,100);
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
    if(event->mimeData()->text()=="doc_button"){
        doc_reader_node* n=new doc_reader_node(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    if(event->mimeData()->text()=="tf_idf_button"){
        tf_idf_node* n=new tf_idf_node(250,250);
        addWidget(n);
        n->add_to_scene(this);
        n->set_position(event->scenePos());
        scene_nodes.push_back(n);
    }
    if(event->mimeData()->text()=="apriori_button"){
        apriori_node* n=new apriori_node(250,250);
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
            bool stop = curr->run();
            // IF WE HAVE AN
            if(stop == false) {
                QMessageBox box;
                box.setText("You have error in your nodes!!!\n");
                box.exec();
                return;
            }
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


void raccoon_scene::save_scene(const std::string &path)
{
    std::ofstream file;
    file.open (path+".rqn");
    for(auto it:scene_nodes){
        it->serialize(file);
        file<<std::endl;
    }
    for(unsigned i=0;i<scene_nodes.size();i++){
        auto input_edges = scene_nodes[i]->get_input_edges();
        for(auto it:input_edges){
            int pos = std::find(scene_nodes.begin(),scene_nodes.end(),it->get_input_node())-scene_nodes.begin();
            auto pair = it->get_indexes();
            file<<"-e "<<pos<<" "<<pair.second<<" "<<i<<" "<<pair.first<<std::endl;
        }
    }
    file.close();
}

void raccoon_scene::load_scene(const std::string &path)
{
    for(auto it:scene_nodes)
        delete it;
    scene_nodes.clear();

    std::ifstream file;
    file.open (path);
    std::string line;
    char putanja[255];

    while(std::getline(file,line)){

       if(line.find("-n csv")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);
           std::getline(file,line);
           sscanf(line.c_str()," text=%s",putanja);

            csv_node *n=new csv_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);

            std::string s = putanja;
            if(s!=""){
                //TODO ADD PATH
            }
       }
        else
       if(line.find("-n aglo")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);
           std::getline(file,line);
           sscanf(line.c_str()," metric=%s",putanja);
           std::getline(file,line);
           sscanf(line.c_str()," cluster_metric=%s",putanja);
           std::getline(file,line);
           sscanf(line.c_str()," dist=%s",putanja);
           std::getline(file,line);
           sscanf(line.c_str()," num=%s",putanja);

            aglo_node *n=new aglo_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       }
        else
       if(line.find("-n aprox")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);
           std::getline(file,line);
           sscanf(line.c_str()," method=%s",putanja);
           std::getline(file,line);
           sscanf(line.c_str()," value=%s",putanja);

            aproximation_node *n=new aproximation_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       } else
       if(line.find("-n bin")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);
           std::getline(file,line);
           sscanf(line.c_str()," shuffle=%s",putanja);
           std::getline(file,line);
           sscanf(line.c_str()," method=%s",putanja);

            binning_node *n=new binning_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       } else
       if(line.find("-n cat_to_bin")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);

            categoricalToBinnary_node *n=new categoricalToBinnary_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       } else
       if(line.find("-n dbscan")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);
           std::getline(file,line);
           sscanf(line.c_str()," neighbour=%s",putanja);
           std::getline(file,line);
           sscanf(line.c_str()," metric=%s",putanja);

            dbscan_node *n=new dbscan_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       } else
       if(line.find("-n dec_tree")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);
           std::getline(file,line);
           sscanf(line.c_str()," min_row=%s",putanja);
           std::getline(file,line);
           sscanf(line.c_str()," max_depth=%s",putanja);
           std::getline(file,line);
           sscanf(line.c_str()," min_clean=%s",putanja);
           std::getline(file,line);
           sscanf(line.c_str()," metric=%s",putanja);

            decision_tree_node *n=new decision_tree_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       } else
       if(line.find("-n remove_na")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);

            delete_na *n=new delete_na(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       } else
       if(line.find("-n doc_reader")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);
           std::getline(file,line);
           sscanf(line.c_str()," min_freq=%s",putanja);
           std::getline(file,line);
           sscanf(line.c_str()," max_freq=%s",putanja);
           std::getline(file,line);
           sscanf(line.c_str()," binary=%s",putanja);
           std::getline(file,line);
           sscanf(line.c_str()," class=%s",putanja);

            doc_reader_node *n=new doc_reader_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       } else
       if(line.find("-n filter")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);

            filter_node *n=new filter_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       } else
       if(line.find("-n k_mean")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);
           std::getline(file,line);
           sscanf(line.c_str()," distance=%s",putanja);
           std::getline(file,line);
           sscanf(line.c_str()," k=%s",putanja);

            k_mean_cluster_node *n=new k_mean_cluster_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       } else
       if(line.find("-n knn")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);
           std::getline(file,line);
           sscanf(line.c_str()," metric=%s",putanja);
           std::getline(file,line);
           sscanf(line.c_str()," k=%s",putanja);

            knn_node *n=new knn_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       } else
       if(line.find("-n bayes")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);
           std::getline(file,line);
           sscanf(line.c_str()," alpha=%s",putanja);
           std::getline(file,line);
           sscanf(line.c_str()," show_table=%s",putanja);

            nb_node *n=new nb_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       } else
       if(line.find("-n normalization")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);


            normalization_node *n=new normalization_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       } else
       if(line.find("-n output_node")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);


            outputTable_node *n=new outputTable_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       } else
       if(line.find("-n partition")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);
           std::getline(file,line);
           sscanf(line.c_str()," percent=%s",putanja);
           std::getline(file,line);
           sscanf(line.c_str()," random=%s",putanja);


            partition_node *n=new partition_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       } else
       if(line.find("-n plot")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);
           std::getline(file,line);
           sscanf(line.c_str()," target=%s",putanja);

            plot_node *n=new plot_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       } else
       if(line.find("-n standardize")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);

            standardization_node *n=new standardization_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       } else
       if(line.find("-n stats")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);

            stats_node *n=new stats_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       } else
       if(line.find("-n tf_idf")==0){

           float x;
           float y;
           std::getline(file,line);
           sscanf(line.c_str()," x=%f",&x);
           std::getline(file,line);
           sscanf(line.c_str()," y=%f",&y);

            tf_idf_node *n=new tf_idf_node(250,250);
            n->set_position(QPointF(x,y));
            scene_nodes.push_back(n);
            n->add_to_scene(this);
            addWidget(n);
       } else
        if(line.find("-e")==0){
            int node_out;
            int out_ind;
            int node_in;
            int in_ind;
            sscanf(line.c_str(),"-e %d %d %d %d",&node_out,&out_ind,&node_in,&in_ind);

            edge* e=new edge(scene_nodes[node_in]->get_input_con(in_ind),scene_nodes[node_out]->get_output_con(out_ind));
            e->add_to_scene(this);
        }
    }
    file.close();
}

//TODO BETTER DESTRUCTOR NOT SURE IF WORKS CURRENTLY. PLACEHOLDER
raccoon_scene::~raccoon_scene(){
    for(auto it:scene_nodes)
        delete it;
}
