#pragma once

#include <functional>

#include "Math/MathUtils.h"

class GameObject;

class SphereCollider;
class CapsuleCollider;

class Collider {
public:
    using CallbackFunc = std::function<void(const Collider&)>;

    virtual ~Collider() {}

    virtual bool IsCollision(Collider* collider) = 0;
    virtual bool IsCollision(SphereCollider* other) = 0;
    virtual bool IsCollision(CapsuleCollider* other) = 0;

    void Callback(Collider* other);

    void SetGameObject(GameObject* gameObject) { gameObject_ = gameObject; }
    GameObject* GetGameObject() const { return gameObject_; }
    uint32_t GetCollisionAttribute() const { return collisionAttribute_; }
    void SetCollisionAttribute(uint32_t attribute) { collisionAttribute_ = attribute; }
    uint32_t GetCollisionMask() const { return collisionMask_; }
    void SetCollisionMask(uint32_t mask) { collisionMask_ = mask; }
    void SetCallback(CallbackFunc callback) { callback_ = callback; }
    void SetIsActive(bool isActive) { isActive_ = isActive; }
    bool IsActive() const { return isActive_; }


protected:
    bool CanCollision(Collider* other) const;

private:
    GameObject* gameObject_;
    uint32_t collisionAttribute_ = 0xFFFFFFFF;
    uint32_t collisionMask_ = 0xFFFFFFFF;
    CallbackFunc callback_;
    bool isActive_ = true;
};

class SphereCollider : public Collider {
public:

    bool IsCollision(Collider* other) override {
        if (CanCollision(other)) {
            return other->IsCollision(this);
        }
        return false;
    }

    bool IsCollision(SphereCollider* other) override;
    bool IsCollision(CapsuleCollider* other) override;

    void SetLocalOffset(const Vector3& offset) { localOffset_ = offset; }
    void SetRadius(float radius) { radius_ = radius; }

    const Vector3& GetLocalOffset() const { return localOffset_; }
    float GetRadius() const { return radius_; }

private:
    Vector3 localOffset_;
    float radius_;

};


class CapsuleCollider : public Collider {
public:

    bool IsCollision(Collider* other) override {
        if (CanCollision(other)) {
            return other->IsCollision(this);
        }
        return false;
    }

    bool IsCollision(SphereCollider* other) override;
    bool IsCollision(CapsuleCollider* other) override;

    void SetLocalStart(const Vector3& start) { localStart_ = start; }
    void SetLocalEnd(const Vector3& end) { localEnd_ = end; }
    void SetRadius(float radius) { radius_ = radius; }

    const Vector3& GetLocalStart() const { return localStart_; }
    const Vector3& GetLocalEnd() const { return localEnd_; }
    float GetRadius() const { return radius_; }

private:
    Vector3 localStart_;
    Vector3 localEnd_;
    float radius_;
};