#include "GameObject.h"
#include "C_Transform.h"

void GameObject::addComponent(Component* c)
{
	components.push_back(c);
}

GameObject::GameObject()
{
	addComponent(new C_Transform());
}


Component* GameObject::getComponent(Component::ComponentType t)
{
	for each (Component * c in components)
	{
		if (c->name == t)
		{
			return c;
		}
	}
	return NULL;
}