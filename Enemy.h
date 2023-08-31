#pragma once
#include "GameObject.h"

#include "Math/Transform.h"

class Enemy : public GameObject {
public:
    using GameObject::GameObject;

    void Initialize() override;
    void Update() override;
    void Draw() override;

    void Damage(int32_t damage);

private:
    class SystemManager* system_;
    ModelInstance model_;

   int32_t helth_;
};
