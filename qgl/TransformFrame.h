#ifndef TRANSFORMFRAME_H
#define TRANSFORMFRAME_H

#include <QObject>
#include "Vectors.h"
#include "Quaternion.h"
#include "Matrices.h"
namespace Etoile
{
	class TransformFrame : public QObject
	{
		Q_OBJECT

	public:
		TransformFrame(TransformFrame* parent = NULL);
		~TransformFrame();
		void setReferenceFrame(TransformFrame* f){ m_referenceFrame = f; }
		void setPosition(const Vec3f& position);
		void setOrientation(const Quaternionf& orientation);
		void setTranslation(const Vec3f& t){ m_t = t; Q_EMIT modified(); }
		void translate(const Vec3f& t){ m_t += t; Q_EMIT modified(); }
		void setRotation(const Quaternionf& r){ m_q = r; Q_EMIT modified(); }
		void rotate(const Quaternionf& r){ m_q *= r;  m_q.normalize(); Q_EMIT modified(); }
		const TransformFrame* referenceFrame() const { return m_referenceFrame; }
		Vec3f translation() const { return m_t; }
		Quaternionf rotation() const { return m_q; }
		Vec3f position() const;
		Quaternionf orientation() const;

		void getMatrix(Matrix4f& matrix);
		void getGLMatrix(Matrix4f& matrix);
		void getWorldMatrix(Matrix4f& matrix);
		void getGLWorldMatrix(Matrix4f& matrix);

		/*! @name Coordinate system transformation of 3D coordinates */
		//@{
		Vec3f coordinatesOf(const Vec3f& src) const;
		Vec3f inverseCoordinatesOf(const Vec3f& src) const;
		Vec3f localCoordinatesOf(const Vec3f& src) const;
		Vec3f localInverseCoordinatesOf(const Vec3f& src) const;

		/***
		*** transform in rotation space
		**/
		Vec3f transformOf(const Vec3f& src) const;
		Vec3f inverseTransformOf(const Vec3f& src) const;
		Vec3f localTransformOf(const Vec3f& src) const;
		Vec3f localInverseTransformOf(const Vec3f& src) const;
	Q_SIGNALS:
		void modified();
	private:
		//orientation
		Quaternionf m_q;
		//position
		Vec3f m_t;
		//reference
		const TransformFrame* m_referenceFrame;
	};
}
#endif // TRANSFORMFRAME_H
