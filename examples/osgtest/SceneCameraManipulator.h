#ifndef _SCENECAMERAMANIPULATOR_H_
#define _SCENECAMERAMANIPULATOR_H_

#include <osgGA/CameraManipulator>


class SceneCamManipulator : public osgGA::CameraManipulator
{
public:
    SceneCamManipulator():mLftBtnDwn(false),mRhtBtnDwn(false)
    {
        setOrigin();
    }

    virtual void setByMatrix(const osg::Matrixd &matrix){}
    virtual void setByInverseMatrix(const osg::Matrixd &matrix){}
    virtual osg::Matrixd getMatrix()const;
    virtual osg::Matrixd getInverseMatrix()const;

    float getFusionDistanceValue() const { return 25; }
    osgUtil::SceneView::FusionDistanceMode getFusionDistanceMode() const
    {
        return osgUtil::SceneView::USE_FUSION_DISTANCE_VALUE;
    }
    virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);

	//屏幕坐标转世界坐标
	osg::Vec3 viewToWolrd(osg::ref_ptr<osg::Camera> cam, const osg::Vec3 &viewpt);

	//世界坐标转经纬度坐标
	osg::Vec3 worldToView(osg::ref_ptr<osg::Camera> cam, const osg::Vec3 &wdpt);
protected:
    inline void setOrigin()
    {
        mCamPt = osg::Vec3(0, 0, 300);
        mTrans.makeTranslate(mCamPt);
        mRotate.makeIdentity();
        mQRot = mRotate.getRotate();
    }

public:
    
    bool            mLftBtnDwn;
    bool            mRhtBtnDwn;

    osg::Vec2       mLftPt;
    osg::Vec2       mRhtPt;
    osg::Vec3       mCamPt;

    osg::Matrixd    mRotate;
    osg::Matrixd    mTrans;

    osg::Quat       mQRot;
};



#endif