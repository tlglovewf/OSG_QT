#include "OSGViewWidget"
#include "osgQOpenGL/OSGRenderer"
#include <QScreen>
#include <QWindow>
#include <QApplication>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <iostream>
OsgViewerWidget::OsgViewerWidget(QWidget* parent, osg::Node* scene)
    :osgQOpenGLWidget(parent)
{
    setMouseTracking(true);//鼠标跟踪生效，没有按键按下也会接收鼠标移动事件
    setFocusPolicy(Qt::StrongFocus);//接收键盘按键
    CreateViewer();// ()，导致渲染后创建对象（异步，通过信号才可使用对象）
    if (scene)
    {
        m_renderer->setSceneData(scene);
    }
}
OsgViewerWidget::~OsgViewerWidget()
{
}
void OsgViewerWidget::initializeGL()
{
    initializeOpenGLFunctions();
    setDefaultDisplaySettings();

    emit initialized();

    if (!m_renderer->getCameraManipulator() && m_renderer->getCamera()->getAllowEventFocus())
    {//初始化之前 没有设置漫游器 则默认使用trackball漫游器
        // m_renderer->setCameraManipulator(new osgGA::TrackballManipulator());
        // m_renderer->setCameraManipulator(new osgGA::OrbitManipulator());
        m_renderer->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);

        m_renderer->setCameraManipulator(new MyCameraManipulator(NULL));

        
    }
}

void OsgViewerWidget::CreateViewer()
{
    m_renderer = new OSGRenderer(this);

    QScreen* screen = windowHandle()
        && windowHandle()->screen() ? windowHandle()->screen() :
        qApp->screens().front();
    m_renderer->setupOSG(width(), height(), screen->devicePixelRatio());
    m_renderer->addEventHandler(new osgViewer::StatsHandler());

    m_renderer->setKeyEventSetsDone(0);
    m_renderer->setQuitEventSetsDone(false);

}


MyCameraManipulator::MyCameraManipulator( osg::Node *node):_root(node)
{
    computeHomePosition();
    _eye = _homeEye;
    _eye.y() = -20;
    _up  = _homeUp;
    _dir = _homeCenter - _homeEye;

}

static bool bol = false;
osg::Matrixd MyCameraManipulator::getInverseMatrix()const
{
    osg::Matrix mat;
    // static 
    static osg::Matrixd rot;
    static float angle = 0.0;
    if(!bol)
    {
        rot.makeRotate(angle,osg::Vec3(0,0,1));
        angle += 0.01;
    }
       
    mat.makeLookAt( rot * _eye, _homeCenter, _up);
    return mat;
}

bool MyCameraManipulator::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us)
{
    switch (ea.getEventType())
    {       
    case osgGA::GUIEventAdapter::PUSH:
        if(ea.getButtonMask() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
        {
            bol = true;
        }
        else if(ea.getButtonMask() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
        {
            bol = false;
        }
        std::cout << "button down." << std::endl;
        return true;
    case osgGA::GUIEventAdapter::DRAG:
        {
            std::cout << "drag" << std::endl;
        }
        return true;
    case osgGA::GUIEventAdapter::RELEASE:
    {
        std::cout << "button release." << std::endl;
    }
        return true;
    case osgGA::GUIEventAdapter::FRAME:
    {
        // std::cout << "frame." << std::endl; 
    }
        return true;
    case osgGA::GUIEventAdapter::MOVE:
    {
        // std::cout << "move." << std::endl;
    }
        return true;
    case osgGA::GUIEventAdapter::DOUBLECLICK:
    {
        std::cout << "double click" << std::endl;
    }
        return true;
    case osgGA::GUIEventAdapter::SCROLL:
        {
            osgViewer::Viewer *viewer  = dynamic_cast<osgViewer::Viewer*>(&us);

            osg::Camera *camera = viewer->getCamera();

            osg::Matrixd mvpw = camera->getViewMatrix() * camera->getProjectionMatrix() * camera->getViewport()->computeWindowMatrix();

            osg::Matrixd invmvpw = osg::Matrixd::inverse(mvpw);

            osg::Vec3 mouseWorld = osg::Vec3(ea.getX() , ea.getY(),0) * invmvpw;

            osg::Vec3 dir = mouseWorld - _eye;

            dir.normalize();
            std::cout << "deltaX " << ea.getScrollingDeltaX() << " " << "deltaY " << ea.getScrollingDeltaY() << std::endl; 
    
            if(ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_UP)
            {
                _eye += dir * 10;
            }
            else if(ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_DOWN)
            {
                _eye -= dir * 10;
            }
            else
            {
                ;
            }
        }
        return true;
    default:
        return false;
    }
}