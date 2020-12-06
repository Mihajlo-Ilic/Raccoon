#include"../../Includes/gui/aglo_node.hpp"
#include"../../Includes/stats.hpp"

#include<QDialog>
#include<QHBoxLayout>
#include<QFormLayout>
#include<QColorDialog>
#include<QComboBox>
#include<QCheckBox>
#include<unordered_set>

aglo_node::aglo_node(int width, int height):node(width,height,1)
{
    header_text.setText("AGLO CLUSTERING");

    combo_func.setParent(&body);
    label_func.setParent(&body);
    combo_cfunc.setParent(&body);
    label_cfunc.setParent(&body);
    spin_dist.setParent(&body);
    label_dist.setParent(&body);
    spin_num.setParent(&body);
    label_num.setParent(&body);
    preview_button.setParent(&body);


    label_func.setText("instance metric:");
    label_cfunc.setText("cluster metric:");
    label_dist.setText("maximum dist:");
    label_num.setText("number of clusters:");
    preview_button.setText("preview");

    spin_dist.setMinimum(0);
    spin_num.setMinimum(1);

    combo_cfunc.addItem("Best link");
    combo_cfunc.addItem("Worst link");
    combo_cfunc.addItem("Average link");

    combo_func.addItem("Euclidean");
    combo_func.addItem("Manhattan");
    combo_func.addItem("Smc");
    combo_func.addItem("Zakard");
    combo_func.addItem("Hamming");

    label_cfunc.setGeometry(5,10,110,30);
    label_func.setGeometry(5,50,110,30);
    label_dist.setGeometry(5,90,110,30);
    label_num.setGeometry(5,130,110,30);

    combo_cfunc.setGeometry(120,10,90,30);
    combo_func.setGeometry(120,50,90,30);
    spin_dist.setGeometry(120,90,90,30);
    spin_num.setGeometry(120,130,90,30);

    preview_button.setGeometry(geometry().x() + 190,geometry().y() + 200,50,20);

    connect(&preview_button, SIGNAL(clicked()), this, SLOT(preview_b()));
    connect(&combo_cfunc,SIGNAL(currentTextChanged(QString )),this,SLOT(combo_clust_changed( QString )));
    connect(&combo_func,SIGNAL(currentTextChanged(QString )),this,SLOT(combo_changed( QString )));
    connect(&spin_dist,SIGNAL(valueChanged(double)),this,SLOT(dist_changed(double)));
    connect(&spin_num,SIGNAL(valueChanged(int)),this,SLOT(num_changed(int)));
    connect(&tab_gscene,SIGNAL(changed(const QList<QRectF>&)),this,SLOT(scene_changed(const QList<QRectF>&)));

}

void aglo_node::on_input_changed()
{
    auto p = inputs[0]->get_packet();
    p.add_column("cluster",column_role::INPUT_COLUMN,column_type::CONTINUOUS);
    outputs[0]->send_packet(p);
}

void aglo_node::run()
{
    table c=inputs[0]->get_table();
    c.pop("partition");
    c.pop(c.get_target());
    c.pop("cluster");
    t = model.predict(c);
    outputs[0]->send_data(c);
}


void aglo_node::preview_b()
{
    t.pop("partition");
    t.pop("cluster");
    t.pop(t.get_target());
    t=model.predict(t);
    preview();
}

void dent_sort(std::vector<int> &res,const std::vector<std::pair<std::pair<int, int>, double>> & arr,int index,std::unordered_set<int>& used){
    if(index<0)
        return;
    //levo
    if(used.find(arr[index].first.first) == used.end())
    for(int j=index-1;j>=0;j--){
        if(arr[j].first.second == arr[index].first.first)
            dent_sort(res,arr,j,used);
    }

    if(used.find(arr[index].first.first) == used.end()){
        res.push_back(arr[index].first.first);
        used.insert(arr[index].first.first);
    }

    //desno
    if(used.find(arr[index].first.second) == used.end())
    for(int j=index-1;j>=0;j--){
        if(arr[j].first.second == arr[index].first.second)
            dent_sort(res,arr,j,used);
    }



    if(used.find(arr[index].first.second) == used.end()){
        res.push_back(arr[index].first.second);
        used.insert(arr[index].first.second);
    }

}

void aglo_node::make_dendogram(QGraphicsScene &scene){
    QPen pen{QColor{255,255,255}};
    pen.setWidth(2);

    x_axis.setLine(30,500,500,500);
    x_axis.setPen(pen);

    y_axis.setLine(30,30,30,500);
    y_axis.setPen(pen);

    scene.addItem(&x_axis);
    scene.addItem(&y_axis);
    y_axis.setZValue(-0.5);

    int y=510;
    std::unordered_map<int,QPointF> cluster_centers;

    auto clusters = model.get_dendogram();
    std::unordered_set<int> used;
    int i=1;
    double max_y =-1;
    std::vector<int> sorted_dend;
    std::unordered_set<int> help;
    for(int j=clusters.size()-1;j>=0;j--)
    dent_sort(sorted_dend,clusters,j,help);

    for(auto it:sorted_dend){
        //std::cout<<it<<" ";
        QGraphicsSimpleTextItem *tx=new QGraphicsSimpleTextItem();
        tx->setText(QString::fromStdString(std::to_string(it)));
        tx->setPos(40+i*40,y);
        cluster_centers[it] = QPointF(40+i*40,500);
        x_texts.push_back(tx);
        tx->setBrush(pen.brush());
        scene.addItem(tx);
        i++;
    }

    for(const auto& it:clusters){
        max_y = std::max(max_y,it.second);
    }

    max_y = std::max(max_y,0.0);

    for(const auto& it:clusters){
        //std::cout<<it.first.first<<" "<<it.first.second<<std::endl;
        QVector<QPointF> rect;
        QPointF p1 = cluster_centers[it.first.first];
        QPointF p2 = cluster_centers[it.first.second];
        double y = it.second/max_y;
        y *= 500.0;
        rect.push_back(p1);
        rect.push_back(QPointF(p1.x(),500.0-y));
        rect.push_back(QPointF(p2.x(),500.0-y));
        rect.push_back(p2);

        cluster_centers[it.first.second] = QPointF((p1.x()+p2.x())/2.0,500.0-y);
        cluster_centers[it.first.first] = QPointF((p1.x()+p2.x())/2.0,500.0-y);
        QPainterPath path{rect.front()};

        for(int i=0;i<rect.size();i++)
            path.lineTo(rect[i]);

        QGraphicsSimpleTextItem *tx=new QGraphicsSimpleTextItem();
        tx->setText(QString::number(it.second,'g',2));
        tx->setPos(-5,500.0-y);
        y_texts.push_back(tx);
        scene.addItem(tx);
        tx->setBrush(pen.brush());

        QGraphicsPathItem *gp=new QGraphicsPathItem();
        QPen cl_pen{cluster_colors[t["cluster"][it.first.second].get_string()]};
        cl_pen.setWidth(2);
        gp->setPen(cl_pen);
        gp->setPath(path);
        dend_rects.push_back(gp);
        gp->setZValue(-1);

        scene.addItem(gp);
    }

}

void aglo_node::preview(){

    for(auto it:dend_rects)
        tab_gscene.removeItem(it);
    for(auto it:x_texts)
        tab_gscene.removeItem(it);
    for(auto it:y_texts)
        tab_gscene.removeItem(it);
    tab_gscene.removeItem(&x_axis);
    tab_gscene.removeItem(&y_axis);

    dend_rects.clear();
    x_texts.clear();
    y_texts.clear();

    QDialog *tablePreview=new QDialog();
    tablePreview->setGeometry(400,400,500,500);
    tablePreview->setModal(true);

    QHBoxLayout *dialog_layout=new QHBoxLayout();
    tablePreview->setLayout(dialog_layout);

    QTabWidget *tabs=new QTabWidget;
    dialog_layout->addWidget(tabs);

    QFrame *dend_frame=new QFrame();
    QHBoxLayout *dend_layout=new QHBoxLayout();
    dend_frame->setLayout(dend_layout);

    QTableWidget data_table;
    make_QTable(data_table,t);


    QFrame *tool_frame=new QFrame();
    QFormLayout *tool_layout=new QFormLayout();
    tool_frame->setLayout(tool_layout);
    tool_layout->setLabelAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    tool_frame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);

    //horizontal scale
    QSpinBox x_seg_box;
    x_seg_box.setMinimum(1);
    x_seg_box.setMaximum(500);
    x_seg_box.setValue(100);
    x_seg_box.setSuffix("%");
    tool_layout->addRow("x axis scale:",&x_seg_box);
    x_seg_box.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    //vertical seg
    QSpinBox y_seg_box;
    y_seg_box.setMinimum(1);
    y_seg_box.setMaximum(500);
    y_seg_box.setValue(100);
    y_seg_box.setSuffix("%");
    tool_layout->addRow("y axis scale:",&y_seg_box);
    y_seg_box.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    //line width
    QSpinBox line_width_box;
    line_width_box.setMinimum(1);
    line_width_box.setValue(2);
    tool_layout->addRow("line width:",&line_width_box);
    line_width_box.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    //text size
    QSpinBox text_size_box;
    text_size_box.setMinimum(1);
    text_size_box.setMaximum(500);
    text_size_box.setValue(100);
    text_size_box.setSuffix("%");
    tool_layout->addRow("text size:",&text_size_box);
    text_size_box.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    //scene color
    QPushButton scene_color_combo;
    scene_color_combo.setStyleSheet("background-color:"+tab_gscene.backgroundBrush().color().name());
    tool_layout->addRow("scene color:",&scene_color_combo);
    scene_color_combo.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    //line color
    QPushButton line_color_combo;
    QColor w(255,255,255);
    line_color_combo.setStyleSheet("background-color:"+w.name());
    tool_layout->addRow("line color:",&line_color_combo);
    line_color_combo.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    //text color
    QPushButton text_color_combo;
    text_color_combo.setStyleSheet("background-color:"+w.name());
    tool_layout->addRow("text color:",&text_color_combo);
    text_color_combo.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);


    cluster_colors.clear();
    //classes colors
    auto clusts = t["cluster"].unique();
    srand(time(NULL));
    for(unsigned i=0;i<clusts.size();i++){
        int rand_number = rand();
        cluster_colors[clusts[i].get_string()] = QColor(130+rand_number%125,130+(rand_number/255)%125,130+(rand_number/(255*255))%125);
    }

    for(const auto &it:clusts){
        for(auto& entry : data_table.findItems(QString::fromStdString(it.get_string()),Qt::MatchExactly)){
                if(data_table.horizontalHeaderItem(entry->column())->text() =="cluster"){
                    entry->setBackground(cluster_colors[it.get_string()]);
                    entry->setForeground(QColor(0,0,0));
                }
        }
    }

    //show text
    QCheckBox check_text;
    check_text.setChecked(true);
    tool_layout->addRow("show text:",&check_text);
    check_text.setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);

    //reset btn
    QPushButton reset_btn;
    reset_btn.setText("Reset");
    reset_btn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
    tool_layout->addWidget(&reset_btn);

    dend_layout->addWidget(&tab_gview);
    dend_layout->addWidget(tool_frame);

    tab_gscene.setSceneRect(0,0,2000,2000);
    tab_gview.setScene(&tab_gscene);
    make_dendogram(tab_gscene);

    QFrame v_fr;
    QVBoxLayout vbox;
    v_fr.setLayout(&vbox);

    vbox.addWidget(&data_table);

    double d = siluette_coef(t);

    vbox.addSpacing(20);
    QLabel sil_l;
    make_siluete(sil_l,d);

    QLabel lab;
    lab.setText("Siluete score: "+QString::number(d));
    vbox.addWidget(&lab);

    vbox.addWidget(&sil_l);

    tabs->addTab(&v_fr,"Table");
    tabs->addTab(dend_frame,"Dendogram");
    //event methods

    connect(&text_color_combo, &QPushButton::clicked, [this,&text_color_combo]() {
        QColor col = QColorDialog::getColor();
        text_color_combo.setStyleSheet("background-color:"+col.name());
        for(auto it:x_texts)
            it->setBrush(QColor(col));
        for(auto it:y_texts)
            it->setBrush(QColor(col));
    });

    connect(&line_color_combo, &QPushButton::clicked, [this,&line_color_combo]() {
        QColor col = QColorDialog::getColor();
        line_color_combo.setStyleSheet("background-color:"+col.name());
        int w =x_axis.pen().width();
        QPen p{col};
        p.setWidth(w);
        x_axis.setPen(p);
        y_axis.setPen(p);
    });

    connect(&scene_color_combo, &QPushButton::clicked, [this,&scene_color_combo]() {
        QColor col = QColorDialog::getColor();
        scene_color_combo.setStyleSheet("background-color:"+col.name());
        tab_gscene.setBackgroundBrush(col);
    });

    connect(&x_seg_box, QOverload<int>::of(&QSpinBox::valueChanged),[=,&y_seg_box](int i){
                tab_gview.resetTransform();
                tab_gview.scale((double)i/100.0,((double)y_seg_box.value()/100.0));

    });

    connect(&y_seg_box, QOverload<int>::of(&QSpinBox::valueChanged),[=,&x_seg_box](int i){
                tab_gview.resetTransform();
                tab_gview.scale(((double)x_seg_box.value()/100.0),(double)i/100.0);

    });

    connect(&text_size_box, QOverload<int>::of(&QSpinBox::valueChanged),[=](int i){
        for(auto it:x_texts){
            it->resetTransform();
            it->setScale((double)i/100.0);
        }
        for(auto it:y_texts){
            it->resetTransform();
            it->setScale((double)i/100.0);
        }

    });

    connect(&line_width_box, QOverload<int>::of(&QSpinBox::valueChanged),[=](int i){
        for(auto it:dend_rects){
            QPen pen = it->pen();
            pen.setWidth(i);
            it->setPen(pen);
        }
        QPen pen = x_axis.pen();
        pen.setWidth(i);
        x_axis.setPen(pen);
        y_axis.setPen(pen);
    });

    connect(&check_text, QOverload<int>::of(&QCheckBox::stateChanged),[=](int i){

        for(auto it:x_texts){
            if(!i)
                it->hide();
            else
                it->show();
        }
        for(auto it:y_texts){
            if(!i)
                it->hide();
            else
                it->show();
        }
    });

    connect(&reset_btn, &QPushButton::clicked, [&]() {
        y_seg_box.setValue(100);
        x_seg_box.setValue(100);
        QColor col{255,255,255};
        line_color_combo.setStyleSheet("background-color:"+col.name());
        text_color_combo.setStyleSheet("background-color:"+col.name());



        QPen p{col};
        p.setWidth(2);
        x_axis.setPen(p);
        y_axis.setPen(p);

        scene_color_combo.setStyleSheet("background-color:"+QColor(0,0,0).name());
        tab_gscene.setBackgroundBrush(QBrush());
        tab_gview.setStyleSheet("QGraphicsView { background : qlineargradient(spread:pad, x1:0.524092, y1:1, x2:0.518, y2:0, stop:0 rgba(60, 60, 60, 255), stop:1 rgba(73, 73, 73, 255)) }");
        for(auto it:x_texts)
            it->setBrush(QColor(col));
        for(auto it:y_texts)
            it->setBrush(QColor(col));

        auto clusts = t["cluster"].unique();
        srand(time(NULL));
        for(unsigned i=0;i<clusts.size();i++){
            int rand_number = rand();
            cluster_colors[clusts[i].get_string()] = QColor(rand_number%255,(rand_number/255)%255,(rand_number/(255*255))%255);
        }
        line_width_box.setValue(2);
        check_text.setChecked(true);
        text_size_box.setValue(100);
    });

    tablePreview->exec();
}

packet aglo_node::get_msg()
{
    packet msg=inputs[0]->get_packet();
    msg.add_column("cluster",column_role::INPUT_COLUMN,column_type::CONTINUOUS);
    return msg;
}


void aglo_node::combo_changed(QString str)
{
    if(str.toStdString() == "Euclidean")
        model.set_instance_metric(euclidean_dist);
    if(str.toStdString() == "Manhattan")
        model.set_instance_metric(manhattan_dist);
    if(str.toStdString() == "Smc")
        model.set_instance_metric(smc_dist);
    if(str.toStdString() == "Zakard")
        model.set_instance_metric(zakard_dist);
    if(str.toStdString() == "Hamming")
        model.set_instance_metric(hamming_dist);
}

void aglo_node::combo_clust_changed(QString str)
{
    if(str.toStdString() == "Worst")
        model.set_cluster_metric(max_linkage);
    if(str.toStdString() == "Best")
        model.set_cluster_metric(min_linkage);
    if(str.toStdString() == "Average")
        model.set_cluster_metric(avg_linkage);
}

void aglo_node::dist_changed(double v)
{
    model.set_distance(v);
}

void aglo_node::num_changed(int v)
{
    model.set_num_clusters(v);
}

void aglo_node::scene_changed(const QList<QRectF> &region)
{
    (void)region;
    QRectF rect = tab_gscene.itemsBoundingRect();
    tab_gscene.setSceneRect(rect);
}
