#include "PlayerStateManager.h"

#include <cassert>

#include "PlayerState.h"

void PlayerStateManager::Initialize(Player* player, std::unique_ptr<PlayerState> initState) {
    assert(player);

    player_ = player;
    state_ = std::move(initState);
}

void PlayerStateManager::Update() {
    if (state_) {
        state_->Update();
    }
}

void PlayerStateManager::ChangeState(std::unique_ptr<PlayerState> nextState) {
    state_ = std::move(nextState);
    if (state_) {
        state_->Initialize();
    }
}
