#include <MainState.h>
#include <System/StateManagement/StateManager.h>
#include <Resource/Shapes.h>
#include <Gui/FontFile.h>
#include <functional>
#include <sstream>

using namespace BearClaw;
namespace BcGame {
MainState::MainState()
{
	m_SceneUnset = false;
}

MainState::~MainState()
{
	RenderSingleton->UnsetScene();
	m_Scene->DeInit();
	delete(m_Scene);
}

void MainState::Init(StateManager* Manager)
{
    m_Scene = new Scene("TestScene");
    m_Scene->Init();
	
    BC_LOG("Main state being initialized\n");

    SceneNode* CamNode = new SceneNode("CameraNode");
    m_Scene->AddChild(CamNode);
    m_CamComp = new CameraComponent("Camera1", true);
    CamNode->AddComponent(m_CamComp);
    m_CamComp->SetAsActiveCamera();
    m_CamComp->Translate(Vec3(0,0,-2));

	SceneNode* Node = new SceneNode("RenderNode");
	m_Scene->AddChild(Node);
	TestRenderComponent* tc = new TestRenderComponent("RenderNode-RenderComp");
	tc->GetMaterial()->SetDiffuseTex("Data/Textures/Checker.png");
	Node->AddComponent(tc);
	
    RenderSingleton->SetScene(m_Scene->GetRenderScene());
	
	using std::placeholders::_1;
	using std::placeholders::_2;
	m_KeyFunction = std::bind(&MainState::KeyCallback, this, _1, _2);
    InputMgr->AddKeyDownFunction(m_KeyFunction);
}

void MainState::DeInit()
{
	InputMgr->RemoveKeyDownFunctions();
	RenderSingleton->UnsetScene();
	m_SceneUnset = true;
	m_Scene->DeInit();
	delete(m_Scene);
}

void MainState::OnActivate()
{
    BC_LOG("Main state being activated\n");
}

void MainState::OnDeactivate()
{

}

void MainState::Update(f64 DeltaTime)
{
	if (!m_SceneUnset) {
		m_Scene->Update(DeltaTime);
	}
}

void MainState::KeyCallback(char button, Action_Type Type)
{
	if (button == BC_KEY_ESCAPE && Type == BC_PRESSED) {
		if (!m_SceneUnset) {
			GameWindow->SetCloseRequested(true);
		}
	}
}
}