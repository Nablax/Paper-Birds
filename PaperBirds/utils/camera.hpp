//
// Created by cosmos on 8/21/20.
//
#ifndef TEST_CAMERA_HPP
#define TEST_CAMERA_HPP

#include "libs.hpp"

namespace camera{

    const float kYaw = -90.0f;
    const float kPitch = 0.0f;
    const float kSpeed = 25.0f;
    const float kSensitivity = 0.1f;
    const float kZoom = 45.0f;
    const float kZoomMin = 1.0f;
    const float kZoomMax = 45.0f;

    enum CameraDirection {
        kForward,
        kBackward,
        kLeft,
        kRight
    };

    class MyCamera {
    public:
        MyCamera(glm::vec3 position, glm::vec3 worldUp = constvalue::kWorldUp, float yaw=kYaw, float pitch=kPitch)
                : mFront(constvalue::kFront), mCameraSpeed(camera::kSpeed), mMouseSensitivity(camera::kSensitivity), mZoom(camera::kZoom){
            mPosition = position;
            mWorldUp = worldUp;
            mYaw = yaw;
            mPitch = pitch;
            updateCamera();
        }
        glm::mat4 getViewMatrix() {
            return glm::lookAt(mPosition, mPosition + mFront, mUp);
        }
		glm::vec3 getPosition() {
			return mPosition;
		}
        void processKeyboardInput(camera::CameraDirection dir, float time) {
            float shift = mCameraSpeed * time;
            switch (dir) {
                case kForward:
                    mPosition += mFront * shift;
                    break;
                case kBackward:
                    mPosition -= mFront * shift;
                    break;
                case kLeft:
                    mPosition -= mRight * shift;
                    break;
                case kRight:
                    mPosition += mRight * shift;
                    break;
            }
        }
        void processMouseMovement(float offsetX, float offsetY, GLboolean noFlip=true) {
            mYaw += offsetX * mMouseSensitivity;
            mPitch += offsetY * mMouseSensitivity;
            if (noFlip)
            {
                if (mPitch > 89.0f)
                    mPitch = 89.0f;
                if (mPitch < -89.0f)
                    mPitch = -89.0f;
            }
            updateCamera();
        }
        void processMouseScroll(float offsetY) {
            mZoom -= offsetY;
            if (mZoom < kZoomMin)
                mZoom = kZoomMin;
            if (mZoom > kZoomMax)
                mZoom = kZoomMax;
        }

    private:
        glm::vec3 mPosition, mFront, mUp, mRight;
        glm::vec3 mWorldUp;
        float mYaw, mPitch;
        float mCameraSpeed, mMouseSensitivity, mZoom;
        void updateCamera() {
            glm::vec3 newFront;
            newFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
            newFront.y = sin(glm::radians(mPitch));
            newFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
            mFront = glm::normalize(newFront);
            mRight = glm::normalize(glm::cross(mFront, mWorldUp));
            mUp = glm::normalize(glm::cross(mRight, mFront));
        }
    };
}

#endif //TEST_CAMERA_HPP
