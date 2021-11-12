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
//gis 场景相机
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


	//经纬度转墨卡托投影
	static inline MecPos lnglat2mector(const GpsPos &lnglat)
	{
		double x = RADIUS * D2R(lnglat.x());
		double y = RADIUS * log(tan(PI * 0.25) + (0.5 * D2R(lnglat.y())));
		return MecPos( x, y , lnglat.z() );
	}
//虚函数 继承
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
	//处理事件
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

	//屏幕坐标转世界坐标
	osg::Vec3 viewToWolrd(osg::ref_ptr<osg::Camera> cam, const osg::Vec3 &viewpt);

	//世界坐标转经纬度坐标
	osg::Vec3 worldToView(osg::ref_ptr<osg::Camera> cam, const osg::Vec3 &wdpt);
protected:
	//更新相机
	void updateCamera();
	//回归初始位置
	void setOrigin();
protected:
	MouseMode mMode = eMapPan;

	bool mLftBtnDwn = false;

	osg::Vec2       mLftPt;

	osg::Vec3 mCamPt;		//当前相机位置
	osg::Vec3 mOriPt;		//记录初始位置
	osg::Vec3 mMapCenter;	//当前地图中心

	osg::Matrixd mTrans;	//转换矩阵
};


class COCSGroundManipulator : public osgGA::OrbitManipulator
{
	typedef OrbitManipulator inherited;
public:

	COCSGroundManipulator();

	virtual ~COCSGroundManipulator();


	// 见父类

	virtual void setCenter(const osg::Vec3d& center);

	// 见父类

	virtual void setByMatrix(const osg::Matrixd& matrix);


	// 见父类

	virtual void zoomModel(const float dy, bool pushForwardIfNeeded = true);

	// 见父类

	virtual bool performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy);


	// 见父类

	virtual bool performMovementMiddleMouseButton(const double eventTimeDelta, const double dx, const double dy);


	// 见父类

	virtual void panModel(const float dx, const float dy, const float dz = 0.f);


	// 见父类

	virtual void rotateTrackball(const float px0, const float py0,
		const float px1, const float py1, const float scale);
public:
	// 地面法向
	osg::Vec3d GetUpVector() const;
	void SetUpVector(const osg::Vec3d &vecUp);

	// 最大距离
	void SetMaxDistance(double dDistance);
	double GetMaxDistance() const;

private:
	osg::Vec3d _upVector;	// 垂直地面向上的向量

	double _maxDistance;	// 定义一个最大距离，当超过这个距离，就不在放大了

};


#endif

