#pragma once

#include <memory>

#include "PlayerState.h"

class Player;

class PlayerStateManager {
public:
    void Initialize(Player* player, std::unique_ptr<PlayerState> initState);
    void Update();

    void ChangeState(std::unique_ptr<PlayerState> state);

private:
    Player* player_;
    std::unique_ptr<PlayerState> state_;
};