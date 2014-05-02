#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#include <Renderer/RenderNode.h>
#include <System/Interfaces.h>
#include <Renderer/Camera.h>
#include <map>

namespace BearClaw {

typedef std::map<string, RenderNode*> RenderNodes;

class Renderer;

class RenderScene
{
public:
    friend class Renderer;

private:
    RenderNodes m_Children;
    BcCamera*   m_ActiveCamera;
    bool        m_Enabled;

public:
    RenderScene();
    ~RenderScene();

    RenderNode* AddChild(RenderNode* Node);
	void RemoveChild(string Name);

    void SetActiveCamera(BcCamera* Cam);
    BcCamera* GetActiveCamera();

    void Init();

    void Enable();
    void Disable();
    bool IsEnabled();
	RenderNode* FindChild(string Name);
};
}

#endif