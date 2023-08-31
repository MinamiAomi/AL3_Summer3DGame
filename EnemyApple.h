#pragma once
#include "GameObject.h"

#include "Collider.h"

class Enemy;

class EnemyApple : public GameObject {
public:
    using GameObject::GameObject;

    ~EnemyApple();
    void Initialize() override;
    void Update() override;
    void Draw() override;

    void SetEnemy(Enemy* enemy) { enemy_ = enemy; }

private:
    void CollisionCallback(const Collider& other);

    Enemy* enemy_;    
    ModelInstance model_;
    SphereCollider collider_;

    uint32_t helth_;
};