QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

RESOURCES += \
    resources.qrc \
    resources.qrc \
    resources.qrc \
    resources.qrc \
    resources.qrc \
    resources.qrc \
    resources.qrc \
    resources.qrc \
    resources.qrc \
    resources.qrc \
    resources.qrc \
    resources.qrc

FORMS += \
    mainwindow.ui

DISTFILES += \
    Resources/Action_icons/delete.svg \
    Resources/Action_icons/merge.svg \
    Resources/Action_icons/start.svg \
    Resources/Action_icons/start_opt.svg \
    Resources/Action_icons/un_merge.svg \
    Resources/Action_icons/warning.svg \
    Resources/Node_icons/aglo.svg \
    Resources/Node_icons/aproximation.svg \
    Resources/Node_icons/bayes.svg \
    Resources/Node_icons/binning.svg \
    Resources/Node_icons/cat_to_num.svg \
    Resources/Node_icons/csv_icon.png \
    Resources/Node_icons/dbscan.svg \
    Resources/Node_icons/filter.svg \
    Resources/Node_icons/k_means.svg \
    Resources/Node_icons/knn.svg \
    Resources/Node_icons/normalization.svg \
    Resources/Node_icons/partition.svg \
    Resources/Node_icons/plot.svg \
    Resources/Node_icons/remove_na.svg \
    Resources/Node_icons/standardization.svg \
    Resources/Node_icons/stats.svg \
    Resources/Node_icons/table_output.svg \
    Resources/Node_icons/tree.svg \
    Resources/iris.csv \
    Resources/style.qss \
    Resources/test.csv

HEADERS += \
    Includes/agglo_cluster.hpp \
    Includes/dbscan.hpp \
    Includes/dbscan_node.hpp \
    Includes/decision_tree.hpp \
    Includes/entry.hpp \
    Includes/file_reader.hpp \
    Includes/gui/aglo_node.hpp \
    Includes/gui/aproximation_node.hpp \
    Includes/gui/binning_node.hpp \
    Includes/gui/categoricaltobinnary_node.hpp \
    Includes/gui/csv_node.hpp \
    Includes/gui/dbscan.hpp \
    Includes/gui/dbscan_node.hpp \
    Includes/gui/decision_tree_node.hpp \
    Includes/gui/delete_na.hpp \
    Includes/gui/filter_node.hpp \
    Includes/gui/k_mean_cluster_node.hpp \
    Includes/gui/knn_node.hpp \
    Includes/gui/mainwindow.h \
    Includes/gui/naive_bayes_node.hpp \
    Includes/gui/node.hpp \
    Includes/gui/normalization_node.hpp \
    Includes/gui/outputtable_node.hpp \
    Includes/gui/partition_node.hpp \
    Includes/gui/raccoon_scene.hpp \
    Includes/gui/standardization_node.hpp \
    Includes/gui/stats_node.hpp \
    Includes/k_mean_cluster.hpp \
    Includes/knn.hpp \
    Includes/naive_bayes.hpp \
    Includes/stats.hpp \
    Includes/table.hpp \
    Includes/transformations.hpp

SOURCES += \
    Sources/agglo_cluster.cpp \
    Sources/dbscan.cpp \
    Sources/decision_tree.cpp \
    Sources/entry.cpp \
    Sources/file_reader.cpp \
    Sources/gui/aglo_node.cpp \
    Sources/gui/aproximation_node.cpp \
    Sources/gui/binning_node.cpp \
    Sources/gui/categoricaltobinnary_node.cpp \
    Sources/gui/csv_node.cpp \
    Sources/gui/dbscan_node.cpp \
    Sources/gui/decision_tree_node.cpp \
    Sources/gui/delete_na.cpp \
    Sources/gui/filter_node.cpp \
    Sources/gui/k_mean_cluster_node.cpp \
    Sources/gui/knn_node.cpp \
    Sources/gui/mainwindow.cpp \
    Sources/gui/naive_bayes_node.cpp \
    Sources/gui/node.cpp \
    Sources/gui/normalization_node.cpp \
    Sources/gui/outputtable_node.cpp \
    Sources/gui/partition_node.cpp \
    Sources/gui/raccoon_scene.cpp \
    Sources/gui/standardization_node.cpp \
    Sources/gui/stats_node.cpp \
    Sources/k_mean_cluster.cpp \
    Sources/knn.cpp \
    Sources/naive_bayes.cpp \
    Sources/stats.cpp \
    Sources/table.cpp \
    Sources/transformations.cpp \
    main.cpp

TARGET = Raccoon

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
