#pragma once
#include "GameObject.h"

#include "Engine/Model.h"
#include "Math/LightStructs.h"

class Ground : public GameObject {
public:
    using GameObject::GameObject;

    void Initialize() override;
    void Draw() override;

private:
    ModelInstance modelInstance_;
    ModelInstance skydomeInstance_;
    DirectionalLight sunLight_;
};