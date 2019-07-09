#pragma once
#include <glm/glm.hpp>

class Camera {
public:
    Camera(glm::vec3 position, glm::vec2 rotation) noexcept;
    void setPosition(const glm::vec3& position) noexcept;
    void setRotation(const glm::vec2& rotation) noexcept;
    const glm::vec3& getPosition() const noexcept;
    const glm::vec2& getRotation() const noexcept;
    void calculatePosition(const glm::vec3& delta) noexcept;
    void calculateRotation(const glm::vec2& delta) noexcept;

    const glm::mat4& getModelViewMatrix() const noexcept;
    const glm::mat4& getProjectionMatrix() const noexcept;
    void setProjectionInfo(float fov, float aspect, float viewDistance) noexcept;
private:
    mutable bool mContentHasBeenChanged;
    glm::vec3 mPosition;
    glm::vec2 mRotation;
    mutable glm::mat4 mModelViewMatrix;
    glm::mat4 mProjectionMatrix;
    void calculateModelViewMatrix() const noexcept;
};
