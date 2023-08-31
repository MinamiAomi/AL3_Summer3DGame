#pragma once
#include "GameObject.h"

#include "Collider.h"

class PlayerBullet : public GameObject {
public:
    using GameObject::GameObject;

    ~PlayerBullet();

    void Initialize() override;
    void Update() override;
    void Draw() override;

    void Fire(Vector3 target);

private:
    void CollisionCallback(const Collider& other);

    ModelInstance model_;
    SphereCollider collider_;

    uint32_t lifeTime_;
    float power_;
    bool waiting_;
};