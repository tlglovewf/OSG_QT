#######################################################
# this are setting used in SETUP_EXAMPLE macro
#######################################################
SET(TARGET_DEFAULT_PREFIX "example_")
SET(TARGET_DEFAULT_LABEL_PREFIX "Examples")

IF(DYNAMIC_OPENSCENEGRAPH)

include_directories(${CMAKE_SOURCE_DIR}/include)

ADD_SUBDIRECTORY(osgqfont)
ADD_SUBDIRECTORY(osgtest)
ENDIF()

