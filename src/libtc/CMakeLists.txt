add_library(libtc SHARED
    README.md

    config.h

    # code
    libtc_global.cpp
    libtc_global.h

    tc/persistent.h
    tc/project.cpp
    tc/project.h
    tc/settings.cpp
    tc/settings.h

    tc/folders/collection.cpp
    tc/folders/collection.h
    tc/folders/folder.cpp
    tc/folders/folder.h
    tc/folders/folderinfo.cpp
    tc/folders/folderinfo.h

    tc/ui/action.h
    tc/ui/delegates.cpp
    tc/ui/delegates.h
    tc/ui/dockablewidget.h
    tc/ui/dockwidget.cpp
    tc/ui/dockwidget.h
    tc/ui/labeledvalue.cpp
    tc/ui/labeledvalue.h
    tc/ui/logwidget.cpp
    tc/ui/logwidget.h
    tc/ui/pixmap.cpp
    tc/ui/pixmap.h
    tc/ui/theme.cpp
    tc/ui/theme.h
    tc/ui/tutorialswidget.cpp
    tc/ui/tutorialswidget.h

    tc/tutorials/tutorial.cpp
    tc/tutorials/tutorial.h
    tc/tutorials/model.cpp
    tc/tutorials/model.h
    tc/tutorials/proxymodel.cpp
    tc/tutorials/proxymodel.h

    # resources
    resources/libtc.qrc
    resources/README.md
    )

configure_file(${PROJECT_SOURCE_DIR}/config.h.in config.h)

target_compile_definitions(libtc PRIVATE -DLIBTC_LIBRARY)

target_include_directories(libtc PUBLIC .)

target_link_libraries(libtc
    libmarkdown
    libqtqmlmodels
    libqtsupermacros
    CONAN_PKG::QScintilla2
    CONAN_PKG::yaml-cpp
    Qt5::Core
    Qt5::Concurrent
    Qt5::Gui
    Qt5::Qml
    Qt5::Sql
    Qt5::Svg
    Qt5::Xml
    Qt5::Widgets
    )
