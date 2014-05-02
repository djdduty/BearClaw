#ifndef AABOUNDINGBOX_H
#define AABOUNDINGBOX_H

#include <stdlib.h>
#include <Utils/Math/Vec3.h>
#include <Resource/Mesh.h>
#include <System/BcTypes.h>

namespace BearClaw {
class AABoundingBox {
public:
	Vec3 HalfDim;
	Vec3 Origin;

	AABoundingBox() : HalfDim(Vec3()), Origin(Vec3()), m_VboSet(false) {}

	AABoundingBox(Vec3 Min, Vec3 Max) : m_VboSet(false)
	{
		HalfDim = Vec3(abs(Max.x - Min.x) / 2, abs(Max.y - Min.y) / 2, abs(Max.z - Min.z) / 2);
		Origin = Max - HalfDim;
		SetupVBO();
	}

	AABoundingBox(Vec3 Position, VertexList Verts) : Origin(Position)
	{
		float MaxX, MaxY, MaxZ, MinX, MinY, MinZ;
		MaxX = MaxY = MaxZ = MinX = MinY = MinZ = 0;
		for (i32 i = 0; i < Verts.size(); i++) {
			Vec3 Pos = Verts[i].Pos;
			if (Pos.x > MaxX) MaxX = Pos.x;
			if (Pos.y > MaxY) MaxY = Pos.y;
			if (Pos.z > MaxZ) MaxZ = Pos.z;

			if (Pos.x < MinX) MinX = Pos.x;
			if (Pos.y < MinY) MinY = Pos.y;
			if (Pos.z < MinZ) MinZ = Pos.z;
		}

		HalfDim = Vec3(abs(MaxX - MinX) / 2, abs(MaxX - MinX) / 2, abs(MaxX - MinX) / 2);
		SetupVBO();
	}

	bool Contains(const AABoundingBox &Other) {
		if (abs(Origin.x - Other.Origin.x) + Other.HalfDim.x >= HalfDim.x*2) return false;
		if (abs(Origin.y - Other.Origin.y) + Other.HalfDim.y >= HalfDim.y*2) return false;
		if (abs(Origin.z - Other.Origin.z) + Other.HalfDim.z >= HalfDim.z*2) return false;

		return true;
	}

	bool Intersects(const AABoundingBox &Other)
	{
		if (abs(Origin.x - Other.Origin.x) >= (HalfDim.x + Other.HalfDim.x)) return false;
		if (abs(Origin.y - Other.Origin.y) >= (HalfDim.y + Other.HalfDim.y)) return false;
		if (abs(Origin.z - Other.Origin.z) >= (HalfDim.z + Other.HalfDim.z)) return false;

		return true;
	}

	void Draw() 
	{
		if (m_VboSet) {

		}
	}

private:
	GLuint m_Vbo;
	bool m_VboSet;

protected:
	void SetupVBO() 
	{
		m_VboSet = true;
	}
};
}

#endif