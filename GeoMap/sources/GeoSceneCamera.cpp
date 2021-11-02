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


COCSGroundManipulator::COCSGroundManipulator()
	:inherited(DEFAULT_SETTINGS)
{
	_upVector = osg::Vec3d(0.0, 1.0, 0.0);

	setVerticalAxisFixed(false);
	setAllowThrow(false);
	// 设置其放大缩小方式与cad一样
	double dOld = getWheelZoomFactor();
	setWheelZoomFactor(-1.0 * dOld);

	//  设置当缩放到这个大小后，在缩放的时候，同时移动center
	setMinimumDistance(1000, false);
	SetMaxDistance(100000.0);
}

COCSGroundManipulator::~COCSGroundManipulator()
{

}

osg::Vec3d COCSGroundManipulator::GetUpVector() const
{
	return _upVector;
}

void COCSGroundManipulator::SetUpVector(const osg::Vec3d &vecUp)
{
	_upVector = vecUp;
}

void COCSGroundManipulator::setByMatrix(const osg::Matrixd& matrix)
{
	_center = osg::Vec3d(0., 0., -_distance) * matrix;
	_center.y() = 5000.0;

	// 应该让y轴投影到视平面上后，是竖直的 计算上比较麻烦，所以直接给一个
	osg::Vec3d f = -getUpVector(matrix);//( 1.0, -1.0, 0.0 );
	//f.normalize();
	osg::Vec3d s = getSideVector(matrix);//( 0.0, 0.0, 1.0 );
	osg::Vec3d u(_upVector);//u( 1.0, 1.0, 0.0 );
	s = f ^ u;
	//u.normalize();

	osg::Matrixd rotation_matrix(s[0], u[0], -f[0], 0.0f,
		s[1], u[1], -f[1], 0.0f,
		s[2], u[2], -f[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	_rotation = rotation_matrix.getRotate().inverse();

}

void COCSGroundManipulator::zoomModel(const float dy, bool pushForwardIfNeeded /*= true */)
{
	// dy 放大为正，缩小为负
	// scale
	float scale = 1.0f + dy;

	// minimum distance
	float minDist = _minimumDistance;
	if (getRelativeFlag(_minimumDistanceFlagIndex))
		minDist *= _modelSize;

	double dResult = _distance * scale;
	if (dResult > minDist)
	{
		if (dResult < GetMaxDistance())
		{
			_distance = dResult;
		}
		else
		{
			float scale = -_distance;

			osg::Matrix rotation_matrix;
			rotation_matrix.makeRotate(_rotation);
			osg::Vec3d sideVector = getSideVector(rotation_matrix);

			osg::Vec3d localUp(0.0f, 1.0f, 0.0f);

			osg::Vec3d forwardVector = localUp ^ sideVector;

			forwardVector.normalize();
			osg::Vec3d dv = forwardVector * (dy * scale);

			_center += dv;
		}
		// regular zoom
		//_distance *= scale;
	}
	else
	{
		if (pushForwardIfNeeded)
		{
			// push the camera forward
			float scale = -_distance;

			osg::Matrix rotation_matrix;
			rotation_matrix.makeRotate(_rotation);
			osg::Vec3d sideVector = getSideVector(rotation_matrix);

			osg::Vec3d localUp(0.0f, 1.0f, 0.0f);

			osg::Vec3d forwardVector = localUp ^ sideVector;

			forwardVector.normalize();
			osg::Vec3d dv = forwardVector * (dy * scale);

			_center += dv;
		}
		else
		{
			// set distance on its minimum value
			_distance = minDist;
		}
	}
}


void COCSGroundManipulator::panModel(const float dx, const float dy, const float dz)
{
	double scaley = 1;//0.001 * _distance;
	double scalex = 1;//0.0003 * _distance;
	osg::Matrix rotation_matrix;
	rotation_matrix.makeRotate(_rotation);


	// compute look vector.
	//osg::Vec3d lookVector = -getUpVector(rotation_matrix);
	osg::Vec3d sideVector = getSideVector(rotation_matrix);
	//osg::Vec3d upVector = getFrontVector(rotation_matrix);

	osg::Vec3d localUp(_upVector);

	osg::Vec3d forwardVector = localUp ^ sideVector;
	sideVector = forwardVector ^ localUp;

	forwardVector.normalize();
	sideVector.normalize();

	osg::Vec3d dv = forwardVector * (dy*scaley) + sideVector * (dx*scalex);

	_center += dv;
}
void COCSGroundManipulator::rotateTrackball(const float px0, const float py0,
	const float px1, const float py1, const float scale)
{
	double dx = px0 - px1;
	double dy = py0 - py1;

	if (::fabs(dx) > ::fabs(dy))
		dy = 0.0;
	else
		dx = 0.0;

	osg::Matrix rotation_matrix;
	rotation_matrix.makeRotate(_rotation);

	//osg::Vec3d lookVector = -getUpVector(rotation_matrix);
	osg::Vec3d sideVector = getSideVector(rotation_matrix);
	//osg::Vec3d upVector = getFrontVector(rotation_matrix);

	osg::Vec3d localUp(_upVector);

	osg::Vec3d forwardVector = localUp ^ sideVector;
	sideVector = forwardVector ^ localUp;

	forwardVector.normalize();
	sideVector.normalize();

	osg::Quat rotate_elevation;
	rotate_elevation.makeRotate(dy, sideVector);

	osg::Quat rotate_azim;
	rotate_azim.makeRotate(-dx, localUp);

	_rotation = _rotation * rotate_elevation * rotate_azim;
}

bool COCSGroundManipulator::performMovementLeftMouseButton(const double eventTimeDelta,
	const double dx, const double dy)
{
	// pan model
	float scale = -0.3f * _distance * getThrowScale(eventTimeDelta);
	panModel(dx*scale, dy*scale);
	return true;
}

bool COCSGroundManipulator::performMovementMiddleMouseButton(const double eventTimeDelta,
	const double dx, const double dy)
{
	rotateTrackball(_ga_t0->getXnormalized(), _ga_t0->getYnormalized(),
		_ga_t1->getXnormalized(), _ga_t1->getYnormalized(),
		getThrowScale(eventTimeDelta));
	return true;
}

void COCSGroundManipulator::setCenter(const osg::Vec3d& center)
{
	_center = center;
	_center.y() = 5000.0;
}

void COCSGroundManipulator::SetMaxDistance(double dValue)
{
	_maxDistance = dValue;
}

double COCSGroundManipulator::GetMaxDistance() const
{
	return _maxDistance;
}