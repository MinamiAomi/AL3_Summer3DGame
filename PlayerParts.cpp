#include "PlayerParts.h"

#include <cassert>

#include "Engine/RenderManager.h"
#include "GameScene.h"
#include "Player.h"

void PlayerParts::Initialize(Player* player) {
    assert(player);
    player_ = player;

    auto& gameScene = player->gameScene;

    modelInstances_[kBody].SetModel(gameScene.GetModel("Resource/player/body.obj"));
    modelInstances_[kHead].SetModel(gameScene.GetModel("Resource/player/head.obj"));
    modelInstances_[kArm_R1].SetModel(gameScene.GetModel("Resource/player/arm.obj"));
    modelInstances_[kArm_R2].SetModel(gameScene.GetModel("Resource/player/arm.obj"));
    modelInstances_[kArm_L1].SetModel(gameScene.GetModel("Resource/player/arm.obj"));
    modelInstances_[kArm_L2].SetModel(gameScene.GetModel("Resource/player/arm.obj"));

    partTransforms_[kBody].parent = &player_->GetTransform();
    partTransforms_[kHead].parent = &partTransforms_[kBody];
    partTransforms_[kArm_L1].parent = &partTransforms_[kBody];
    partTransforms_[kArm_L2].parent = &partTransforms_[kArm_L1];
    partTransforms_[kArm_R1].parent = &partTransforms_[kBody];
    partTransforms_[kArm_R2].parent = &partTransforms_[kArm_R1];

    partTransforms_[kHead].translate = { 0.0f,1.4f,0.0f };
    partTransforms_[kArm_L1].translate = { 0.2f,1.5f,0.0f };
    partTransforms_[kArm_L2].translate = { 0.0f,-0.6f,0.0f };
    partTransforms_[kArm_R1].translate = { -0.2f,1.5f,0.0f };
    partTransforms_[kArm_R2].translate = { 0.0f,-0.6f,0.0f };

    DefaultPose();
}

void PlayerParts::UpdateTransform() {
    // アニメーションを再生中
    if (isPlaying_ && animation_ != kNone) {
        animationParameter_ += 1.0f / float(animationCycle_);
        if (animationParameter_ >= 1.0f) {
            if (isLoop_) {
                animationParameter_ -= 1.0f;
            }
            else {
                isPlaying_ = false;
                animationParameter_ = 1.0f;
            }
        }

        switch (animation_)
        {
        case PlayerParts::kWait:
            WaitAnimation(animationParameter_);
            break;
        case PlayerParts::kWalk:
            WalkAnimation(animationParameter_);
            break;
        case PlayerParts::kDash:
            DashAnimation(animationParameter_);
            break;
        case PlayerParts::kAim:
            AimAnimation(animationParameter_);
            break;
        case PlayerParts::kAttack:
            AttackAnimation(animationParameter_);
            break;
        default:
            DefaultPose();
            break;
        }
    }

    for (auto& transform : partTransforms_) {
        transform.UpdateMatrix();
    }
}

void PlayerParts::Draw() {
    auto& renderManager = RenderManager::Get();
    for (int i = 0; i < kNumParts; ++i) {
        modelInstances_[i].SetWorldMatrix(partTransforms_[i].worldMatrix);
        renderManager.AddModelInstance(modelInstances_[i]);
    }
}

void PlayerParts::PlayAnimation(Animation animation, uint32_t cycle, bool isLoop, bool resetPose) {
    animation_ = animation;
    animationCycle_ = cycle;
    isLoop_ = isLoop;
    animationParameter_ = 0.0f;
    isPlaying_ = true;
    if (resetPose) {
        DefaultPose();
    }
}

void PlayerParts::StopAnimation() {
    isPlaying_ = false;
}

void PlayerParts::RestartAnimation() {
    isPlaying_ = true;
}

void PlayerParts::DefaultPose() {
    partTransforms_[kBody].translate = Vector3::zero;
    partTransforms_[kBody].rotate = Quaternion::identity;
    partTransforms_[kHead].rotate = Quaternion::identity;
    partTransforms_[kArm_L1].rotate = Quaternion::MakeForZAxis(30.0f * Math::ToRadian);
    partTransforms_[kArm_L2].rotate = Quaternion::MakeForXAxis(-40.0f * Math::ToRadian);
    partTransforms_[kArm_R1].rotate = Quaternion::MakeForZAxis(-30.0f * Math::ToRadian);
    partTransforms_[kArm_R2].rotate = Quaternion::MakeForXAxis(-40.0f * Math::ToRadian);

    auto& collider = player_->GetCollider();
    collider.SetLocalStart({0.0f, 0.5f, 0.0f});
    collider.SetLocalEnd({0.0f, 1.5f, 0.0f});
}

void PlayerParts::WaitAnimation(float t) {
    float sin = std::sin(t * Math::TwoPi);
    partTransforms_[kBody].translate.y = sin * 0.1f;

    //partTransforms_[kHead].rotate = Quaternion::MakeForXAxis(Math::HalfPi / 180.0f) * partTransforms_[kHead].rotate;
}

void PlayerParts::WalkAnimation(float t) {
    static const uint32_t kNumKeyFrames = 4;
    static const Quaternion kKeyFrameTable_Arm_L1[kNumKeyFrames] = {
         Quaternion::MakeForZAxis(30.0f * Math::ToRadian) ,
         Quaternion::MakeForZAxis(30.0f * Math::ToRadian) * Quaternion::MakeForXAxis(70.0f * Math::ToRadian),
         Quaternion::MakeForZAxis(30.0f * Math::ToRadian) ,
         Quaternion::MakeForZAxis(30.0f * Math::ToRadian) * Quaternion::MakeForXAxis(-70.0f * Math::ToRadian)
    };
    static const Quaternion kKeyFrameTable_Arm_R1[kNumKeyFrames] = {
        Quaternion::MakeForZAxis(-30.0f * Math::ToRadian) ,
        Quaternion::MakeForZAxis(-30.0f * Math::ToRadian) * Quaternion::MakeForXAxis(-70.0f * Math::ToRadian),
        Quaternion::MakeForZAxis(-30.0f * Math::ToRadian) ,
        Quaternion::MakeForZAxis(-30.0f * Math::ToRadian) * Quaternion::MakeForXAxis(70.0f * Math::ToRadian)
    };

    float sin = std::sin(t * Math::TwoPi);
    partTransforms_[kBody].translate.y = sin * 0.1f;

    uint32_t keyFrame = uint32_t(t * float(kNumKeyFrames));
    uint32_t nextKeyFrame = (keyFrame + 1) % kNumKeyFrames;
    t = fmod(t, 1.0f / kNumKeyFrames) * kNumKeyFrames;

    // t = -(std::cos(Math::Pi * t) - 1.0f) * 0.5f;


    partTransforms_[kArm_L1].rotate = Quaternion::Slerp(t, kKeyFrameTable_Arm_L1[keyFrame], kKeyFrameTable_Arm_L1[nextKeyFrame]);
    partTransforms_[kArm_R1].rotate = Quaternion::Slerp(t, kKeyFrameTable_Arm_R1[keyFrame], kKeyFrameTable_Arm_R1[nextKeyFrame]);
}

void PlayerParts::DashAnimation(float t) {
    static const uint32_t kNumKeyFrames = 3;
    static const Vector3 kBodyTranslateTable[kNumKeyFrames] = {
        { 0.0f, 0.0f,  0.0f },
        { 0.0f, 0.2f, -1.0f },
        { 0.0f, 0.2f, -1.0f },
    };
    static const Quaternion kBodyRotateTable[kNumKeyFrames] = {
        Quaternion::identity,
        Quaternion::MakeForXAxis(Math::HalfPi),
        Quaternion::MakeForXAxis(Math::HalfPi)
    };
    static const Quaternion kHeadRotateTable[kNumKeyFrames] = {
        Quaternion::identity,
        Quaternion::MakeForXAxis(-Math::HalfPi),
        Quaternion::MakeForXAxis(-Math::HalfPi)
    };
    static const Quaternion kArm_L1RotateTable[kNumKeyFrames] = {
         Quaternion::MakeForZAxis(30.0f * Math::ToRadian),
         Quaternion::MakeForZAxis(15.0f * Math::ToRadian),
         Quaternion::MakeForZAxis(15.0f * Math::ToRadian),
    };
    static const Quaternion kArm_L2RotateTable[kNumKeyFrames] = {
         Quaternion::MakeForXAxis(-40.0f * Math::ToRadian),
         Quaternion::MakeForZAxis(-30.0f * Math::ToRadian),
         Quaternion::MakeForZAxis(-30.0f * Math::ToRadian),
    };
    static const Quaternion kArm_R1RotateTable[kNumKeyFrames] = {
        Quaternion::MakeForZAxis(-30.0f * Math::ToRadian),
        Quaternion::MakeForZAxis(-15.0f * Math::ToRadian),
        Quaternion::MakeForZAxis(-15.0f * Math::ToRadian),
    };
    static const Quaternion kArm_R2RotateTable[kNumKeyFrames] = {
       Quaternion::MakeForXAxis(-40.0f * Math::ToRadian),
       Quaternion::MakeForZAxis(30.0f * Math::ToRadian),
       Quaternion::MakeForZAxis(30.0f * Math::ToRadian),
    };

    static const float kKeyTable[kNumKeyFrames] = {
        0.3f, 0.7f, 1.0f
    };

    uint32_t keyFrame = 0;
    for (uint32_t i = 0; i < kNumKeyFrames; ++i) {
        if (t <= kKeyTable[i]) {
            keyFrame = i;
            break;
        }
    }

    float keyLength = kKeyTable[keyFrame];
    if (keyFrame != 0) {
        keyLength = kKeyTable[keyFrame] - kKeyTable[keyFrame - 1];
        t -= kKeyTable[keyFrame - 1];
    }
    t *= 1.0f / keyLength;

    t = std::min(t, 1.0f);

    uint32_t nextKeyFrame = (keyFrame + 1) % kNumKeyFrames;

    partTransforms_[kBody].translate = Vector3::Lerp(t, kBodyTranslateTable[keyFrame], kBodyTranslateTable[nextKeyFrame]);
    partTransforms_[kBody].rotate = Quaternion::Slerp(t, kBodyRotateTable[keyFrame], kBodyRotateTable[nextKeyFrame]);

    partTransforms_[kHead].rotate = Quaternion::Slerp(t, kHeadRotateTable[keyFrame], kHeadRotateTable[nextKeyFrame]);
    partTransforms_[kArm_L1].rotate = Quaternion::Slerp(t, kArm_L1RotateTable[keyFrame], kArm_L1RotateTable[nextKeyFrame]);
    partTransforms_[kArm_L2].rotate = Quaternion::Slerp(t, kArm_L2RotateTable[keyFrame], kArm_L2RotateTable[nextKeyFrame]);
    partTransforms_[kArm_R1].rotate = Quaternion::Slerp(t, kArm_R1RotateTable[keyFrame], kArm_R1RotateTable[nextKeyFrame]);
    partTransforms_[kArm_R2].rotate = Quaternion::Slerp(t, kArm_R2RotateTable[keyFrame], kArm_R2RotateTable[nextKeyFrame]);

}

void PlayerParts::AimAnimation(float t) {
    static const Quaternion kArm_L1RotateStart = Quaternion::MakeForZAxis(30.0f * Math::ToRadian);
    static const Quaternion kArm_L1RotateEnd = Quaternion::MakeFromAngleAxis(-110.0f * Math::ToRadian, Vector3(-0.2f, 0.0f, -1.0f).Normalized()) /*Quaternion::MakeForZAxis(30.0f * Math::ToRadian) * Quaternion::MakeForXAxis(70.0f * Math::ToRadian)*/;

    static const Quaternion kArm_L2RotateStart = Quaternion::identity;
    static const Quaternion kArm_L2RotateEnd = Quaternion::MakeFromAngleAxis(40.0f * Math::ToRadian, Vector3(0.0f, 0.0f, 1.0f).Normalized());

    static const Quaternion kArm_R1RotateStart = Quaternion::MakeForZAxis(-30.0f * Math::ToRadian);
    static const Quaternion kArm_R1RotateEnd = Quaternion::MakeFromAngleAxis(110.0f * Math::ToRadian, Vector3(0.2f, 0.0f, -1.0f).Normalized());

    static const Quaternion kArm_R2RotateStart = Quaternion::identity;
    static const Quaternion kArm_R2RotateEnd = Quaternion::MakeFromAngleAxis(-40.0f * Math::ToRadian, Vector3(0.0f, 0.0f, 1.0f).Normalized());

    partTransforms_[kArm_L1].rotate = Quaternion::Slerp(t, kArm_L1RotateStart, kArm_L1RotateEnd);
    partTransforms_[kArm_L2].rotate = Quaternion::Slerp(t, kArm_L2RotateStart, kArm_L2RotateEnd);
    partTransforms_[kArm_R1].rotate = Quaternion::Slerp(t, kArm_R1RotateStart, kArm_R1RotateEnd);
    partTransforms_[kArm_R2].rotate = Quaternion::Slerp(t, kArm_R2RotateStart, kArm_R2RotateEnd);

}

void PlayerParts::AttackAnimation(float t) {
    static const Quaternion kArm_L1RotateStart = Quaternion::MakeFromAngleAxis(-110.0f * Math::ToRadian, Vector3(-0.2f, 0.0f, -1.0f).Normalized());
    static const Quaternion kArm_L1RotateEnd = Quaternion::MakeFromAngleAxis(-60.0f * Math::ToRadian, Vector3(0.9f, 0.0f, -1.0f).Normalized());

    static const Quaternion kArm_L2RotateStart = Quaternion::MakeFromAngleAxis(40.0f * Math::ToRadian, Vector3(0.0f, 0.0f, 1.0f).Normalized());
    static const Quaternion kArm_L2RotateEnd = Quaternion::MakeFromAngleAxis(-80.0f * Math::ToRadian, Vector3(1.0f, 0.5f, 0.0f).Normalized());

    static const Quaternion kArm_R1RotateStart = Quaternion::MakeFromAngleAxis(110.0f * Math::ToRadian, Vector3(0.2f, 0.0f, -1.0f).Normalized());
    static const Quaternion kArm_R1RotateEnd = Quaternion::MakeFromAngleAxis(60.0f * Math::ToRadian, Vector3(-0.9f, 0.0f, -1.0f).Normalized());

    static const Quaternion kArm_R2RotateStart = Quaternion::MakeFromAngleAxis(-40.0f * Math::ToRadian, Vector3(0.0f, 0.0f, 1.0f).Normalized());
    static const Quaternion kArm_R2RotateEnd = Quaternion::MakeFromAngleAxis(80.0f * Math::ToRadian, Vector3(-1.0f, 0.5f, 0.0f).Normalized());

    if (t <= 0.25f) {

        auto EaseInBack = [](float x) {
            const float c1 = 1.70158f;
            const float c3 = c1 + 1.0f;
            return c3 * x * x * x - c1 * x * x;
        };
        t = EaseInBack(t * 4.0f);
        partTransforms_[kArm_L1].rotate = Quaternion::Slerp(t, kArm_L1RotateStart, kArm_L1RotateEnd);
        partTransforms_[kArm_L2].rotate = Quaternion::Slerp(t, kArm_L2RotateStart, kArm_L2RotateEnd);
        partTransforms_[kArm_R1].rotate = Quaternion::Slerp(t, kArm_R1RotateStart, kArm_R1RotateEnd);
        partTransforms_[kArm_R2].rotate = Quaternion::Slerp(t, kArm_R2RotateStart, kArm_R2RotateEnd);
    }
    else if (t <= 0.5f) {
        partTransforms_[kArm_L1].rotate = kArm_L1RotateEnd;
        partTransforms_[kArm_L2].rotate = kArm_L2RotateEnd;
        partTransforms_[kArm_R1].rotate = kArm_R1RotateEnd;
        partTransforms_[kArm_R2].rotate = kArm_R2RotateEnd;
    }
    else {
        static const Quaternion kArm_L1RotateDef = Quaternion::MakeForZAxis(30.0f * Math::ToRadian);
        static const Quaternion kArm_L2RotateDef = Quaternion::MakeForXAxis(-40.0f * Math::ToRadian);
        static const Quaternion kArm_R1RotateDef = Quaternion::MakeForZAxis(-30.0f * Math::ToRadian);
        static const Quaternion kArm_R2RotateDef = Quaternion::MakeForXAxis(-40.0f * Math::ToRadian);

        t = (t - 0.5f) * 2.0f;
        partTransforms_[kArm_L1].rotate = Quaternion::Slerp(t, kArm_L1RotateEnd, kArm_L1RotateDef);
        partTransforms_[kArm_L2].rotate = Quaternion::Slerp(t, kArm_L2RotateEnd, kArm_L2RotateDef);
        partTransforms_[kArm_R1].rotate = Quaternion::Slerp(t, kArm_R1RotateEnd, kArm_R1RotateDef);
        partTransforms_[kArm_R2].rotate = Quaternion::Slerp(t, kArm_R2RotateEnd, kArm_R2RotateDef);
    }
}

