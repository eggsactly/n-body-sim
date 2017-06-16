/*
 * Copyright (c) 2016 - 2017 W.A. Garrett Weaver
 *
 * This file is part of n-body-sim.
 *
 * n-body-sim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * n-body-sim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with n-body-sim.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <cmath>
#include <boost/serialization/array_wrapper.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "NBodyTypes.h"
#include "ParticlePlotter.h"

template <class T>
NBodySim::ParticlePlotter<T>::ParticlePlotter(void){
	theta = 0;
	phi = 0;
	A = calculateGraphicsMatrix(theta, phi);
}

template <class T>
NBodySim::ParticlePlotter<T>::~ParticlePlotter(void){
	// Do nothing
}

// Source: http://www.crystalclearsoftware.com/cgi-bin/boost_wiki/wiki.pl?LU_Matrix_Inversion
template<class T> 
bool NBodySim::ParticlePlotter<T>::InvertMatrix (const boost::numeric::ublas::matrix<T>& input, boost::numeric::ublas::matrix<T>& inverse) { 
    typedef  boost::numeric::ublas::permutation_matrix<std::size_t> pmatrix; 
    // create a working copy of the input 
    boost::numeric::ublas::matrix<T> A(input); 
    // create a permutation matrix for the LU-factorization 
    pmatrix pm(A.size1()); 
    // perform LU-factorization 
    int res = lu_factorize(A,pm); 
    if( res != 0 )
        return false; 
    // create identity matrix of "inverse" 
    inverse.assign(boost::numeric::ublas::identity_matrix<T>(A.size1())); 
    // backsubstitute to get the inverse 
    boost::numeric::ublas::lu_substitute(A, pm, inverse); 
    return true; 
}

template<class T> 
boost::numeric::ublas::matrix<T> NBodySim::ParticlePlotter<T>::calculateGraphicsMatrix (T theta, T phi){
	boost::numeric::ublas::matrix<T> A(3, 3);
	
	// Calculate the graphics matrix
	A(0, 0) = cos(theta);
	A(0, 1) = -1.0f * sin(theta) * cos(phi);
	A(0, 2) = -1.0f * sin(theta) * sin(phi);
	A(1, 0) = sin(theta);
	A(1, 1) = cos(theta) * cos(phi);
	A(1, 2) = cos(theta) * sin(phi);
	A(2, 0) = 0.0f;
	A(2, 1) = sin(phi);
	A(2, 2) = -1.0f * cos(phi);
	
	return A;
}

template <class T>
void NBodySim::ParticlePlotter<T>::setAngle(T az, T el){
	if((theta != az) || (phi != el)){
		theta = az;
		phi = el;
		
		A = calculateGraphicsMatrix(theta, phi);
		
		// Invert the graphics matrix
		InvertMatrix (A, A);
	}
}

template <class T>
boost::numeric::ublas::vector<T> NBodySim::ParticlePlotter<T>::calculateProjection(NBodySim::Particle<T> particle){
	boost::numeric::ublas::vector<NBodySim::FloatingType> particlePoints(3);
	boost::numeric::ublas::vector<NBodySim::FloatingType> projectedPoints(3);
	boost::numeric::ublas::vector<NBodySim::FloatingType> returnVal(2);
	
	particlePoints(0) = particle.getPos().x;
	particlePoints(1) = particle.getPos().y;
	particlePoints(2) = particle.getPos().z;
	
	// Calculate the projected points of the particles onto the plane
	boost::numeric::ublas::axpy_prod(A, particlePoints, projectedPoints, true);
	
	returnVal(0) = projectedPoints(0);
	returnVal(1) = projectedPoints(1);
	
	return returnVal;
}

template class NBodySim::ParticlePlotter<NBodySim::FloatingType>;