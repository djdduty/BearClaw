#ifndef MAINSTATE_H
#define MAINSTATE_H

#include <Scene/Scene.h>
#include <Components/CameraComponent.h>
#include <Components/TestRenderComponent.h>
#include <System/InputManager.h>
#include <Renderer/Renderer.h>
#include <System/StateManagement/State.h>
#include <Gui/FontString.h>

using namespace BearClaw;

namespace BcGame {
class MainState : public State
{
private:
    CameraComponent* m_CamComp;
    Scene* m_Scene;
	bool m_SceneUnset;
	KeyFunc m_KeyFunction;

public:
    MainState();
    ~MainState();

    void Init(StateManager* Manager);
    void DeInit();
    void OnActivate();
    void OnDeactivate();
    void Update(f64 DeltaTime);

	void KeyCallback(char button, Action_Type Type);

    //void KeyCallback(char button, Action_Type Type);
};
}

#endif