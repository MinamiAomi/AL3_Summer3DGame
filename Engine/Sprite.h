#pragma once

#include <memory>

#include "Core/GPUResource.h"
#include "Math/MathUtils.h"

class Texture;


class Sprite {
    friend class RenderManager;
public:
    Sprite();

    void SetTranslate(const Vector2& translate) { translate_ = translate; }
    void SetAnchor(const Vector2& anchor) { anchor_ = anchor; }
    void SetRotate(float rotate) { rotate_ = rotate; }
    void SetSize(const Vector2& size) { size_ = size; }
    void SetTexcoordOffset(const Vector2& offset) { texcoordOffset_ = offset; }
    void SetTexcoordSize(const Vector2& size) { texcoordSize_ = size; }
    void SetColor(const Vector4& color) { color_ = color; }
    void SetTexture(const std::shared_ptr<Texture>& texture) { texture_ = texture; }

private:
    void UpdateBuffers(const Matrix4x4& matrix) const;

    UploadBuffer vertexBuffer_;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

    Vector2 translate_;
    Vector2 anchor_ = { 0.5f, 0.5f };
    float rotate_;
    Vector2 size_ = { 100.0f, 100.0f };
    Vector2 texcoordOffset_;
    Vector2 texcoordSize_ = { 64.0f, 64.0f };
    Vector4 color_ = Vector4::one;
    std::shared_ptr<Texture> texture_;
};