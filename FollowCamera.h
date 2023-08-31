#pragma once
#include "GameObject.h"

#include "Math/Camera.h"

class FollowCamera : public GameObject {
public:
    using GameObject::GameObject;

    void Initialize() override;
    void Update() override;

private:
    class SystemManager* system_;

    Camera camera_;
    float horizontalAngle_;
    float verticalAngle_;

    GameObject* target_;
};