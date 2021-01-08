#include <osg/io_utils>
#include <osg/Projection>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osgText/Text>
#include <osgGA/GUIEventHandler>
#include <osgUtil/Optimizer>
#include <osgUtil/SmoothingVisitor>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
class KeyboardEventHandler : public osgGA::GUIEventHandler
{ // extra event handler traps 'n' key to re-tessellate any tessellated geodes.
public:

    KeyboardEventHandler(osg::Node *nd):
        _scene(nd) {}

        virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&)
        {
            switch(ea.getEventType())
            {
                case(osgGA::GUIEventAdapter::KEYDOWN):
                {
                    if (_scene && ea.getKey()=='n')
                    {
                    ///////////////////////////


                       return true;
                    }
                    break;
                }
                default:
                    break;
            }
            return false;
        }

        osg::Node *_scene;

};

float toRadians(float angle)
{
    return angle/180*osg::PI;
}

osg::ref_ptr<osg::Geode>skydemo (float radius,osg::Vec3 postioncenter)
{
    osg::ref_ptr<osg::Geode>skygeode=new osg::Geode();

    osg::ref_ptr<osg::Geometry>gskydome= new osg::Geometry;

    osg::ref_ptr<osg::Vec3Array> coords = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec3Array> nrms = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec2Array> tcs = new osg::Vec2Array;
    osg::Vec3 nrm(0,-1,0);

    //*************************************///
    float ANGLE_SPAN=18;//切分间隔
        float angleV=90;//纵向上的起始度数
        //获取切分整图的纹理数组
        int bw=(int)(360/ANGLE_SPAN);//纹理图切分的列数
        int bh= (int)(angleV/ANGLE_SPAN);  //纹理图切分的行数
        float *result=new float[bw*bh*6*2];//数组

        float sizew=1.0f/bw;//列数
        float sizeh=1.0f/bh;//行数
        int c=0;
        for(int i=0;i<bh;i++)
        {
            for(int j=0;j<bw;j++)
            {
                //每行列一个矩形，由两个三角形构成，共六个点，12个纹理坐标
                float s=j*sizew;
                float t=i*sizeh;

                result[c++]=s;
                result[c++]=t;

                result[c++]=s+sizew;
                result[c++]=t;

                result[c++]=s;
                result[c++]=t+sizeh;

                result[c++]=s;
                result[c++]=t+sizeh;

                result[c++]=s+sizew;
                result[c++]=t;

                result[c++]=s+sizew;
                result[c++]=t+sizeh;                
            }
        }
        int tc=0;
        int ts=bw*bh*6*2;//纹理数组长度
        float tx1,tx2;


      for(float vAngle=angleV;vAngle>0;vAngle=vAngle-ANGLE_SPAN)//垂直方向angleSpan度一份
        {
            for(float hAngle=360;hAngle>0;hAngle=hAngle-ANGLE_SPAN)//水平方向angleSpan度一份
            {
                //纵向横向各到一个角度后计算对应的此点在球面上的四边形顶点坐标
                //并构建两个组成四边形的三角形
                double xozLength=radius*cos(toRadians(vAngle));
                float x1=(float)(xozLength*cos(toRadians(hAngle)));
                float z1=(float)(xozLength*sin(toRadians(hAngle)));
                float y1=(float)(radius*sin(toRadians(vAngle)));

                xozLength=radius*cos(toRadians(vAngle-ANGLE_SPAN));
                float x2=(float)(xozLength*cos(toRadians(hAngle)));
                float z2=(float)(xozLength*sin(toRadians(hAngle)));
                float y2=(float)(radius*sin(toRadians(vAngle-ANGLE_SPAN)));

                xozLength=radius*cos(toRadians(vAngle-ANGLE_SPAN));
                float x3=(float)(xozLength*cos(toRadians(hAngle-ANGLE_SPAN)));
                float z3=(float)(xozLength*sin(toRadians(hAngle-ANGLE_SPAN)));
                float y3=(float)(radius*sin(toRadians(vAngle-ANGLE_SPAN)));

                xozLength=radius*cos(toRadians(vAngle));
                float x4=(float)(xozLength*cos(toRadians(hAngle-ANGLE_SPAN)));
                float z4=(float)(xozLength*sin(toRadians(hAngle-ANGLE_SPAN)));
                float y4=(float)(radius*sin(toRadians(vAngle)));   



                coords->push_back(osg::Vec3(x1,-z1,y1));
                coords->push_back(osg::Vec3(x4,-z4,y4));
                coords->push_back(osg::Vec3(x2,-z2,y2));
                //构建第一三角形


                //构建第二三角形

                coords->push_back(osg::Vec3(x2,-z2,y2));
                coords->push_back(osg::Vec3(x4,-z4,y4));
                coords->push_back(osg::Vec3(x3,-z3,y3));


                //第一三角形3个顶点的6个纹理坐标


                tx1=result[tc++%ts];
                tx2=result[tc++%ts];
                tcs->push_back(osg::Vec2(tx1,tx2));
                tx1=result[tc++%ts];
                tx2=result[tc++%ts];
                tcs->push_back(osg::Vec2(tx1,tx2));
                tx1=result[tc++%ts];
                tx2=result[tc++%ts];
                tcs->push_back(osg::Vec2(tx1,tx2));




                //第二三角形3个顶点的6个纹理坐标

                tx1=result[tc++%ts];
                tx2=result[tc++%ts];
                tcs->push_back(osg::Vec2(tx1,tx2));
                tx1=result[tc++%ts];
                tx2=result[tc++%ts];
                tcs->push_back(osg::Vec2(tx1,tx2));
                tx1=result[tc++%ts];
                tx2=result[tc++%ts];
                tcs->push_back(osg::Vec2(tx1,tx2));

            }
        }

     delete result;result=NULL;//删除纹理数组

      nrms->push_back(nrm);


      gskydome->setVertexArray(coords.get());
      gskydome->setTexCoordArray(0,tcs.get());

      //三角形
      gskydome->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES,0,coords->size()));
//自动生成法线吧，这样看起来更自然
      osgUtil::SmoothingVisitor::smooth(*(gskydome.get()));


      skygeode->addDrawable(gskydome.get());

      //背景图片;
    osg::ref_ptr<osg::Texture2D> baseTex = new osg::Texture2D;
    osg::ref_ptr<osg::Image> iamge = osgDB::readImageFile("/home/tu/Desktop/skybox.jpeg");
    baseTex->setImage(iamge);
    baseTex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
    baseTex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
    skygeode->getOrCreateStateSet()->setTextureAttributeAndModes(0, baseTex, 1);


      return skygeode.get();
}

class RoateCallback: public osg::NodeCallback
{
public:
    RoateCallback():_rotateZ(0.0){}

    virtual void operator()(osg::Node* node,osg::NodeVisitor* nv)
    {
        osg::PositionAttitudeTransform *pat=
            dynamic_cast<osg::PositionAttitudeTransform*>(node);
        if(pat)
        {
            osg::Quat quat(osg::DegreesToRadians(_rotateZ),osg::Z_AXIS);
            pat->setAttitude(quat);
            _rotateZ+=0.030;
        }
        traverse(node,nv);
    }


protected:
    double _rotateZ;
};

class InfoCallback: public osg::NodeCallback
{
public:
    virtual void operator()(osg::Node*node,osg::NodeVisitor* nv)
    {
        osg::PositionAttitudeTransform* pat=
            dynamic_cast<osg::PositionAttitudeTransform*>(node);

        if(pat)
        {
            double angle=0.0;
            osg::Vec3 axis;
            pat->getAttitude().getRotate(angle,axis);
            std::cout<<"node roat axis "<<axis<<", "<<"angle "<<angle<<std::endl;
        }
        traverse(node,nv);
    }

};

int main( int argc, char **argv )
{
    // use an ArgumentParser object to manage the program arguments.
    osg::ArgumentParser arguments(&argc,argv);

    // construct the viewer.
    osgViewer::Viewer viewer;

    // read the scene from the list of file specified commandline args.
    osg::ref_ptr<osg::Group> root = new osg::Group();

    osg::ref_ptr<osg::PositionAttitudeTransform>pat =new osg::PositionAttitudeTransform;

    pat->addChild(skydemo(3000.0f,osg::Vec3(1,1,1)).get());

    pat->setUpdateCallback(new RoateCallback);
    pat->addUpdateCallback(new InfoCallback);

    root->addChild(pat.get());

    osgUtil::Optimizer optimizer;
    optimizer.optimize(root.get() );

    // set the scene to render
    viewer.setSceneData(root.get());

    viewer.addEventHandler(new KeyboardEventHandler(root.get()));

    osgDB::writeNodeFile(*(root.get()), "saved_model.osgt");

    return viewer.run();
}