#pragma once

#ifndef GameObject_H
#define GameObject_H

#include <vector>
#include "Component.h"

#endif

class GameObject		//game object as a base storing the different components
{
public: 
	GameObject();

	void addComponent(Component* c);

	Component* getComponent(Component::ComponentType t);

private:
	std::vector<Component*> components;
};

