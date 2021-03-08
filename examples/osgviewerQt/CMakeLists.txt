SET( TARGET_SRC
    osgviewerQt.cpp
)

SET(TARGET_LIBRARIES_VARS
    libosg.so
    libosgGA.so
    libosgViewer.so
    libosgUtil.so
    libosgDB.so
    libOpenThreads.so
)


SET (EXAMPLE_NAME QtViewer )

SET(TARGET_LIBRARIES Qt5::Widgets Qt5::OpenGL)

add_executable(example_${EXAMPLE_NAME} ${TARGET_SRC})

target_link_libraries(example_${EXAMPLE_NAME} osgQOpenGL)

target_link_libraries(example_${EXAMPLE_NAME} ${TARGET_LIBRARIES_VARS} ${TARGET_LIBRARIES})

qt5_use_modules( example_${EXAMPLE_NAME} Widgets OpenGL )

