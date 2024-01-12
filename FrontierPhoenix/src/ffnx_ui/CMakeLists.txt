find_package(QT NAMES Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(OGDF REQUIRED)

add_executable(ffnx_ui main.cpp graph/graph_widget.cpp)

target_link_libraries(ffnx_ui PRIVATE frontier-phoenix Qt${QT_VERSION_MAJOR}::Widgets OGDF)
