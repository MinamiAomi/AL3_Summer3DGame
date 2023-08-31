#pragma once
#include "GameObject.h"

#include "Math/Transform.h"

#include "Collider.h"
#include "PlayerStateManager.h"
#include "PlayerParts.h"

class Player : public GameObject {
public:
    using GameObject::GameObject;
    ~Player() override;

    void Initialize() override;
    void Update() override;
    void Draw() override;

    const GameObject* GetCamera()const { return camera_; };
    PlayerParts& GetParts() { return parts_; }
    PlayerStateManager& GetStateManager() { return stateManager_; }
    CapsuleCollider& GetCollider() { return collider_; }

private:
    class SystemManager* system_;

    CapsuleCollider collider_;
    PlayerParts parts_;
    PlayerStateManager stateManager_;

    const GameObject* camera_ = nullptr;
};