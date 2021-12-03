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

	//��Ļ����ת��������
	osg::Vec3 viewToWolrd(osg::ref_ptr<osg::Camera> cam, const osg::Vec3 &viewpt);

	//��������ת��γ������
	osg::Vec3 worldToView(osg::ref_ptr<osg::Camera> cam, const osg::Vec3 &wdpt);

	//�����������
	void setCameraType(CameraType type) {
		mType = type;
	}
protected:
	//�ع��ʼλ��
	void setOrigin();
protected:
	MouseMode  mMode = MouseMode::eMapPan;
	CameraType mType = CameraType::eFirstMode;

	bool mLftBtnDwn = false;
	bool mRhtBtnDwn = false;

	osg::Vec2       mLftPt;
	osg::Vec2       mRhtPt;

	osg::Vec3 mCamPt;		//��ǰ���λ��
	osg::Vec3 mObjPt;		//Ŀ��λ��
	osg::Vec3 mOriPt;		//��¼��ʼλ��
	osg::Vec3 mLstPt;		//��¼�ϴ�λ��
	osg::Vec3 mUp;			//Ĭ���Ϸ���
	osg::Quat mLstRt;		//��¼�ϴ���ת
	osg::Vec3 mMapCenter;	//��ǰ��ͼ����

	osg::Matrixd mTrans;	//ת������
	osg::Quat	 mRot;		//��ת
};

#endif

