/*==================================================================================
    Copyright (c) 2008, DAVA Consulting, LLC
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of the DAVA Consulting, LLC nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE DAVA CONSULTING, LLC AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL DAVA CONSULTING, LLC BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    Revision History:
        * Created by Vitaliy Borodovsky 
=====================================================================================*/
#include "Scene3D/MeshInstanceNode.h"
#include "Scene3D/Scene.h"
#include "Render/3D/StaticMesh.h"
#include "Render/RenderManager.h"
#include "Render/RenderHelper.h"

namespace DAVA 
{
MeshInstanceNode::MeshInstanceNode(Scene * _scene)
:	SceneNode3d(_scene)
,	visible(true)
,	debugFlags(DEBUG_DRAW_NONE)
{
	
}
	
MeshInstanceNode::~MeshInstanceNode()
{
	
}

void MeshInstanceNode::AddPolygonGroup(StaticMesh * mesh, int32 polygonGroupIndex, Material* material)
{
	meshes.push_back(mesh);
	polygonGroupIndexes.push_back(polygonGroupIndex);
	materials.push_back(material);
	
	PolygonGroup * group = mesh->GetPolygonGroup(polygonGroupIndex);
	bbox.AddAABBox(group->GetBoundingBox());
}

void MeshInstanceNode::Draw()
{
	if (!visible)return;
		


    
	Matrix4 prevMatrix = RenderManager::Instance()->GetMatrix(RenderManager::MATRIX_MODELVIEW); 
	Matrix4 meshFinalMatrix = worldTransform * prevMatrix;
    
    
    /* float32 proj[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, proj);
    
    for (int32 k = 0; k < 16; ++k)
    {
        if (proj[k] != prevMatrix.data[k])
        {
            printf("k:%d - %0.3f = %0.3f\n", k, proj[k], prevMatrix.data[k]);
        }
    } */
    
    RenderManager::Instance()->SetMatrix(RenderManager::MATRIX_MODELVIEW, meshFinalMatrix);
    
    //glMatrixMode(GL_MODELVIEW);
    //glPushMatrix();
    //glMultMatrixf(worldTransform.data);

	for (uint32 k = 0; k < meshes.size(); ++k)
	{
		meshes[k]->DrawPolygonGroup(polygonGroupIndexes[k], materials[k]);
	}
	
	if (debugFlags != DEBUG_DRAW_NONE)
	{
		glDisable(GL_DEPTH_TEST);
		RenderManager::Instance()->EnableTexturing(false);
		RenderManager::Instance()->FlushState();
		
		
		if (debugFlags & DEBUG_DRAW_AABBOX)
		{
			RenderManager::Instance()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			RenderHelper::DrawBox(bbox);
		
		}
		if (debugFlags & DEBUG_DRAW_LOCAL_AXIS)
		{
			RenderManager::Instance()->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
			RenderHelper::Instance()->DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(50.0f, 0.0f, 0.0f));
			
			RenderManager::Instance()->SetColor(0.0f, 1.0f, 0.0f, 1.0f);
			RenderHelper::Instance()->DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 50.0f, 0.0f));
			
			RenderManager::Instance()->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
			RenderHelper::Instance()->DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 50.0f));
		}
		glEnable(GL_DEPTH_TEST);
		RenderManager::Instance()->EnableTexturing(true);
        RenderManager::Instance()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	//glPopMatrix();
    RenderManager::Instance()->SetMatrix(RenderManager::MATRIX_MODELVIEW, prevMatrix);
}

SceneNode* MeshInstanceNode::Clone(SceneNode *dstNode)
{
    if (!dstNode) 
    {
        dstNode = new MeshInstanceNode(scene);
    }

    SceneNode3d::Clone(dstNode);
    MeshInstanceNode *nd = (MeshInstanceNode *)dstNode;
    nd->meshes = meshes;
    nd->polygonGroupIndexes = polygonGroupIndexes;
    nd->materials = materials;
    nd->visible = visible;
    nd->bbox = bbox;
    nd->debugFlags = debugFlags;
    
    return dstNode;
}

    
};
