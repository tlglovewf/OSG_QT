#include "SceneCameraManipulator.h"
#include "osgViewer/Viewer"
#include <iostream>

osg::Matrixd SceneCamManipulator::getMatrix()const
{
    auto mat = mRotate * mTrans;
    return std::move(mat);
}
 
osg::Matrixd SceneCamManipulator::getInverseMatrix()const
{
    return osg::Matrixd::inverse(getMatrix());
}


//屏幕坐标转世界坐标
osg::Vec3 SceneCamManipulator::viewToWolrd(osg::ref_ptr<osg::Camera> cam, const osg::Vec3 &viewpt)
{	   
	if (cam.valid())
	{
		osg::Matrix viewmtrx = cam->getViewMatrix();

		osg::Matrix projmtrx = cam->getProjectionMatrix();

		osg::Matrix vpotmtrx = cam->getViewport()->computeWindowMatrix();

		return viewpt * osg::Matrix::inverse(viewmtrx * projmtrx * vpotmtrx);
	}
	else
	{
		return osg::Vec3();
	}
}

//世界坐标转经纬度坐标
osg::Vec3 SceneCamManipulator::worldToView(osg::ref_ptr<osg::Camera> cam, const osg::Vec3 &wdpt)
{
	if (cam.valid())
	{
		osg::Matrix viewmtrx = cam->getViewMatrix();

		osg::Matrix projmtrx = cam->getProjectionMatrix();

		osg::Matrix vpotmtrx = cam->getViewport()->computeWindowMatrix();

		return wdpt * viewmtrx * projmtrx * vpotmtrx;
	}
	else
	{
		return osg::Vec3();
	}
}

bool SceneCamManipulator::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us)
{
	osgViewer::View *view = dynamic_cast<osgViewer::View*>(&us);
    switch (ea.getEventType())
    {
         case osgGA::GUIEventAdapter::SCROLL:
         {
             constexpr float det = 10.0f;
             if(ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_UP)
             {//UP
                mCamPt.z() += det;
             }
             else
            {//DOWN
                mCamPt.z() -= det;
            }
            if(mCamPt.z() < 1)
            {
                mCamPt.z() = 1;
            }
            mTrans.makeTranslate(mCamPt);
         }
            break;
        case osgGA::GUIEventAdapter::PUSH:
            {   
                switch (ea.getButtonMask())
                {
                    case osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON:
                    {
                        mRhtBtnDwn = true;
                        mRhtPt.x() = ea.getX();
                        mRhtPt.y() = ea.getY();
                    }
                        break;
                    case osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON:
                    {
                        mLftBtnDwn = true;
                        mLftPt.x() = ea.getX();
                        mLftPt.y() = ea.getY();
                    }
                        break;
                    case osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON:
                    {
                        //add more information ...
                    }
                        break;
                    default:
                        break;
                }
            }
            break;

        case osgGA::GUIEventAdapter::RELEASE:
            {
                if(!ea.getButtonMask())
                {
                    if(mLftBtnDwn)
                    {
                        mCamPt = mTrans.getTrans();

                        mLftBtnDwn = false;
                    }
                    else if(mRhtBtnDwn)
                    {
                        mQRot = mRotate.getRotate();
                        mRhtBtnDwn = false;
                    }
                    else
                    {
                        ;
                    }
                    
                    return true;
                }
               switch (ea.getButtonMask())
                {
                    case osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON:
                    {
                        mRhtBtnDwn = false;
                    }
                        break;
                    case osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON:
                    {
                        mLftBtnDwn = false;
                    }
                        break;
                    case osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON:
                    {
                        
                    }
                        break;
                    default:
                        break;
                }
            }
            break;
        case osgGA::GUIEventAdapter::DRAG:
        {
            osg::Vec2 temp(ea.getX(), ea.getY());
            if(mRhtBtnDwn)
            {
                osg::Quat rotX;
                osg::Quat rotY;

                osg::Vec2 det = (temp - mRhtPt);

                constexpr float scale = 100.0f;
                rotX.makeRotate(det.y() / scale , osg::Vec3(1,0,0));
                rotY.makeRotate(det.x() / scale , osg::Vec3(0,1,0));

                auto change = mQRot * rotX * rotY;

                mRotate.makeRotate(change);
            }

            if(mLftBtnDwn)
            {
				osg::Camera *cam = view->getCamera();
                osg::Vec3 panPt(mCamPt);

				osg::Vec3 pt1 = viewToWolrd(cam, osg::Vec3(temp.x(), temp.y(), 0.0));

				osg::Vec3 pt2 = viewToWolrd(cam, osg::Vec3(mLftPt.x(), mLftPt.y(),0.0));

				osg::Vec3 det = pt1 - pt2;
				std::cout << det.x() << " " << det.y() << std::endl;

                panPt.x() = mCamPt.x() - det.x();
                panPt.y() = mCamPt.y() - det.y();
                
                mTrans.makeTranslate(panPt);
            }
        }
            break;
        case osgGA::GUIEventAdapter::KEY_Space:
        {
             setOrigin();
        }
            break;
        default:
            //add more
            break;
    }

    return true;
}