#include <Scene/Scene.h>
#include <System/Environment.h>

namespace BearClaw {
Scene::Scene(string Name, Vec3 HalfDimension, Vec3 Origin) : SceneNode(Name)
{
    m_RenderScene = new RenderScene();
	m_Octree = new Octree(Origin, HalfDimension);
	m_BoundingBox = new AABoundingBox(Origin, HalfDimension);
    BC_LOG("Scene %s is being created.\n", m_Name);
}

Scene::~Scene()
{}

bool Scene::OnChildAdd(SceneNode* Node) {
	if (m_BoundingBox->Contains(Node->GetAABB())) {
		m_Octree->Insert(Node);
		return true;
	}

	return false;
}

void Scene::OnUpdate(f64 DeltaTime) {
	m_Octree->Update();
}

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
