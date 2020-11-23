SET( TARGET_SRC
    osgviewerQt.cpp
)

SET(TARGET_LIBRARIES_VARS
    libosgd.so
    libosgGAd.so
    libosgViewerd.so
    libosgUtild.so
    libosgDBd.so
    libOpenThreadsd.so
)


SET (EXAMPLE_NAME osgviewerQt )

SET(TARGET_LIBRARIES Qt5::Widgets Qt5::OpenGL)

add_executable(example_${EXAMPLE_NAME} ${TARGET_SRC})

target_link_libraries(example_${EXAMPLE_NAME} osgQOpenGL)

target_link_libraries(example_${EXAMPLE_NAME} ${TARGET_LIBRARIES_VARS} ${TARGET_LIBRARIES})

qt5_use_modules( example_${EXAMPLE_NAME} Widgets OpenGL )

