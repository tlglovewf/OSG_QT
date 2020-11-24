#include "OSGViewWidget"
#include "osgQOpenGL/OSGRenderer"
#include <QScreen>
#include <QWindow>
#include <QApplication>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>

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
        m_renderer->setCameraManipulator(new osgGA::TrackballManipulator());
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