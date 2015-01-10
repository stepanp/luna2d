//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2014 Stepan Prokofjev
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#include "lunamesh.h"
#include "lunarenderer.h"
#include "lunaassets.h"
#include "lunagraphics.h"

using namespace luna2d;

LUNAMesh::LUNAMesh(int assetId)
{
	SetTexture(assetId);
}

void LUNAMesh::Clear()
{
	vertexes.clear();
}

void LUNAMesh::SetTexture(int assetId)
{
	if(!LUNAEngine::SharedAssets()->IsAssetA(assetId, LUNAAssetType::TEXTURE))
	{
		LUNA_LOGE("Asset with id \"%d\" is not texture", assetId);
		return;
	}

	texture = LUNAEngine::SharedAssets()->GetAsset<LUNATexture>(assetId, LUNAAssetType::TEXTURE);
}

void LUNAMesh::AddVertex(float x, float y, float r, float g, float b, float alpha, float u, float v)
{
	// Position
	vertexes.push_back(x);
	vertexes.push_back(y);

	// Color
	vertexes.push_back(r);
	vertexes.push_back(g);
	vertexes.push_back(b);
	vertexes.push_back(alpha);

	// Texture coordinates
	vertexes.push_back(u);
	vertexes.push_back(v);
}

void LUNAMesh::Render()
{
	if(texture.expired())
	{
		LUNA_LOGE("Attempt to render invalid mesh");
		return;
	}

	// Do not draw empty mesh
	if(vertexes.size() == 0) return;

	LUNARenderer* renderer = LUNAEngine::SharedGraphics()->GetRenderer();
	renderer->RenderVertexArray(texture.lock().get(), vertexes);
}
