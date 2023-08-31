#pragma once
#include "GameObject.h"

#include "Engine/Sprite.h"

class SystemManager : public GameObject {
public:
    enum State {
        kTitle,
        kInGame,
        kClear
    };

    using GameObject::GameObject;

    void Initialize() override;
    void Update() override;
    void Draw() override;

    void ChangeState(State state) { state_ = state; }
    State GetState() const { return state_; }

private:
    Sprite titleSprite_;
    Sprite clearSprite_;
    Sprite pushSprite_;
    State state_;
};