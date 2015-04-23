#include "ManipulatedTransformFrame.h"
namespace Etoile
{
	ManipulatedTransformFrame::ManipulatedTransformFrame(TransformFrame* parent)
		: TransformFrame(parent)
	{

	}

	ManipulatedTransformFrame::~ManipulatedTransformFrame()
	{

	}

	void ManipulatedTransformFrame::startSpinning(int updateInterval)
	{
		m_isSpinning = true;
		m_spinningTimer.start(updateInterval);
	}

	void ManipulatedTransformFrame::spin()
	{
		rotate(spinningQuaternion());
	}

	void ManipulatedTransformFrame::spinUpdate()
	{
		spin();
		Q_EMIT spun();
	}

	void ManipulatedTransformFrame::mousePressEvent(QMouseEvent* const event)
	{
		
	}

	void ManipulatedTransformFrame::mouseDoubleClickEvent(QMouseEvent* const event)
	{
		
	}

	void ManipulatedTransformFrame::mouseReleaseEvent(QMouseEvent* const event)
	{

	}

	void ManipulatedTransformFrame::mouseMoveEvent(QMouseEvent* const event)
	{
		
	}

	void ManipulatedTransformFrame::wheelEvent(QWheelEvent* const event)
	{

	}
}