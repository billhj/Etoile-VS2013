/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ExponentialMap.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Vectors.h"
#include "Quaternion.h"

namespace Etoile
{
	class ExponentialMapf
	{
	public:
		ExponentialMapf()
		{

		}

		ExponentialMapf(Vec3f axis, float angle)
		{
			_axis = axis.normalized();
			_angle = angle;
		}

		ExponentialMapf(Quaternionf value)
		{
			_axis = value.axis().normalized();
			_angle = value.angle();
		}

		Quaternionf getQuaternionf()
		{
			Quaternionf q;
			q.setAxisAngle(_axis, _angle);
			return q;
		}

		Vec3f operator *(Vec3f v)
		{
			return rotate(v);
		}

		ExponentialMapf inverse() const
		{
			return ExponentialMapf(_axis, -_angle);
		}

		Vec3f rotate(const Vec3f & v) const
		{
			Vec3f r = v * cos(_angle)
				+ _axis.cross3(v) * sin(_angle)
				+ _axis * _axis.dot(v) * (1 - cos(_angle));
			return r;
		}

		Vec3f inverseRotate(const Vec3f & v) const
		{
			return inverse().rotate(v);
		}

		void invert()
		{
			_angle = -_angle;
		}
	private:
		Vec3f _axis;
		float _angle;

	};
}