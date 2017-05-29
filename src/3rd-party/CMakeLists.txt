# libmarkdown (BSD)
# https://github.com/eduardonunesp/Anomade
# https://github.com/Orc/discount
add_library(libmarkdown SHARED
    markdown/README
    markdown/libs/amalloc.h
    markdown/libs/config.h
    markdown/libs/cstring.h
    markdown/libs/docheader.c
    markdown/libs/dumptree.c
    markdown/libs/generate.c
    markdown/libs/markdown.c
    markdown/libs/markdown.h
    markdown/libs/markdowncxx.cpp
    markdown/libs/markdowncxx.h
    markdown/libs/mkdio.c
    markdown/libs/resource.c
    markdown/libs/version.c
    )

target_include_directories(libmarkdown PUBLIC markdown/libs)

# libqtsupermacros (WTFPL)
# http://gitlab.unique-conception.org/qt-qml-tricks/qt-supermacros
add_library(libqtsupermacros STATIC
    qt-supermacros/LICENSE.md
    qt-supermacros/README.md
    qt-supermacros/QQmlAutoPropertyHelpers.h
    qt-supermacros/QQmlConstRefPropertyHelpers.h
    qt-supermacros/QQmlEnumClassHelper.h
    qt-supermacros/QQmlHelpers.cpp
    qt-supermacros/QQmlHelpersCommon.h
    qt-supermacros/QQmlListPropertyHelper.h
    qt-supermacros/QQmlPtrPropertyHelpers.h
    qt-supermacros/QQmlVarPropertyHelpers.h
    )

target_include_directories(libqtsupermacros PUBLIC qt-supermacros)

target_link_libraries(libqtsupermacros
    Qt5::Core
    Qt5::Qml
    )

# libqtqmlmodels (WTFPL)
# http://gitlab.unique-conception.org/qt-qml-tricks/qt-qml-models
add_library(libqtqmlmodels STATIC
    qt-qml-models/LICENSE.md
    qt-qml-models/README.md
    qt-qml-models/QQmlObjectListModel.cpp
    qt-qml-models/QQmlObjectListModel.h
    qt-qml-models/QQmlVariantListModel.cpp
    qt-qml-models/QQmlVariantListModel.h
    qt-qml-models/QtQmlTricksPlugin_SmartDataModels.h
    )

target_include_directories(libqtqmlmodels PUBLIC qt-qml-models)

target_link_libraries(libqtqmlmodels
    Qt5::Core
    Qt5::Qml
    )
