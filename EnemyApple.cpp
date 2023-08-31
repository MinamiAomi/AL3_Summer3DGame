#include "EnemyApple.h"

#include <cassert>

#include "GameScene.h"
#include "Enemy.h"
#include "CollisionManager.h"
#include "CollisionConfig.h"


EnemyApple::~EnemyApple() {
    CollisionManager::Get().RemoveCollider(&collider_);
}

void EnemyApple::Initialize() {
    model_.SetModel(gameScene.GetModel("Resource/enemy/apple.obj"));
    assert(enemy_);
    transform_.parent = &enemy_->GetTransform();
    transform_.scale = Vector3(2.0f);

    collider_.SetIsActive(true);
    collider_.SetLocalOffset({});
    collider_.SetRadius(1.0f);
    collider_.SetCollisionAttribute(CollisionConfig::kAttributeEnemy);
    collider_.SetCollisionMask(~CollisionConfig::kAttributeEnemy);
    collider_.SetGameObject(this);
    collider_.SetCallback([this](const Collider& other) {CollisionCallback(other); });
    CollisionManager::Get().AddCollider(&collider_);

    helth_ = 5;
}

void EnemyApple::Update() {
    transform_.UpdateMatrix();
}

void EnemyApple::Draw() {
    model_.SetWorldMatrix(transform_.worldMatrix);
    auto& renderManager = RenderManager::Get();
    renderManager.AddModelInstance(model_);
}

void EnemyApple::CollisionCallback(const Collider& other) {
    if (other.GetCollisionAttribute() & CollisionConfig::kAttributePlayer) {
        helth_--;
        if (helth_ == 0) {
            enemy_->Damage(20);
            SetState(kDead);
            collider_.SetIsActive(false);
            return;
        }
    }
}
