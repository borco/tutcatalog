add_library(libtc SHARED
    README.md

    # code
    libtc_global.h
    tc/project.cpp
    tc/project.h
    tc/folders/foldersequence.cpp
    tc/folders/foldersequence.h
    tc/folders/folder.cpp
    tc/folders/folder.h
    tc/folders/folderinfo.cpp
    tc/folders/folderinfo.h
    tc/ui/dockablewidget.h
    tc/ui/dockwidget.cpp
    tc/ui/dockwidget.h
    tc/ui/logwidget.cpp
    tc/ui/logwidget.h
    tc/ui/pixmap.cpp
    tc/ui/pixmap.h
    tc/ui/theme.cpp
    tc/ui/theme.h

    # resources
    resources/libtc.qrc
    resources/README.md
    )

target_compile_definitions(libtc PRIVATE -DLIBTC_LIBRARY)

target_include_directories(libtc PUBLIC .)

target_link_libraries(libtc
    libqtsupermacros
    libqtqmlmodels
    libmarkdown
    CONAN_PKG::QScintilla2
    CONAN_PKG::yaml-cpp
    Qt5::Widgets
    Qt5::Gui
    Qt5::Svg
    Qt5::Xml
    Qt5::Concurrent
    Qt5::Core
    Qt5::Qml
    )
