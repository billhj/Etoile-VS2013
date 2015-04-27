/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Matrices.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <Eigen/Dense>
#include <Eigen/StdVector>
namespace Etoile
{
	typedef Eigen::Matrix2d Matrix2d;
	typedef Eigen::Matrix3d Matrix3d;
	typedef Eigen::Matrix4d Matrix4d;
	typedef Eigen::MatrixXd MatrixXd;

	typedef Eigen::Matrix2f Matrix2f;
	typedef Eigen::Matrix3f Matrix3f;
	typedef Eigen::Matrix4f Matrix4f;
	typedef Eigen::MatrixXf MatrixXf;
}

EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Etoile::Matrix2d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Etoile::Matrix3d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Etoile::Matrix4d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Etoile::MatrixXd)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Etoile::Matrix2f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Etoile::Matrix3f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Etoile::Matrix4f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Etoile::MatrixXf)