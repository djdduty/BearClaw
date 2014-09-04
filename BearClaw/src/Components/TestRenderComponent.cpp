#include <Components/TestRenderComponent.h>
#include <Scene/Scene.h>
#include <System/Environment.h>
#include <Resource/Shapes.h>

namespace BearClaw {
TestRenderComponent::TestRenderComponent(string Name) : Component(Name), RenderNode(Name + "-RenderNode")
{
    Timer = 0.0f;
    M = new Mesh();
}

TestRenderComponent::~TestRenderComponent()
{
	delete M;
}

void TestRenderComponent::OnInit()
{
    m_Node = m_Owner;
    m_Owner->GetScene()->GetRenderScene()->AddChild(this);
}

void TestRenderComponent::InitRenderNode()
{
	VertexList Verts = VertexList();
    Verts = GetSphereVerts(1, 16, 32);
	IndexList Indices = IndexList();
    Indices = GetSphereIndices(16, 32);

    M->LoadMesh(Verts, Indices);
	m_Owner->SetAABB(Verts);
}

void TestRenderComponent::OnDeInit()
{
	M->DeInit();
}

void TestRenderComponent::OnEnable()
{

}

void TestRenderComponent::OnDisable()
{

}

void TestRenderComponent::Update(f64 DeltaTime)
{

}

void TestRenderComponent::Render()
{
    M->Render();
}
}
