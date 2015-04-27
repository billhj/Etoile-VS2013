/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Quaternionf.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <stdio.h>
#include <iostream>
#include "Vectors.h"
#include "Matrices.h"

#ifndef M_PI
#define M_PI 3.14159265f
#endif

namespace Etoile
{
	using namespace std;
#ifndef THRESHOLD_ROTATION
#define THRESHOLD_ROTATION 0.01
#endif

	class Quaternionf
	{
	private:
		/*! The internal data representation is private, use operator[] to access values. */
		float q[4];

	public:

		Quaternionf()
		{ 
			q[0]=q[1]=q[2]=0.0f;  
			q[3]=1.0f; 
		}

		Quaternionf(const Vec3f& axis, float angle)
		{
			setAxisAngle(axis, angle);
		}

		Quaternionf(float  q0, float  q1, float  q2, float  q3)
		{ 
			q[0]=q0;    q[1]=q1;    q[2]=q2;    q[3]=q3; 
		}

		/*! Copy constructor. */
		Quaternionf(const Quaternionf& Q)
		{ 
			for (int i=0; i<4; ++i) 
			{
				q[i] = Q.q[i]; 
			}
		}

		/*! Sets the Quaternionf as a rotation of axis \p axis and angle \p angle (in radians).
		\p axis does not need to be normalized. A null \p axis will result in an identity Quaternionf. */
		void setAxisAngle(const Vec3f & axis, float  angle)
		{
			const float norm = axis.norm();
			if (norm < 1E-8)
			{
				// Null rotation
				q[0] = 0.0;      q[1] = 0.0;      q[2] = 0.0;      q[3] = 1.0;
			}
			else
			{
				const float  sin_half_angle = sin(angle / 2.0f);
				q[0] = sin_half_angle * axis[0]/norm;
				q[1] = sin_half_angle * axis[1]/norm;
				q[2] = sin_half_angle * axis[2]/norm;
				q[3] = cos(angle / 2.0f);
			}
		}

		/*! Sets the Quaternionf value. See the Quaternionf(float , float , float , float ) constructor documentation. */
		void setValue(float  q0, float  q1, float  q2, float  q3)
		{ 
			q[0]=q0;    
			q[1]=q1;    
			q[2]=q2;    
			q[3]=q3; 
		}

		void setValue(const Quaternionf& Q)
		{ 
			for (int i=0; i<4; ++i)
			{ 
				q[i] = Q.q[i]; 
			}
		}


		/*
		**
		* operator.
		*/
		friend ostream& operator<<(ostream& o, const Quaternionf& Q)
		{
			return o << Q[0] << '\t' << Q[1] << '\t' << Q[2] << '\t' << Q[3];
		}

		float  operator[](int i) const 
		{ 
			return q[i]; 
		}

		float & operator[](int i) 
		{ 
			return q[i]; 
		}

		Quaternionf operator *(float  scale) const
		{ 
			return Quaternionf(q[0]*scale,q[1]*scale,q[2]*scale,q[3]*scale); 
		}

		Quaternionf operator /(float  scale) const
		{ 
			return Quaternionf(q[0]/scale,q[1]/scale,q[2]/scale,q[3]/scale); 
		}


		Quaternionf &operator =(const Quaternionf &q)		
		{ 
			this->q[0] = q[0]; 
			this->q[1] = q[1]; 
			this->q[2] = q[2];  
			this->q[3] = q[3];
			return *this; 
		}

		Quaternionf operator +(const Quaternionf &q) const	
		{ 
			return Quaternionf(this->q[0]+q[0], this->q[1]+ q[1], this->q[2]+q[2],  this->q[3]+q[3]);
		}

		Quaternionf operator -(const Quaternionf &q) const	
		{ 
			return Quaternionf(this->q[0]-q[0], this->q[1]-q[1], this->q[2]-q[2], this->q[3]-q[3]); 
		}

		friend Quaternionf operator*(const Quaternionf& a, const Quaternionf& b)
		{
			return Quaternionf(a.q[3]*b.q[0] + b.q[3]*a.q[0] + a.q[1]*b.q[2] - a.q[2]*b.q[1],
				a.q[3]*b.q[1] + b.q[3]*a.q[1] + a.q[2]*b.q[0] - a.q[0]*b.q[2],
				a.q[3]*b.q[2] + b.q[3]*a.q[2] + a.q[0]*b.q[1] - a.q[1]*b.q[0],
				a.q[3]*b.q[3] - b.q[0]*a.q[0] - a.q[1]*b.q[1] - a.q[2]*b.q[2]);
		}

		friend Quaternionf operator/(const Quaternionf& a, const Quaternionf& b)
		{
			Quaternionf p(b); 
			p.invert(); 
			return a * p;
		}


		bool operator==(const Quaternionf &q)
		{
			if(q[3]==q.q[3] && q[2]==q.q[2] &&q[1]==q.q[1] &&q[0]==q.q[0])
				return true;
			return false;
		}

		bool operator!=(const Quaternionf &q)
		{
			if(q[3]==q.q[3] && q[2]==q.q[2] &&q[1]==q.q[1] &&q[0]==q.q[0])
				return false;
			return true;
		}

		Quaternionf& operator*=(const Quaternionf &q)
		{
			*this = (*this)*q;
			return *this;
		}

		Quaternionf& operator/=(const Quaternionf &q)
		{
			*this = (*this)*q.inverse();
			return *this;
		}

		friend Vec3f  operator*(const Quaternionf& q, const Vec3f & v)
		{
			return q.rotate(v);
		}

		float x()
		{
			return q[0];
		}

		float y()
		{
			return q[1];
		}

		float z()
		{
			return q[2];
		}

		float w()
		{
			return q[3];
		}

		//Matrix
		void setFromRotationMatrix(const Matrix3f m)
		{
			// Compute one plus the trace of the matrix
			const float  onePlusTrace = 1.0 + m(0,0) + m(1,1) + m(2,2);

			if (onePlusTrace > 1E-5)
			{
				// Direct computation
				const float  s = sqrt(onePlusTrace) * 2.0;
				q[0] = (m(2,1) - m(1,2)) / s;
				q[1] = (m(0,2) - m(2,0)) / s;
				q[2] = (m(1,0) - m(0,1)) / s;
				q[3] = 0.25 * s;
			}
			else
			{
				// Computation depends on major diagonal term
				if ((m(0,0) > m(1,1))&(m(0,0) > m(2,2)))
				{ 
					const float  s = sqrt(1.0 + m(0,0) - m(1,1) - m(2,2)) * 2.0; 
					q[0] = 0.25 * s;
					q[1] = (m(0,1) + m(1,0)) / s; 
					q[2] = (m(0,2) + m(2,0)) / s; 
					q[3] = (m(2,1) - m(1,2)) / s;
				}
				else
					if (m(1,1) > m(2,2))
					{ 
						const float  s = sqrt(1.0 + m(1,1) - m(0,0) - m(2,2)) * 2.0; 
						q[0] = (m(0,1) + m(1,0)) / s; 
						q[1] = 0.25 * s;
						q[2] = (m(1,2) + m(2,1)) / s; 
						q[3] = (m(0,2) - m(2,0)) / s;
					}
					else
					{ 
						const float  s = sqrt(1.0 + m(2,2) - m(0,0) - m(1,1)) * 2.0; 
						q[0] = (m(0,2) + m(2,0)) / s; 
						q[1] = (m(1,2) + m(2,1)) / s; 
						q[2] = 0.25 * s;
						q[3] = (m(1,0) - m(0,1)) / s;
					}
			}
			normalize();
		}


		void setFromRotatedBasis(const Vec3f & X, const Vec3f & Y, const Vec3f & Z)
		{
			Matrix3f  m;
			float  normX = X.norm();
			float  normY = Y.norm();
			float  normZ = Z.norm();

			for (int i=0; i<3; ++i)
			{
				m(i,0) = X[i] / normX;
				m(i,1) = Y[i] / normY;
				m(i,2) = Z[i] / normZ;
			}

			setFromRotationMatrix(m);
		}


		//@{
		Vec3f  axis() const
		{
			Vec3f  res = Vec3f(q[0], q[1], q[2]);
			const float sinus = res.norm();
			if (sinus > 1E-8)
				res /= sinus;
			return (acos(q[3]) <= M_PI/2.0) ? res : -res;
		}

		float  angle() const
		{
			const float angle = 2.0 * acos(q[3]);
			return (angle <= M_PI) ? angle : 2.0*M_PI - angle;
		}

		void getAxisAngle(Vec3f & axis, float & angle) const
		{
			angle = 2.0*acos(q[3]);
			axis = Vec3f(q[0], q[1], q[2]);
			const float sinus = axis.norm();
			if (sinus > 1E-8)
				axis /= sinus;

			if (angle > M_PI)
			{
				angle = 2.0*M_PI - angle;
				axis = -axis;
			}
		}




		Vec3f rotate(const Vec3f & v) const{
			Vec3f vn(v);
			Quaternionf vecQuat, resQuat;
			vecQuat[0] = vn.x();
			vecQuat[1] = vn.y();
			vecQuat[2] = vn.z();
			vecQuat[3] = 0.0f;

			resQuat = vecQuat * inverse();
			resQuat = *this * resQuat;
			Vec3f re = Vec3f(resQuat[0], resQuat[1], resQuat[2]);

			return re;
		}

		Vec3f inverseRotate(const Vec3f & v) const
		{
			return inverse().rotate(v);
		}


		Quaternionf conjugate( void ) const
		{
			return Quaternionf(-q[0], -q[1], -q[2], q[3]);;
		}


		void invert() { q[0] = -q[0]; q[1] = -q[1]; q[2] = -q[2]; }

		Quaternionf inverse() const 
		{ 
			float  scalar = 1 / ( q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
			return conjugate() * scalar; 
		}


		void negate() { invert(); q[3] = -q[3]; }

		float norm()
		{
			return  (sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]));
		}

		float normalize()
		{
			const float norm = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
			for (int i=0; i<4; ++i)
				q[i] /= norm;
			return norm;
		}

		Quaternionf normalized() const
		{
			float Q[4];
			const float norm = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
			for (int i=0; i<4; ++i)
			{
				Q[i] = q[i] / norm;
			}
			return Quaternionf(Q[0], Q[1], Q[2], Q[3]);
		}


		void getMatrix(Matrix4f&  m) const
		{
			const float  q00 = 2.0 * q[0] * q[0];
			const float  q11 = 2.0 * q[1] * q[1];
			const float  q22 = 2.0 * q[2] * q[2];

			const float  q01 = 2.0 * q[0] * q[1];
			const float  q02 = 2.0 * q[0] * q[2];
			const float  q03 = 2.0 * q[0] * q[3];

			const float  q12 = 2.0 * q[1] * q[2];
			const float  q13 = 2.0 * q[1] * q[3];

			const float  q23 = 2.0 * q[2] * q[3];
			
			m(0,0) = 1.0 - q11 - q22;
			m(0,1) =        q01 - q23;
			m(0,2) =        q02 + q13;

			m(1,0) =        q01 + q23;
			m(1,1) = 1.0 - q22 - q00;
			m(1,2) =        q12 - q03;

			m(2,0) =        q02 - q13;
			m(2,1) =        q12 + q03;
			m(2,2) = 1.0 - q11 - q00;

			m(0,3) = 0.0;
			m(1,3) = 0.0;
			m(2,3) = 0.0;

			m(3,0) = 0.0;
			m(3,1) = 0.0;
			m(3,2) = 0.0;
			m(3,3) = 1.0;
			//cout << m;
		}

		void getRotationMatrix(Matrix3f&  m) const
		{
			const float  q00 = 2.0 * q[0] * q[0];
			const float  q11 = 2.0 * q[1] * q[1];
			const float  q22 = 2.0 * q[2] * q[2];

			const float  q01 = 2.0 * q[0] * q[1];
			const float  q02 = 2.0 * q[0] * q[2];
			const float  q03 = 2.0 * q[0] * q[3];

			const float  q12 = 2.0 * q[1] * q[2];
			const float  q13 = 2.0 * q[1] * q[3];

			const float  q23 = 2.0 * q[2] * q[3];

			m(0,0) = 1.0 - q11 - q22;
			m(0,1) =        q01 - q23;
			m(0,2) =        q02 + q13;

			m(1,0) =        q01 + q23;
			m(1,1) = 1.0 - q22 - q00;
			m(1,2) =        q12 - q03;

			m(2,0) =        q02 - q13;
			m(2,1) =        q12 + q03;
			m(2,2) = 1.0 - q11 - q00;
		}

		const Matrix4f inverseMatrix() const
		{
			static Matrix4f m;
			getInverseMatrix(m);
			return m;
		}

		void getInverseMatrix(Matrix4f& m) const
		{
			inverse().getMatrix(m);
		}

		void getInverseRotationMatrix(Matrix3f& m) const
		{
			inverse().getInverseRotationMatrix(m);
		}


		/*! @name linear lerp interpolation */
		static Quaternionf lerp(const Quaternionf &q1, const Quaternionf &q2, float t) 
		{ 
			return (q1*(1-t) + q2*t).normalized(); 
		}

		/*! @name spherical linear Slerp interpolation */
		static Quaternionf slerp(const Quaternionf& a, const Quaternionf& b, float t, bool allowFlip=true)
		{
			float cosAngle = Quaternionf::dot(a, b);

			float c1, c2;
			// Linear interpolation for close orientations
			if ((1.0 - fabs(cosAngle)) < THRESHOLD_ROTATION)
			{
				c1 = 1.0f - t;
				c2 = t;
			}
			else
			{
				// Spherical interpolation
				float angle    = acos(fabs(cosAngle));
				float sinAngle = sin(angle);
				c1 = sin(angle * (1.0f - t)) / sinAngle;
				c2 = sin(angle * t) / sinAngle;
			}

			// Use the shortest path
			if (allowFlip && (cosAngle < 0.0))
				c1 = -c1;

			return Quaternionf(c1*a[0] + c2*b[0], c1*a[1] + c2*b[1], c1*a[2] + c2*b[2], c1*a[3] + c2*b[3]);
		}

		static Quaternionf squad(const Quaternionf& a, const Quaternionf& tgA, const Quaternionf& tgB, const Quaternionf& b, float t)
		{
			Quaternionf ab = Quaternionf::slerp(a, b, t);
			Quaternionf tg = Quaternionf::slerp(tgA, tgB, t, false);
			return Quaternionf::slerp(ab, tg, 2.0*t*(1.0-t), false);
		}


		//! Shoemake-Bezier interpolation using De Castlejau algorithm
		static Quaternionf bezier(const Quaternionf &q1,const Quaternionf &q2,const Quaternionf &a,const Quaternionf &b, float t)
		{
			// level 1
			Quaternionf q11= slerp(q1,a,t),
				q12= slerp(a,b,t),
				q13= slerp(b,q2,t);		
			// level 2 and 3
			return slerp(slerp(q11,q12,t), slerp(q12,q13,t), t);
		}

		//! Given 3 Quaternionfs, qn-1,qn and qn+1, calculate a control point to be used in spline interpolation
		static Quaternionf spline(const Quaternionf &qnm1,const Quaternionf &qn,const Quaternionf &qnp1)
		{
			Quaternionf qni(qn.inverse());	
			return qn * (( (qni*qnm1).log()+(qni*qnp1).log() )/(-4)).exp();
		}

		static float  dot(const Quaternionf& a, const Quaternionf& b) 
		{ 
			return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3]; 
		}

		Quaternionf log()
		{
			float len = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2]);

			if (len < 1E-6)
				return Quaternionf(q[0], q[1], q[2], 0.0);
			else
			{
				float coef = acos(q[3]) / len;
				return Quaternionf(q[0]*coef, q[1]*coef, q[2]*coef, 0.0);
			}
		}

		Quaternionf exp()
		{
			float theta = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2]);

			if (theta < 1E-6)
				return Quaternionf(q[0], q[1], q[2], cos(theta));
			else
			{
				float coef = sin(theta) / theta;
				return Quaternionf(q[0]*coef, q[1]*coef, q[2]*coef, cos(theta));
			}
		}

		static Quaternionf lnDif(const Quaternionf& a, const Quaternionf& b)
		{
			Quaternionf dif = a.inverse()*b;
			dif.normalize();
			return dif.log();
		}

		static Quaternionf squadTangent(const Quaternionf& before, const Quaternionf& center, const Quaternionf& after)
		{
			Quaternionf l1 = Quaternionf::lnDif(center,before);
			Quaternionf l2 = Quaternionf::lnDif(center,after);
			Quaternionf e;
			for (int i=0; i<4; ++i)
			{
				e.q[i] = -0.25 * (l1.q[i] + l2.q[i]);
			}
			e = center*(e.exp());

			// if (Quaternionf::dot(e,b) < 0.0)
			// e.negate();

			return e;
		}


		/*! @name Random Quaternionf */
		static Quaternionf randomQuaternionf()
		{
			// The rand() function is not very portable and may not be available on your system.
			// Add the appropriate include or replace by an other random function in case of problem.
			float seed = rand()/RAND_MAX;
			float r1 = sqrt(1.0 - seed);
			float r2 = sqrt(seed);
			float t1 = 2.0 * M_PI * (rand()/RAND_MAX);
			float t2 = 2.0 * M_PI * (rand()/RAND_MAX);
			return Quaternionf(sin(t1)*r1, cos(t1)*r1, sin(t2)*r2, cos(t2)*r2);
		}

		// Convert from Z Y X Euler Angles individual RZ - RY - RX,  mulipli RZ* RY * RX   
		void fromEulerXYZ(float roll, float pitch, float yaw)
		{
			float y = yaw  / 2.0;
			float p = pitch  / 2.0;
			float r = roll  / 2.0;

			float sinp = sin(p);
			float siny = sin(y);
			float sinr = sin(r);
			float cosp = cos(p);
			float cosy = cos(y);
			float cosr = cos(r);

			q[0] = sinr * cosp * cosy - cosr * sinp * siny;
			q[1] = cosr * sinp * cosy + sinr * cosp * siny;
			q[2] = cosr * cosp * siny - sinr * sinp * cosy;
			q[3] = cosr * cosp * cosy + sinr * sinp * siny;

			normalize();

		}



		void fromEulerXYZByAngle(float roll, float pitch, float yaw){	
			const float PIOVER180 = 3.14159265 / 180.0;
			float y = yaw * PIOVER180 / 2.0;
			float p = pitch * PIOVER180 / 2.0;
			float r = roll * PIOVER180 / 2.0;

			float sinp = sin(p);
			float siny = sin(y);
			float sinr = sin(r);
			float cosp = cos(p);
			float cosy = cos(y);
			float cosr = cos(r);

			q[0] = sinr * cosp * cosy - cosr * sinp * siny;
			q[1] = cosr * sinp * cosy + sinr * cosp * siny;
			q[2] = cosr * cosp * siny - sinr * sinp * cosy;
			q[3] = cosr * cosp * cosy + sinr * sinp * siny;

			normalize();
		}


		// Convert from Z Y X Euler Angles individual RZ - RY - RX,  mulipli RZ * RY * RX  
		Vec3f  getEulerAngleXYZ()const
		{
			//atan2
			float roll = atan2(2 * (q[3]* q[0] + q[1]* q[2]),   1 - 2*( q[0] * q[0] + q[1] * q[1]));
			float v = q[3] * q[1] - q[2] * q[0];
			if(v > 0.5) v = 0.5;
			if(v < -0.5) v = -0.5;
			float pitch = asin(2* ( v ));
			float yaw = atan2( 2 * ( q[3] * q[2] +  q[0] * q[1]), 1 - 2*( q[1] * q[1] + q[2] * q[2]));

			if(v == 0.5)
			{
				roll = 2 * atan2(q[0], q[3]);
				pitch = M_PI * 0.5;
				yaw = 0;
			}
			else if(v == - 0.5)
			{
				roll = -2 * atan2(q[0], q[3]);
				pitch = -M_PI * 0.5;
				yaw = 0;
			}
			return Vec3f( roll, pitch, yaw);
		}

		Vec3f getEulerAngleXYZBYAngle() const
		{
			const float PIOVER180 =  180.0 /3.14159265;
			Vec3f xyz = getEulerAngleXYZ();
			return Vec3f( xyz.x() * PIOVER180, xyz.y() * PIOVER180, xyz.z() * PIOVER180);
		}

		void fromEulerZYZ(float a, float b, float c)
		{
			q[3] = -cos( (a - c) * 0.5) * sin(b * 0.5);
			q[0] = -sin( (a - c) * 0.5) * sin(b * 0.5);
			q[1] = -sin( (a + c) * 0.5) * cos(b * 0.5);
			q[2] =  sin( (a + c) * 0.5) * cos(b * 0.5);
		}

		Vec3f  getEulerAngleZYZ()const
		{
			float a = atan2( (q[3] *q[1] + q[0]*q[2]), (q[0]*q[1] - q[3]*q[2]));
			float b = acos(-q[3] * q[3] - q[0] *q[0] + q[1] * q[1] + q[2] *q[2]);
			float c = - atan2( (q[3] *q[1] - q[0]*q[2]), (q[0]*q[1] + q[3]*q[2]));
			return Vec3f(a,b,c);
		}

	};

}