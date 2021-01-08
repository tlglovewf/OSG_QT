#include <iostream>
#include <osg/Geode>
#include <osg/Projection>
#include <osgViewer/Viewer>
#include <osgText/Text>
#include "OSGViewWidget"
#include "SceneCameraManipulator.h"
#include <QApplication>
using namespace std;

int main(int argc, char **argv)
{

    QApplication a(argc, argv);

    osg::ref_ptr<osg::Group> root = new osg::Group;

    OsgViewerWidget viewer(NULL,root);

    viewer.getOsgViewer()->setCameraManipulator(new SceneCamManipulator);

    osg::ref_ptr<osg::Projection> prj = new osg::Projection(osg::Matrix::ortho2D(0, viewer.width(), 0, viewer.height()));

    prj->setCullingActive(false);

    osg::Vec4 fontColor(1.0f,0.5f,0.0f,1.0f);

    osg::ref_ptr<osg::Geode>    leaf = new osg::Geode;

    osg::ref_ptr<osgText::Text> text = new osgText::Text;

    text->setColor(fontColor);

    leaf->addDrawable(text);

    text->setFont(0);

    text->setText("hello world");

    text->setAlignment(osgText::Text::LEFT_BOTTOM);

    text->setBackdropType(osgText::Text::OUTLINE);

    text->setBackdropColor(osg::Vec4(0.0,1.0,0.0,1.0));

    text->setCharacterSize(15);

    text->setPosition(osg::Vec3(0,0,0));

    prj->addChild(text);

    root->addChild(prj);

    viewer.show();

    return a.exec();
}