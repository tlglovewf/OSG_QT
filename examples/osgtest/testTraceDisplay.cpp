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
#include <osgFX/OutLine>
#include <osgUtil/LineSegmentIntersector>
#include <osg/ShapeDrawable>

#include <osg/LineSegment>

#include "OSGViewWidget"
//#include "SceneCameraManipulator.h"

#include "GeoSceneCamera.h"


#include <QPushButton>
#include <QMainWindow>
#include <QLayout>

#include <QDebug>

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

		osg::ref_ptr<osg::Vec3Array>           vex = new osg::Vec3Array;
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


//const GpsPos testgeos[2] = { GeoSceneCamera::lnglat2mector({116.3277796, 39.8445912, 0.0}),
//							 GeoSceneCamera::lnglat2mector({116.3038648, 39.9167585, 0.0}) };
const GpsPos testgeos[2] = {{-1.0, -1.0, 0.0},
							{ 1.0,  1.0, 0.0} };

GpsPos centerpt = (testgeos[0] + testgeos[1]) / 2.0;
class TraceLine : public osg::Geometry
{
public:
	TraceLine()
	{
		build_mesh();
	}

	void setColor(const osg::Vec4& color)
	{
		osg::Vec4Array* colors = dynamic_cast<osg::Vec4Array*>(_colorArray.get());

		(*colors)[0] = color;
		colors->dirty();
		setColorArray(colors);
		dirtyGLObjects();
	}

	void highlight()
	{
		setColor({ 0.0,1.0,0.0,1.0 });
	}
protected:


	void build_mesh()
	{
		osg::ref_ptr<osg::Vec3Array>   veices = new osg::Vec3Array;
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

		osg::ref_ptr<osg::Vec4Array> seccolor = new osg::Vec4Array;
		color->push_back({ 0.0,0.0,1.0,1.0 });
		setSecondaryColorArray(seccolor);
		setSecondaryColorBinding(osg::Geometry::BIND_OVERALL);

		addPrimitiveSet(primitiveset);

		osg::ref_ptr<osg::LineWidth> linewidth = new osg::LineWidth(3);
		this->getOrCreateStateSet()->setAttribute(linewidth, osg::StateAttribute::ON);
		this->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		this->getOrCreateStateSet()->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);

		
	}
	const osg::Array* getColorArray()const
	{
		qDebug() << "error" << endl;
		return _colorArray.get();
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

		osg::ref_ptr<osg::Vec3Array>           vex = new osg::Vec3Array;
		osg::ref_ptr<osg::Vec4Array>            colors = new osg::Vec4Array;
		osg::ref_ptr<osg::DrawElementsUShort>   primitiveSet = new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLES);

		vex->push_back({ -1,-1,0 });
		vex->push_back({ 1, 1,0 });
		vex->push_back({ 1,-1,0 });
		colors->push_back({ 1.0, 0.0, 0.0, 1.0 });
		colors->push_back({ 0.0, 1.0, 0.0, 1.0 });
		colors->push_back({ 0.0, 0.0, 1.0, 1.0 });
		primitiveSet->push_back(0);
		primitiveSet->push_back(1);
		primitiveSet->push_back(2);

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

class ObjectSelectHandler : public osgGA::GUIEventHandler
{
public:
	virtual bool handle(const osgGA::GUIEventAdapter&ea, osgGA::GUIActionAdapter&aa)override
	{
		osgViewer::View* view = dynamic_cast<osgViewer::Viewer*>(&aa);
		if (!view) {
			return false;
		}
		osgGA::GUIEventAdapter::EventType eventType = ea.getEventType(); 
		switch (eventType) {
		case osgGA::GUIEventAdapter::PUSH://��갴��
			if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) {

				pick(view, { ea.getX(), ea.getY() });
			}
			break;
		case osgGA::GUIEventAdapter::DRAG://����϶�

			break;
		case osgGA::GUIEventAdapter::RELEASE://����ɿ�
			if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) {

			}
			break;
		default:
			break;
		}

		return false;
	}

	void pick(osgViewer::View *viewer, const osg::Vec2 pt)
	{
		constexpr float spaclen = 5;
		osgUtil::PolytopeIntersector *intersector = new osgUtil::PolytopeIntersector(osgUtil::Intersector::WINDOW, pt.x() - spaclen, pt.y() - spaclen, pt.x() + spaclen, pt.y() + spaclen);

		osgUtil::IntersectionVisitor iv(intersector);

		osg::Camera* camera = viewer->getCamera();
		if (!camera) return ;
		camera->accept(iv);
			 
		if (!iv.getIntersector()->containsIntersections() && !intersector->containsIntersections()) {
			std::cout << "no intersections found" << std::endl;
			return ;
		}
		else
		{
			std::cout << "select one." << std::endl;
			const osgUtil::PolytopeIntersector::Intersection &rst = *intersector->getIntersections().begin();

			TraceLine *shape = dynamic_cast<TraceLine*>(rst.drawable.get()); 
			if (shape)
				shape->highlight();
		}
		return;
	}
};


int main(int argc, char **argv)
{
	QApplication a(argc, argv);

	osg::ref_ptr<Chess> node = new Chess();

	osg::ref_ptr<Axis>  axis = new Axis();

	osg::ref_ptr<TraceLine> traceline = new TraceLine();

	osg::ref_ptr<osg::Group> root = new osg::Group();


	auto shape = new osg::ShapeDrawable(new osg::Box(osg::Vec3(), 2));
	shape->setColor(osg::Vec4(1.0, 0.0, 0.0, 1.0));

	//root->addChild(node);
	//root->addChild(axis);

	//root->addChild(shape);
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
	centerpt.z() = 1.0f;
	geocam->setCameraPos(centerpt);
	viewer.getOsgViewer()->setCameraManipulator(geocam);
	viewer.getOsgViewer()->addEventHandler(new ObjectSelectHandler());

	float ratio = viewer.width() / viewer.height();

	viewer.getOsgViewer()->getCamera()->getProjectionMatrix().makePerspective(45.0f, ratio, 1, 1000);
    

	main.show();

	a.exec();
	return 0;
}