/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided 
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the 
"Apache License"); you may not use this file except in compliance with the 
Apache License. You may obtain a copy of the Apache License at 
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2025 Audiokinetic Inc.
*******************************************************************************/

// AkLock.h

/// \file 
/// Platform independent synchronization services for plug-ins.

#ifndef _AK_TOOLS_COMMON_AKLOCK_H
#define _AK_TOOLS_COMMON_AKLOCK_H

#include <AK/AkPlatforms.h>

#if defined(AK_NULL_PLATFORM)
// null platform can just rely on std::mutex for locking
#include <mutex>
class CAkLock
{
public:
    inline AKRESULT Lock(void)
    {
        m_lock.lock();
        return AK_Success;
    }
    inline AKRESULT Unlock(void)
    {
        m_lock.unlock();
        return AK_Success;
    }
private:
    std::mutex m_lock;
};

#elif defined(AK_WIN) || defined(AK_XBOX)
#include <AK/Tools/Win32/AkLock.h>

#elif defined (AK_APPLE) 
#include <AK/Tools/POSIX/AkLock.h>

#elif defined (AK_ANDROID)
#include <AK/Tools/POSIX/AkLock.h>

#elif defined (AK_HARMONY)
#include <AK/Tools/POSIX/AkLock.h>

#elif defined (AK_PS4)
#include <AK/Tools/PS4/AkLock.h>

#elif defined (AK_PS5)
#include <AK/Tools/PS5/AkLock.h>

#elif defined (AK_LINUX)
#include <AK/Tools/POSIX/AkLock.h>

#elif defined (AK_EMSCRIPTEN)
#include <AK/Tools/POSIX/AkLock.h>

#elif defined (AK_QNX)
#include <AK/Tools/POSIX/AkLock.h>

#elif defined (AK_NX)
#include <AK/Tools/NX/AkLock.h>

#elif defined (AK_OUNCE)
#include <AK/Tools/Ounce/AkLock.h>

#else
#error AkLock.h: Undefined platform
#endif

#endif // _AK_TOOLS_COMMON_AKLOCK_H
