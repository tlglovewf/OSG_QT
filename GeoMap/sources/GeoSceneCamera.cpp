#include "GeoSceneCamera.h"
#include "osgViewer/Viewer"
#include "osgUtil/RayIntersector"
#include <qdebug.h>

void GeoSceneCamera::setOrigin()
{
	mCamPt = mOriPt;
	mObjPt.set(0.0, 0.0, 0.0);
	mTrans.makeIdentity();
	mTrans.makeTranslate(mCamPt);
	mRot = osg::Quat();
	mUp = osg::Vec3(0, 1, 0);
}

osg::Matrixd GeoSceneCamera::getMatrix()const
{
	return mTrans; 
}

osg::Matrixd GeoSceneCamera::getInverseMatrix()const
{
	if (mType == CameraType::eThirdMode)
	{
		osg::Matrixd mat;
		mat.makeLookAt(mCamPt, mObjPt, mUp);
		return mat;
	}
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

		osg::Vec4 temp = { viewpt,1.0 };
		
		temp = temp * osg::Matrix::inverse(viewmtrx * projmtrx * vpotmtrx);

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
		osg::Vec4 temp = { wdpt, 1.0 };
		temp = temp * viewmtrx;
		osg::Matrix projmtrx = cam->getProjectionMatrix();
		temp = temp * projmtrx;
		osg::Matrix vpotmtrx = cam->getViewport()->computeWindowMatrix();
		temp = temp * vpotmtrx;
		return { temp.x(), temp.y(), temp.z() };
	}
	else
	{
		return osg::Vec3();
	}
}

osg::Vec3  GetWorldPtFromViewPoint(osg::Camera *cam, int x, int y)
{
	osg::ref_ptr<osgUtil::RayIntersector> rayline = new osgUtil::RayIntersector(osgUtil::Intersector::WINDOW, x, y);
	osgUtil::IntersectionVisitor iv;
	iv.setIntersector(rayline);
	cam->accept(iv);
	
	if (!iv.getIntersector()->containsIntersections() && !rayline->containsIntersections())
	{
		return osg::Vec3();
	}
	else
	{
		const osgUtil::RayIntersector::Intersection &rst = *rayline->getIntersections().begin();
		return rst.getWorldIntersectPoint();
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
		auto dir = mObjPt - mCamPt;
		float len = dir.normalize();
		float det = len / 10.0;
		if (ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_UP)
		{//UP
			mCamPt += dir * det;
		}
		else
		{//DOWN
			mCamPt -= dir * det;
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
			osg::Vec2 det = (temp - mRhtPt);
		
			constexpr float scale = 1.0;//
			float h = (mRhtPt.x() - 0) * scale;
			

			if (mType == CameraType::eThirdMode)
			{
				
				osg::Vec3 xaxis(1.0, 0.0, 0.0);
				osg::Vec3 zaxis(0.0, 0.0, 1.0);

				osg::Quat rotX;
				osg::Quat rotY;
				rotY.makeRotate(scale * det.x() / h, zaxis);

				auto dir = mObjPt - mLstPt;
				float len = dir.normalize();
				auto preUp = mUp;
				mUp = rotY * mLstRt.inverse() * mUp;
				mLstRt = rotY;
				osg::Quat qua;
				auto tem = dir ^ preUp;
				rotX.makeRotate(scale * det.y() / h, tem);
				mCamPt = mObjPt - rotX * dir * len;

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
			
			auto pt1 = GetWorldPtFromViewPoint(cam, temp.x(), temp.y());
			auto pt2 = GetWorldPtFromViewPoint(cam, mLftPt.x(), mLftPt.y());

			if (pt1.length() < 1e-6 || pt2.length() < 1e-6)
			{
				qDebug() << "no object selected." << endl;
				return true;
			}
			osg::Vec3 det = pt1 - pt2;
		
			if (mType == CameraType::eFirstMode)
			{
				mCamPt = mLstPt -  det ;
			}
			else
			{
				auto movpt = osg::Vec3(det.x(), det.y(), 0);
				
				auto dir = mObjPt - mCamPt;
				auto len = dir.normalize();

				auto left = dir ^ osg::Vec3(0.0, 1.0, 0.0);
				auto up = left ^ dir;

				mCamPt = mLstPt - left * det.x()  - up * det.y() ;

				mObjPt = mCamPt + dir * len;
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