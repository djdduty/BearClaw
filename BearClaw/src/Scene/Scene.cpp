#include <Scene/Scene.h>
#include <System/Environment.h>

namespace BearClaw {
Scene::Scene(string Name, Vec3 HalfDimension, Vec3 Origin) : SceneNode(Name)
{
    m_RenderScene = new RenderScene();
	m_Octree = new Octree(Origin, HalfDimension);
	m_BoundingBox = new AABoundingBox(Origin, HalfDimension);
    BC_LOG("Scene is being created.\n");
}

Scene::~Scene()
{}

bool Scene::OnChildAdd(SceneNode* Node) {
	if (m_BoundingBox->Contains(Node->GetAABB())) {
        //m_Octree->Insert(Node);
		return true;
	}

	return false;
}

void Scene::OnUpdate(f64 DeltaTime) {
	m_Octree->Update();
}

void Scene::OnInit()
{
    BC_LOG("Scene is being initialized.\n");
    Enable();
    m_RenderScene->Init();
    //TODO
}

void Scene::OnDeInit()
{
	delete(m_RenderScene);
    BC_LOG("Scene is being deinitialized.\n");

    //TODO
}

void Scene::OnEnable()
{
    BC_LOG("Scene is being Enabled.\n");
    m_RenderScene->Enable();
    //TODO
}

void Scene::OnDisable()
{
    BC_LOG("Scene is being Disabled.\n");
    m_RenderScene->Disable();
    //TODO
}

//TODO PhysicsWorld* Scene::GetPhysocsWorld()
//{

//}
}
