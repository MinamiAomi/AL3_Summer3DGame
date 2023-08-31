#include "Enemy.h"

#include "GameScene.h"
#include "EnemyApple.h"
#include "SystemManager.h"

void Enemy::Initialize() {
    model_.SetModel(gameScene.GetModel("Resource/enemy/enemy.obj"));
    model_.SetReceiveShadow(false);

    transform_.translate = { 0.0f,0.0f, 0.0f };
    transform_.rotate = Quaternion::MakeForYAxis(Math::Pi);

    system_ = static_cast<SystemManager*>(gameScene.FindGameObject("System"));

    const uint32_t kNumApples = 5;
    Vector3 kAppleTranslateTable[kNumApples] = {
        {0.0f,  13.0f, 20.0f},
        {7.0f,  13.0f, -16.0f},
        {-10.0f, 13.0f, -10.0f},
        {-18.0f, 13.0f, 9.0f},
        {10.0f,  13.0f, 11.0f},
    };
    for (uint32_t i = 0; i < kNumApples; ++i) {
        auto apple = gameScene.AddGameObject<EnemyApple>("Apple" + std::to_string(i));
        apple->SetEnemy(this);
        apple->GetTransform().translate = kAppleTranslateTable[i];
        apple->Initialize();
    }

    helth_ = 100;
}

void Enemy::Update() {

    transform_.UpdateMatrix();

}

void Enemy::Draw() {
    model_.SetWorldMatrix(transform_.worldMatrix);
    auto& renderManager = RenderManager::Get();
    renderManager.AddModelInstance(model_);
}

void Enemy::Damage(int32_t damage) {
    helth_ -= damage;

    if (helth_ <= 0) {
        system_->ChangeState(SystemManager::kClear);
}
}
