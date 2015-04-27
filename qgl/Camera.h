/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Camera.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <QObject>
#include "ManipulatedCameraTransformFrame.h"

namespace Etoile
{
	enum CameraType {PERSPECTIVE, ORTHOGRAPHIC};

	class Camera : public QObject
	{
		Q_OBJECT

		int m_screenWidth, m_screenHeight;  // size of the window, in pixels
		float m_fieldOfView; // in radians
		float m_nearPlane;
		float m_farPlane;
		//Vec3f m_sceneCenter;
		//qreal m_sceneRadius; // OpenGL units

		Matrix4f m_modelviewMatrix;
		Matrix4f m_projectionMatrix;;

		CameraType m_type;
		ManipulatedCameraTransformFrame* m_frame;
	public:
		Camera(const Vec3f& position, const Quaternionf& orientation, TransformFrame* parent = NULL);
		Camera(const Vec3f& direction, const Vec3f& up, const Vec3f& position, TransformFrame* parent = NULL);
		Camera(TransformFrame* parent = NULL);
		void resetCamera();
		CameraType type();
		void setType(CameraType type);
		float getAspectRatio() const;
		void setZNearPlane(float zNear);
		float getZNearPlane() const;
		void setZFarPlane(float zFar);
		float getZFarPlane() const;
		void setFieldOfView(float fov);
		float getFieldOfView() const;

		int getWidth() const;
		void setWidth(int screenWidth);
		int getHeight() const;
		void setHeight(int m_screenHeight);

		void setTarget(const Vec3f& target);
		void setUpVector(const Vec3f& up);
		Vec3f getUpVector() const;
		void setViewDirection(const Vec3f& direction);
		Vec3f getViewDirection() const;
		void setPivotPoint(const Vec3f& point);

		void setPerspective(int width = 800, int height = 600, float near = 0.1f, float far = 10000.0f, float fieldOfView =  M_PI / 4.0);
		void setOrthogonal(int width = 800, int height = 600, float near = 0.1f, float far = 10000.0f);

		Matrix4f getModelViewMatrix() const;
		Matrix4f getProjectionMatrix() const;
		Matrix4f getGLModelViewMatrix() const;
		Matrix4f getGLProjectionMatrix() const;
		void getGLModelViewMatrixf(float* ) ;
		void getGLProjectionMatrixf(float* );

		void setupCameraOrientation(const Vec3f& direction, const Vec3f& upVector);
		void computeProjectionMatrix();
		void computeModelViewMatrix();

		Vec3f position() const { return m_frame->position(); }
		Vec3f upVector() const
		{
			return m_frame->inverseTransformOf(Vec3f(0.0, 1.0, 0.0));
		}
		Vec3f viewDirection() const { return m_frame->inverseTransformOf(Vec3f(0.0, 0.0, -1.0)); }
		Vec3f rightVector() const
		{
			return m_frame->inverseTransformOf(Vec3f(1.0, 0.0, 0.0));
		}
		Quaternionf orientation() const { return m_frame->orientation(); }
		void setPosition(const Vec3f& pos) { m_frame->setPosition(pos); }

		Vec3f cameraCoordinatesOf(const Vec3f& src) const { return m_frame->coordinatesOf(src); }
		Vec3f worldCoordinatesOf(const Vec3f& src) const { return m_frame->inverseCoordinatesOf(src); }

		Vec3f pivotPoint() const { return m_frame->pivotPoint(); }
		ManipulatedCameraTransformFrame* frame(){ return m_frame; }

	};


	class FrontViewCamera: public Camera
	{
	public:
		FrontViewCamera(const Vec3f& position);
	};

	class LeftViewCamera: public Camera
	{
	public:
		LeftViewCamera(const Vec3f& position);
	};

	class TopViewCamera: public Camera
	{
	public:
		TopViewCamera(const Vec3f& position);
	};
}