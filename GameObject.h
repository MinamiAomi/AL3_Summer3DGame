#pragma once

#include <string>

#include "Engine/Model.h"
#include "Math/Transform.h"

class GameScene;

class GameObject {
public:
    enum State {
        kActive,// 更新描画を行う
        kWait,  // 更新描画を行わない
        kDead,  // フレーム終了後GameSceneによって削除される
    };

    GameObject(GameScene* gameScene, std::string name, State state = kActive) :
        gameScene(*gameScene), name_(name), state_(state) {}
    virtual ~GameObject() {}

    virtual void Initialize() {}
    virtual void Update() {}
    virtual void Draw() {}

    const Transform& GetTransform() const { return transform_; }
    Transform& GetTransform() { return transform_; }
    const std::string& GetName() const { return name_; }
    State GetState() const { return state_; }

    void SetState(State state) { state_ = state; }

    GameScene& gameScene;
protected:
    Transform transform_;

private:
    const std::string name_;
    State state_;
};