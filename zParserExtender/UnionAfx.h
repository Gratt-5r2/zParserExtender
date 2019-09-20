// Supported with union (c) 2018 Union team

#ifndef __UNION_AFX_H__
#define __UNION_AFX_H__

#include ".build_settings.inl"

#define XCALL(uAddr)			\
	__asm { mov esp, ebp	}	\
	__asm { pop ebp			}	\
	__asm { mov eax, uAddr	}	\
	__asm { jmp eax			}

#ifndef CORE_SECURE_DISABLE
#define UNSAFE_MESSAGE "Unsafe function. Don't call it or use CORE_SECURE_DISABLE"
#define UNSAFE __declspec(deprecated(UNSAFE_MESSAGE)) 
#else
#define UNSAFE
#endif

#define ASTAPI __declspec (dllimport)
#define SHWAPI __declspec (dllimport)
#define VDFAPI __declspec (dllimport)
#define __INLINE inline

extern "C"
{
  #include <Windows.h>
  #include <stdlib.h>
  #include <stdio.h>
  #include <math.h>
  #include <cmath>
}

#pragma comment (lib, "Psapi.lib")
#include <Psapi.h>

#pragma comment (lib, "Shw32.lib")
#pragma comment (lib, "Vdfs32g.lib")

#include "Common\UnionTypes\Macro.h"
#include "Common\UnionTypes\Types.h"
#include "Memory\Shw32.h"
#include "Common\Common.h"
#include "Core\Core.h"
#include "Vdfs\Vdfs.h"
#include "SystemPack\Union.Patch\CPatchFile.h"

#include "ZenGin\zGothicAPI.h"

#endif // __UNION_AFX_H__