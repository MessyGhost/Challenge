#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec2 rotation) noexcept
    :mPosition(position), mRotation(rotation), mContentHasBeenChanged(true)
{

}

void Camera::setPosition(const glm::vec3& position) noexcept {
    mPosition = position;
    mContentHasBeenChanged = true;
}

void Camera::setRotation(const glm::vec2& rotation) noexcept {
    mRotation = rotation;
    mContentHasBeenChanged = true;
}

const glm::vec3& Camera::getPosition() const noexcept {
    return mPosition;
}

const glm::vec2& Camera::getRotation() const noexcept {
    return mRotation;
}

void Camera::caculatePosition(const glm::vec3& delta) noexcept {
    mPosition += delta;
    mContentHasBeenChanged = true;
}

void Camera::caculateRotation(const glm::vec2& delta) noexcept {
    mRotation += delta;
    if (mRotation.x > glm::pi<float>() / 2.0f || mRotation.x < -glm::pi<float>() / 2.0f) {
        mRotation.x = (mRotation.x > 0 ? 1.0f : -1.0f) * glm::pi<float>() / 2.0f;
    }
    while (mRotation.y > glm::pi<float>() * 2.0f) {
        mRotation.y -= glm::pi<float>() * 2.0f;
    }
    while (mRotation.y < 0.0f) {
        mRotation.y += glm::pi<float>();
    }
    mContentHasBeenChanged = true;
}

const glm::mat4& Camera::getModelViewMatrix() const noexcept {
    return mContentHasBeenChanged ? (calculateModelViewMatrix(), mModelViewMatrix) : mModelViewMatrix;
}

const glm::mat4& Camera::getProjectionMatrix() const noexcept {
    return mProjectionMatrix;
}

void Camera::setProjectionInfo(float fov, float aspect, float viewDistance) noexcept {
    mProjectionMatrix = glm::perspective(fov, aspect, 0.0f, viewDistance);
}

void Camera::calculateModelViewMatrix() const noexcept {
    mContentHasBeenChanged = false;
    glm::vec3 center(
        mPosition.x + glm::cos(mRotation.y) * glm::cos(mRotation.x),
        mPosition.y + glm::sin(mRotation.x),
        mPosition.z + glm::sin(mRotation.y) * glm::cos(mRotation.x)
    );
    mModelViewMatrix = glm::lookAt(mPosition, center, glm::vec3(0.0f, 1.0f, 0.0f));
}
