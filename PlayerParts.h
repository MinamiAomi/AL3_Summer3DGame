#pragma once

#include "Engine/Model.h"
#include "Math/Transform.h"

class Player;
class GameScene;

class PlayerParts {
public:
    enum Parts {
        kBody,
        kHead,
        kArm_L1,
        kArm_L2,
        kArm_R1,
        kArm_R2,

        kNumParts
    };

    enum Animation {
        kNone,
        kWait,
        kWalk,
        kDash,
        kAim,
        kAttack,

        kNumAnimations
    };

    void Initialize(Player* player);
    void UpdateTransform();
    void Draw();

    void PlayAnimation(Animation animation, uint32_t cycle, bool isLoop, bool resetPose = true);
    void StopAnimation();
    void RestartAnimation();
    Animation GetPlayingAnimation() const { return animation_; }
    bool IsPlayingAnimation() const { return isPlaying_; }

    ModelInstance& GetModelInstance(Parts parts) { return modelInstances_[parts]; }
    Transform& GetTransform(Parts parts) { return partTransforms_[parts]; }

private:

    void DefaultPose();
    void WaitAnimation(float t);
    void WalkAnimation(float t);
    void DashAnimation(float t);
    void AimAnimation(float t);
    void AttackAnimation(float t);

    Player* player_ = nullptr;

    ModelInstance modelInstances_[kNumParts];
    Transform partTransforms_[kNumParts];

    Animation animation_;
    float animationParameter_;
    uint32_t animationCycle_;
    bool isLoop_;
    bool isPlaying_;

};