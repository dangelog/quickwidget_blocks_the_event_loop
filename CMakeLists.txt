cmake_minimum_required(VERSION 3.19)
project(widget_event_loop LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets QuickWidgets)

qt_standard_project_setup()

qt_add_executable(widget_event_loop
    WIN32 MACOSX_BUNDLE
    main.cpp
    widget.cpp
    widget.h
)

target_link_libraries(widget_event_loop
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::QuickWidgets
)

include(GNUInstallDirs)

install(TARGETS widget_event_loop
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET widget_event_loop
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
