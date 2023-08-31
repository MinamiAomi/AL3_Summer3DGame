#include "FollowCamera.h"

#include "GameScene.h"

#include "Core/ImGuiManager.h"
#include "Engine/Input.h"
#include "SystemManager.h"

void FollowCamera::Initialize() {
    system_ = static_cast<SystemManager*>(gameScene.FindGameObject("System"));

    target_ = gameScene.FindGameObject("Player");
    horizontalAngle_ = 0.0f;
    verticalAngle_ = 0.0f;

    RenderManager::Get().SetCamera(camera_);
}

void FollowCamera::Update() {
    assert(target_);

    auto& input = Input::Get();
    auto& state = input.GetXInputState();

    const float kAngleSpeed = 2.0f * Math::ToRadian / float(SHRT_MAX);
    const float kVerticalAngleMinLimit = -70.0f * Math::ToRadian;
    const float kVerticalAngleMaxLimit = 80.0f * Math::ToRadian;

    if (system_->GetState() == SystemManager::kInGame) {
        if (std::abs(state.Gamepad.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
            horizontalAngle_ += float(state.Gamepad.sThumbRX) * kAngleSpeed;
            horizontalAngle_ = std::fmod(horizontalAngle_, Math::TwoPi);
        }
        if (std::abs(state.Gamepad.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
            verticalAngle_ += -float(state.Gamepad.sThumbRY) * kAngleSpeed;
            verticalAngle_ = std::clamp(verticalAngle_, kVerticalAngleMinLimit, kVerticalAngleMaxLimit);
        }
    }

    transform_.rotate = Quaternion::MakeFromEulerAngle({ verticalAngle_, horizontalAngle_, 0.0f });

    const Vector3 targetOffset = { 0.0f, 2.0f, 0.0f };
    const float distanceFromTarget = 7.0f;

    transform_.translate = transform_.rotate * Vector3::back * distanceFromTarget;
    transform_.translate += targetOffset + target_->GetTransform().translate;

    transform_.UpdateMatrix();

    camera_.SetRotate(transform_.rotate);
    camera_.SetPosition(transform_.translate);

    camera_.UpdateMatrices();
}

