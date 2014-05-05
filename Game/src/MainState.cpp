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
    m_Scene = new Scene("TestScene", Vec3(6,6,6));
    m_Scene->Init();
	
    BC_LOG("Main state being initialized\n");

    m_CamNode = new SceneNode("CameraNode");
    m_Scene->AddChild(m_CamNode);
    CameraComponent* CamComp = new CameraComponent("Camera1", true);
    m_CamNode->AddComponent(CamComp);
	CamComp->SetAsActiveCamera();
    m_CamNode->Translate(Vec3(0,0,1));

	i32 num = 0;
	for (i32 i = 0; i < 6; i++) {
		for (i32 y = 0; y < 6; y++) {
			for (i32 z = 0; z < 1; z++) {
				Vec3 Pos = Vec3(i + 0.7, y+0.5, z + 1);
				ostringstream ss;
				ss << num++;
				SceneNode* Node = new SceneNode("RenderNode" + ss.str());
				m_Scene->AddChild(Node);
				TestRenderComponent* tc = new TestRenderComponent("RenderNode-RenderComp" + ss.str());
				tc->GetMaterial()->SetDiffuseTex("Data/Textures/Checker.png");
				Node->AddComponent(tc);
				Node->Translate(Pos*0.9);
			}
		}
	}
	
    RenderSingleton->SetScene(m_Scene->GetRenderScene());
	
	using std::placeholders::_1;
	using std::placeholders::_2;
	m_KeyFunction = std::bind(&MainState::KeyCallback, this, _1, _2);
	m_MouseFunc = std::bind(&MainState::OnMouseMove, this, _1, _2);
    InputMgr->AddKeyDownFunction(m_KeyFunction);
	InputMgr->AddMouseMoveFunction(m_MouseFunc);
}

void MainState::DeInit()
{
	InputMgr->RemoveKeyDownFunctions();
	InputMgr->RemoveMouseMoveFunctions();
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
	bool Changed = false;
	Vec3 Delta;
	if (InputMgr->GetKey(BC_KEY_W) == BC_PRESSED) {
		Delta.z -= 0.005*DeltaTime;
		Changed = true;
	}

	if (InputMgr->GetKey(BC_KEY_S) == BC_PRESSED) {
		Delta.z += 0.005*DeltaTime;
		Changed = true;
	}

	if (InputMgr->GetKey(BC_KEY_A) == BC_PRESSED) {
		Delta.x -= 0.005*DeltaTime;
		Changed = true;
	}

	if (InputMgr->GetKey(BC_KEY_D) == BC_PRESSED) {
		Delta.x += 0.005*DeltaTime;
		Changed = true;
	}

	if (InputMgr->GetKey(BC_KEY_Q) == BC_PRESSED) {
		Delta.y -= 0.005*DeltaTime;
		Changed = true;
	}

	if (InputMgr->GetKey(BC_KEY_E) == BC_PRESSED) {
		Delta.y += 0.005*DeltaTime;
		Changed = true;
	}

	if (Changed)
		m_CamNode->Translate(Delta);

	f64 Time = BCGetTimeInMS();
	std::vector<SceneNode*> Nodes;
	m_Scene->GetOctree()->GetNodesNearPosition(Vec3(4.7*0.9, 0.5, 0), Nodes, 3);
	for (i32 i = 0; i < Nodes.size(); i++) {
		Nodes[i]->GetAABB()->m_Material->SetDiffuseColor(Vec4(0, 1, 0, 1));
	}
	BC_LOG("Time %f\n", BCGetTimeInMS() - Time);
}

void MainState::OnMouseMove(double x, double y) {
	f32 dx = (WindowWidth / 2) - x;
	f32 dy = (WindowHeight / 2) - y;
	m_CamNode->Rotate(Vec3(dy / 20, dx / 10, 0));
	InputMgr->SetMousePosition(Vec2(WindowWidth / 2, WindowHeight / 2));
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