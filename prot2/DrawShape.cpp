#include "DrawShape.h"

const WORD DrawShape::rectIndex[6] = { 0, 1, 2, 3, 2, 1 };
VERTEX3D DrawShape::rectVertex[4] = {};

namespace
{
static float left = 0.0f;
static float right = 0.0f;
static float up = 0.0f;
static float down = 0.0f;
}

DrawShape* DrawShape::Get()
{
	static DrawShape instance = {};
	return &instance;
}

DrawShape::DrawShape()
{
	Init();
}

void DrawShape::Init()
{
	static bool isInit = false;

	if (isInit)
	{
		return;
	}

	for (size_t i = 0; i < sizeof(rectVertex) / sizeof(rectVertex[0]); i++)
	{
		rectVertex[i].norm = Vector3(0.0f, 0.0f, -1.0f);
		rectVertex[i].spc = GetColorU8(0xFF, 0xFF, 0xFF, 0x00);
		rectVertex[i].su = 0.0f;
		rectVertex[i].sv = 0.0f;
	}

	rectVertex[0].u = 0.0f;
	rectVertex[0].v = 0.0f;
	rectVertex[1].u = 1.0f;
	rectVertex[1].v = 0.0f;
	rectVertex[2].u = 0.0f;
	rectVertex[2].v = 1.0f;
	rectVertex[3].u = 1.0f;
	rectVertex[3].v = 1.0f;

	isInit = true;
}

void DrawShape::DrawPlane(const Vector3& pos1, const Vector3& pos2, const unsigned int& color)
{
	static int r, g, b;
	//static const int graph = LoadGraph("./Resources/block.png");

	Init();

	left = pos1.x;
	right = pos2.x;
	up = -pos2.y;
	down = -pos1.y;

	rectVertex[0].pos = Vector3(left, up, 0.0f);
	rectVertex[1].pos = Vector3(right, up, 0.0f);
	rectVertex[2].pos = Vector3(left, down, 0.0f);
	rectVertex[3].pos = Vector3(right, down, 0.0f);

	GetColor2(color, &r, &g, &b);

	for (size_t i = 0; i < sizeof(rectVertex) / sizeof(rectVertex[0]); i++)
	{
		rectVertex[i].dif = GetColorU8(r, g, b, 0xFF);
	}

	DxLib::DrawPolygonIndexed3D(rectVertex, 4, rectIndex, 2, DX_NONE_GRAPH, false);
}

void DrawShape::DrawPlane(const Vector3& pos1, const Vector3& pos2, const COLOR_U8& color)
{
	Init();

	left = pos1.x;
	right = pos2.x;
	up = -pos2.y;
	down = -pos1.y;

	rectVertex[0].pos = Vector3(left, down, 0.0f);
	rectVertex[1].pos = Vector3(right, down, 0.0f);
	rectVertex[2].pos = Vector3(left, up, 0.0f);
	rectVertex[3].pos = Vector3(right, up, 0.0f);

	for (size_t i = 0; i < sizeof(rectVertex) / sizeof(rectVertex[0]); i++)
	{
		rectVertex[i].dif = color;
	}

	DxLib::DrawPolygonIndexed3D(rectVertex, 4, rectIndex, 2, DX_NONE_GRAPH, true);
}
