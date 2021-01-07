#include "pch.h"
#include "Camera.h"
#include "Windows.h"

Camera::Camera(Transform* parent, CameraType type)
{
	m_parentTransform = parent;

	m_type = type;
	switch (m_type)
	{
	case ORTHOGRAPHIC:
		SetProjOrtho(0, (float)WINDOW_W, 0, (float)WINDOW_H, m_nearPlane, m_farPlane);
		break;
	case PERSPECTIVE:
		SetProjPersp(m_fov, m_ratio, m_nearPlane, m_farPlane);
		break;
	}
}

void Camera::Recalculate()
{
		m_viewMatrix = glm::lookAt(m_parentTransform->GetPosition() + positionModifer,
			m_parentTransform->GetPosition() + m_parentTransform->GetForward(),
			m_parentTransform->GetUp());
		VP = m_projMatrix * m_viewMatrix;	
}

void Camera::SetProjPersp(float fov, float ratio, float nearPlane, float farPlane)
{
	m_type = CameraType::PERSPECTIVE;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
	m_fov = fov;
	m_ratio = ratio;
	m_projMatrix = glm::perspective(fov, ratio, nearPlane, farPlane);
}

void Camera::SetProjOrtho(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
	m_type = CameraType::ORTHOGRAPHIC;
	float m_nearPlane = nearPlane;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
	m_bottom = bottom;
	m_left = left;
	m_top = top;
	m_right = right;
	m_projMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
}

void Camera::SetCamera1()
{
	positionModifer = glm::vec3(0.0f, 8.0f, -10.0f);
}

void Camera::SetCamera2()
{
	positionModifer = glm::vec3(10.0f, 18.0f, -10.0f);
}