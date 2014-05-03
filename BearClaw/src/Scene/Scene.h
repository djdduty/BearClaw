#ifndef SCENE_H
#define SCENE_H

#include <Scene/SceneNode.h>
#include <Renderer/RenderScene.h>
#include <Utils/Octree.h>

namespace BearClaw {
class Scene : public SceneNode
{
private:

protected:
    RenderScene*	m_RenderScene;
	Octree*			m_Octree;

public:
	Scene(string Name, Vec3 HalfDimension, Vec3 Origin=Vec3(0,0,0));
    ~Scene();
    void OnInit();
    void OnDeInit();
    void OnEnable();
    void OnDisable();
	bool OnChildAdd(SceneNode* Node);
	void OnUpdate(f64 DeltaTime);

	SceneNodes GetChildren() { return m_Children; }
    //TODO PhysicsWorld* GetPhysicsWorld();

    bool IsScene()
    {
        return true;
    }

    RenderScene* GetRenderScene()
    {
        return m_RenderScene;
    }

    Scene* GetScene()
    {
        return this;
    } 

	inline Octree* GetOctree() { return m_Octree; }
};
}

#endif
