#include "PlayerState.h"

#include "Engine/Input.h"
#include "Core/TextureManager.h"

#include "GameScene.h"
#include "Player.h"
#include "PlayerStateManager.h"
#include "PlayerBullet.h"

void PlayerStateDefault::Initialize() {
    walkAnimation_ = 0.0f;
}

void PlayerStateDefault::Update() {
    auto& input = Input::Get();

    auto& transform = player_->GetTransform();

    auto& gamepadState = input.GetXInputState();

    Vector3 move;

    const float kStickMargin = 0.8f;
    const float kMoveSpeed = 0.1f;

    move = { float(gamepadState.Gamepad.sThumbLX) / float(SHRT_MAX), 0.0f, float(gamepadState.Gamepad.sThumbLY) / float(SHRT_MAX) };
    if (move.Length() < kStickMargin) {
        move = Vector3::zero;
    }

    if (input.IsKeyPressed(DIK_D) || input.IsKeyPressed(DIK_A) || input.IsKeyPressed(DIK_S) || input.IsKeyPressed(DIK_W)) {
        move = Vector3::zero;
        move.x += input.IsKeyPressed(DIK_D) ? 1.0f : 0.0f;
        move.x += input.IsKeyPressed(DIK_A) ? -1.0f : 0.0f;
        move.z += input.IsKeyPressed(DIK_W) ? 1.0f : 0.0f;
        move.z += input.IsKeyPressed(DIK_S) ? -1.0f : 0.0f;
    }

    if (move != Vector3::zero) {
        move = move.Normalized() * kMoveSpeed;
        auto cameraRotate = player_->GetCamera()->GetTransform().rotate;
        Vector3 dir = cameraRotate * Vector3::forward;
        dir.y = 0.0f;
        move = Quaternion::MakeLookRotation(dir) * move;

        transform.translate += move;

        auto q = Quaternion::MakeLookRotation(move);
        transform.rotate = Quaternion::Slerp(0.1f, transform.rotate, q);

        auto& parts = player_->GetParts();
        if (parts.GetPlayingAnimation() != PlayerParts::kWalk) {
            const uint32_t kWalkAnimationCycle = 60;
            parts.PlayAnimation(PlayerParts::kWalk, kWalkAnimationCycle, true);
        }
    }
    else {
        auto& parts = player_->GetParts();
        if (parts.GetPlayingAnimation() != PlayerParts::kWait) {
            const uint32_t kWaitAnimationCycle = 60;
            parts.PlayAnimation(PlayerParts::kWait, kWaitAnimationCycle, true);
        }
    }

    if (gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
        player_->GetStateManager().ChangeState(std::make_unique<PlayerStateDash>(player_));
        return;
    }

    if (gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
        player_->GetStateManager().ChangeState(std::make_unique<PlayerStateAim>(player_));
        return;
    }
}

void PlayerStateDash::Initialize() {
    auto& parts = player_->GetParts();
    const uint32_t kNumFrames = 20;
    parts.PlayAnimation(PlayerParts::kDash, kNumFrames, false);
    player_->GetCollider().SetIsActive(false);
}

void PlayerStateDash::Update() {
    auto& parts = player_->GetParts();
    if (!parts.IsPlayingAnimation()) {
        const uint32_t kWaitAnimationCycle = 60;
        parts.PlayAnimation(PlayerParts::kWait, kWaitAnimationCycle, true);
        player_->GetCollider().SetIsActive(true);
        player_->GetStateManager().ChangeState(std::make_unique<PlayerStateDefault>(player_));
        return;
    }

    auto& transform = player_->GetTransform();

    const float kDashSpeed = 0.6f;
    Vector3 direction = transform.rotate * Vector3::forward;
    Vector3 velocity = direction * kDashSpeed;
    transform.translate += velocity;
}

void PlayerStateAim::Initialize() {
    auto& parts = player_->GetParts();
    const uint32_t kNumFrames = 20;
    parts.PlayAnimation(PlayerParts::kAim, kNumFrames, false);

    bulletCoolTime_ = 0;

    sprite_.SetAnchor({ 0.5f,0.5f });
    sprite_.SetTranslate({ 640.0f, 360.0f });
    sprite_.SetSize({ 75.0f, 75.0f });
    sprite_.SetTexcoordSize({ 128.0f, 128.0f });
    sprite_.SetTexture(TextureManager::Load("Resource/aim.png"));
}

void PlayerStateAim::Update() {
    auto& input = Input::Get();

    auto& gamepadState = input.GetXInputState();

    const uint32_t kMaxNumBullets = 5;

    auto& transform = player_->GetTransform();
    auto& parts = player_->GetParts();

    if (!parts.IsPlayingAnimation()) {
        if (gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
            for (auto& bullet : waitingBullets_) {
                bullet->SetState(GameObject::kDead);
            }
            player_->GetStateManager().ChangeState(std::make_unique<PlayerStateDash>(player_));
            return;
        }
    }


    static const Vector3 bulletOffsetTable[kMaxNumBullets] = {
        { 0.0f, 0.5f, 0.0f }, 
        { 1.0f,-0.0f, 0.0f }, 
        {-1.0f,-0.0f, 0.0f }, 
        { 2.0f,-0.5f, 0.0f }, 
        {-2.0f,-0.5f, 0.0f }, 
    };

    if (parts.GetPlayingAnimation() == PlayerParts::kAim) {
        if (bulletCoolTime_ == 0) {
            bulletCoolTime_ = 60;
            if (waitingBullets_.size() < kMaxNumBullets) {
                auto bullet = player_->gameScene.AddGameObject<PlayerBullet>("bullet");
                bullet->Initialize();

                bullet->GetTransform().translate += bulletOffsetTable[waitingBullets_.size()];
                waitingBullets_.emplace_back(bullet);
            }
        }
        else {
            bulletCoolTime_--;
        }
        
        auto& renderManager = RenderManager::Get();
        renderManager.AddSprite(sprite_);
    }

    if (parts.GetPlayingAnimation() == PlayerParts::kAttack && !parts.IsPlayingAnimation()) {
        player_->GetStateManager().ChangeState(std::make_unique<PlayerStateDefault>(player_));
        return;
    }

    auto& camera = player_->GetCamera()->GetTransform();
    Vector3 dir = camera.rotate * Vector3::forward;

    auto q = Quaternion::MakeLookRotation({ dir.x, 0.0f, dir.z });
    transform.rotate = Quaternion::Slerp(0.3f, transform.rotate, q);

    auto& head = parts.GetTransform(PlayerParts::kHead);
    head.rotate = transform.rotate.Inverse() * Quaternion::MakeLookRotation(dir);

    if (!(gamepadState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
        if (parts.GetPlayingAnimation() == PlayerParts::kAim && !parts.IsPlayingAnimation()) {
            parts.PlayAnimation(PlayerParts::kAttack, 60, false);
            for (auto bullet : waitingBullets_) {
                bullet->Fire((dir * 50.0f) + camera.translate);
            }
            waitingBullets_.clear();
        }
    }
}
