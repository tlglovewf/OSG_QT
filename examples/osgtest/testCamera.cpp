#include <iostream>
#include <QApplication>
#include "OSGViewWidget"

#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/Geode>
#include <osg/TexEnv>
#include <osgUtil/SmoothingVisitor>
#include <osg/Texture2D>
#include <osgText/Text>
#include <osgDB/ReadFile>
#include <QFont>
#include <memory>
using namespace std;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    osg::ref_ptr<osg::Sphere> sp = new osg::Sphere(osg::Vec3(0,0,0),  1);

    osg::ref_ptr<osg::TessellationHints> hint = new osg::TessellationHints();
    hint->setDetailRatio(0.8f);

    osg::ref_ptr<osg::Geode> node = new osg::Geode();
    osg::Vec4 red(1,1,1,1);
    node->addChild(osg::convertShapeToGeometry(*sp,hint.get(),red));
    
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D(); 
    texture->setDataVariance(osg::Object::DYNAMIC);
    osg::ref_ptr<osg::Image> img = osgDB::readImageFile("/home/tu/Desktop/skybox.jpeg");
    if(!img.valid())
        return -1;
    texture->setImage(img);

    texture->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR_MIPMAP_LINEAR);
    texture->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR);
    texture->setWrap(osg::Texture::WRAP_S,osg::Texture::CLAMP_TO_EDGE);
    texture->setWrap(osg::Texture::WRAP_T,osg::Texture::CLAMP_TO_EDGE);
    node->getChild(0)->getOrCreateStateSet()->setTextureAttributeAndModes(0,texture.get(),osg::StateAttribute::ON);
    // node->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

    osg::ref_ptr<osg::TexEnv> texenv = new osg::TexEnv;
    texenv->setMode(osg::TexEnv::DECAL);
    node->getChild(0)->getOrCreateStateSet()->setTextureAttribute(0,texenv);
    osg::ref_ptr<osg::Group> root = new osg::Group;

    root->addChild(node);

    OsgViewerWidget viewer(NULL,root);

    osg::ref_ptr<osgText::Font> font = osgText::readFontFile("/home/tu/Downloads/Fonts/simhei.ttf");
    osg::ref_ptr<osgText::Text> text = new osgText::Text();

    text->setCharacterSize(15);

    text->setFont(font);

    std::wstring wstr(L"你好么 世界...");

    text->setText(wstr.c_str());
    text->setPosition(osg::Vec3( 10, viewer.height() * 0.1,0));
    text->setColor(osg::Vec4(1,0,0,1));
    
    //固定视角相机
    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setProjectionMatrix(osg::Matrix::ortho2D(0, viewer.width(),0,viewer.height()));
    camera->setViewMatrix(osg::Matrix::identity());
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setRenderOrder(osg::Camera::NESTED_RENDER);
    camera->addChild(text);
    root->addChild(camera);
    viewer.show();

    return app.exec();
}