set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
IF ( Qt5Widgets_FOUND )
    include(GenerateExportHeader)

    SET(LIB_NAME osgQOpenGL)
    SET(HEADER_INSTALL_DIR osgQOpenGL)

    set(CMAKE_INCLUDE_CURRENT_DIR yes)

    SET(HEADER_PATH ../../include/osgQOpenGL)

    SET(SOURCE_H 
        ${HEADER_PATH}/osgQOpenGLWidget
        ${HEADER_PATH}/osgQOpenGLWindow
        ${HEADER_PATH}/OSGRenderer
        ${HEADER_PATH}/../OSGViewWidget
    )

    qt5_wrap_cpp(SOURCES_H_MOC ${SOURCE_H} #[[OPTIONS ${MOC_OPTIONS}]])

    SET(TARGET_H
        ${SOURCE_H}
        ${CMAKE_CURRENT_BINARY_DIR}/Export
    )

    SET(TARGET_SRC
        osgQOpenGLWidget.cpp
        osgQOpenGLWindow.cpp
        OSGRenderer.cpp
        OSGViewWidget.cpp
        ${SOURCES_H_MOC}
        ${OPENSCENEGRAPH_VERSIONINFO_RC}
    )

    SET(TARGET_LIBRARIES Qt5::Widgets Qt5::OpenGL)

    SET(TARGET_LIBRARIES_VARS
    libosg.so
    libosgGA.so
    libosgViewer.so
    libosgUtil.so
    libosgDB.so
    libOpenThreads.so
)

    # SETUP_LIBRARY(${LIB_NAME})
    add_library(${LIB_NAME} ${TARGET_SRC})
    target_link_libraries(${LIB_NAME} ${TARGET_LIBRARIES_VARS} ${TARGET_LIBRARIES})
    

    generate_export_header(${LIB_NAME} EXPORT_FILE_NAME "Export")

ENDIF()
