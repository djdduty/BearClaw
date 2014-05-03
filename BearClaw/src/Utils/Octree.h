#ifndef OCTREE_H
#define OCTREE_H

#include <Utils/AABoundingBox.h>
#include <Scene/SceneNode.h>

namespace BearClaw {
class Octree
{
private:
	Octree* m_Children[8];
	i32 m_MaxObjectsPerNode;
	i32 m_MinObjectsPerNode;
	Octree* m_Parent;
	bool m_IsLeafNode;
	bool m_HasParent;

public:
	AABoundingBox BoundingBox;
	std::vector<SceneNode*> Data;
	
	Octree(Vec3 Origin, Vec3 HalfDim, Octree* Parent = nullptr, bool HasParent = false, i32 MaxObj = 5, i32 MinObj = 1) : BoundingBox(Origin, HalfDim) 
	{
		m_MaxObjectsPerNode = MaxObj;
		m_MinObjectsPerNode = MinObj;

		Data = std::vector<SceneNode*>();
		m_Parent = Parent;
		m_HasParent = HasParent;
		m_IsLeafNode = true;

		for (i32 i = 0; i < 8; i++) {
			m_Children[i] = NULL;
		}
	}

	~Octree() 
	{
		if (!IsLeafNode())
			for (i32 i = 0; i < 8; i++)
				delete m_Children[i];
	}

	inline i32 OctantContainingPoint(Vec3 Point) 
	{
		i32 Oct = 0;
		int oct = 0;
		if (Point.x >= BoundingBox.Origin.x) oct |= 4;
		if (Point.y >= BoundingBox.Origin.y) oct |= 2;
		if (Point.z >= BoundingBox.Origin.z) oct |= 1;
		return oct;
	}

	inline bool IsLeafNode() 
	{
		return m_IsLeafNode;
	}

	bool Insert(SceneNode* Node)
	{
		if (!BoundingBox.Contains(Node->GetAABB()) && m_HasParent)
			return false;

		if (IsLeafNode()) {
			if (Data.size() < m_MaxObjectsPerNode) {
				Data.push_back(Node);
				return true;
			} else {
				Split();
				Insert(Node);
				return true;
			}
		} else {
			i32 i = OctantContainingPoint(Node->GetAABB()->Origin);
			if (m_Children[i]->Insert(Node) == false){
				Data.push_back(Node);
				//BC_LOG("Node overlapping octree boundary\n");
			}
		}
	}

	void Split() 
	{
		m_IsLeafNode = false;
		for (i32 i = 0; i < 8; i++) {
			Vec3 ChildOrigin = BoundingBox.Origin;
			ChildOrigin.x += BoundingBox.HalfDim.x * (i & 4 ? .5f : -.5f);
			ChildOrigin.y += BoundingBox.HalfDim.y * (i & 2 ? .5f : -.5f);
			ChildOrigin.z += BoundingBox.HalfDim.z * (i & 1 ? .5f : -.5f);
			m_Children[i] = new Octree(ChildOrigin, BoundingBox.HalfDim * 0.5, this, true);
		}

		std::vector<SceneNode*> DataCopy = Data;
		Data.clear();
		for (i32 i = 0; i < DataCopy.size(); i++) {
			Insert(DataCopy[i]);
		}

	}

	void Merge()
	{
		for (i32 i = 0; i < 8; i++) {
			if (m_Children[i]->Data.size() > 0)
				return;
		}

		for (i32 i = 0; i < 8; i++) {
			delete m_Children[i];
		}
		m_IsLeafNode = true;
	}

	/*
	* So, this is a bit weird and requires explanation.... basically if SearchChildren is true it will only return scene nodes from the children, however if IncludeParentNodes is true
	* it will also include whatever scene nodes are from the octree node that is directly the parent of the octree node containing the Vec3. This is for any scene nodes that are left in a
	* parent octree node after a split because none of the child octree nodes completey contained that scene node. This way you will still test against scene nodes even if they are not 
	* considered in your octree node, but are actually still overlapping your octree node, a little inefficient but meh, better than having scene nodes in multiple octree nodes.
	*/
	void GetNodesNearPosition(Vec3 Pos, std::vector<SceneNode*>& Results, bool IncludeParentNodes = false, bool SearchChildren = true) 
	{
		if (SearchChildren) {  
			if (IsLeafNode()) {//I has no children :( Sad unfulfilled life...
				if (Data.size() > 0) {
					for (i32 i = 0; i < Data.size(); i++)
						Results.push_back(Data[i]);

					if (m_HasParent && IncludeParentNodes)
						m_Parent->GetNodesNearPosition(Pos, Results, true, false);
					return;
				}
			}
			else {//Point was somewhere in this node, but I have children so it must be somewhere in them.
				i32 Oct = OctantContainingPoint(Pos);//Find which child has the point in it's bounds
				m_Children[Oct]->GetNodesNearPosition(Pos, Results, IncludeParentNodes);//the last two bools make sure that that node searches children if it has them, if not it will include my nodes
			}
		} else {//You want MY nodes and not my children's nodes? Okay. (most likely called from a child of mine(if it's leaf node) with IncludeParentNodes = true as the param
			for (i32 i = 0; i < Data.size(); i++)
				if (m_Children[OctantContainingPoint(Pos)]->BoundingBox.Intersects(Data[i]->GetAABB()))
					Results.push_back(Data[i]);
		}
	}

	void Update()
	{
		if (m_IsLeafNode == false)
			for (i32 i = 0; i < 8; i++) 
				if (m_IsLeafNode == false) {
					m_Children[i]->Update();
					//BC_LOG("Child node updating\n");
				}

		Draw();
		if (Data.size() == 0 && m_HasParent) {
			m_Parent->Merge();
			return;
		}

		std::vector<SceneNode*> ReinsertList = std::vector<SceneNode*>();
		for (i32 i = Data.size(); i > 0; --i) {
			if (Data[i-1]->GetModified()) {
				Data[i-1]->ResolveModification();
				ReinsertList.push_back(Data[i-1]);
				Data.erase(Data.begin() + i-1);
			}
		}

		for (i32 i = 0; i < ReinsertList.size(); i++) {
			if (!BoundingBox.Contains(ReinsertList[i]->GetAABB()))
				GetTopNode()->Insert(ReinsertList[i]);
			else
				Insert(ReinsertList[i]);
		}

		
	}

	Octree* GetTopNode() 
	{
		if (!m_HasParent)
			return this;

		return m_Parent->GetTopNode();
	}

	void Draw()
	{
		BoundingBox.Draw();
	}
};
}

#endif