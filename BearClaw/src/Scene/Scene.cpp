#include <Scene/Scene.h>
#include <System/Environment.h>

namespace BearClaw {
	Scene::Scene(string Name) : SceneNode(Name)
{
    m_RenderScene = new RenderScene();
    BC_LOG("Scene %s is being created.\n", m_Name);
}

Scene::~Scene()
{}

void Scene::OnInit()
{
    BC_LOG("Scene %s is being initialized.\n", m_Name);
    Enable();
    m_RenderScene->Init();
    //TODO
}

void Scene::OnDeInit()
{
	delete(m_RenderScene);
    BC_LOG("Scene %s is being deinitialized.\n", m_Name);

    //TODO
}

void Scene::OnEnable()
{
    BC_LOG("Scene %s is being Enabled.\n", m_Name);
    m_RenderScene->Enable();
    //TODO
}

void Scene::OnDisable()
{
    BC_LOG("Scene %s is being Disabled.\n", m_Name);
    m_RenderScene->Disable();
    //TODO
}

//TODO PhysicsWorld* Scene::GetPhysocsWorld()
//{

//}
}
