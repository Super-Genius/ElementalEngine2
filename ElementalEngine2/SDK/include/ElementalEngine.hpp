///============================================================================
/// \file	ElementalEngine.hpp
/// \note   Elemental Engine
///         Copyright (c)  2005-2008 Signature Devices, Inc.
///
///         This code is redistributable under the terms of the EE License.
///
///         This code is distributed without warranty or implied warranty of
///			merchantability or fitness for a particular purpose. See the 
///			EE License for more details.
///
///         You should have received a copy of the EE License along with this
///			code; If not, write to Signature Devices, Inc.,
///			3200 Bridge Parkway Suite 102, Redwood City, CA 94086 USA.
///============================================================================ 

#include <map>
#include <list>
#include <hash_map>
#include <set>
#include <queue>
#include <stdarg.h>
#include <algorithm>
#include <stack>

#include "Platform.h"

using namespace std;

//Moved to win32 platform header file.
//#ifdef _MSC_VER
//using namespace stdext;
//#endif

//Moved to apropriate platform header files.
//#ifdef _WIN32
//#define GLOBALALLOC(size) GlobalAlloc(GMEM_FIXED, size)
//#define GLOBALFREE(x) GlobalFree((HGLOBAL)x)
//#else
//#define GLOBALALLOC malloc
//#define GLOBALFREE free
//#endif

#define SAFE_DELETE(p)        {if (p) {delete p;p=NULL;}}
#define SAFE_DELETE_ARRAY(p)  {if (p) {delete[] p;p=NULL;}}

#define SAFE_REMOVE(p)        {if (p) {p->Remove();p=NULL;}}
#define SAFE_RELEASE(p)       {if (p) {p->Release();p=NULL;}}

//using namespace std; // it is already on the top of the file
#include "StdString.h"
#include "EEErrorCodes.hpp"
#include "MessageError.hpp"
#include "IHashString.hpp"
#include "IArchive.hpp"
#include "IXMLArchive.h"
#include "IComponent.hpp"
#include "IEEStream.h"
#include "IListener.h"
#include "IListenerFactory.h"
#include "IManager.h"
#include "IObject.h"
#include "ISceneObject.h"
#include "IAttributeObject.h"
#include "IToolBox.h"
#include "ITimer.h"
#include "IVisitor.h"
#include "ITextureObject.h"
#include "RenderFlags.h"
#include "RenderStructs.h"
#include "MathUtils.h"
#include "ISceneSettings.h"
#include "IMeshObject.h"
#include "IVertexFormat.h"
#include "IShader.h"
#include "IShaderPath.h"
#include "IRenderObject.h"
#include "IRenderer.h"
#include "IRenderContext.h"
#include "ICamera.h"
#include "ITextureObject.h"
#include "IGeometry.h"
#include "IFont.h"
#include "IVertexBufferObject.h"
#include "IIndexBuffer.h"
#include "IController.h"
#include "IConfig.h"
#include "ILightObject.h"
#include "KeyDefs.h"
#include "JoystickKeyDefs.h"
#include "IJoystick.h"
#include "IJoystickHandler.h"
#include "IScene.h"
#include "ISceneNode.h"
#include "IAnimatedTexture.h"
#include "IMaterialTemplate.h"
#include "IMaterial.h"
#include "IMaterialOverride.h"
#include "IGeometry.h"
#include "IParticle.h"
#include "IParticleEmitter.h"
#include "IEffect.h"

using namespace ElementalEngine;
