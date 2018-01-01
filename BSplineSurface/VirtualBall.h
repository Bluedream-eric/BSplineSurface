/**************************
�㷨��    ��������ת�㷨
���ߣ�    aijiaming
ʱ�䣺    2017/12/3
��ϵ��ʽ��1907059378@qq.com 
**************************/
#ifndef VIRTUALBALL_H
#define VIRTUALBALL_H
#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>
const float PI = 3.1415926;
class VirtualBall
{
public:
	VirtualBall()
	{
		//��ʼ��ת��Ϊ��λ��
		LastRotate = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 0),0.0f);
		currentRotate = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 0), 0.0f);
	}

	VirtualBall(float width,float height)
		:_width(width),_height(height)
	{
		//��ʼ��ת��Ϊ��λ��
		LastRotate = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 0), 0.0f);
		currentRotate = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 0), 0.0f);
	}
	~VirtualBall(){}
	
	void setBounds(float newwidth, float newheight)
	{
		_width = newwidth;
		_height = newheight;
	}
	//����Ԫ��ת��Ϊ��Ӧ��4x4��ת����
	QMatrix4x4 RotateMatrix()
	{
		return quatToMat(currentRotate);
	}
	//����Ļ����ӳ�䵽��������
	QVector3D mapToSphere(double x, double y)
	{
		//���Ƚ���[0,width],[0,height]ӳ�䵽[-1,1],[1,-1]��
		x = x*2.0 / _width - 1;
		y = y*(-2.0) / _height + 1;
		double d = x*x + y*y;
		
		if (d <= 1)
		{
			return QVector3D(x, y, sqrt(1 - d));
		}
		else
		{
			return QVector3D(x / sqrt(d), y / sqrt(d), 0);
		}
	}
	//���ʱ����Ļ����ӳ�䵽��������
	void click(double x,double y)
	{
		_lastpos = mapToSphere(x, y);
	}
	//�϶��󣬽��������Ļ����ӳ�䵽�������꣬��
	//ͨ��ǰ���������������ת�ᣬ��ת�Ƕȣ����ɶ�Ӧ��Ԫ��
	void dragTo(double x, double y)
	{
		_newpos = mapToSphere(x, y);

		QVector3D axis = QVector3D::crossProduct(_lastpos, _newpos); //���Ϊ��ת��
		axis.normalize();
		float angle = asin((QVector3D::crossProduct(_lastpos, _newpos)).length()); //��ת�Ƕ�
		angle = angle / PI * 180;
		//Quaternion�����еĽǶȲ��ǻ���
		QQuaternion temp = QQuaternion::fromAxisAndAngle( axis,2*angle); //������Ԫ��������Ϊ����ת360�ȣ��Ƕ�Ϊ2��
		currentRotate = temp*LastRotate; //���浽currentRotate��
	}
	void push()
	{
		LastRotate = currentRotate; //һ���϶���ɣ���currentRotate����
	}

	inline QMatrix4x4 quatToMat(QQuaternion q)
	{
		//based on algorithm on wikipedia
		// http://en.wikipedia.org/wiki/Rotation_matrix#Quaternion
		float w = q.scalar();
		float x = q.x();
		float y = q.y();
		float z = q.z();

		float n = q.lengthSquared();
		float s = n == 0 ? 0 : 2 / n;
		float wx = s * w * x, wy = s * w * y, wz = s * w * z;
		float xx = s * x * x, xy = s * x * y, xz = s * x * z;
		float yy = s * y * y, yz = s * y * z, zz = s * z * z;

		float m[16] = { 1 - (yy + zz),         xy + wz ,         xz - wy ,0,
			xy - wz ,    1 - (xx + zz),         yz + wx ,0,
			xz + wy ,         yz - wx ,    1 - (xx + yy),0,
			0 ,               0 ,               0 ,1 };
		QMatrix4x4 result = QMatrix4x4(m, 4, 4);
		result.optimize();
		return result;
	}

private:
	QVector3D _lastpos; //�����Ļʱ��ӳ�䵽���������
	QVector3D _newpos;  //�϶����ͷ�ʱ������ӳ�䵽���������
	float _width;   
	float _height;
	QQuaternion currentRotate;  //��ǰ��ת ��Ԫ��
	QQuaternion LastRotate;  //ǰһ����ת ��Ԫ��
	
	
};

#endif //VIRTUALBALL_H