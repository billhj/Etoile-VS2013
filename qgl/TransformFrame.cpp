#include "TransformFrame.h"
namespace Etoile
{
	TransformFrame::TransformFrame(TransformFrame* parent)
		: m_referenceFrame(parent)
	{

	}

	TransformFrame::~TransformFrame()
	{

	}

	void TransformFrame::setPosition(const Vec3f& position)
	{
		if (referenceFrame())
			setTranslation(referenceFrame()->coordinatesOf(position));
		else
			setTranslation(position);
	}

	void TransformFrame::setOrientation(const Quaternionf& orientation)
	{
		if (referenceFrame())
			setRotation(referenceFrame()->orientation().inverse() * orientation);
		else
			setRotation(orientation);
	}

	Vec3f TransformFrame::position() const {
		if (m_referenceFrame)
			return inverseCoordinatesOf(Vec3f(0.0, 0.0, 0.0));
		else
			return m_t;
	}

	Quaternionf TransformFrame::orientation() const
	{
		Quaternionf res = rotation();
		const TransformFrame* fr = referenceFrame();
		while (fr != NULL)
		{
			res = fr->rotation() * res;
			fr = fr->referenceFrame();
		}
		return res;
	}


	void TransformFrame::getMatrix(Matrix4f& matrix){
		m_q.getMatrix(matrix);
		matrix(0,3) = m_t[0];
		matrix(1,3) = m_t[1];
		matrix(2,3) = m_t[2];
	}

	void TransformFrame::getGLMatrix(Matrix4f& matrix){
		m_q.getMatrix(matrix);
		matrix(0, 3) = m_t[0];
		matrix(1, 3) = m_t[1];
		matrix(2, 3) = m_t[2];
		matrix.transposeInPlace();
	}

	void TransformFrame::getWorldMatrix(Matrix4f& matrix){
		orientation().getMatrix(matrix);
		Vec3f pos = position();
		matrix(0, 3) = pos[0];
		matrix(1, 3) = pos[1];
		matrix(2, 3) = pos[2];
	}

	void TransformFrame::getGLWorldMatrix(Matrix4f& matrix){
		orientation().getMatrix(matrix);
		Vec3f pos = position();
		matrix(0, 3) = pos[0];
		matrix(1, 3) = pos[1];
		matrix(2, 3) = pos[2];
		matrix.transposeInPlace();
	}

	/*! Returns the Frame coordinates of a point \p src defined in the world coordinate system (converts
	from world to Frame).*/
	Vec3f TransformFrame::coordinatesOf(const Vec3f& src) const
	{
		if (referenceFrame())
			return localCoordinatesOf(referenceFrame()->coordinatesOf(src));
		else
			return localCoordinatesOf(src);
	}

	/*! Returns the world coordinates of the point whose position in the Frame coordinate system is \p
	src (converts from Frame to world).*/
	Vec3f TransformFrame::inverseCoordinatesOf(const Vec3f& src) const
	{
		const TransformFrame* fr = this;
		Vec3f res = src;
		while (fr != NULL)
		{
			res = fr->localInverseCoordinatesOf(res);
			fr = fr->referenceFrame();
		}
		return res;
	}

	/*! Returns the Frame coordinates of a point \p src defined in the referenceFrame() coordinate
	system (converts from referenceFrame() to Frame).*/
	Vec3f TransformFrame::localCoordinatesOf(const Vec3f& src) const
	{
		return rotation().inverseRotate(src - translation());
	}

	/*! Returns the referenceFrame() coordinates of a point \p src defined in the Frame coordinate
	system (converts from Frame to referenceFrame()).*/
	Vec3f TransformFrame::localInverseCoordinatesOf(const Vec3f& src) const
	{
		return rotation().rotate(src) + translation();
	}

	Vec3f TransformFrame::transformOf(const Vec3f& src) const
	{
		if (referenceFrame())
			return localTransformOf(referenceFrame()->transformOf(src));
		else
			return localTransformOf(src);
	}

	/*! Returns the world transform of the vector whose coordinates in the Frame coordinate
	system is \p src (converts vectors from Frame to world).
	transformOf() performs the inverse transformation. Use inverseCoordinatesOf() to transform 3D
	coordinates instead of 3D vectors. */
	Vec3f TransformFrame::inverseTransformOf(const Vec3f& src) const
	{
		const TransformFrame* fr = this;
		Vec3f res = src;
		while (fr != NULL)
		{
			res = fr->localInverseTransformOf(res);
			fr = fr->referenceFrame();
		}
		return res;
	}

	/*! Returns the Frame transform of a vector \p src defined in the referenceFrame() coordinate system
	(converts vectors from referenceFrame() to Frame).
	localInverseTransformOf() performs the inverse transformation. See also localCoordinatesOf(). */
	Vec3f TransformFrame::localTransformOf(const Vec3f& src) const
	{
		return rotation().inverseRotate(src);
	}

	/*! Returns the referenceFrame() transform of a vector \p src defined in the Frame coordinate
	system (converts vectors from Frame to referenceFrame()).
	localTransformOf() performs the inverse transformation. See also localInverseCoordinatesOf(). */
	Vec3f TransformFrame::localInverseTransformOf(const Vec3f& src) const
	{
		return rotation().rotate(src);
	}
}
