#include "PlayerBullet.h"

#include <cassert>

#include "GameScene.h"
#include "CollisionManager.h"
#include "CollisionConfig.h"

PlayerBullet::~PlayerBullet() {
    CollisionManager::Get().RemoveCollider(&collider_);
}

void PlayerBullet::Initialize() {
    model_.SetModel(gameScene.GetModel("Resource/player/bullet.obj"));
    auto player = gameScene.FindGameObject("Player");
    assert(player);
    transform_.translate = { 0.0f,3.0f,0.0f };

    collider_.SetIsActive(false);
    collider_.SetGameObject(this);
    collider_.SetRadius(0.3f);
    collider_.SetCollisionAttribute(CollisionConfig::kAttributePlayer);
    collider_.SetCollisionMask(~CollisionConfig::kAttributePlayer);
    collider_.SetCallback([this](const Collider& other) { CollisionCallback(other); });
    CollisionManager::Get().AddCollider(&collider_);

    transform_.parent = &player->GetTransform();
    waiting_ = true;
}

void PlayerBullet::Update() {
    static const uint32_t kLifeTimeLimits = 120;

    transform_.rotate = transform_.rotate * Quaternion::MakeForZAxis(10.0f * Math::ToRadian);

    if (!waiting_) {
        lifeTime_++;
        if (lifeTime_ > kLifeTimeLimits) {
            SetState(kDead);
            return;
        }
        transform_.translate += transform_.rotate * Vector3::forward;
    }

    transform_.UpdateMatrix();
}

void PlayerBullet::Draw() {
    model_.SetWorldMatrix(transform_.worldMatrix);
    auto& renderManager = RenderManager::Get();
    renderManager.AddModelInstance(model_);
}

void PlayerBullet::Fire(Vector3 target) {
    Vector3 worldPosition = transform_.translate * transform_.parent->worldMatrix;
    transform_.parent = nullptr;
    transform_.translate = worldPosition;
    transform_.rotate = /*Quaternion::MakeForYAxis(Math::Pi).Inverse() **/ Quaternion::MakeLookRotation(target - worldPosition);
    waiting_ = false;

    collider_.SetIsActive(true);
}

void PlayerBullet::CollisionCallback(const Collider& other) {
    if (other.GetCollisionAttribute() & CollisionConfig::kAttributeEnemy) {
        collider_.SetIsActive(false);
        SetState(kDead);
        return;
    }
}
