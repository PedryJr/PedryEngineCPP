#pragma once

#include "PedryEngine.h"

Vector<GameObject> GameObject::objArr = Vector<GameObject>();

GameObject::GameObject() {}

GameObject::GameObject(GLuint newId)
{
    ID = newId;
    components = std::unordered_map<GLuint, GLuint>();
}

void GameObject::UpdateAll()
{

    MyRotator::ProcessAll();
    Transform::ProcessAll();
    Renderer::ProcessAll();
    StareAtPlayerComp::ProcessAll();

    MyRotator::UpdateAll();
    Transform::UpdateAll();
    Renderer::UpdateAll();
    StareAtPlayerComp::UpdateAll();

}

GameObjectHandle GameObject::CreateGameObject()
{
    GLuint objId = objArr.size();
    GameObjectHandle newHandle = GameObjectHandle(objId);
    GameObject& newObj = objArr.emplace_back(objId);
    newObj.AddComponent<Transform>();

    return newHandle;
}
