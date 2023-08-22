#include "Material.h"

#include "Core/CommandList.h"
#include "Core/Texture.h"

Material::Material() {
    constantBuffer_.Create(sizeof(MaterialConstant));
    constantData_ = reinterpret_cast<MaterialConstant*>(constantBuffer_.GetCPUData());
}

void Material::SetColor(const Vector4& color) {
    constantData_->color = color;
}
