#include <Scene/SceneNode.h>
#include <System/Environment.h>

namespace BearClaw {
SceneNode::SceneNode(string Name)
{
    m_Name = Name;
    m_Id = Environ->GetUuid(BC_SCENE_NODE);

    m_Enabled               = false;
    m_Initialized           = false;
    m_IsUpdatingAfterChange = false;
    m_ShouldDie             = false;
    m_ParentHasBeenSet      = false;
	
	m_Position = Vec3(0, 0, 0);
	m_Rotation = Vec3(0, 0, 0);
	m_TransNeedsUpdate = true;
	m_Transform = Mat4(1.0f);

	m_BoundingBox = new AABoundingBox();
}

SceneNode::~SceneNode()
{
	delete m_BoundingBox;
}

void SceneNode::OnInit() {}
void SceneNode::Init()
{
    if(!m_Initialized)
    {
        m_Initialized = true;
        OnInit();
    }
}

void SceneNode::OnDeInit() {}
void SceneNode::DeInit()
{
    if(m_Initialized)
    {
        m_Initialized = false;
        OnDeInit();

        while(m_Children.size() > 0)
        {
            RemoveChild(m_Children.begin()->first);
			//BC_LOG("HELLO");
        }

        while(m_Components.size() > 0)
        {
            RemoveComponent(m_Components.begin()->first);
        }

    }

	m_Children.clear();
	m_Components.clear();
}

StrongNodePtr SceneNode::AddChild(SceneNode* Node)
{
    if(Node)
    {
		string Key = Node->GetName();
        StrongNodePtr ChildPtr(Node);
        m_Children.insert(std::make_pair(Key, ChildPtr));
        ChildPtr->SetParent(this);
        ChildPtr->Init();

        if(!m_Enabled)
            ChildPtr->Disable();
        else
            ChildPtr->Enable();

        return FindChild(Key, false);
    }
    else
    {
        return StrongNodePtr();
    }
}

StrongNodePtr SceneNode::FindChild(string Name, bool recursive)
{
    for(SceneNodes::iterator it = m_Children.begin(); it != m_Children.end(); it++)
    {
            if(it->first == Name)
                return it->second;
    }

    if(recursive)
    {
        for(SceneNodes::iterator it = m_Children.begin(); it != m_Children.end(); it++)
        {
            if(it->first == Name)
                return it->second;
            else
            {
                const StrongNodePtr& Child = it->second->FindChild(Name, recursive);
                if(Child)
                    return Child;
            }
        }
    }

    return StrongNodePtr();
}

StrongNodePtr SceneNode::FindChild(uid ID, bool recursive)
{
	for (SceneNodes::iterator it = m_Children.begin(); it != m_Children.end(); it++)
	{
		if (it->second->GetID() == ID)
			return it->second;
	}

	if (recursive)
	{
		for (SceneNodes::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			if (it->second->GetID() == ID)
				return it->second;
			else
			{
				const StrongNodePtr& Child = it->second->FindChild(ID, recursive);
				if (Child)
					return Child;
			}
		}
	}

	return StrongNodePtr();
}

bool SceneNode::HasComponent(string name)
{
	Components::iterator it = m_Components.find(name);
    return it != m_Components.end();
}

void SceneNode::RemoveChild(string Name)
{
    if(FindChild(Name, false))
    {
        StrongNodePtr n = FindChild(Name, false);
		n->DeInit();
        m_Children.erase(Name);
		n.reset();
    }
}

void SceneNode::RemoveComponent(string Name)
{
    if(HasComponent(Name))
    {
        StrongComponentPtr c = m_Components[Name];
		c->DeInit();
        m_Components.erase(Name);
		//c.reset();
    }
}

string SceneNode::GetName()
{
    return m_Name;
}

void SceneNode::SetParent(SceneNode* Parent)
{
    if(Parent)
    {
        if(!Parent->FindChild(m_Name, false))
        {
            if(m_ParentHasBeenSet)
            {
                SceneNodes::iterator it = m_Parent->m_Children.find(m_Name);
                Parent->m_Children.insert(std::make_pair(m_Name, it->second));
                m_Parent->m_Children.erase(it);
                m_Parent = Parent;
            }
            else
                Parent->AddChild(this);

            return;
        }
    }

    m_ParentHasBeenSet = true;
    m_Parent = Parent;
    UpdateAllComponents(0);
}

Scene* SceneNode::GetScene()
{
    return m_Parent->GetScene();
}

void SceneNode::OnUpdate(f64 DeltaTime) {}
void SceneNode::Update(f64 DeltaTime)
{
    if(m_Enabled)
    {
        UpdateAllChildren(DeltaTime);
        UpdateAllComponents(DeltaTime);
    }

	if (m_TransNeedsUpdate) {
		m_Transform = Mat4(1.0f);
		m_Transform.Rotate(m_Rotation);
		m_Transform.Translate(m_Position);
		m_TransNeedsUpdate = false;
	}

	OnUpdate(DeltaTime);
}

bool SceneNode::IsScene()
{
    return false;
}

void SceneNode::UpdateAllComponents(f64 DeltaTime)
{
    m_IsUpdatingAfterChange = (DeltaTime == 0);

    for(Components::iterator it = m_Components.begin(); it != m_Components.end(); it++)
    {
        if(it->second->GetEnabled())
            it->second->Update(DeltaTime);
    }

    m_IsUpdatingAfterChange = false;
}

void SceneNode::UpdateAllChildren(f64 DeltaTime)
{
    m_IsUpdatingAfterChange = (DeltaTime == 0);

    for(SceneNodes::iterator it = m_Children.begin(); it != m_Children.end(); it++)
    {
        if(it->second->m_ShouldDie)
        {
            SceneNode* Node = it->second.get();
            it--;
			string Name = Node->GetName();
            RemoveChild(Name);
        }
        else
            it->second->Update(DeltaTime);
    }

    m_IsUpdatingAfterChange = false;
}

void SceneNode::Kill()
{
    m_ShouldDie = true;
}

bool SceneNode::GetEnabled()
{
    return m_Enabled;
}

void SceneNode::OnEnable() {}
void SceneNode::Enable()
{
	if (!m_Enabled) {
		if (!m_ParentHasBeenSet || m_Parent->GetEnabled())
		{
			m_Enabled = true;

			for (SceneNodes::iterator it = m_Children.begin(); it != m_Children.end(); it++)
				it->second->Enable();

			for (Components::iterator it = m_Components.begin(); it != m_Components.end(); it++)
				it->second->Enable();

			OnEnable();
		}
	}
}


void SceneNode::OnDisable() {}
void SceneNode::Disable()
{
    if(m_Enabled)
    {
        m_Enabled = false;

        for(SceneNodes::iterator it = m_Children.begin(); it != m_Children.end(); it++)
            it->second->Disable();

        for(Components::iterator it = m_Components.begin(); it != m_Components.end(); it++)
            it->second->Disable();

        OnDisable();
    }
}

void SceneNode::OnAction(i32 type) {}
void SceneNode::Action(i32 type) {
	OnAction(type);
}
}
