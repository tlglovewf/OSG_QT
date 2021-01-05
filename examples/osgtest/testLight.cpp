#include "OSGViewWidget"
#include <osgDB/ReadFile>

#include <osg/Camera>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/LineWidth>
#include <osg/BlendFunc>
#include <osg/Material>
#include <osg/LightSource>
#include <osgViewer/Viewer>

#include <string>

#include <QApplication>

void createLight(osg::ref_ptr<osg::Group> root)
{
    osg::ref_ptr<osg::StateSet> stateset = root->getOrCreateStateSet();

    stateset->setMode(GL_LIGHTING, osg::StateAttribute::ON);
    stateset->setMode(GL_LIGHT0  , osg::StateAttribute::ON);

    osg::ref_ptr<osg::LightSource> ls = new osg::LightSource();

    osg::ref_ptr<osg::Light> lt = new osg::Light();

    lt->setLightNum(0);
    // lt->setDirection(osg::Vec3(0,-1,0));
    //位置最后一个 0 平行光  1 点光源
    lt->setPosition(osg::Vec4(5,-1.5,5,1));
    lt->setAmbient(osg::Vec4(0,0,0,1));
    lt->setDiffuse(osg::Vec4(1,1,1,1));
    //设置镜面反射颜色
    // lt->setSpecular(osg::Vec4(0.8,0.8,0.8,1.0));
    ls->setLight(lt);

    root->addChild(ls);
}

int main(int argc, char **argv)
{
    osg::ref_ptr<osg::Group> root = new osg::Group;

    osg::ref_ptr<osg::MatrixTransform> matrix = new osg::MatrixTransform;

    matrix->setMatrix(osg::Matrix::translate(0,0,2));
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
    osg::ref_ptr<osg::Vec3dArray> vex = new osg::Vec3dArray;
    constexpr float len = -5.0;
    vex->push_back(osg::Vec3(-len, 0.0, 0.0));
	vex->push_back(osg::Vec3( len, 0.0, 0.0));
	vex->push_back(osg::Vec3( len, 0.0, len));
	vex->push_back(osg::Vec3(-len, 0.0, len));
    geom->setVertexArray(vex);

    // osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // colors->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
	// colors->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
	// colors->push_back(osg::Vec4(1.0, 0.0, 1.0, 1.0));
	// colors->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
    // geom->setColorArray(colors);
    
    // geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

    osg::ref_ptr<osg::Vec3Array> normal = new osg::Vec3Array;
    normal->push_back(osg::Vec3(0,-1,0));
    geom->setNormalArray(normal);
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

    osg::ref_ptr<osg::DrawElementsUByte> primitiveSet = new osg::DrawElementsUByte(osg::PrimitiveSet::TRIANGLES);

    primitiveSet->push_back(0);
    primitiveSet->push_back(1);
    primitiveSet->push_back(2);

    primitiveSet->push_back(0);
    primitiveSet->push_back(2);
    primitiveSet->push_back(3);
    
	geom->addPrimitiveSet(primitiveSet);
    //设置线宽
	osg::ref_ptr<osg::LineWidth> lw = new osg::LineWidth(6.0);
	geom->getOrCreateStateSet()->setAttribute(lw, osg::StateAttribute::ON);
    geom->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::ON);

    osg::ref_ptr<osg::Image> img = osgDB::readImageFile("/media/tu/Work/Datas/OpenSceneGraph-Data/Images/skymap.jpg");
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
    texture->setImage(img);
    texture->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR_MIPMAP_LINEAR);
    texture->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR);
    texture->setWrap(osg::Texture::WRAP_S,osg::Texture::REPEAT);
    texture->setWrap(osg::Texture::WRAP_T,osg::Texture::REPEAT);
    texture->setDataVariance(osg::Object::DYNAMIC);
    
    osg::ref_ptr<osg::Vec2Array> vt = new osg::Vec2Array();
    vt->push_back(osg::Vec2(0,0));
    vt->push_back(osg::Vec2(1,0));
    vt->push_back(osg::Vec2(1,1));
    vt->push_back(osg::Vec2(0,1));
    geom->setTexCoordArray(0,vt);

    geom->getOrCreateStateSet()->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);

    osg::ref_ptr<osg::Material> material = new osg::Material;
    material->setColorMode(osg::Material::ColorMode::AMBIENT);
    material->setAmbient(osg::Material::Face::FRONT_AND_BACK,osg::Vec4(1,1,1,1));
    material->setSpecular(osg::Material::Face::FRONT_AND_BACK,osg::Vec4(1,1,1,1));
    material->setShininess(osg::Material::Face::FRONT_AND_BACK,10.0);
    material->setDiffuse(osg::Material::Face::FRONT_AND_BACK,osg::Vec4(1,0,0,1));


    geom->getOrCreateStateSet()->setAttributeAndModes(material,osg::StateAttribute::ON);

    matrix->addChild(geom);

    createLight(root);
    root->addChild(matrix);
    QApplication a(argc, argv);
    OsgViewerWidget viewer(NULL,root);

    viewer.show();

    
    return a.exec();
}