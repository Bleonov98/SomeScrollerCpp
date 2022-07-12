#include "GameObject.h"

int GameObject::GetX()
{
	return _x;
}

int GameObject::GetY()
{
	return _y;
}

void GameObject::SetX(int x)
{
	_x = x;
}

void GameObject::SetY(int y)
{
	_y = y;
}

void GameObject::DeleteObject()
{
	_deleteObject = true;
}

bool GameObject::IsObjectDelete()
{
	return _deleteObject;
}
