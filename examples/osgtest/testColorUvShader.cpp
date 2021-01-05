#include "OSGViewWidget"
#include <osgDB/ReadFile>

#include <osg/Camera>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/LineWidth>
#include <osg/BlendFunc>
#include <osg/LightSource>
#include <osgViewer/Viewer>

#include <string>

#include <QApplication>

static void  ColorShader(osg::ref_ptr<osg::Node> node)
{
    const char *vertexShader = {
        "void main(void ){\n"
        "gl_TexCoord[0] = gl_MultiTexCoord0;"
		"   gl_FrontColor = gl_Color;\n"
		"   gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;\n"
		"}\n"
    };
    const char *fragShader   = {
        "uniform sampler2D baseTexture;\n"
		"void main(void){\n"
        "   vec2 coord = gl_TexCoord[0].xy;\n"
        "   vec4 C = texture2D(baseTexture, coord)\n;"
        "	gl_FragColor = gl_Color * C;\n"
        "   gl_FragColor.a = 0.5;\n"
		"}\n"
	};

    osg::StateSet *stateset = node->getOrCreateStateSet();
    osg::ref_ptr<osg::Program> program = new osg::Program();
    program->addShader(new osg::Shader(osg::Shader::VERTEX  , vertexShader));
    program->addShader(new osg::Shader(osg::Shader::FRAGMENT, fragShader));
    stateset->setAttributeAndModes(program,osg::StateAttribute::ON);
}

int main(int argc, char **argv)
{

    osg::ref_ptr<osg::MatrixTransform> matrix = new osg::MatrixTransform;

    matrix->setMatrix(osg::Matrix::translate(0,0,2));

    osg::ref_ptr<osg::Vec3dArray> vex = new osg::Vec3dArray;
    constexpr float len = -5.0;
    vex->push_back(osg::Vec3(-len, 0.0, 0.0));
	vex->push_back(osg::Vec3( len, 0.0, 0.0));
	vex->push_back(osg::Vec3( len, 0.0, len));
	vex->push_back(osg::Vec3(-len, 0.0, len));

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
	colors->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
	colors->push_back(osg::Vec4(1.0, 0.0, 1.0, 1.0));
	colors->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
    geom->setVertexArray(vex);
    geom->setColorArray(colors);
    
    geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

    // osg::ref_ptr<osg::PrimitiveSet> primitiveSet = new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4);

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
    geom->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

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

    geom->getOrCreateStateSet()->setMode(GL_BLEND,osg::StateAttribute::ON);

    osg::ref_ptr<osg::BlendFunc> func = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);

    geom->getOrCreateStateSet()->setAttributeAndModes(func,osg::StateAttribute::ON);

    ColorShader(geom);

    matrix->addChild(geom);

    QApplication a(argc, argv);

    OsgViewerWidget viewer(NULL,matrix);

    viewer.show();

    return a.exec();
}