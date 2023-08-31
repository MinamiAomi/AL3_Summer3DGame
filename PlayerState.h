#pragma once

#include <cstdint>
#include <vector>

#include "Engine/Sprite.h"

class Player;

class PlayerState {
public:
    PlayerState(Player* player) : player_(player) {}
    virtual ~PlayerState() {}
    virtual void Initialize() = 0;
    virtual void Update() = 0;

protected:
    Player* player_;
};

class PlayerStateDefault : public PlayerState {
public:
    using PlayerState::PlayerState;

    void Initialize() override;
    void Update() override;

private:
    float walkAnimation_;
};


class PlayerStateDash : public PlayerState {
public:
    using PlayerState::PlayerState;

    void Initialize() override;
    void Update() override;

};

class PlayerStateAim : public PlayerState {
public:
    using PlayerState::PlayerState;

    void Initialize() override;
    void Update() override;

private:
    std::vector<class PlayerBullet*> waitingBullets_;
    uint32_t bulletCoolTime_;
    Sprite sprite_;
};