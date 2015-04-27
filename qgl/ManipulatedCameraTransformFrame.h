#ifndef MANIPULATEDCAMERATRANSFORMFRAME_H
#define MANIPULATEDCAMERATRANSFORMFRAME_H

#include "ManipulatedTransformFrame.h"
#include "QGLRenderWidget.h"
#include "Vectors.h"

namespace Etoile
{
	class Camera;
	class ManipulatedCameraTransformFrame : public ManipulatedTransformFrame
	{
		Q_OBJECT

	public:
		ManipulatedCameraTransformFrame(Camera* camera, TransformFrame* parent = NULL);
		~ManipulatedCameraTransformFrame();
		Vec3f pivotPoint() const { return m_pivotPoint; }
		/*! Sets the pivotPoint(), defined in the world coordinate system. */
		void setPivotPoint(const Vec3f& point) { m_pivotPoint = point; }
		void zoom(qreal delta);
		bool zoomsOnPivotPoint() const { return m_zoomsOnPivotPoint; }
		void setZoomsOnPivotPoint(bool enabled) { m_zoomsOnPivotPoint = enabled; }
	public:
		virtual void mousePressEvent(QMouseEvent* const event) override;
		virtual void mouseDoubleClickEvent(QMouseEvent* const event) override;
		/*! Mouse release event callback method. See mousePressEvent(). */
		virtual void mouseReleaseEvent(QMouseEvent* const event) override;
		virtual void mouseMoveEvent(QMouseEvent* const event) override;
		virtual void wheelEvent(QWheelEvent* const event) override;

		Quaternionf trackBall(float p1x, float p1y, float p2x, float p2y);
		float tb_project_to_sphere(float r, float x, float y);
	private:
		Camera * m_camera;
		Vec3f m_pivotPoint;
		bool m_zoomsOnPivotPoint;
	};
}

#endif // MANIPULATEDCAMERATRANSFORMFRAME_H
