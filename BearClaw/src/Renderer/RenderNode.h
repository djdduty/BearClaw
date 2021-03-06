#ifndef RENDER_NODE_H
#define RENDER_NODE_H

#include <Utils/BcString.h>
#include <Utils/Math/Mat4.h>
#include <Renderer/Material.h>
#include <Scene/SceneNode.h>

namespace BearClaw {
class Material;

class RenderNode
{
private:

protected:
    SceneNode*  m_Node;
    Material*   m_Material;
	string		m_RenderHandle;
    bool        m_IsVisible;
    Mat4        m_Transform;
    //TODO BoundingBox

public:
	bool pRenderLast;

	RenderNode(string Handle)		{ pRenderLast = false; m_RenderHandle = Handle; m_Transform = new Mat4(); m_Material = new Material(this); }
	virtual ~RenderNode()			{ delete m_Material; }
    virtual void InitRenderNode()	{ };
	void PreRender()				{ m_Material->Bind(); m_Material->PrepareForRender(); }
    virtual void Render()			=0;
    void PostRender()				{m_Material->UnBind();}

    //TODO spatial and others
    SceneNode* GetNode()
    {
        return m_Node;
    }

    Material* GetMaterial()
    {
        return m_Material;
    }

    void SetMaterial(Material* Mat)
    {
        m_Material = Mat;
    }

    bool IsVisible()
    {
        return m_IsVisible;
    }

    void SetVisible(bool Visible)
    {
        m_IsVisible = Visible;
    }

	string GetHandle()
    {
        return m_RenderHandle;
    }

    Mat4* GetTransformPtr()
    {
        return &m_Transform;
    }

    Mat4 GetTransform()
    {
        //return m_Transform;
		if (m_Node != nullptr) {
			return m_Node->GetTransform();
		} else {
			return m_Transform;
		}
    }
};
}

#endif