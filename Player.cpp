#include "Player.h"

#include <cassert>

#include "Engine/Input.h"
#include "Core/ImGuiManager.h"
#include "Core/TextureManager.h"

#include "GameScene.h"
#include "PlayerState.h"
#include "FollowCamera.h"
#include "CollisionManager.h"
#include "CollisionConfig.h"
#include "SystemManager.h"

Player::~Player() {
    CollisionManager::Get().RemoveCollider(&collider_);
}

void Player::Initialize() {
    camera_ = gameScene.FindGameObject("FollowCamera");

    system_ = static_cast<SystemManager*>(gameScene.FindGameObject("System"));

    collider_.SetGameObject(this);
    collider_.SetRadius(0.5f);
    collider_.SetCollisionAttribute(CollisionConfig::kAttributePlayer);
    collider_.SetCollisionMask(~CollisionConfig::kAttributePlayer);
    CollisionManager::Get().AddCollider(&collider_);

    transform_.translate = { 0.0f,0.0f,-50.0f };

    parts_.Initialize(this);
    stateManager_.Initialize(this, std::make_unique<PlayerStateDefault>(this));

}

void Player::Update() {

    if (system_->GetState() == SystemManager::kInGame) {
        stateManager_.Update();
    }
        // partTransforms_[kBody].rotate = partTransforms_[kBody].rotate * Quaternion::MakeForYAxis(Math::Pi / 360.0f);
        //partTransforms_[kArm_R2].rotate = partTransforms_[kArm_R2].rotate * Quaternion::MakeForZAxis(Math::Pi / 360.0f);

        transform_.UpdateMatrix();
        parts_.UpdateTransform();
}

void Player::Draw() {
    parts_.Draw();
}

