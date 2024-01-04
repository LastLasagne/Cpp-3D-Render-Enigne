#pragma once
#ifndef Component_H
#define Component_H

#endif

class Component
{
public:
	//polymorphic wrapper class for components
	virtual ~Component() {}
	virtual void doSomething() = 0;

	bool active = true;

	static enum ComponentType {
		TRANSFORM,
		MESH_RENDERER,
		CAMERA,
		LIGHT
	};
	ComponentType name;

};

