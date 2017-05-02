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
    tc/ui/actioninfo.cpp
    tc/ui/actioninfo.h
    tc/ui/constants.cpp
    tc/ui/constants.h
    tc/ui/logdockwidget.cpp
    tc/ui/logdockwidget.h
    tc/ui/logwidget.cpp
    tc/ui/logwidget.h

    # resources
    resources/libtc.qrc
    resources/README.md
    resources/fonts/FontAwesome.otf
    resources/images/document-scanning.svg
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
