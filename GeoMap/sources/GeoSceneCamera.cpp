#include "GeoSceneCamera.h"


void GeoSceneCamera::updateCamera()
{
	mTrans.makeTranslate(mCamPt);
}

void GeoSceneCamera::setOrigin()
{
	mCamPt = mOriPt;
	mTrans.makeTranslate(mCamPt);
}

osg::Matrixd GeoSceneCamera::getMatrix()const
{
	return mTrans;
}

osg::Matrixd GeoSceneCamera::getInverseMatrix()const
{
	return osg::Matrixd::inverse(getMatrix());
}

#include <qdebug.h>

//处理事件
bool GeoSceneCamera::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
	switch (ea.getEventType())
	{
	case osgGA::GUIEventAdapter::SCROLL:
	{
		//constexpr float det =  // 1.0f;
		float det = 0.2f;
		if (ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_UP)
		{//UP
			mCamPt.z() += det;
		}
		else
		{//DOWN
			mCamPt.z() -= det;
		}
		if (mCamPt.z() < 0.1)
		{
			mCamPt.z() = 0.1;
		}
		mTrans.makeTranslate(mCamPt);
	}
	break;
	case osgGA::GUIEventAdapter::PUSH:
	{
		switch (ea.getButtonMask())
		{
		//case osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON:
		//{
		//	mRhtBtnDwn = true;
		//	mRhtPt.x() = ea.getX();
		//	mRhtPt.y() = ea.getY();
		//}
		//break;
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
				mCamPt = mTrans.getTrans();

				mLftBtnDwn = false;
			}
			//else if (mRhtBtnDwn)
			//{
			//	mQRot = mRotate.getRotate();
			//	mRhtBtnDwn = false;
			//}
			else
			{
				;
			}

			return true;
		}
		switch (ea.getButtonMask())
		{
		//case osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON:
		//{
		//	mRhtBtnDwn = false;
		//}
		//break;
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
		//if (mRhtBtnDwn)
		//{
		//	osg::Quat rotX;
		//	osg::Quat rotY;
		//
		//	osg::Vec2 det = (temp - mRhtPt);
		//
		//	constexpr float scale = 100.0f;
		//	rotX.makeRotate(det.y() / scale, osg::Vec3(1, 0, 0));
		//	rotY.makeRotate(det.x() / scale, osg::Vec3(0, 1, 0));
		//
		//	auto change = mQRot * rotX * rotY;
		//
		//	mRotate.makeRotate(change);
		//}

		if (mLftBtnDwn)
		{
			osg::Vec3 panPt(mCamPt);

			osg::Vec2 det = (temp - mLftPt);

			panPt.x() = mCamPt.x() - det.x() / (100 * mCamPt.z());
			panPt.y() = mCamPt.y() - det.y() / (100 * mCamPt.z());
			qDebug() << det.x() << " " << det.y() << " " <<  mCamPt.z() << endl;
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