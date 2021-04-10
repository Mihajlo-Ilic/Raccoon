cmake_minimum_required(VERSION 3.16)
project(raccoon VERSION 1.0.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage -pthread")
set(CMAKE_EXE_LINKER_FLAGS "-lgcov")



find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Charts DataVisualization)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Charts REQUIRED)
find_package(Qt5DataVisualization REQUIRED)

set(HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Includes")
set(GUI_HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Includes/gui")
set(SOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Sources")
set(GUI_SOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Sources/gui") 

set(HEADERS
    "${HEADER_PATH}/agglo_cluster.hpp"
    "${HEADER_PATH}/apriori.hpp"
    "${HEADER_PATH}/dbscan.hpp"
    "${HEADER_PATH}/decision_tree.hpp"
    "${HEADER_PATH}/entry.hpp"
    "${HEADER_PATH}/file_reader.hpp"
    "${HEADER_PATH}/k_mean_cluster.hpp"
    "${HEADER_PATH}/knn.hpp"
    "${HEADER_PATH}/naive_bayes.hpp"
    "${HEADER_PATH}/stats.hpp"
    "${HEADER_PATH}/table.hpp"
    "${HEADER_PATH}/transformations.hpp"

    "${GUI_HEADER_PATH}/aglo_node.hpp"
    "${GUI_HEADER_PATH}/apriori_node.hpp"
    "${GUI_HEADER_PATH}/aproximation_node.hpp"
    "${GUI_HEADER_PATH}/binning_node.hpp"
    "${GUI_HEADER_PATH}/categoricaltobinnary_node.hpp"
    "${GUI_HEADER_PATH}/csv_node.hpp"
    "${GUI_HEADER_PATH}/dbscan_node.hpp"
    "${GUI_HEADER_PATH}/decision_tree_node.hpp"
    "${GUI_HEADER_PATH}/delete_na.hpp"
    "${GUI_HEADER_PATH}/doc_reader_node.hpp"
    "${GUI_HEADER_PATH}/filter_node.hpp"
    "${GUI_HEADER_PATH}/k_mean_cluster_node.hpp"
    "${GUI_HEADER_PATH}/knn_node.hpp"
    "${GUI_HEADER_PATH}/mainwindow.h"
    "${GUI_HEADER_PATH}/naive_bayes_node.hpp"
    "${GUI_HEADER_PATH}/node.hpp"
    "${GUI_HEADER_PATH}/normalization_node.hpp"
    "${GUI_HEADER_PATH}/outputtable_node.hpp"
    "${GUI_HEADER_PATH}/partition_node.hpp"
    "${GUI_HEADER_PATH}/plot_node.hpp"
    "${GUI_HEADER_PATH}/raccoon_scene.hpp"
    "${GUI_HEADER_PATH}/standardization_node.hpp"
    "${GUI_HEADER_PATH}/stats_node.hpp"
    "${GUI_HEADER_PATH}/tf_idf_node.hpp"



)

set(SOURCES

    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"

    "${SOURCES_PATH}/agglo_cluster.cpp"
    "${SOURCES_PATH}/apriori.cpp"
    "${SOURCES_PATH}/dbscan.cpp"
    "${SOURCES_PATH}/decision_tree.cpp"
    "${SOURCES_PATH}/entry.cpp"
    "${SOURCES_PATH}/file_reader.cpp"
    "${SOURCES_PATH}/k_mean_cluster.cpp"
    "${SOURCES_PATH}/knn.cpp"
    "${SOURCES_PATH}/naive_bayes.cpp"
    "${SOURCES_PATH}/stats.cpp"
    "${SOURCES_PATH}/table.cpp"
    "${SOURCES_PATH}/transformations.cpp"

    "${GUI_SOURCES_PATH}/aglo_node.cpp"
    "${GUI_SOURCES_PATH}/apriori_node.cpp"
    "${GUI_SOURCES_PATH}/aproximation_node.cpp"
    "${GUI_SOURCES_PATH}/binning_node.cpp"
    "${GUI_SOURCES_PATH}/categoricaltobinnary_node.cpp"
    "${GUI_SOURCES_PATH}/csv_node.cpp"
    "${GUI_SOURCES_PATH}/dbscan_node.cpp"
    "${GUI_SOURCES_PATH}/decision_tree_node.cpp"
    "${GUI_SOURCES_PATH}/delete_na.cpp"
    "${GUI_SOURCES_PATH}/doc_reader_node.cpp"
    "${GUI_SOURCES_PATH}/filter_node.cpp"
    "${GUI_SOURCES_PATH}/k_mean_cluster_node.cpp"
    "${GUI_SOURCES_PATH}/knn_node.cpp"
    "${GUI_SOURCES_PATH}/mainwindow.cpp"
    "${GUI_SOURCES_PATH}/naive_bayes_node.cpp"
    "${GUI_SOURCES_PATH}/node.cpp"
    "${GUI_SOURCES_PATH}/normalization_node.cpp"
    "${GUI_SOURCES_PATH}/outputtable_node.cpp"
    "${GUI_SOURCES_PATH}/partition_node.cpp"
    "${GUI_SOURCES_PATH}/plot_node.cpp"
    "${GUI_SOURCES_PATH}/raccoon_scene.cpp"
    "${GUI_SOURCES_PATH}/standardization_node.cpp"
    "${GUI_SOURCES_PATH}/stats_node.cpp"
    "${GUI_SOURCES_PATH}/tf_idf_node.cpp"
)


qt5_add_resources(RESOURCES 
"${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc"
)


set(UI
"${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui"
    
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES} ${UI})
target_link_libraries(${PROJECT_NAME})
qt5_use_modules(${PROJECT_NAME} Core Gui Widgets Charts DataVisualization)
