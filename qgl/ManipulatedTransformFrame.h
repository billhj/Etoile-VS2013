#ifndef MANIPULATEDTRANSFORMFRAME_H
#define MANIPULATEDTRANSFORMFRAME_H

#include <QTime>
#include <QTimer>
#include "TransformFrame.h"
#include "MouseGrabber.h"

namespace Etoile
{
	class ManipulatedTransformFrame : public TransformFrame, public MouseGrabber
	{
		Q_OBJECT

	public:
		ManipulatedTransformFrame(TransformFrame* parent = NULL);
		~ManipulatedTransformFrame();



		qreal rotationSensitivity() const { return m_rotationSensitivity; }
		qreal translationSensitivity() const { return m_translationSensitivity; }
		qreal spinningSensitivity() const { return m_spinningSensitivity; }
		qreal zoomSensitivity() const { return m_zoomSensitivity; }
		qreal wheelSensitivity() const { return m_wheelSensitivity; }
		bool isSpinning() const { return m_isSpinning; }
		Quaternionf spinningQuaternion() const { return m_spinningQuaternion; }

	Q_SIGNALS:
		void manipulated();
		void spun();

		public
		Q_SLOTS:
		/*! Defines the rotationSensitivity(). */
		void setRotationSensitivity(qreal sensitivity) { m_rotationSensitivity = sensitivity; }
		/*! Defines the translationSensitivity(). */
		void setTranslationSensitivity(qreal sensitivity) { m_translationSensitivity = sensitivity; }
		/*! Defines the spinningSensitivity(), in pixels per milliseconds. */
		void setSpinningSensitivity(qreal sensitivity) { m_spinningSensitivity = sensitivity; }
		/*! Defines the wheelSensitivity(). */
		void setWheelSensitivity(qreal sensitivity) { m_wheelSensitivity = sensitivity; }
		/*! Defines the zoomSensitivity(). */
		void setZoomSensitivity(qreal sensitivity) { m_zoomSensitivity = sensitivity; }

		void setSpinningQuaternion(const Quaternionf& spinningQuaternion) { m_spinningQuaternion = spinningQuaternion; }
		virtual void startSpinning(int updateInterval);
		/*! Stops the spinning motion started using startSpinning(). isSpinning() will return \c false
		after this call. */
		virtual void stopSpinning() { m_spinningTimer.stop(); m_isSpinning = false; }

		protected Q_SLOTS:
		virtual void spin();
		private Q_SLOTS:
		void spinUpdate();

	public:
		virtual void mousePressEvent(QMouseEvent* const event) override;
		virtual void mouseDoubleClickEvent(QMouseEvent* const event) override;
		/*! Mouse release event callback method. See mousePressEvent(). */
		virtual void mouseReleaseEvent(QMouseEvent* const event) override;
		virtual void mouseMoveEvent(QMouseEvent* const event) override;
		virtual void wheelEvent(QWheelEvent* const event) override;
	protected:
		qreal m_rotationSensitivity;
		qreal m_translationSensitivity;
		qreal m_spinningSensitivity;
		qreal m_wheelSensitivity;
		qreal m_zoomSensitivity;

		// Mouse speed and spinning
		QTime m_last_move_time;
		qreal m_mouseSpeed;
		int m_delay;
		bool m_isSpinning;
		QTimer m_spinningTimer;
		Quaternionf m_spinningQuaternion;

		// Whether the SCREEN_TRANS direction (horizontal or vertical) is fixed or not.
		bool m_dirIsFixed;

		// MouseGrabber
		bool m_keepsGrabbingMouse;
		QPoint m_prevPos, m_pressPos;
	};
}

#endif // MANIPULATEDTRANSFORMFRAME_H
