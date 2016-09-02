/*
 * Copyright (c) 2016 W.A. Garrett Weaver
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
 * NBodyTypes contains enumerations and typedefs used by n-body-sim.
 */

#ifndef N_BODY_TYPES_H
#define N_BODY_TYPES_H

namespace NBodySim{
	/**
	 * ThreeVector is a templatized class containing three things labled 'x', 'y' and 'z'.
	 *
	 * @author W.A. Garrett Weaver
	 */
	template <class T> class ThreeVector{
	public:
		T x;
		T y;
		T z;
	};
	
	// Set up our types
	typedef double FloatingType;
	typedef unsigned long UnsignedType;
	
	// Universal Gravitation Constant in m^3kg^-1s^-2
	static const FloatingType G = 6.67408e-11;
}

#endif // N_BODY_TYPES_H
