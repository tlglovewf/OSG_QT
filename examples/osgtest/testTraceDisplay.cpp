#include <iostream>
#include <osgViewer/Viewer>
#include <QApplication>
#include <osg/Geometry>
#include <osg/Point>
#include <osg/LineWidth>
#include <osgGA/GUIEventHandler>
#include <osgGA/CameraManipulator>
#include <osgGA/FirstPersonManipulator>
#include <osgGA/OrbitManipulator>
#include "OSGViewWidget"
//#include "SceneCameraManipulator.h"

#include "GeoSceneCamera.h"


#include <QPushButton>
#include <QMainWindow>
#include <QLayout>
class Axis : public osg::Geometry
{
public:
	Axis()
	{
		build_mesh();
	}
protected:

	void build_mesh()
	{
		constexpr float totalLen = 60;

		osg::ref_ptr<osg::Vec3dArray>           vex = new osg::Vec3dArray;
		osg::ref_ptr<osg::Vec4Array>            colors = new osg::Vec4Array;
		osg::ref_ptr<osg::DrawElementsUByte>    primitiveSet = new osg::DrawElementsUByte(osg::PrimitiveSet::LINES);

		vex->push_back(osg::Vec3(0, 0, 0));
		vex->push_back(osg::Vec3(totalLen, 0, 0));
		vex->push_back(osg::Vec3(0, totalLen, 0));
		vex->push_back(osg::Vec3(0, 0, totalLen));

		colors->push_back(osg::Vec4(1, 1, 1, 1));
		colors->push_back(osg::Vec4(1, 0, 0, 1));
		colors->push_back(osg::Vec4(0, 1, 0, 1));
		colors->push_back(osg::Vec4(0, 0, 1, 1));

		primitiveSet->push_back(0);
		primitiveSet->push_back(1);

		primitiveSet->push_back(0);
		primitiveSet->push_back(2);

		primitiveSet->push_back(0);
		primitiveSet->push_back(3);

		this->setVertexArray(vex);
		this->setColorArray(colors);

		this->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

		addPrimitiveSet(primitiveSet);

		osg::ref_ptr<osg::LineWidth> linewidth = new osg::LineWidth(2);
		this->getOrCreateStateSet()->setAttribute(linewidth, osg::StateAttribute::ON);
		this->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		this->getOrCreateStateSet()->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
	}

protected:
};


const GpsPos testgeos[2] = { GeoSceneCamera::lnglat2mector({116.3277796, 39.8445912, 0.0}),
							 GeoSceneCamera::lnglat2mector({116.3038648, 39.9167585, 0.0}) };
GpsPos center = (testgeos[0] + testgeos[1]) / 2.0;
class TraceLine : public osg::Geometry
{
public:
	TraceLine()
	{
		build_mesh();
	}

protected:

	void build_mesh()
	{
		osg::ref_ptr<osg::Vec3dArray>   veices = new osg::Vec3dArray;
		osg::ref_ptr<osg::Vec4Array>    color = new osg::Vec4Array;
		osg::ref_ptr<osg::DrawElementsUShort> primitiveset = new osg::DrawElementsUShort(osg::PrimitiveSet::LINES);

		veices->push_back(testgeos[0]);
		veices->push_back(testgeos[1]);

		color->push_back({ 1.0,0.0,0.0,1.0 });
		color->push_back({ 0.0,1.0,0.0,1.0 });
		primitiveset->push_back(0);
		primitiveset->push_back(1);

		setVertexArray(veices);
		setColorArray(color);
		setColorBinding(osg::Geometry::BIND_PER_VERTEX);
		addPrimitiveSet(primitiveset);

		osg::ref_ptr<osg::LineWidth> linewidth = new osg::LineWidth(3);
		this->getOrCreateStateSet()->setAttribute(linewidth, osg::StateAttribute::ON);
		this->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		this->getOrCreateStateSet()->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
		//this->getOrCreateStateSet()->setAttribute(new osg::Point(5));
	}
};

class Chess : public osg::Geometry
{
public:

	Chess()
	{
		build_mesh();
	}

protected:

	void build_mesh()
	{

		constexpr float totalLen = 300;
		constexpr float stride = 30;
		constexpr int   len = (int)(totalLen / stride) << 1;
		constexpr int   quadsz = (len - 1) * (len - 1);
		static_assert(len, "len value error.");

		osg::ref_ptr<osg::Vec3dArray>           vex = new osg::Vec3dArray;
		osg::ref_ptr<osg::Vec4Array>            colors = new osg::Vec4Array;
		osg::ref_ptr<osg::DrawElementsUShort>   primitiveSet = new osg::DrawElementsUShort(osg::PrimitiveSet::LINE_STRIP);
		for (int i = 0; i <= len; ++i)
		{
			float row = -totalLen + i * stride;

			for (int j = 0; j <= len; ++j)
			{
				float col = -totalLen + j * stride;
				vex->push_back(osg::Vec3(row, col, 0));
				colors->push_back(osg::Vec4(1, 0, 0, 1));
				int index = primitiveSet->size();
				primitiveSet->push_back(index);
			}
		}

		this->setVertexArray(vex);
		this->setColorArray(colors);

		this->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

		addPrimitiveSet(primitiveSet);

		osg::ref_ptr<osg::LineWidth> linewidth = new osg::LineWidth(3);
		this->getOrCreateStateSet()->setAttribute(linewidth, osg::StateAttribute::ON);
		this->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		this->getOrCreateStateSet()->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
		this->getOrCreateStateSet()->setAttribute(new osg::Point(5));
	}

protected:
};

int main(int argc, char **argv)
{
	QApplication a(argc, argv);

	osg::ref_ptr<Chess> node = new Chess();

	osg::ref_ptr<Axis>  axis = new Axis();

	osg::ref_ptr<TraceLine> traceline = new TraceLine();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//root->addChild(node);
	//root->addChild(axis);

	root->addChild(traceline);

	osg::DisplaySettings::instance()->setNumMultiSamples(4);

	QMainWindow main;
	QVBoxLayout layer;
	main.setCentralWidget(new QWidget);
	main.centralWidget()->setLayout(&layer);

	main.setWindowTitle("hello world");

	OsgViewerWidget viewer(NULL, root);

	QPushButton *button = new QPushButton();
	button->setText("world");
	layer.addWidget(button);
	layer.addWidget(&viewer);

	osg::ref_ptr<GeoSceneCamera> geocam = new GeoSceneCamera;
	center.z() = 1.0f;
	geocam->setCameraPos(center);
	viewer.getOsgViewer()->setCameraManipulator(geocam);

	float ratio = viewer.width() / viewer.height();

	viewer.getOsgViewer()->getCamera()->getProjectionMatrix().makePerspective(45.0f, ratio, 1, 1000);

	main.show();

	a.exec();
	return 0;
}