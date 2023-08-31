#pragma once

#include <list>

class Collider;

class CollisionManager {
public:
    static CollisionManager& Get();

    void AddCollider(Collider* collider) { colliders_.push_back(collider); }
    void RemoveCollider(Collider* collider) { colliders_.remove(collider); }

    void CheckCollision();
    void Clear() { colliders_.clear(); }

private:
    CollisionManager() = default;
    CollisionManager(const CollisionManager&) = delete;
    CollisionManager& operator=(const CollisionManager&) = delete;

    std::list<Collider*> colliders_;
};