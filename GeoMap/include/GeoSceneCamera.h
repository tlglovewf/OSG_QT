#ifndef _GEOSCENECAMERAMANIPULATOR_H_
#define _GEOSCENECAMERAMANIPULATOR_H_

#include <osgGA/CameraManipulator>
#include <osgGA/OrbitManipulator>


typedef osg::Vec3 GpsPos; //x:lng  y:lat  z:alt
typedef osg::Vec3 MecPos;
 
#define PI     3.141592657
#define D2R(d) (d * 0.0174532925388889)                  // PI * d / 180.0
#define RADIUS 100  //6378137
#define DEFAULTHEIGHT 500
//gis �������
class GeoSceneCamera : public osgGA::CameraManipulator
{
public:
	enum MouseMode
	{
	   	eMapPan,
		eMapSelect
	};
	
	GeoSceneCamera()
	{

	}

	void setCameraPos(const GpsPos &pos)
	{
		mCamPt = pos;
		mOriPt = pos;
		updateCamera();
	}


	//��γ��תī����ͶӰ
	static inline MecPos lnglat2mector(const GpsPos &lnglat)
	{
		double x = RADIUS * D2R(lnglat.x());
		double y = RADIUS * log(tan(PI * 0.25) + (0.5 * D2R(lnglat.y())));
		return MecPos( x, y , lnglat.z() );
	}
//�麯�� �̳�
public:
	virtual void setByMatrix(const osg::Matrixd &matrix) {}
	virtual void setByInverseMatrix(const osg::Matrixd &matrix) {}
	virtual osg::Matrixd getMatrix()const;
	virtual osg::Matrixd getInverseMatrix()const;

	float getFusionDistanceValue() const { return 25; }
	osgUtil::SceneView::FusionDistanceMode getFusionDistanceMode() const
	{
		return osgUtil::SceneView::USE_FUSION_DISTANCE_VALUE;
	}
	//�����¼�
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

protected:
	//�������
	void updateCamera();
	//�ع��ʼλ��
	void setOrigin();
protected:
	MouseMode mMode = eMapPan;

	bool mLftBtnDwn = false;

	osg::Vec2       mLftPt;

	osg::Vec3 mCamPt;		//��ǰ���λ��
	osg::Vec3 mOriPt;		//��¼��ʼλ��
	osg::Vec3 mMapCenter;	//��ǰ��ͼ����

	osg::Matrixd mTrans;	//ת������
};


class COCSGroundManipulator : public osgGA::OrbitManipulator
{
	typedef OrbitManipulator inherited;
public:

	COCSGroundManipulator();

	virtual ~COCSGroundManipulator();


	// ������

	virtual void setCenter(const osg::Vec3d& center);

	// ������

	virtual void setByMatrix(const osg::Matrixd& matrix);


	// ������

	virtual void zoomModel(const float dy, bool pushForwardIfNeeded = true);

	// ������

	virtual bool performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy);


	// ������

	virtual bool performMovementMiddleMouseButton(const double eventTimeDelta, const double dx, const double dy);


	// ������

	virtual void panModel(const float dx, const float dy, const float dz = 0.f);


	// ������

	virtual void rotateTrackball(const float px0, const float py0,
		const float px1, const float py1, const float scale);
public:
	// ���淨��
	osg::Vec3d GetUpVector() const;
	void SetUpVector(const osg::Vec3d &vecUp);

	// ������
	void SetMaxDistance(double dDistance);
	double GetMaxDistance() const;

private:
	osg::Vec3d _upVector;	// ��ֱ�������ϵ�����

	double _maxDistance;	// ����һ�������룬������������룬�Ͳ��ڷŴ���

};


#endif
