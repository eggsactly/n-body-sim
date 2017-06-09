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

#ifndef PARTICLE_PLOTTER_H
#define PARTICLE_PLOTTER_H

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "NBodyTypes.h"
#include "Particle.h"

namespace NBodySim{
	template <class T> class ParticlePlotter;
}
/**
 * @brief A class to handle 3D space projection onto a 2D plane.
 *
 * @author: W.A. Garrett Weaver
 */
template <class T>
class NBodySim::ParticlePlotter {
private:
	/**
	 * InvertMatrix inverts a matrix using LU decomposition
	 * 
	 * @param input reference to input matrix
	 * @param inverse reference to the memory space for the inverted matrix
	 * @return true if the matrix is invertable 
	 */
	static bool InvertMatrix (const boost::numeric::ublas::matrix<T>& input, boost::numeric::ublas::matrix<T>& inverse);
	
	/**
	 * calculateGraphicsMatrix takes in azimuth and elevation of normal vector and calculates a new graphics matrix
	 * 
	 * @param theta the horizontal rotation of the normal vector
	 * @param phi the vertical rotation of the normal vector
	 * @return the graphics matrix resulting from the az and el
	 */
	static boost::numeric::ublas::matrix<T> calculateGraphicsMatrix (T theta, T phi);

protected:
	/** 
	 * theta indicates the horizontal rotation of the normal vector
	 */ 
	T theta;
	
	/**
	 * phi indicates the vertical rotation of the normal vector
	 */
	T phi;
	
	/**
	 * A is the graphics matrix
	 */
	boost::numeric::ublas::matrix<T> A;
	
public:
	/**
	 * Default constructor
	 */
	ParticlePlotter(void);
	
	/**
	 * Destructor
	 */
	virtual ~ParticlePlotter(void);
	
	/**
	 * setAngle takes in azimuth and elevation of normal vector and calculates a new graphics matrix
	 * 
	 * @param az the horizontal rotation of the normal vector
	 * @param el the vertical rotation of the normal vector
	 */
	void setAngle(T az, T el);
	
	/**
	 * calculateProjection calculates how a point in 3 space maps to a 2D plane 
	 * 
	 * @param particle is a particle that shall be plotted 
	 * @return a vector of dimension 2 representing the projected point on the plane
	 */
	boost::numeric::ublas::vector<T> calculateProjection(NBodySim::Particle<T> particle);
};
#endif //PARTICLE_PLOTTER_H