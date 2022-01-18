#include <iostream>
#include <osgViewer/Viewer>
#include <QApplication>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/Point>
#include <osg/Multisample>
#include <osg/LineWidth>
#include <osgGA/GUIEventHandler>
#include <osgGA/CameraManipulator>
#include <osgGA/FirstPersonManipulator>
#include <osgGA/OrbitManipulator>
#include <osgFX/OutLine>
#include <osgUtil/LineSegmentIntersector>
#include <osg/ShapeDrawable>
#include <osgText/Text>
#include <osg/LineSegment>

#include "OSGViewWidget"
#include "osg/Transform"
//#include "SceneCameraManipulator.h"

#include "GeoSceneCamera.h"


#include <QPushButton>
#include <QMainWindow>
#include <QLayout>

#include <QDebug>

constexpr int Radius_Len = 1e4;
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
		constexpr float totalLen = Radius_Len;
		constexpr float stride = 100;
		constexpr int   len = (int)(totalLen / stride) << 1;
		static_assert(len, "len value error.");

		osg::ref_ptr<osg::Vec3Array>           vex = new osg::Vec3Array;
		osg::ref_ptr<osg::Vec4Array>            colors = new osg::Vec4Array;
		
		osg::ref_ptr<osg::DrawElementsUInt>   primitiveSet = new osg::DrawElementsUInt(osg::PrimitiveSet::POINTS);
		for (int i = 0; i <= len; ++i)
		{
			float row = -totalLen + i * stride;

			for (int j = 0; j <=  len; ++j)
			{
				float col = -totalLen + j * stride;
				vex->push_back(osg::Vec3(row, col, 0));
				colors->push_back(osg::Vec4(0, 0.8, 0, 1));
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
		this->getOrCreateStateSet()->setAttribute(new osg::Point(3));
	}

protected:
};

class BackPlane : public osg::Geometry
{
public:
	BackPlane()
	{
		build_mesh();
	}

protected:
	void build_mesh()
	{
		constexpr float totalLen = Radius_Len;

		osg::ref_ptr<osg::Vec3Array>           vex = new osg::Vec3Array;
		osg::ref_ptr<osg::Vec4Array>            colors = new osg::Vec4Array;
		osg::ref_ptr<osg::DrawElementsUShort>   primitiveSet = new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLES);
	
		vex->push_back({ -totalLen,  totalLen,0 });
		vex->push_back({ -totalLen, -totalLen,0 });
		vex->push_back({  totalLen, -totalLen,0 });
		vex->push_back({  totalLen,  totalLen,0 });

		primitiveSet->push_back(0);
		primitiveSet->push_back(1);
		primitiveSet->push_back(2);
		primitiveSet->push_back(0);
		primitiveSet->push_back(2);
		primitiveSet->push_back(3);

		colors->push_back({ 0.1,1.0,0.0,0.4 });


		this->setVertexArray(vex);
		this->setColorArray(colors);

		this->setColorBinding(osg::Geometry::BIND_OVERALL);

		addPrimitiveSet(primitiveSet);

		this->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		this->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
		this->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
	}
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
		case osgGA::GUIEventAdapter::PUSH://鼠标按下
			if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) {

				pick(view, { ea.getX(), ea.getY() });
			}
			break;
		case osgGA::GUIEventAdapter::DRAG://鼠标拖动

			break;
		case osgGA::GUIEventAdapter::RELEASE://鼠标松开
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
		return;
		constexpr float spaclen = 5;
		//osgUtil::PolytopeIntersector *intersector = new osgUtil::PolytopeIntersector(osgUtil::Intersector::WINDOW, pt.x() - spaclen, pt.y() - spaclen, pt.x() + spaclen, pt.y() + spaclen);
		osgUtil::LineSegmentIntersector *intersector = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, pt.x(), pt.y());
		qDebug() << "-- " << pt.x() << " " << pt.y() << endl;
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
			//const osgUtil::PolytopeIntersector::Intersection &rst = *intersector->getIntersections().begin();
			const osgUtil::LineSegmentIntersector::Intersection &rst = *intersector->getIntersections().begin();
			//TraceLine *shape = dynamic_cast<TraceLine*>(rst.drawable.get()); 
			//if (shape)
			//	shape->highlight();
 
			qDebug() << rst.getWorldIntersectPoint().x() << " " << rst.getWorldIntersectPoint().y() << " " << rst.getWorldIntersectPoint().z() << endl;
			
		}
		return;
	}
};

int main(int argc, char **argv)
{	 
	QSurfaceFormat format = QSurfaceFormat::defaultFormat();

#ifdef OSG_GL3_AVAILABLE
	format.setVersion(3, 2);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setOption(QSurfaceFormat::DebugContext);
#else
	format.setVersion(2, 0);
	format.setProfile(QSurfaceFormat::CompatibilityProfile);
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setOption(QSurfaceFormat::DebugContext);
#endif
	format.setDepthBufferSize(24);
	//format.setAlphaBufferSize(8);0,
	format.setSamples(16);
	format.setStencilBufferSize(8);
	format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
	QSurfaceFormat::setDefaultFormat(format);

	QApplication a(argc, argv);

	osg::ref_ptr<Chess> chess = new Chess();

	osg::ref_ptr<Axis>  axis = new Axis();

	osg::ref_ptr<TraceLine> traceline = new TraceLine();

	osg::ref_ptr<osg::Group> root = new osg::Group();
	osg::Multisample* pms = new osg::Multisample;
	pms->setSampleCoverage(1, true);
	root->getOrCreateStateSet()->setAttribute(pms, osg::StateAttribute::ON);
	osg::ref_ptr<osg::MatrixTransform> transnode = new osg::MatrixTransform();
	transnode->addChild(axis);
	transnode->setMatrix(osg::Matrix::rotate(30, osg::Vec3d(0, 0, 1.0)));
	auto shape = new osg::ShapeDrawable(new osg::Box(osg::Vec3(), 2));
	shape->setColor(osg::Vec4(1.0, 0.0, 0.0, 1.0));
	auto plane = new BackPlane();

	root->addChild(plane);
	root->addChild(chess);
	root->addChild(transnode);
	root->addChild(shape);
	root->addChild(traceline);
	osg::DisplaySettings::instance()->setNumMultiSamples(8);

	osg::ref_ptr<osg::Camera> camera = new osg::Camera();
	camera->setProjectionMatrix(osg::Matrix::ortho2D(-512, 512, -360, 360));
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrix(osg::Matrix::identity());
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setAllowEventFocus(false);
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	auto text = new osgText::Text;
	//auto font = osgText::readFontFile("C:/Windows/Fonts/SIMYOU.TTF");	  需要编译osgdb_freetype插件库
	//text->setFont(font);
	text->setCharacterSize(16);
	text->setPosition(osg::Vec3(0.0, 10.0f, 0.0f));
	text->setColor(osg::Vec4(1, 0, 0, 1));
	text->setDataVariance(osg::Object::DYNAMIC);
	text->setText(L"Hello World.");

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(text);
	osg::ref_ptr<osg::StateSet> stateset = geode->getOrCreateStateSet();

	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
	camera->addChild(geode);
	camera->addChild(transnode);

	QMainWindow main;
	main.setFixedSize(QSize(1024, 720));
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
	geocam->setCameraType(GeoSceneCamera::CameraType::eThirdMode);
	centerpt.z() = 100.0f;
	geocam->setCameraPos(centerpt);
	viewer.getOsgViewer()->setCameraManipulator(geocam);
	viewer.getOsgViewer()->addEventHandler(new ObjectSelectHandler());

	root->addChild(camera);
	float ratio = viewer.width() / viewer.height();

	viewer.getOsgViewer()->getCamera()->getProjectionMatrix().makePerspective(45.0f, ratio, 1, 1000);

	main.show();

	a.exec();
	return 0;
}