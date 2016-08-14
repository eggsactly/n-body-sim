/*
 * Copyright (c) 2016 Garrett Weaver
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef N_BODY_SYSTEM_H
#define N_BODY_SYSTEM_H

#include <string>
#include <vector>

#include "NBodyTypes.h"
#include "Particle.h"

namespace NBodySim{
    class NBodySystem;
}

/**
 * NBodySystem contains a vector of Particles and is used to calculate each new step of the simulation.
 *
 * @author Garrett Weaver
 */

class NBodySim::NBodySystem {
private:
    
protected:
    std::vector<NBodySim::Particle> system;
public:
    NBodySystem(void);
    virtual ~NBodySystem(void);
    void addParticle(NBodySim::Particle p);
    NBodySim::Particle getParticle(NBodySim::UnsignedType index);
    NBodySim::UnsignedType numParticles(void);
    void removeParticle(NBodySim::UnsignedType index);
    void step(NBodySim::FloatingType deltaT);
};

#endif // N_BODY_SYSTEM_H