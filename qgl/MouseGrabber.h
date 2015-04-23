#pragma once
#include <QMouseEvent>

namespace Etoile
{

	class MouseGrabber
	{
		friend class QGLRenderWidget;
	public:
		MouseGrabber();
		~MouseGrabber();
		void setGrabsMouse(bool grabs) { m_grabsMouse = grabs; }
		bool grabsMouse() const { return m_grabsMouse; }
	protected:
		virtual void mousePressEvent(QMouseEvent* const event) = 0;
		virtual void mouseDoubleClickEvent(QMouseEvent* const event) = 0;
		/*! Mouse release event callback method. See mousePressEvent(). */
		virtual void mouseReleaseEvent(QMouseEvent* const event) = 0;
		virtual void mouseMoveEvent(QMouseEvent* const event) = 0;
		virtual void wheelEvent(QWheelEvent* const event) = 0;

		bool m_grabsMouse;
		bool m_MouseMovePressed = false;
		bool m_MouseRotatePressed = false;
		bool m_MouseZoomPressed = false;
	};
}

