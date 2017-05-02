add_executable(tut-catalog WIN32
    config.h
    icon.ico
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    tut-catalog.qrc
    tut-catalog.rc
    )

target_link_libraries(tut-catalog libtc)

configure_file(${PROJECT_SOURCE_DIR}/config.h.in config.h)
