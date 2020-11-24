#include "OSGViewWidget"
#include <osgDB/ReadFile>

#include <osg/Camera>
#include <osg/Group>

#include <osgViewer/Viewer>

#include <string>

#include <QApplication>

int main(int argc, char **argv)
{
    auto root = new osg::Group();

    std::string path(argv[1]);

    auto node = osgDB::readNodeFile(path);

    root->addChild(node);

    QApplication a(argc, argv);

    OsgViewerWidget viewer(NULL,root);

    viewer.show();

    return a.exec();
}