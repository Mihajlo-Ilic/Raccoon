/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_run;
    QAction *action_run_opt;
    QAction *action_delete;
    QAction *action_merge;
    QAction *action_unmerge;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QGraphicsView *agraphicsView;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    QMenu *menuHelp;
    QStatusBar *statusbar;
    QToolBar *toolbar_actions;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_3;
    QToolButton *csv_button;
    QFrame *line_2;
    QToolButton *stats_button;
    QToolButton *table_output_button;
    QToolButton *plot_button;
    QFrame *line_4;
    QToolButton *filter_button;
    QToolButton *normalization_button;
    QToolButton *standardize_button;
    QToolButton *approximate_button;
    QToolButton *partition_button;
    QToolButton *binning_button;
    QToolButton *remove_na_button;
    QToolButton *to_numeric_button;
    QFrame *line;
    QToolButton *dbscan_button;
    QToolButton *k_means_button;
    QToolButton *aglo_button;
    QFrame *line_3;
    QToolButton *dec_tree_button;
    QToolButton *knn_button;
    QToolButton *bayes_button;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(798, 996);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMouseTracking(true);
        MainWindow->setTabletTracking(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/res/Resources/Node_icons/trash_panda.svg"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        MainWindow->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        action_run = new QAction(MainWindow);
        action_run->setObjectName(QString::fromUtf8("action_run"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/res/Resources/Action_icons/start.svg"), QSize(), QIcon::Normal, QIcon::Off);
        action_run->setIcon(icon1);
        action_run_opt = new QAction(MainWindow);
        action_run_opt->setObjectName(QString::fromUtf8("action_run_opt"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/res/Resources/Action_icons/start_opt.svg"), QSize(), QIcon::Normal, QIcon::Off);
        action_run_opt->setIcon(icon2);
        action_delete = new QAction(MainWindow);
        action_delete->setObjectName(QString::fromUtf8("action_delete"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/res/Resources/Action_icons/delete.svg"), QSize(), QIcon::Normal, QIcon::Off);
        action_delete->setIcon(icon3);
        action_merge = new QAction(MainWindow);
        action_merge->setObjectName(QString::fromUtf8("action_merge"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/res/Resources/Action_icons/merge.svg"), QSize(), QIcon::Normal, QIcon::Off);
        action_merge->setIcon(icon4);
        action_unmerge = new QAction(MainWindow);
        action_unmerge->setObjectName(QString::fromUtf8("action_unmerge"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/res/Resources/Action_icons/un_merge.svg"), QSize(), QIcon::Normal, QIcon::Off);
        action_unmerge->setIcon(icon5);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setMouseTracking(true);
        centralwidget->setAcceptDrops(true);
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, -1, -1, -1);
        agraphicsView = new QGraphicsView(centralwidget);
        agraphicsView->setObjectName(QString::fromUtf8("agraphicsView"));
        agraphicsView->setMouseTracking(true);
        QBrush brush(QColor(0, 0, 0, 10));
        brush.setStyle(Qt::CrossPattern);
        agraphicsView->setBackgroundBrush(brush);
        QBrush brush1(QColor(0, 0, 0, 10));
        brush1.setStyle(Qt::NoBrush);
        agraphicsView->setForegroundBrush(brush1);
        agraphicsView->setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing);

        horizontalLayout->addWidget(agraphicsView);


        verticalLayout_2->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 798, 23));
        menubar->setStyleSheet(QString::fromUtf8("QMenu:item{\n"
"color:red\n"
"}"));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuHelp->setStyleSheet(QString::fromUtf8(""));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolbar_actions = new QToolBar(MainWindow);
        toolbar_actions->setObjectName(QString::fromUtf8("toolbar_actions"));
        toolbar_actions->setMovable(false);
        toolbar_actions->setIconSize(QSize(32, 32));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolbar_actions);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(dockWidget->sizePolicy().hasHeightForWidth());
        dockWidget->setSizePolicy(sizePolicy1);
        dockWidget->setMinimumSize(QSize(139, 900));
        dockWidget->setFloating(false);
        dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        sizePolicy1.setHeightForWidth(dockWidgetContents->sizePolicy().hasHeightForWidth());
        dockWidgetContents->setSizePolicy(sizePolicy1);
        dockWidgetContents->setMinimumSize(QSize(139, 868));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(dockWidgetContents);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setFrameShape(QFrame::StyledPanel);
        scrollArea->setFrameShadow(QFrame::Plain);
        scrollArea->setLineWidth(0);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 174, 1596));
        verticalLayout_3 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        csv_button = new QToolButton(scrollAreaWidgetContents);
        csv_button->setObjectName(QString::fromUtf8("csv_button"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(csv_button->sizePolicy().hasHeightForWidth());
        csv_button->setSizePolicy(sizePolicy2);
        csv_button->setMinimumSize(QSize(80, 80));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/res/Resources/Node_icons/csv_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        csv_button->setIcon(icon6);
        csv_button->setIconSize(QSize(64, 64));
        csv_button->setCheckable(false);
        csv_button->setPopupMode(QToolButton::DelayedPopup);
        csv_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        csv_button->setAutoRaise(true);
        csv_button->setArrowType(Qt::NoArrow);

        verticalLayout_3->addWidget(csv_button);

        line_2 = new QFrame(scrollAreaWidgetContents);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShadow(QFrame::Plain);
        line_2->setLineWidth(1);
        line_2->setFrameShape(QFrame::HLine);

        verticalLayout_3->addWidget(line_2);

        stats_button = new QToolButton(scrollAreaWidgetContents);
        stats_button->setObjectName(QString::fromUtf8("stats_button"));
        sizePolicy2.setHeightForWidth(stats_button->sizePolicy().hasHeightForWidth());
        stats_button->setSizePolicy(sizePolicy2);
        stats_button->setMinimumSize(QSize(80, 80));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/res/Resources/Node_icons/stats.svg"), QSize(), QIcon::Normal, QIcon::Off);
        stats_button->setIcon(icon7);
        stats_button->setIconSize(QSize(64, 64));
        stats_button->setCheckable(false);
        stats_button->setPopupMode(QToolButton::DelayedPopup);
        stats_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        stats_button->setAutoRaise(true);
        stats_button->setArrowType(Qt::NoArrow);

        verticalLayout_3->addWidget(stats_button);

        table_output_button = new QToolButton(scrollAreaWidgetContents);
        table_output_button->setObjectName(QString::fromUtf8("table_output_button"));
        sizePolicy2.setHeightForWidth(table_output_button->sizePolicy().hasHeightForWidth());
        table_output_button->setSizePolicy(sizePolicy2);
        table_output_button->setMinimumSize(QSize(80, 80));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/res/Resources/Node_icons/table_output.svg"), QSize(), QIcon::Normal, QIcon::Off);
        table_output_button->setIcon(icon8);
        table_output_button->setIconSize(QSize(64, 64));
        table_output_button->setCheckable(false);
        table_output_button->setPopupMode(QToolButton::DelayedPopup);
        table_output_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        table_output_button->setAutoRaise(true);
        table_output_button->setArrowType(Qt::NoArrow);

        verticalLayout_3->addWidget(table_output_button);

        plot_button = new QToolButton(scrollAreaWidgetContents);
        plot_button->setObjectName(QString::fromUtf8("plot_button"));
        sizePolicy2.setHeightForWidth(plot_button->sizePolicy().hasHeightForWidth());
        plot_button->setSizePolicy(sizePolicy2);
        plot_button->setMinimumSize(QSize(80, 80));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/res/Resources/Node_icons/plot.svg"), QSize(), QIcon::Normal, QIcon::Off);
        plot_button->setIcon(icon9);
        plot_button->setIconSize(QSize(64, 64));
        plot_button->setCheckable(false);
        plot_button->setPopupMode(QToolButton::DelayedPopup);
        plot_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        plot_button->setAutoRaise(true);
        plot_button->setArrowType(Qt::NoArrow);

        verticalLayout_3->addWidget(plot_button);

        line_4 = new QFrame(scrollAreaWidgetContents);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShadow(QFrame::Plain);
        line_4->setLineWidth(1);
        line_4->setFrameShape(QFrame::HLine);

        verticalLayout_3->addWidget(line_4);

        filter_button = new QToolButton(scrollAreaWidgetContents);
        filter_button->setObjectName(QString::fromUtf8("filter_button"));
        sizePolicy2.setHeightForWidth(filter_button->sizePolicy().hasHeightForWidth());
        filter_button->setSizePolicy(sizePolicy2);
        filter_button->setMinimumSize(QSize(80, 80));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/res/Resources/Node_icons/filter.svg"), QSize(), QIcon::Normal, QIcon::Off);
        filter_button->setIcon(icon10);
        filter_button->setIconSize(QSize(64, 64));
        filter_button->setCheckable(false);
        filter_button->setPopupMode(QToolButton::DelayedPopup);
        filter_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        filter_button->setAutoRaise(true);
        filter_button->setArrowType(Qt::NoArrow);

        verticalLayout_3->addWidget(filter_button);

        normalization_button = new QToolButton(scrollAreaWidgetContents);
        normalization_button->setObjectName(QString::fromUtf8("normalization_button"));
        sizePolicy2.setHeightForWidth(normalization_button->sizePolicy().hasHeightForWidth());
        normalization_button->setSizePolicy(sizePolicy2);
        normalization_button->setMinimumSize(QSize(80, 80));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/res/Resources/Node_icons/normalization.svg"), QSize(), QIcon::Normal, QIcon::Off);
        normalization_button->setIcon(icon11);
        normalization_button->setIconSize(QSize(64, 64));
        normalization_button->setCheckable(false);
        normalization_button->setPopupMode(QToolButton::DelayedPopup);
        normalization_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        normalization_button->setAutoRaise(true);
        normalization_button->setArrowType(Qt::NoArrow);

        verticalLayout_3->addWidget(normalization_button);

        standardize_button = new QToolButton(scrollAreaWidgetContents);
        standardize_button->setObjectName(QString::fromUtf8("standardize_button"));
        sizePolicy2.setHeightForWidth(standardize_button->sizePolicy().hasHeightForWidth());
        standardize_button->setSizePolicy(sizePolicy2);
        standardize_button->setMinimumSize(QSize(80, 80));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/res/Resources/Node_icons/standardization.svg"), QSize(), QIcon::Normal, QIcon::Off);
        standardize_button->setIcon(icon12);
        standardize_button->setIconSize(QSize(64, 64));
        standardize_button->setCheckable(false);
        standardize_button->setPopupMode(QToolButton::DelayedPopup);
        standardize_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        standardize_button->setAutoRaise(true);
        standardize_button->setArrowType(Qt::NoArrow);

        verticalLayout_3->addWidget(standardize_button);

        approximate_button = new QToolButton(scrollAreaWidgetContents);
        approximate_button->setObjectName(QString::fromUtf8("approximate_button"));
        sizePolicy2.setHeightForWidth(approximate_button->sizePolicy().hasHeightForWidth());
        approximate_button->setSizePolicy(sizePolicy2);
        approximate_button->setMinimumSize(QSize(80, 80));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/res/Resources/Node_icons/aproximation.svg"), QSize(), QIcon::Normal, QIcon::Off);
        approximate_button->setIcon(icon13);
        approximate_button->setIconSize(QSize(64, 64));
        approximate_button->setCheckable(false);
        approximate_button->setPopupMode(QToolButton::DelayedPopup);
        approximate_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        approximate_button->setAutoRaise(true);
        approximate_button->setArrowType(Qt::NoArrow);

        verticalLayout_3->addWidget(approximate_button);

        partition_button = new QToolButton(scrollAreaWidgetContents);
        partition_button->setObjectName(QString::fromUtf8("partition_button"));
        sizePolicy2.setHeightForWidth(partition_button->sizePolicy().hasHeightForWidth());
        partition_button->setSizePolicy(sizePolicy2);
        partition_button->setMinimumSize(QSize(80, 80));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/res/Resources/Node_icons/partition.svg"), QSize(), QIcon::Normal, QIcon::Off);
        partition_button->setIcon(icon14);
        partition_button->setIconSize(QSize(64, 64));
        partition_button->setCheckable(false);
        partition_button->setPopupMode(QToolButton::DelayedPopup);
        partition_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        partition_button->setAutoRaise(true);
        partition_button->setArrowType(Qt::NoArrow);

        verticalLayout_3->addWidget(partition_button);

        binning_button = new QToolButton(scrollAreaWidgetContents);
        binning_button->setObjectName(QString::fromUtf8("binning_button"));
        sizePolicy2.setHeightForWidth(binning_button->sizePolicy().hasHeightForWidth());
        binning_button->setSizePolicy(sizePolicy2);
        binning_button->setMinimumSize(QSize(80, 80));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/res/Resources/Node_icons/binning.svg"), QSize(), QIcon::Normal, QIcon::Off);
        binning_button->setIcon(icon15);
        binning_button->setIconSize(QSize(64, 64));
        binning_button->setCheckable(false);
        binning_button->setPopupMode(QToolButton::DelayedPopup);
        binning_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        binning_button->setAutoRaise(true);
        binning_button->setArrowType(Qt::NoArrow);

        verticalLayout_3->addWidget(binning_button);

        remove_na_button = new QToolButton(scrollAreaWidgetContents);
        remove_na_button->setObjectName(QString::fromUtf8("remove_na_button"));
        sizePolicy2.setHeightForWidth(remove_na_button->sizePolicy().hasHeightForWidth());
        remove_na_button->setSizePolicy(sizePolicy2);
        remove_na_button->setMinimumSize(QSize(80, 80));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/res/Resources/Node_icons/remove_na.svg"), QSize(), QIcon::Normal, QIcon::Off);
        remove_na_button->setIcon(icon16);
        remove_na_button->setIconSize(QSize(64, 64));
        remove_na_button->setCheckable(false);
        remove_na_button->setPopupMode(QToolButton::DelayedPopup);
        remove_na_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        remove_na_button->setAutoRaise(true);
        remove_na_button->setArrowType(Qt::NoArrow);

        verticalLayout_3->addWidget(remove_na_button);

        to_numeric_button = new QToolButton(scrollAreaWidgetContents);
        to_numeric_button->setObjectName(QString::fromUtf8("to_numeric_button"));
        sizePolicy2.setHeightForWidth(to_numeric_button->sizePolicy().hasHeightForWidth());
        to_numeric_button->setSizePolicy(sizePolicy2);
        to_numeric_button->setMinimumSize(QSize(80, 80));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/res/Resources/Node_icons/cat_to_num.svg"), QSize(), QIcon::Normal, QIcon::Off);
        to_numeric_button->setIcon(icon17);
        to_numeric_button->setIconSize(QSize(64, 64));
        to_numeric_button->setCheckable(false);
        to_numeric_button->setPopupMode(QToolButton::DelayedPopup);
        to_numeric_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        to_numeric_button->setAutoRaise(true);
        to_numeric_button->setArrowType(Qt::NoArrow);

        verticalLayout_3->addWidget(to_numeric_button);

        line = new QFrame(scrollAreaWidgetContents);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShadow(QFrame::Plain);
        line->setLineWidth(1);
        line->setFrameShape(QFrame::HLine);

        verticalLayout_3->addWidget(line);

        dbscan_button = new QToolButton(scrollAreaWidgetContents);
        dbscan_button->setObjectName(QString::fromUtf8("dbscan_button"));
        sizePolicy2.setHeightForWidth(dbscan_button->sizePolicy().hasHeightForWidth());
        dbscan_button->setSizePolicy(sizePolicy2);
        dbscan_button->setMinimumSize(QSize(80, 80));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/res/Resources/Node_icons/dbscan.svg"), QSize(), QIcon::Normal, QIcon::Off);
        dbscan_button->setIcon(icon18);
        dbscan_button->setIconSize(QSize(64, 64));
        dbscan_button->setCheckable(false);
        dbscan_button->setPopupMode(QToolButton::DelayedPopup);
        dbscan_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        dbscan_button->setAutoRaise(true);
        dbscan_button->setArrowType(Qt::NoArrow);

        verticalLayout_3->addWidget(dbscan_button);

        k_means_button = new QToolButton(scrollAreaWidgetContents);
        k_means_button->setObjectName(QString::fromUtf8("k_means_button"));
        sizePolicy2.setHeightForWidth(k_means_button->sizePolicy().hasHeightForWidth());
        k_means_button->setSizePolicy(sizePolicy2);
        k_means_button->setMinimumSize(QSize(80, 80));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/res/Resources/Node_icons/k_means.svg"), QSize(), QIcon::Normal, QIcon::Off);
        k_means_button->setIcon(icon19);
        k_means_button->setIconSize(QSize(64, 64));
        k_means_button->setCheckable(false);
        k_means_button->setPopupMode(QToolButton::DelayedPopup);
        k_means_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        k_means_button->setAutoRaise(true);
        k_means_button->setArrowType(Qt::NoArrow);

        verticalLayout_3->addWidget(k_means_button);

        aglo_button = new QToolButton(scrollAreaWidgetContents);
        aglo_button->setObjectName(QString::fromUtf8("aglo_button"));
        sizePolicy2.setHeightForWidth(aglo_button->sizePolicy().hasHeightForWidth());
        aglo_button->setSizePolicy(sizePolicy2);
        aglo_button->setMinimumSize(QSize(80, 80));
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/res/Resources/Node_icons/aglo.svg"), QSize(), QIcon::Normal, QIcon::Off);
        aglo_button->setIcon(icon20);
        aglo_button->setIconSize(QSize(64, 64));
        aglo_button->setCheckable(false);
        aglo_button->setPopupMode(QToolButton::DelayedPopup);
        aglo_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        aglo_button->setAutoRaise(true);
        aglo_button->setArrowType(Qt::NoArrow);

        verticalLayout_3->addWidget(aglo_button);

        line_3 = new QFrame(scrollAreaWidgetContents);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShadow(QFrame::Plain);
        line_3->setLineWidth(1);
        line_3->setFrameShape(QFrame::HLine);

        verticalLayout_3->addWidget(line_3);

        dec_tree_button = new QToolButton(scrollAreaWidgetContents);
        dec_tree_button->setObjectName(QString::fromUtf8("dec_tree_button"));
        sizePolicy2.setHeightForWidth(dec_tree_button->sizePolicy().hasHeightForWidth());
        dec_tree_button->setSizePolicy(sizePolicy2);
        dec_tree_button->setMinimumSize(QSize(80, 80));
        QIcon icon21;
        icon21.addFile(QString::fromUtf8(":/res/Resources/Node_icons/tree.svg"), QSize(), QIcon::Normal, QIcon::Off);
        dec_tree_button->setIcon(icon21);
        dec_tree_button->setIconSize(QSize(64, 64));
        dec_tree_button->setCheckable(false);
        dec_tree_button->setPopupMode(QToolButton::DelayedPopup);
        dec_tree_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        dec_tree_button->setAutoRaise(true);
        dec_tree_button->setArrowType(Qt::NoArrow);

        verticalLayout_3->addWidget(dec_tree_button);

        knn_button = new QToolButton(scrollAreaWidgetContents);
        knn_button->setObjectName(QString::fromUtf8("knn_button"));
        sizePolicy2.setHeightForWidth(knn_button->sizePolicy().hasHeightForWidth());
        knn_button->setSizePolicy(sizePolicy2);
        knn_button->setMinimumSize(QSize(80, 80));
        QIcon icon22;
        icon22.addFile(QString::fromUtf8(":/res/Resources/Node_icons/knn.svg"), QSize(), QIcon::Normal, QIcon::Off);
        knn_button->setIcon(icon22);
        knn_button->setIconSize(QSize(64, 64));
        knn_button->setCheckable(false);
        knn_button->setPopupMode(QToolButton::DelayedPopup);
        knn_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        knn_button->setAutoRaise(true);
        knn_button->setArrowType(Qt::NoArrow);

        verticalLayout_3->addWidget(knn_button);

        bayes_button = new QToolButton(scrollAreaWidgetContents);
        bayes_button->setObjectName(QString::fromUtf8("bayes_button"));
        sizePolicy2.setHeightForWidth(bayes_button->sizePolicy().hasHeightForWidth());
        bayes_button->setSizePolicy(sizePolicy2);
        bayes_button->setMinimumSize(QSize(80, 80));
        QIcon icon23;
        icon23.addFile(QString::fromUtf8(":/res/Resources/Node_icons/bayes.svg"), QSize(), QIcon::Normal, QIcon::Off);
        bayes_button->setIcon(icon23);
        bayes_button->setIconSize(QSize(64, 64));
        bayes_button->setCheckable(false);
        bayes_button->setPopupMode(QToolButton::DelayedPopup);
        bayes_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        bayes_button->setAutoRaise(true);
        bayes_button->setArrowType(Qt::NoArrow);

        verticalLayout_3->addWidget(bayes_button);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuHelp->menuAction());
        toolbar_actions->addAction(action_run);
        toolbar_actions->addAction(action_run_opt);
        toolbar_actions->addSeparator();
        toolbar_actions->addAction(action_delete);
        toolbar_actions->addAction(action_merge);
        toolbar_actions->addAction(action_unmerge);
        toolbar_actions->addSeparator();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Raccoon", nullptr));
        action_run->setText(QCoreApplication::translate("MainWindow", "Run", nullptr));
#if QT_CONFIG(tooltip)
        action_run->setToolTip(QCoreApplication::translate("MainWindow", "starts the simulation", nullptr));
#endif // QT_CONFIG(tooltip)
        action_run_opt->setText(QCoreApplication::translate("MainWindow", "Run opt", nullptr));
#if QT_CONFIG(tooltip)
        action_run_opt->setToolTip(QCoreApplication::translate("MainWindow", "Run the simulation ignoring nodes that don't need to be updated", nullptr));
#endif // QT_CONFIG(tooltip)
        action_delete->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
#if QT_CONFIG(tooltip)
        action_delete->setToolTip(QCoreApplication::translate("MainWindow", "delete current selection", nullptr));
#endif // QT_CONFIG(tooltip)
        action_merge->setText(QCoreApplication::translate("MainWindow", "Merge", nullptr));
#if QT_CONFIG(tooltip)
        action_merge->setToolTip(QCoreApplication::translate("MainWindow", "merges selected nodes into 1 node", nullptr));
#endif // QT_CONFIG(tooltip)
        action_unmerge->setText(QCoreApplication::translate("MainWindow", "Un-merge ", nullptr));
#if QT_CONFIG(tooltip)
        action_unmerge->setToolTip(QCoreApplication::translate("MainWindow", "Transform super node to base components", nullptr));
#endif // QT_CONFIG(tooltip)
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "View", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        toolbar_actions->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar_2", nullptr));
        dockWidget->setWindowTitle(QCoreApplication::translate("MainWindow", "Nodes", nullptr));
        csv_button->setText(QCoreApplication::translate("MainWindow", "Csv File", nullptr));
        stats_button->setText(QCoreApplication::translate("MainWindow", "Table Stats", nullptr));
        table_output_button->setText(QCoreApplication::translate("MainWindow", "Table output", nullptr));
        plot_button->setText(QCoreApplication::translate("MainWindow", "Plot", nullptr));
        filter_button->setText(QCoreApplication::translate("MainWindow", "Filter", nullptr));
        normalization_button->setText(QCoreApplication::translate("MainWindow", "Normalize", nullptr));
        standardize_button->setText(QCoreApplication::translate("MainWindow", "Standarize", nullptr));
        approximate_button->setText(QCoreApplication::translate("MainWindow", "Approximate na", nullptr));
        partition_button->setText(QCoreApplication::translate("MainWindow", "Partition", nullptr));
        binning_button->setText(QCoreApplication::translate("MainWindow", "Binning", nullptr));
        remove_na_button->setText(QCoreApplication::translate("MainWindow", "Remove na", nullptr));
        to_numeric_button->setText(QCoreApplication::translate("MainWindow", "To numeric", nullptr));
        dbscan_button->setText(QCoreApplication::translate("MainWindow", "Dbscan", nullptr));
        k_means_button->setText(QCoreApplication::translate("MainWindow", "K_means", nullptr));
        aglo_button->setText(QCoreApplication::translate("MainWindow", "Hierarchy cluster", nullptr));
        dec_tree_button->setText(QCoreApplication::translate("MainWindow", "C4.5 tree", nullptr));
        knn_button->setText(QCoreApplication::translate("MainWindow", "Knn", nullptr));
        bayes_button->setText(QCoreApplication::translate("MainWindow", "Naive Bayes", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
