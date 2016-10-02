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
 */

#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <streambuf>

#include "rapidxml.hpp"

#include "NBodyTypes.h"
#include "Particle.h"
#include "NBodySystem.h"

NBodySim::NBodySystem::NBodySystem(void){
	// Initialize G to be our universe's gravitation constant.
	G = 6.67408e-11;
}

NBodySim::NBodySystem::~NBodySystem(void){
	
}

void NBodySim::NBodySystem::addParticle(NBodySim::Particle p){
	system.push_back(p);
}

NBodySim::Particle NBodySim::NBodySystem::getParticle(NBodySim::UnsignedType index){
	return system.at(index);
}

NBodySim::UnsignedType NBodySim::NBodySystem::numParticles(void){
	return system.size();
}

void NBodySim::NBodySystem::removeParticle(NBodySim::UnsignedType index){
	system.erase(system.begin() + index);
}

void NBodySim::NBodySystem::step(NBodySim::FloatingType deltaT){
	std::vector<NBodySim::Particle> systemCopy = system;
	NBodySim::ThreeVector <NBodySim::FloatingType> distanceComponent;
	NBodySim::FloatingType distance;
	NBodySim::FloatingType acceleration;
	NBodySim::ThreeVector <NBodySim::FloatingType> newPosition;
	NBodySim::ThreeVector <NBodySim::FloatingType> newVelocity;
	
	for(NBodySim::UnsignedType i = 0; i < systemCopy.size(); i++){
		// Calculate new position of particle first from current velocity
		newPosition = system.at(i).getPos();
		newPosition.x += systemCopy.at(i).getVel().x * deltaT;
		newPosition.y += systemCopy.at(i).getVel().y * deltaT;
		newPosition.z += systemCopy.at(i).getVel().z * deltaT;
		system[i].setPos(newPosition);
		
		newVelocity = system.at(i).getVel();
		// Sum forces on body from all bodys to obtain new velocity of paricle
		for(NBodySim::UnsignedType j = 0; j < systemCopy.size(); j++){
			distanceComponent.x = systemCopy.at(j).getPos().x - systemCopy.at(i).getPos().x;
			distanceComponent.y = systemCopy.at(j).getPos().y - systemCopy.at(i).getPos().y;
			distanceComponent.z = systemCopy.at(j).getPos().z - systemCopy.at(i).getPos().z;
			
			distance = sqrt(pow(distanceComponent.x, 2) + pow(distanceComponent.y, 2) + pow(distanceComponent.z, 2));
			
			acceleration = (distance != 0.0f) ? (G * system.at(i).getMass()) / (pow(distanceComponent.x, 2) + pow(distanceComponent.y, 2) + pow(distanceComponent.z, 2)) : 0;
			
			newVelocity.x += (distance != 0.0f) ? ((acceleration) * deltaT) * (distanceComponent.x / distance) : 0;
			newVelocity.y += (distance != 0.0f) ? ((acceleration) * deltaT) * (distanceComponent.y / distance) : 0;
			newVelocity.z += (distance != 0.0f) ? ((acceleration) * deltaT) * (distanceComponent.z / distance) : 0;
		}
		system[i].setVel(newVelocity);
	}
}

std::string NBodySim::NBodySystem::errorToString(NBodySim::NBodySystemSpace::error errorCode){
	switch(errorCode){
		case NBodySim::NBodySystemSpace::SUCCESS: return "success"; break;
		case NBodySim::NBodySystemSpace::FAILED_TO_ALLOCATE_MEMORY: return "failed to allocate memory"; break;
		case NBodySim::NBodySystemSpace::MORE_THAN_ONE_SYSTEM: return "more than one system specified in xml file"; break;
		case NBodySim::NBodySystemSpace::NO_SYSTEM: return "no system node found"; break;
		case NBodySim::NBodySystemSpace::NO_PARTICLES: return "no particles found in system"; break;
		case NBodySim::NBodySystemSpace::INDEX_EXCEEDED: return "index exceeded"; break;
		case NBodySim::NBodySystemSpace::NO_POSX: return "no PosX attribute found for a particle"; break;
		case NBodySim::NBodySystemSpace::NO_POSY: return "no PosY attribute found for a particle"; break;
		case NBodySim::NBodySystemSpace::NO_POSZ: return "no PosZ attribute found for a particle"; break;
		case NBodySim::NBodySystemSpace::NO_VELX: return "no VelX attribute found for a particle"; break;
		case NBodySim::NBodySystemSpace::NO_VELY: return "no VelY attribute found for a particle"; break;
		case NBodySim::NBodySystemSpace::NO_VELZ: return "no VelZ attribute found for a particle"; break;
		case NBodySim::NBodySystemSpace::NO_MASS: return "no Mass attribute found for a particle"; break;
		case NBodySim::NBodySystemSpace::NO_NAME: return "no Name attribute found for a particle"; break;
		default: return "unknown error"; break;
	}
}

NBodySim::NBodySystemSpace::error NBodySim::NBodySystem::parse(std::string xmlText){
	char * buffer = NULL;
	rapidxml::xml_node<> *node;
	rapidxml::xml_node<> *secondNode;
	rapidxml::xml_attribute<> *attr;
	rapidxml::xml_document<> doc;
	
	buffer = new char[xmlText.size() + 1];
	if(buffer == NULL){
		return NBodySim::NBodySystemSpace::FAILED_TO_ALLOCATE_MEMORY;
	}
	strcpy(buffer, xmlText.c_str());
	
	// Parse the file
	doc.parse<0>(buffer);
	
	node = doc.first_node("system");
	if(node != NULL){
		if(node->next_sibling("system") != NULL){
			delete [] buffer;
			return NBodySim::NBodySystemSpace::MORE_THAN_ONE_SYSTEM;
		}
	}
	else{
		delete [] buffer;
		return NBodySim::NBodySystemSpace::NO_SYSTEM;
	}
	// Get the gravitation constant of the system if it is given
	if(node->first_attribute("G") != NULL){
		this->setGravitation(atof(node->first_attribute("G")->value()));
	}
	
	secondNode = node->first_node("particle");
	if(secondNode == NULL){
		delete [] buffer;
		return NBodySim::NBodySystemSpace::NO_PARTICLES;
	}
	
	while(secondNode != NULL){
		NBodySim::Particle p;
		for(unsigned i = 0; i < NBodySim::NBodySystemSpace::particleAttributeListLength; i++){
			attr = secondNode->first_attribute(NBodySim::NBodySystemSpace::particleAttributeList[i]);
			if(attr == NULL){
				delete [] buffer;
				switch(i){
					case NBodySim::NBodySystemSpace::POSX: return NBodySim::NBodySystemSpace::NO_POSX; break;
					case NBodySim::NBodySystemSpace::POSY: return NBodySim::NBodySystemSpace::NO_POSY; break;
					case NBodySim::NBodySystemSpace::POSZ: return NBodySim::NBodySystemSpace::NO_POSZ; break;
					case NBodySim::NBodySystemSpace::VELX: return NBodySim::NBodySystemSpace::NO_VELX; break;
					case NBodySim::NBodySystemSpace::VELY: return NBodySim::NBodySystemSpace::NO_VELY; break;
					case NBodySim::NBodySystemSpace::VELZ: return NBodySim::NBodySystemSpace::NO_VELZ; break;
					case NBodySim::NBodySystemSpace::MASS: return NBodySim::NBodySystemSpace::NO_MASS; break;
					case NBodySim::NBodySystemSpace::NAME: return NBodySim::NBodySystemSpace::NO_NAME; break;
					default: return NBodySim::NBodySystemSpace::INDEX_EXCEEDED;
				}
			}
			switch(i){
				case NBodySim::NBodySystemSpace::POSX: p.setPosX(atof(attr->value())); break;
				case NBodySim::NBodySystemSpace::POSY: p.setPosY(atof(attr->value())); break;
				case NBodySim::NBodySystemSpace::POSZ: p.setPosZ(atof(attr->value())); break;
				case NBodySim::NBodySystemSpace::VELX: p.setVelX(atof(attr->value())); break;
				case NBodySim::NBodySystemSpace::VELY: p.setVelY(atof(attr->value())); break;
				case NBodySim::NBodySystemSpace::VELZ: p.setVelZ(atof(attr->value())); break;
				case NBodySim::NBodySystemSpace::MASS: p.setMass(atof(attr->value())); break;
				case NBodySim::NBodySystemSpace::NAME: p.setName(std::string(attr->value())); break;
				default: return NBodySim::NBodySystemSpace::INDEX_EXCEEDED;
			}
		}
		
		secondNode = secondNode->next_sibling();
		this->addParticle(p);
	}
	
	delete [] buffer;
	
	return NBodySim::NBodySystemSpace::SUCCESS;
}

void NBodySim::NBodySystem::setGravitation(NBodySim::FloatingType gravitationConstant){
	G = gravitationConstant;
}

NBodySim::FloatingType NBodySim::NBodySystem::getGravitation(void){
	return G;
}
