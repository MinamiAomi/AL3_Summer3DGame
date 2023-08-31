#include "Model.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "Core/TextureManager.h"

namespace {
    struct ModelInstanceConstant {
        Matrix4x4 worldMatrix;
    };

    struct Vertex {
        Vector3 position;
        Vector3 normal;
        Vector2 texcoord;
    };
}

ModelInstance::ModelInstance() : castShadow_(true), receiveShadow_(true) {
    modelInstanceBuffer_.Create(sizeof(ModelInstanceConstant));
    ModelInstanceConstant data;
    data.worldMatrix = Matrix4x4::identity;
    modelInstanceBuffer_.Copy(data);
}

void Model::CreateFromOBJ(const std::filesystem::path& path) {
    LoadOBJFile(path);
}

void Model::LoadOBJFile(const std::filesystem::path& path) {
    std::ifstream file(path);
    assert(file.is_open());

    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<Vector2> texcoords;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    std::unordered_map<std::string, uint32_t > vertexDefinitionMap;

    std::string line;
    while (std::getline(file, line)) {
        std::string identifier;
        std::istringstream iss(line);
        iss >> identifier;

        if (identifier == "#") {
            continue;
        }
        else if (identifier == "mtllib") {
            std::string materialFileName;
            iss >> materialFileName;
            LoadMTLFile(path.parent_path() / materialFileName);
        }
        else if (identifier == "v") {
            Vector3& position = positions.emplace_back();
            iss >> position.x >> position.y >> position.z;
           // position.z = -position.z;
        }
        else if (identifier == "vn") {
            Vector3& normal = normals.emplace_back();
            iss >> normal.x >> normal.y >> normal.z;
           // normal.z = -normal.z;
        }
        else if (identifier == "vt") {
            Vector2& texcoord = texcoords.emplace_back();
            iss >> texcoord.x >> texcoord.y;
            texcoord.y = 1.0f - texcoord.y;
        }
        else if (identifier == "usemtl") {
            std::string materialName;
            iss >> materialName;
            auto iter = std::find_if(materials_.begin(), materials_.end(),
                [&](const auto& material) {
                    return material.GetName() == materialName;
                });
            if (iter != materials_.end()) {
                Mesh mesh{};
                mesh.indexStartLocation = uint32_t(indices.size());
                if (!meshes_.empty()) {
                    meshes_.back().indexCount = uint32_t(indices.size()) - meshes_.back().indexStartLocation;
                }
                mesh.materialIndex = uint32_t(std::distance(materials_.begin(), iter));
                meshes_.emplace_back(mesh);
            }
            else {
                assert(false);
            }
        }
        else if (identifier == "f") {
            // 面の頂点を取得
            std::vector<std::string> vertexDefinitions;
            while (true) {
                std::string vertexDefinition;
                iss >> vertexDefinition;
                if (vertexDefinition.empty()) {
                    break;
                }
                vertexDefinitions.emplace_back(std::move(vertexDefinition));
            }
            assert(vertexDefinitions.size() > 2);
            std::vector<uint32_t> face(vertexDefinitions.size());
            for (uint32_t i = 0; i < vertexDefinitions.size(); ++i) {
                // 頂点が登録済み
                if (vertexDefinitionMap.contains(vertexDefinitions[i])) {
                    face[i] = vertexDefinitionMap[vertexDefinitions[i]];
                }
                else {
                    std::istringstream viss(vertexDefinitions[i]);
                    uint32_t elementIndices[3]{};
                    bool useElement[3]{};
                    for (uint32_t j = 0; j < 3; ++j) {
                        std::string index;
                        std::getline(viss, index, '/');
                        if (!index.empty()) {
                            elementIndices[j] = static_cast<uint32_t>(std::stoi(index)) - 1;
                            useElement[j] = true;
                        }
                    }
                    auto& vertex = vertices.emplace_back();
                    vertex.position = positions[elementIndices[0]];
                    if (useElement[1]) {
                        vertex.texcoord = texcoords[elementIndices[1]];
                    }
                    if (useElement[2]) {
                        vertex.normal = normals[elementIndices[2]];
                    }
                    face[i] = vertexDefinitionMap[vertexDefinitions[i]] = static_cast<uint32_t>(vertices.size() - 1);
                }
            }

            for (uint32_t i = 0; i < face.size() - 2; ++i) {
                indices.emplace_back(face[0]);
                indices.emplace_back(face[i + 1]);
                indices.emplace_back(face[i + 2]);
            }
        }
    }

    if (!meshes_.empty()) {
        meshes_.back().indexCount = uint32_t(indices.size()) - meshes_.back().indexStartLocation;
    }

    file.close();


    vertexBuffer_.Create(vertices.size() * sizeof(vertices[0]));
    UploadBuffer vertexUploadBuffer;
    vertexUploadBuffer.Create(vertexBuffer_.GetBufferSize());
    vertexUploadBuffer.Copy(vertices.data(), vertexUploadBuffer.GetBufferSize());
    vertexBuffer_.Upload(vertexUploadBuffer);
    vertexBufferView_.BufferLocation = vertexBuffer_.Get()->GetGPUVirtualAddress();
    vertexBufferView_.StrideInBytes = uint32_t(sizeof(vertices[0]));
    vertexBufferView_.SizeInBytes = uint32_t(vertexBuffer_.GetBufferSize());

    indexBuffer_.Create(indices.size() * sizeof(indices[0]));
    UploadBuffer indexUploadBuffer;
    indexUploadBuffer.Create(indexBuffer_.GetBufferSize());
    indexUploadBuffer.Copy(indices.data(), indexUploadBuffer.GetBufferSize());
    indexBuffer_.Upload(indexUploadBuffer);
    indexBufferView_.BufferLocation = indexBuffer_.Get()->GetGPUVirtualAddress();
    indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
    indexBufferView_.SizeInBytes = uint32_t(indexBuffer_.GetBufferSize());
}

void Model::LoadMTLFile(const std::filesystem::path& path) {
    std::ifstream file(path);
    assert(file.is_open());

    std::string line;
    while (std::getline(file, line)) {
        std::string identifier;
        std::istringstream iss(line);
        iss >> identifier;

        // コメントをスキップ
        if (identifier == "#") {
            continue;
        }
        else if (identifier == "newmtl") {
            std::string materialName;
            iss >> materialName;
            materials_.emplace_back();
            materials_.back().SetName(materialName);
        }
        else if (identifier == "map_Kd") {
            std::string textureName;
            iss >> textureName;
            /*auto p = path.parent_path();
            auto v = path.parent_path() / textureName;
            p;
            v;*/
            materials_.back().SetTexture(TextureManager::Load(path.parent_path() / textureName));
            materials_.back().SetColor({ 1.0f,1.0f,1.0f,1.0f });
        }
        else if (identifier == "Kd") {
            Vector4 color;
            iss >> color.x >> color.y >> color.z;
            color.w = 1.0f;
            materials_.back().SetColor(color);
        }
    }
}
