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

	enum class CameraType
	{
	   eFirstMode,
	   eThirdMode
	};

	enum class MouseMode
	{
	   	eMapPan,
		eMapSelect
	};
	
	GeoSceneCamera():mUp(0.0, 1.0, 0.0)
	{

	}

	void setCameraPos(const GpsPos &pos)
	{
		mCamPt = pos;
		mOriPt = pos;
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

	//设置相机类型
	void setCameraType(CameraType type) {
		mType = type;
	}
protected:
	//回归初始位置
	void setOrigin();
protected:
	MouseMode  mMode = MouseMode::eMapPan;
	CameraType mType = CameraType::eFirstMode;

	bool mLftBtnDwn = false;
	bool mRhtBtnDwn = false;

	osg::Vec2       mLftPt;
	osg::Vec2       mRhtPt;

	osg::Vec3 mCamPt;		//当前相机位置
	osg::Vec3 mObjPt;		//目标位置
	osg::Vec3 mOriPt;		//记录初始位置
	osg::Vec3 mLstPt;		//记录上次位置
	osg::Vec3 mUp;			//默认上方向
	osg::Quat mLstRt;		//记录上次旋转
	osg::Vec3 mMapCenter;	//当前地图中心

	osg::Matrixd mTrans;	//转换矩阵
	osg::Quat	 mRot;		//旋转
};

#endif

