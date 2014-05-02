#include <Renderer/Camera.h>
#include <System/Environment.h>

namespace BearClaw {
BcCamera::BcCamera(CameraInitializer &Settings)
{
	m_Settings = Settings;
	m_TransNeedsUpdate = false;

	if (m_Settings.Persp)
		m_Projection = PerspectiveProjection(m_Settings.Fov, WindowWidth, WindowHeight, m_Settings.Near, m_Settings.Far);
	else
		m_Projection = OrthoProjection(0, WindowWidth, WindowHeight, 0, 0, m_Settings.Far);
}

void BcCamera::Init()
{
    m_Rotation = Vec3();
}

void BcCamera::Translate(Vec3 Trans)
{
	m_Position += Trans.Multiply(GetTransform());
	m_TransNeedsUpdate = true;
}

void BcCamera::Rotate(Vec3 Rot)
{
    m_Rotation = m_Rotation + Rot;
	m_TransNeedsUpdate = true;
}

void BcCamera::Update()
{
	if (m_TransNeedsUpdate)
		UpdateTransformation();
}

void BcCamera::UpdateProjection() 
{
	if (m_Settings.Persp)
		m_Projection = PerspectiveProjection(m_Settings.Fov, WindowWidth, WindowHeight, m_Settings.Near, m_Settings.Far);
	else
		m_Projection = OrthoProjection(0, WindowWidth, WindowHeight, 0, 0, m_Settings.Far);
}

Mat4 BcCamera::GetProjection()
{
	return m_Projection;
}

Mat4 BcCamera::GetTransform()
{
	if (m_TransNeedsUpdate)
		UpdateTransformation();
	return m_Transformation;
}

void BcCamera::UpdateTransformation() {
	m_Transformation = Mat4(1.0f);
	m_Transformation.Translate(m_Position);
	m_Transformation.Rotate(m_Rotation);
}
}
