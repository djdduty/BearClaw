#include <Renderer/RenderScene.h>
#include <Renderer/Renderer.h>

namespace BearClaw {
RenderScene::RenderScene()
{

}

RenderScene::~RenderScene()
{
	while (m_Children.size() > 0)
	{
		RemoveChild(m_Children.begin()->first);
	}
}

RenderNode* RenderScene::AddChild(RenderNode* Node)
{
    if(Node != nullptr)
    {
		string Key = Node->GetHandle();
        RenderNode* ChildPtr = Node;
        m_Children.insert(std::make_pair(Key, ChildPtr));
        ChildPtr->InitRenderNode();

        if(!m_Enabled)
            ChildPtr->SetVisible(false);
        else
            ChildPtr->SetVisible(true);

        return ChildPtr;
    }
	
	return nullptr;
}

void RenderScene::Init()
{}

void RenderScene::RemoveChild(string Name)
{
	RenderNode* N = FindChild(Name);
	m_Children.erase(Name);
}

void RenderScene::SetActiveCamera(BcCamera* Cam)
{
    m_ActiveCamera = Cam;
}

BcCamera* RenderScene::GetActiveCamera()
{
    return m_ActiveCamera;
}

void RenderScene::Enable()
{
    if(!m_Enabled)
    { 
        m_Enabled = true;
        for(RenderNodes::iterator it = m_Children.begin(); it != m_Children.end(); it++)
        {
            it->second->SetVisible(true);
        }
    }
}

void RenderScene::Disable()
{
    if(m_Enabled)
    { 
        m_Enabled = false;
        for(RenderNodes::iterator it = m_Children.begin(); it != m_Children.end(); it++)
        {
            it->second->SetVisible(false);
        }
    }
}

bool RenderScene::IsEnabled()
{
    return m_Enabled;
}

RenderNode* RenderScene::FindChild(string Name)
{
	for (RenderNodes::iterator it = m_Children.begin(); it != m_Children.end(); it++)
	{
		if (it->first == Name)
			return it->second;
	}

	return nullptr;
}
}