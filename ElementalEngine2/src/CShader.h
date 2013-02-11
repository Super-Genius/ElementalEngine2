#ifndef CSHADER_H
#include "IShader.h"

struct Shader_t;

//move once we start expanding on the shader system:
class CShader : public IShader
{
	UINT	m_NumStages;
	IBaseTextureObject *	m_StageTextures[ MAX_TEXTURE_STAGES ];
	CHashString				m_StageTextureNames[ MAX_TEXTURE_STAGES ];
	IShaderCallback	* m_FrameCallback;
	IShaderCallback * m_ObjectCallback;
public:
	CShader( Shader_t * shad )
	{
		m_Shader = shad;
		m_NumStages = 0;
		m_FrameCallback = NULL;
		m_ObjectCallback = NULL;
		memset( m_StageTextures, 0, sizeof(m_StageTextures) );
		//memset( m_StageTextureNames, 0, sizeof( m_StageTextureNames  ) );
		for( int i = 0; i < MAX_TEXTURE_STAGES;i++)
		{
			m_StageTextureNames[ i ].Init( _T("") );
		}
	}

	virtual UINT GetNumStages()
	{
		return m_NumStages;
	}
	/// Sets the name of a texture stage. Used for loading a shader and associating texture stages with names
	/// that the model or other surfaces will also have
	virtual void SetTextureStage( UINT stage, IHashString * stagename )
	{
		if( stage >= 0 && stage < MAX_TEXTURE_STAGES )
		{
			m_StageTextureNames[ stage ].Init( stagename->GetString() );
			int numstages = 0;
			for( int i = 0; i < MAX_TEXTURE_STAGES; i++ )
			{
				if( _tcslen( m_StageTextureNames[ i ].GetString() ) <= 0 )
				{
					break;
				}
				numstages++;
			}
			m_NumStages = numstages;
		}
	}
	///  gets the name of the stage, or NULL if there's no name assocaited with this stage( uses custom tex or stage not used )
	virtual IHashString * GetTextureStageName( UINT stage )
	{
		if( stage >= 0 && stage < MAX_TEXTURE_STAGES )
		{
			return (IHashString*)&m_StageTextureNames[ stage ];
		}
		else
		{
			return NULL;
		}
	}
	/// does the stage use a custom texture
	virtual bool	IsStageCustom( UINT stage )
	{
		if( stage >= 0 && stage < MAX_TEXTURE_STAGES )
		{
			if( m_StageTextures[ stage ] != NULL )
			{
				return true;
			}
		}
		return false;
	}
	
	virtual void SetShaderTextureStage( UINT stage, IBaseTextureObject * texture )
	{
		if( stage >= 0 && stage < MAX_TEXTURE_STAGES )
		{
			m_StageTextures[ stage ] = texture;
		}
	}
	/// Gets the custom texture associate with a stage, or NULL if the stage does not use a custom texture or does not
	/// use a texture at all
	virtual IBaseTextureObject * GetShaderTextureFromStage( UINT stage )
	{
		if( stage >= 0 && stage < MAX_TEXTURE_STAGES )
		{
			return m_StageTextures[ stage ];
		}
		else
		{
			return NULL;
		}
	}
	/// gets the shader version
	virtual DWORD GetPixelShaderVersion()
	{
		return 0;
	}
	virtual DWORD GetVertexShaderVersion()
	{
		return 0;
	}


	virtual bool SetPerFrameCallback( IShaderCallback * callback )
	{
		m_FrameCallback = callback;
		return true;
	}
	virtual bool SetPerObjectCallback( IShaderCallback * callback )
	{
		m_ObjectCallback = callback;
		return true;
	}
	virtual IShaderCallback * GetFrameCallback()
	{
		return m_FrameCallback;
	}
	virtual IShaderCallback * GetObjectCallback()
	{
		return m_ObjectCallback;
	}
	virtual bool Serialize( IArchive &ar )
	{
		return true;
	}

	bool SetVertexShaderConstant( const int shaderconstant, float x, float y, float z, float w );
  	bool SetVertexShaderConstant( const int shaderconstant, float array[] );
 
  	void SetClipDistance( const float distance );
  	int Temp_GetNumStages();
  	void Temp_GetCustomTex(int index, ITextureObject *customTex);
  	void Temp_SetCustomTex(int index, ITextureObject *customTex);
  	void Temp_CopyTexRefPlaneS(int index, int firstStage);
  	void Temp_CopyTexRefPlaneT(int index, int firstStage);
 	void Temp_CopyTexRefPlaneR(int index, int firstStage);
  	void Temp_CopyTexRefPlaneQ(int index, int firstStage);
  	void Temp_CopyOwnTextureMatrix(int index, int firstStage);
  	void Temp_AddToTextureMatrix(int index, int row, int col, float value);
  	float* Temp_GetTexPlanePtr(int index, int plane);
  	float* Temp_GetOwnTextureMatPtr(int index);
  	float Temp_GetAdditionalParam(int index);
  	void Temp_SetAdditionalParam(int index, float param);

  	//non interface function:
  	Shader_t * GetVisionShader()
  	{
  		return m_Shader;
  	}

	virtual bool LoadFromFile( TCHAR * filename )
	{
		return false;

	}
	Shader_t * m_Shader;
};

#endif