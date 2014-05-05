#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <Utils/AABoundingBox.h>
#include <Scene/Component.h>
#include <Utils/Math.h>
#include <map>

using namespace std::tr1;

namespace BearClaw {
class Scene;

typedef std::map<string, StrongNodePtr>       SceneNodes;
typedef std::map<string, StrongComponentPtr>  Components;

class SceneNode
{
private:
    bool        m_Enabled;//               = false;
    bool        m_Initialized;//           = false;
    bool        m_IsUpdatingAfterChange;// = false;
    bool        m_ShouldDie;//             = false;
    bool        m_ParentHasBeenSet;//      = false;
	bool		m_Modified;//			   = false; This is so the Octree knows when to update the node.

protected:
    uid         m_Id;
	string		m_Name;
    bool        IsScene();
    SceneNode*  m_Parent;
	
	Vec3 m_Position;
	Vec3 m_Rotation;
	Mat4 m_Transform;
	bool m_TransNeedsUpdate;

    //Functions
    void UpdateAllComponents(f64 DeltaTime);
    void UpdateAllChildren(f64 DeltaTime);
    //
    
    //Components
    Components m_Components;

    //Children
    SceneNodes m_Children;

	//Misc
	AABoundingBox* m_BoundingBox;

public:
    //Constructor/Destructor
	SceneNode(string Name);
    virtual ~SceneNode();
    //

    //misc
    void Init();
    void DeInit();
    void Enable();
    void Disable();
    void Kill();

    //Children functions
    StrongNodePtr   AddChild(SceneNode* Node);
	StrongNodePtr   FindChild(string Name, bool recursive = true);
	StrongNodePtr   FindChild(uid ID, bool recursive = true);
	void            RemoveChild(string Name);
    //

    //Component functions
	bool HasComponent(string name);

    template <typename ComponentType>
    shared_ptr<ComponentType> AddComponent(ComponentType* component)
    {
		string name = component->GetName();
        if(!HasComponent(name))
        {
            StrongComponentPtr  ptr(component);
            ptr->SetOwner(this);
            ptr->Init();
            m_Components.insert(std::make_pair(name, ptr));

            if(!m_Enabled)
                component->Disable();
            else
                component->Enable();

            UpdateAllComponents(0);
        }
        return FindComponent<ComponentType>(name);
    }

    template <typename ComponentType>
	shared_ptr<ComponentType> FindComponent(string Name)
    {
        if(!HasComponent(Name))
            return shared_ptr<ComponentType>();

        return dynamic_pointer_cast<ComponentType>(m_Components[Name]);
    }

	void RemoveComponent(string Name);
    //

    //Setters
    void SetParent(SceneNode* Parent);
	void Translate(Vec3 Pos)			{ m_Position += Pos; m_TransNeedsUpdate = true;	m_BoundingBox->Origin += Pos; m_Modified = true;	}
	void Rotate(Vec3 Rot)				{ m_Rotation += Rot; m_TransNeedsUpdate = true;														}
	void SetAABB(AABoundingBox* AABB)	{ if (AABB != nullptr){delete m_BoundingBox;} m_BoundingBox = AABB; m_Modified = true;				}
	void SetAABB(VertexList Verts)		{ delete m_BoundingBox; m_BoundingBox = new AABoundingBox(m_Position, Verts); m_Modified = true;	}
    //

    //Getters
	string GetName();
    bool GetEnabled();
    SceneNode* GetParent();
    virtual Scene* GetScene();
	inline inline Vec3 GetPosition()	{ return m_Position;	}
	inline inline Vec3 GetRotation()	{ return m_Rotation;	}
	inline inline uid GetID()			{ return m_Id;			}
	inline inline Mat4 GetTransform()	{ return m_Transform;	}
	inline AABoundingBox* GetAABB()		{ return m_BoundingBox; }
	inline bool GetModified()			{ return m_Modified;	}
	//

    //virtual misc
    virtual void OnInit();
    virtual void OnDeInit();
    virtual void OnEnable();
    virtual void OnDisable();
	virtual void OnUpdate(f64 DeltaTime);
    virtual void Update(f64 DeltaTime);
	virtual bool OnChildAdd(SceneNode* Node) { return true; }
    //

	//hacky hacky
	void Action(i32 type);
	virtual void OnAction(i32 type);
	void ResolveModification() { m_Modified = false; }
};
}

#endif
