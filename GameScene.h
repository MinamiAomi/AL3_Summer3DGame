#pragma once

#include <map>
#include <memory>
#include <string>

#include "Engine/RenderManager.h"
#include "Engine/Model.h"
#include "Math/Camera.h"
#include "Math/LightStructs.h"
#include "GameObject.h"

class GameScene {
public:
    GameScene();
    ~GameScene();
    
    void Initialize();
    void Update();
    void Draw();
    void EndFrame();

    std::shared_ptr<Model> GetModel(const std::string& key) const { return models_.at(key); }

    template<class T>
    T* AddGameObject(const std::string& name) {
        T* o = new T(this, name);
        gameObjects_.emplace_back(o);
        return o;
    }
    GameObject* FindGameObject(const std::string& name);

    void ReloadScene();

private:
    void LoadResource();
    void CreateGameObjects();

    void LoadModel(const std::string& path);

    RenderManager& renderManager_;

    std::map<std::string, std::shared_ptr<Model>> models_;
    std::list<std::unique_ptr<GameObject>> gameObjects_;

    bool reload_;
};