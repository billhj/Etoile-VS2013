#include "ManipulatedCameraTransformFrame.h"
#include "Camera.h"
namespace Etoile
{
	ManipulatedCameraTransformFrame::ManipulatedCameraTransformFrame(Camera* camera, TransformFrame* parent)
		: m_camera(camera), ManipulatedTransformFrame(parent)
	{
		m_zoomsOnPivotPoint = true;
	}

	ManipulatedCameraTransformFrame::~ManipulatedCameraTransformFrame()
	{

	}

	void ManipulatedCameraTransformFrame::mousePressEvent(QMouseEvent* const event)
	{
		const QPoint delta = event->pos() - m_prevPos;
		if (event->button() == Qt::MouseButton::LeftButton){
			m_MouseMovePressed = false;
			m_MouseRotatePressed = true;
			m_MouseZoomPressed = false;
		}
		else if (event->button() == Qt::MouseButton::MiddleButton){
			m_MouseMovePressed = false;
			m_MouseRotatePressed = false;
			m_MouseZoomPressed = true;
		}
		else if (event->button() == Qt::MouseButton::RightButton){
			m_MouseMovePressed = true;
			m_MouseRotatePressed = false;
			m_MouseZoomPressed = false;
		}

		m_prevPos = m_pressPos = event->pos();
		ManipulatedTransformFrame::mousePressEvent(event);
	}

	void ManipulatedCameraTransformFrame::mouseDoubleClickEvent(QMouseEvent* const event)
	{


		ManipulatedTransformFrame::mouseDoubleClickEvent(event);
	}

	void ManipulatedCameraTransformFrame::mouseReleaseEvent(QMouseEvent* const event)
	{
		m_MouseMovePressed = false;
		m_MouseRotatePressed = false;
		m_MouseZoomPressed = false;
		ManipulatedTransformFrame::mouseReleaseEvent(event);
	}

	void ManipulatedCameraTransformFrame::mouseMoveEvent(QMouseEvent* const event) 
	{
		QPoint currentPos = event->pos();
		if (m_MouseMovePressed)
		{
			const QPoint delta = m_prevPos - event->pos();
			Vec3f trans(delta.x(), -delta.y(), 0.0);
			// Scale to fit the screen mouse displacement
			switch (m_camera->type())
			{
				case CameraType::PERSPECTIVE:
					trans *= 2.0 * tan(m_camera->getFieldOfView() / 2.0) *
						fabs((m_camera->frame()->coordinatesOf(pivotPoint())).z()) / m_camera->getHeight();
					break;
				case CameraType::ORTHOGRAPHIC:
				{
					trans[0] *= 2.0;
					trans[1] *= 2.0;
					break;
				}
			}
			translate(inverseTransformOf(translationSensitivity()*trans));
			emit manipulated();
		}
		else if (m_MouseRotatePressed)
		{

			Quaternionf rot = trackBall(
				(2.0f * m_prevPos.x() - m_camera->getWidth()) / m_camera->getWidth(),
				(m_camera->getHeight() - 2.0f * m_prevPos.y()) / m_camera->getHeight(),
				(2.0f * currentPos.x() - m_camera->getWidth()) / m_camera->getWidth(),
				(m_camera->getHeight() - 2.0f * currentPos.y()) / m_camera->getHeight());
			
			m_prevPos = currentPos;
			setSpinningQuaternion(rot);
			spin();
			emit manipulated();
		}
		else if (m_MouseZoomPressed)
		{
			qreal dx = qreal(event->x() - m_prevPos.x()) / m_camera->getWidth();
			qreal dy = qreal(event->y() - m_prevPos.y()) / m_camera->getHeight();
			qreal value = fabs(dx) > fabs(dy) ? dx : dy;
			zoom(value * zoomSensitivity());
			emit manipulated();
		}

		ManipulatedTransformFrame::mouseMoveEvent(event);
	}

	void ManipulatedCameraTransformFrame::wheelEvent(QWheelEvent* const event) 
	{
		static const qreal WHEEL_SENSITIVITY_COEF = 8E-4;
		float value = event->delta() * wheelSensitivity() * WHEEL_SENSITIVITY_COEF;
		zoom(value * zoomSensitivity());
		emit manipulated();
		ManipulatedTransformFrame::wheelEvent(event);
	}

	void ManipulatedCameraTransformFrame::zoom(qreal delta) {
		//const qreal sceneRadius = camera->sceneRadius();
		if (m_zoomsOnPivotPoint) {
			Vec3f direction = position() - m_camera->pivotPoint();
			if (direction.norm() > 0.02  || delta > 0.0)
				translate(delta * direction);
		}
		else {
			const qreal coef = fabs((m_camera->frame()->coordinatesOf(m_camera->pivotPoint())).z());
			Vec3f trans(0.0, 0.0, -coef * delta);
			translate(inverseTransformOf(trans));
		}
	}


	Quaternionf ManipulatedCameraTransformFrame::trackBall(float p1x, float p1y, float p2x, float p2y) {
		Quaternionf q;

		if (p1x == p2x && p1y == p2y) {
			/* Zero rotation */
			return q;
		}

		float TRACKBALLSIZE = 0.8f;

		/*
		* First, figure out z-coordinates for projection of P1 and P2 to
		* deformed sphere
		*/
		Vec3f p1(p1x, p1y, tb_project_to_sphere(TRACKBALLSIZE, p1x, p1y));
		Vec3f p2(p2x, p2y, tb_project_to_sphere(TRACKBALLSIZE, p2x, p2y));

		/*
		*  Now, we want the cross product of P1 and P2
		*/
		Vec3f a = p2.cross(p1);   /* Axis of rotation */

		/*
		*  Figure out how much to rotate around that axis.
		*/
		Vec3f d = p1 - p2;
		float t = d.norm() / (2.0f * TRACKBALLSIZE);

		/*
		* Avoid problems with out-of-control values...
		*/
		if (t > 1.0) {
			t = 1.0f;
		}
		if (t < -1.0) {
			t = -1.0f;
		}

		float phi = (float)(2.0 * asin(t));  /* how much to rotate about axis */
		q.setAxisAngle(a, -phi);

		return q;
	}

	float ManipulatedCameraTransformFrame::tb_project_to_sphere(float r, float x, float y) {
		float d, t, z;
		d = sqrt(x * x + y * y);
		if (d < r * 0.70710678118654752440) {    /* Inside sphere */
			z = sqrt(r * r - d * d);
		}
		else {           /* On hyperbola */
			t = r / 1.41421356237309504880f;
			z = t * t / d;
		}
		return z;
	}
}
