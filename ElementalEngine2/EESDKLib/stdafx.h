///============================================================================
/// \file	stdafx.h
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

#define NOMINMAX
#include "Platform.h"

#include <map>
#include <list>
#include <hash_map>
#include <set>
#include <vector>

using namespace std;

#include "ElementalEngine.hpp"
#include "BaseMessages.h"
#include "CHashString.hpp"
#include "StdString.h"
#include "EEErrorCodes.hpp"
#include "MessageError.hpp"
#include "IHashString.hpp"
#include "IArchive.hpp"
#include "IComponent.hpp"
#include "IObject.h"
#include "ISceneObject.h"
#include "IToolBox.h"
using namespace ElementalEngine;
#include "IVisitor.h"
#include "IMeshObject.h"
#include "IAttributeObject.h"

#include "EE-SDK.h"
#include "BaseMessages.h"
#include "CObjectTemplate.hpp"
#include "PerformanceProfiler.h"
