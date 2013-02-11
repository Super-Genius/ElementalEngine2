#include "stdafx.h"
#include "CShader.h"
#include <Vision.hpp>
#include "IVec3.h"
#include "Vec3.h"
#include "CTextureObject.h"

bool CShader::SetVertexShaderConstant( const int shaderconstant, float x, float y, float z, float w )
{
	int constval = shaderconstant - 48;
	if( constval >= 0 && constval < 16 )
	{//vision limits
		m_Shader->vprog_custom_param[constval].param[0] = x;
		m_Shader->vprog_custom_param[constval].param[1] = y;
		m_Shader->vprog_custom_param[constval].param[2] = z;
		m_Shader->vprog_custom_param[constval].param[3] = w;
		return true;
	}
	return false;
}

bool CShader::SetVertexShaderConstant( const int shaderconstant, float array[] )
{
	int constval = shaderconstant - 48;
	if( constval >= 0 && constval < 16 )
	{//vision limits
		m_Shader->vprog_custom_param[constval].param[0] = array[0];
		m_Shader->vprog_custom_param[constval].param[1] = array[1];
		m_Shader->vprog_custom_param[constval].param[2] = array[2];
		m_Shader->vprog_custom_param[constval].param[3] = array[3];
		return true;
	}
	return false;
}

void  CShader::SetClipDistance( const float distance )
{
	m_Shader->detailpassdistance = (int)distance;
}

int CShader::Temp_GetNumStages()
{
	return m_Shader->numstages;
}

void CShader::Temp_GetCustomTex(int index, ITextureObject *customTex)
{

}

void CShader::Temp_SetCustomTex(int index, ITextureObject *customTex)
{
	ShaderPass_t *pStage = &m_Shader->passes[index];
	pStage->spCustomTex = ((CTextureObject*)customTex)->GetVisionTexture();
}

void CShader::Temp_CopyTexRefPlaneS(int index, int firstStage)
{
	ShaderPass_t *pStage = &m_Shader->passes[index];

	memcpy(&pStage->texrefplane_s,&m_Shader->passes[firstStage].texrefplane_s, sizeof(m_Shader->passes[firstStage].texrefplane_s));
}

void CShader::Temp_CopyTexRefPlaneT(int index, int firstStage) 
{
	ShaderPass_t *pStage = &m_Shader->passes[index];

	memcpy(&pStage->texrefplane_t,&m_Shader->passes[firstStage].texrefplane_t, sizeof(m_Shader->passes[firstStage].texrefplane_t));
}

void CShader::Temp_CopyTexRefPlaneR(int index, int firstStage)
{
	ShaderPass_t *pStage = &m_Shader->passes[index];

	memcpy(&pStage->texrefplane_r,&m_Shader->passes[firstStage].texrefplane_r, sizeof(m_Shader->passes[firstStage].texrefplane_r));
}

void CShader::Temp_CopyTexRefPlaneQ(int index, int firstStage)
{
	ShaderPass_t *pStage = &m_Shader->passes[index];

	memcpy(&pStage->texrefplane_q,&m_Shader->passes[firstStage].texrefplane_q, sizeof(m_Shader->passes[firstStage].texrefplane_q));
}

void CShader::Temp_CopyOwnTextureMatrix(int index, int firstStage)
{
	ShaderPass_t *pStage = &m_Shader->passes[index];

	memcpy(&pStage->own_texturematrix,&m_Shader->passes[firstStage].own_texturematrix, sizeof(m_Shader->passes[firstStage].own_texturematrix));
}

void CShader::Temp_AddToTextureMatrix(int index, int row, int col, float value)
{
	ShaderPass_t *pStage = &m_Shader->passes[index];

	pStage->own_texturematrix[row][col] += value;
}

float* CShader::Temp_GetTexPlanePtr(int index, int plane)
{
	ShaderPass_t *pStage = &m_Shader->passes[index];

	switch (plane)
	{
		case 0:
			return pStage->texrefplane_s;
			break;
		case 1:
			return pStage->texrefplane_t;
			break;
		case 2:
			return pStage->texrefplane_r;
			break;
		case 3:
			return pStage->texrefplane_q;
			break;
		default:
			return NULL;
			break;
	}
}

float* CShader::Temp_GetOwnTextureMatPtr(int index)
{
	ShaderPass_t *pStage = &m_Shader->passes[index];

	return &pStage->own_texturematrix[0][0];
}

float CShader::Temp_GetAdditionalParam(int index)
{
	return m_Shader->additional_param[index];
}

void CShader::Temp_SetAdditionalParam(int index, float param)
{
	m_Shader->additional_param[index] = param;
}

