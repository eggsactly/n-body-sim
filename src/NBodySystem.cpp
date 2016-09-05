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
#include <iostream>
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
	// Do literally nothing, this class is basically a wrapper for vector
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
			
			acceleration = (distance != 0.0f) ? (NBodySim::G * system.at(i).getMass()) / (pow(distanceComponent.x, 2) + pow(distanceComponent.y, 2) + pow(distanceComponent.z, 2)) : 0;
			
			newVelocity.x += (distance != 0.0f) ? ((acceleration) * deltaT) * (distanceComponent.x / distance) : 0;
			newVelocity.y += (distance != 0.0f) ? ((acceleration) * deltaT) * (distanceComponent.y / distance) : 0;
			newVelocity.z += (distance != 0.0f) ? ((acceleration) * deltaT) * (distanceComponent.z / distance) : 0;
		}
		system[i].setVel(newVelocity);
	}
}

void NBodySim::NBodySystem::parse(std::string xmlText){
	char * buffer = NULL;
	rapidxml::xml_node<> *node;
	rapidxml::xml_node<> *secondNode;
	rapidxml::xml_attribute<> *attr;
	rapidxml::xml_document<> doc;
	doc.parse<0>(buffer);
	const unsigned attributeListLength = 8;
	char attributeList [][attributeListLength] = {"posX", "posY", "posZ", "velX", "velY", "velZ", "mass", "name"};
	
	buffer = new char[xmlText.size() + 1];
	if(buffer == NULL){
		std::cout << "Failed to allocate memory for scenario text." << std::endl;
		return;
	}
	
	strcpy(buffer, xmlText.c_str());
	
	node = doc.first_node("system");
	if(node != NULL){
		if(node->next_sibling("system") != NULL){
			std::cout << "More than one system specified in xml file." << std::endl;
			delete [] buffer;
			return;
		}
	}
	else{
		std::cout << "No system node found." << std::endl;
		delete [] buffer;
		return;
	}
	
	secondNode = node->first_node("particle");
	if(secondNode == NULL){
		std::cout << "No particles in system." << std::endl;
		delete [] buffer;
		return;
	}
	
	while(secondNode != NULL){
		NBodySim::Particle p;
		for(unsigned i = 0; i < attributeListLength; i++){
			attr = secondNode->first_attribute(attributeList[i]);
			if(attr == NULL){
				std::cout << "No " << attributeList[i] << " attribute found for particle." << std::endl;
				delete [] buffer;
				return;
			}
			switch(i){
				case 1: p.setPosX(atof(attr->value())); break;
				case 2: p.setPosY(atof(attr->value())); break;
				case 3: p.setPosZ(atof(attr->value())); break;
				case 4: p.setVelX(atof(attr->value())); break;
				case 5: p.setVelY(atof(attr->value())); break;
				case 6: p.setVelZ(atof(attr->value())); break;
				case 7: p.setMass(atof(attr->value())); break;
				case 8: p.setName(std::string(attr->value())); break;
				default: std::cout << "Index exceded somehow" << std::endl; return;
			}
		}
		
		secondNode = secondNode->next_sibling();
		this->addParticle(p);
	}
	
	delete [] buffer;
	
	return;
}