/**
* Copyright(C) 2009-2012
* @author Jing HUANG
* @file Camera.cpp
* @brief
* @date 1/2/2011
*/

#include "Camera.h"
/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{

	Camera::Camera(const Vec3f& position, const Quaternionf& orientation, TransformFrame* parent)
	{
		m_frame = new ManipulatedCameraTransformFrame(this, parent);
		//m_frame->setReferenceFrame(parent);
		m_frame->setPosition(position);
		m_frame->setOrientation(orientation);
		if (position.x() != 0 && position.y() != 0 && position.z() != 0)
			setPivotPoint(Vec3f());
	}

	Camera::Camera(const Vec3f& direction, const Vec3f& up, const Vec3f& position, TransformFrame* parent)
	{
		m_frame = new ManipulatedCameraTransformFrame(this, parent);
		//m_frame->setReferenceFrame(parent);
		m_frame->setPosition(position);
		this->setupCameraOrientation(direction, up);
		//if (position.x() != 0 && position.y() != 0 && position.z() != 0)
		setPivotPoint(direction * 2);
	}

	Camera::Camera(TransformFrame* parent)
	{
		m_frame = new ManipulatedCameraTransformFrame(this, parent);
		m_frame->setPosition(Vec3f(0,0,-2));
		setPivotPoint(Vec3f());
		//m_frame->setReferenceFrame(parent);
		reset();
	}

	void Camera::reset()
	{
		m_type = PERSPECTIVE;
		m_fieldOfView = M_PI / 4.0;
		m_nearPlane = 0.1f;
		m_farPlane = 10000.0f;
		m_screenWidth = 800;
		m_screenHeight = 600;
	}

	CameraType Camera::type(){ return m_type; }
	void Camera::setType(CameraType type){ m_type = type; }
	float Camera::getAspectRatio() const{ return static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight); }
	void Camera::setZNearPlane(float zNear){ m_nearPlane = zNear; }
	float Camera::getZNearPlane() const{ return m_nearPlane; }
	void Camera::setZFarPlane(float zFar){ m_farPlane = zFar; }
	float Camera::getZFarPlane() const{ return m_farPlane; }
	void Camera::setFieldOfView(float fov){ m_fieldOfView = fov; }
	float Camera::getFieldOfView() const{ return m_fieldOfView; }

	int Camera::getWidth() const{ return m_screenWidth; }
	void Camera::setWidth(int screenWidth){ m_screenWidth = screenWidth; }
	int Camera::getHeight() const{ return m_screenHeight; }
	void Camera::setHeight(int m_screenHeight){ m_screenHeight = m_screenHeight; }

	void Camera::setPivotPoint(const Vec3f& point)
	{
		m_frame->setPivotPoint(point);
		setTarget(point);
	}

	void Camera::setTarget(const Vec3f& target)
	{
		setupCameraOrientation(target - m_frame->position(), getUpVector());
	}

	void Camera::setUpVector(const Vec3f& up)
	{
		setupCameraOrientation(getViewDirection(), up);
	}
	Vec3f Camera::getUpVector() const{ return m_frame->orientation() * Vec3f(0, 1, 0); }
	void Camera::setViewDirection(const Vec3f& direction)
	{
		setupCameraOrientation(direction, getUpVector());
	}
	Vec3f Camera::getViewDirection() const{ return m_frame->orientation() * Vec3f(0, 0, -1); }

	void Camera::setPerspective(int width, int height, float nearP, float farP, float fieldOfView)
	{
		this->setWidth(width);
		this->setHeight(height);
		this->setZNearPlane(nearP);
		this->setZFarPlane(farP);
		this->setFieldOfView(fieldOfView);
		this->setType(CameraType::PERSPECTIVE);
		computeProjectionMatrix();
	}
	void Camera::setOrthogonal(int width, int height, float nearP, float farP)
	{
		this->setWidth(width);
		this->setHeight(height);
		this->setZNearPlane(nearP);
		this->setZFarPlane(farP);
		this->setType(CameraType::ORTHOGRAPHIC);
		computeProjectionMatrix();
	}

	Matrix4f Camera::getModelViewMatrix() const{ return m_modelviewMatrix; }
	Matrix4f Camera::getProjectionMatrix() const{ return m_projectionMatrix; }
	Matrix4f Camera::getGLModelViewMatrix() const{ return m_modelviewMatrix.transpose(); }
	Matrix4f Camera::getGLProjectionMatrix() const{ return m_projectionMatrix.transpose(); }

	void Camera::setupCameraOrientation(const Vec3f& direction, const Vec3f& upVector)
	{
		if (direction.norm() < 1E-10)
			return;

		Vec3f xAxis = direction.cross(upVector);
		if (xAxis.norm() < 1E-10)
		{
			// target is aligned with upVector, this means a rotation around X axis
			// X axis is then unchanged, let's keep it !
			xAxis = Vec3f(1.0, 0.0, 0.0); //computeGlobalPosition(Vec3f(1.0, 0.0, 0.0));
		}

		Quaternionf q;
		q.setFromRotatedBasis(xAxis, xAxis.cross(direction), -direction);
		m_frame->setOrientation(q);
	}

	void Camera::computeTransformationMatrix()
	{
		computeProjectionMatrix();
		computeModelViewMatrix();
	}

	void Camera::computeProjectionMatrix()
	{
		switch (m_type)
		{
		case CameraType::PERSPECTIVE:
		{
										const float f = 1.0 / tan(m_fieldOfView / 2.0);
										m_projectionMatrix(0, 0) = f / getAspectRatio();
										m_projectionMatrix(1, 1) = f;
										m_projectionMatrix(2, 2) = (m_nearPlane + m_farPlane) / (m_nearPlane - m_farPlane);
										m_projectionMatrix(3, 2) = -1.0;
										m_projectionMatrix(2, 3) = 2.0 * m_nearPlane * m_farPlane / (m_nearPlane - m_farPlane);
										m_projectionMatrix(3, 3) = 0.0;
										// same as gluPerspective( 180.0*fieldOfView()/M_PI, aspectRatio(), zNear(), zFar() );
										break;
		}
		case CameraType::ORTHOGRAPHIC:
		{
										 m_projectionMatrix(0, 0) = 1.0 / static_cast<float>(m_screenWidth);
										 m_projectionMatrix(1, 1) = 1.0 / static_cast<float>(m_screenHeight);
										 m_projectionMatrix(2, 2) = -2.0 / (m_farPlane - m_nearPlane);
										 m_projectionMatrix(3, 2) = 0.0;
										 m_projectionMatrix(2, 3) = (m_farPlane + m_nearPlane) / (m_nearPlane - m_farPlane);
										 m_projectionMatrix(3, 3) = 1.0;
										 // same as glOrtho( -w, w, -h, h, zNear(), zFar() );
										 break;
		}
		}
	}

	void Camera::computeModelViewMatrix()
	{
		const Quaternionf q = m_frame->orientation();
		q.getInverseMatrix(m_modelviewMatrix);      //inverse matrix for camera that is modelview for obj
		Vec3f t = q.inverseRotate(m_frame->position());  // distance in project space  
		m_modelviewMatrix(0, 3) = -t[0];
		m_modelviewMatrix(1, 3) = -t[1];
		m_modelviewMatrix(2, 3) = -t[2];
		m_modelviewMatrix(3, 3) = 1.0;
	}

	void Camera::getGLModelViewMatrixf(float* matrix)
	{
		for (unsigned int i = 0; i < 4; ++i){
			for (unsigned int j = 0; j < 4; ++j){
				matrix[i * 4 + j] = m_modelviewMatrix(j, i);
			}
		}
	}

	void Camera::getGLProjectionMatrixf(float* matrix)
	{
		for (unsigned int i = 0; i < 4; ++i){
			for (unsigned int j = 0; j < 4; ++j){
				matrix[i * 4 + j] = m_projectionMatrix(j, i);
			}
		}
	}

	void Camera::updateMatrix(){
		computeTransformationMatrix();
	}

	FrontViewCamera::FrontViewCamera(const Vec3f& position) : Camera(Vec3f(0, 0, -1), Vec3f(0, 1, 0), position)
	{

	}

	LeftViewCamera::LeftViewCamera(const Vec3f& position) : Camera(Vec3f(1, 0, 0), Vec3f(0, 1, 0), position)
	{

	}

	TopViewCamera::TopViewCamera(const Vec3f& position) : Camera(Vec3f(0, -1, 0), Vec3f(0, 0, -1), position)
	{

	}
}