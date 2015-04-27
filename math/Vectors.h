/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Vectors.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <Eigen/Dense>
#include <Eigen/StdVector>
namespace Etoile
{
	typedef Eigen::Vector2d Vec2d;
	typedef Eigen::Vector3d Vec3d;
	typedef Eigen::Vector4d Vec4d;
	typedef Eigen::VectorXd VecXd;

	typedef Eigen::Vector2f Vec2f;
	typedef Eigen::Vector3f Vec3f;
	typedef Eigen::Vector4f Vec4f;
	typedef Eigen::VectorXf VecXf;
}

EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Etoile::Vec2d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Etoile::Vec3d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Etoile::Vec4d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Etoile::VecXd)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Etoile::Vec2f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Etoile::Vec3f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Etoile::Vec4f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Etoile::VecXf)