#pragma once

#include <memory>
#include <string>

#include "Core/GPUResource.h"
#include "Math/MathUtils.h"

class CommandList;
class Texture;

class Material {
    friend class RenderManager;
public:
    Material();

    void SetName(const std::string& name) { name_ = name; }
    void SetColor(const Vector4& color);
    void SetTexture(const std::shared_ptr<Texture>& texture) { texture_ = texture; }

    const std::string& GetName() const { return name_; }

private:
    struct MaterialConstant {
        Vector4 color;
    };
    std::string name_;
    UploadBuffer constantBuffer_;
    MaterialConstant* constantData_;
    std::shared_ptr<Texture> texture_;
};