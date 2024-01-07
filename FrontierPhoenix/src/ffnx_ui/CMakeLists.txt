find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

add_executable(ffnx_ui main.cpp)

target_link_libraries(ffnx_ui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
