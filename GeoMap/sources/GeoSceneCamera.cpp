#include "GeoSceneCamera.h"
#include "osgViewer/Viewer"
#include "osgUtil/LineSegmentIntersector"
#include <qdebug.h>

void GeoSceneCamera::updateTrans()
{	
	mTrans.setTrans(mObjPt - mCamPt );
}

void GeoSceneCamera::setOrigin()
{
	mCamPt = mOriPt;
	mObjPt.set(0.0, 0.0, 0.0);
	mTrans.makeIdentity();
	mTrans.makeTranslate(mCamPt);
	mRot = osg::Quat();
}

osg::Matrixd GeoSceneCamera::getMatrix()const
{
	return mTrans; 
}

osg::Matrixd GeoSceneCamera::getInverseMatrix()const
{
	if (mType == CameraType::eThirdMode)
		return osg::Matrixd::rotate(mRot) * osg::Matrixd::translate(-mCamPt);
	else
		return osg::Matrixd::translate(-mCamPt) * osg::Matrixd::rotate(mRot) ;
}




//屏幕坐标转世界坐标
osg::Vec3 GeoSceneCamera::viewToWolrd(osg::ref_ptr<osg::Camera> cam, const osg::Vec3 &viewpt)
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
osg::Vec3 GeoSceneCamera::worldToView(osg::ref_ptr<osg::Camera> cam, const osg::Vec3 &wdpt)
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

//处理事件
bool GeoSceneCamera::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
	osgViewer::View *view = dynamic_cast<osgViewer::View*>(&us);
	switch (ea.getEventType())
	{
	case osgGA::GUIEventAdapter::SCROLL:
	{
		float det = mCamPt.z() / 10.0;
		if (ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_UP)
		{//UP
			mCamPt.z() += det;
		}
		else
		{//DOWN
			mCamPt.z() -= det;
		}
	}
	break;
	case osgGA::GUIEventAdapter::PUSH:
	{
		
		mLstPt = mCamPt;
		mLstRt = mRot;
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
			mLftPt.x() = ea.getX() ;
			mLftPt.y() = ea.getY() ;
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
		if (!ea.getButtonMask())
		{
			if (mLftBtnDwn)
			{
				mLftBtnDwn = false;
			}
			else if (mRhtBtnDwn)
			{
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
		if (mRhtBtnDwn)
		{
			osg::Quat rotX;
			osg::Quat rotZ;

			osg::Camera *cam = view->getCamera();
			
			osg::Vec2 det = (temp - mRhtPt);
		
			constexpr float scale = 2.0;//
			float h = (mRhtPt.x() - 0) * scale;
			

			if (mType == CameraType::eThirdMode)
			{
				
				osg::Vec3 xaxis(1.0, 0.0, 0.0);
				osg::Vec3 zaxis(0.0, 0.0, 1.0);

				osg::Quat rotX;
				osg::Quat rotY;
				rotX.makeRotate(scale * det.y() / h, xaxis);
				rotY.makeRotate(scale * det.x() / h, zaxis);

				mRot = mLstRt * rotX * rotY;

			}
			else
			{
				osg::Vec3 xaxis(1.0, 0.0, 0.0);
				osg::Vec3 yaxis(0.0, 1.0, 0.0);

				osg::Quat rotX;
				osg::Quat rotY;
				rotX.makeRotate(scale * det.y() / h, xaxis);
				rotY.makeRotate(scale * det.x() / h, yaxis);

				mRot = mLstRt * rotX * rotY;
				 
			}
			
		}

		if (mLftBtnDwn)
		{
			osg::Camera *cam = view->getCamera();
			

			osg::Vec3 pt1 = viewToWolrd(cam, osg::Vec3(temp.x(), temp.y(), 0.0));

			osg::Vec3 pt2 = viewToWolrd(cam, osg::Vec3(mLftPt.x(), mLftPt.y(), 0.0));

			osg::Vec3 det = pt1 - pt2;
			qDebug() << det.x() << " " << det.y() << " " << det.z() << endl;
			if (mType == CameraType::eFirstMode)
			{
				mCamPt = mLstPt -  det ;
			}
			else
			{
				mCamPt = mLstPt - mRot * osg::Vec3(det.x(), det.y(), 0);
			}
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