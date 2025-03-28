/*
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) 2008 Google Inc. All rights reserved.
 * Copyright (C) 2011, 2012, 2013 Electronic Arts, Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CurrentTime_h
#define CurrentTime_h

#include <time.h>

namespace WTF {

// Returns the current UTC time in seconds, counted from January 1, 1970.
// Precision varies depending on platform but is usually as good or better
// than a millisecond.
double currentTime();

// Same thing, in milliseconds.
inline double currentTimeMS()
{
    return currentTime() * 1000.0;
}

inline void getLocalTime(const time_t* localTime, struct tm* localTM)
{
	//+EAWebKitChange
	//10/17/2011
	//10/18/2012
#if COMPILER(MSVC7_OR_LOWER) || COMPILER(MINGW) || OS(WINCE) || defined(CS_UNDEFINED_STRING) 
	*localTM = *localtime(localTime);
#elif COMPILER(MSVC)
	//-EAWebKitChange
    localtime_s(localTM, localTime);
#else
    localtime_r(localTime, localTM);
#endif
}

// Provides a monotonically increasing time in seconds since an arbitrary point in the past.
// On unsupported platforms, this function only guarantees the result will be non-decreasing.
double monotonicallyIncreasingTime();

} // namespace WTF

using WTF::currentTime;
using WTF::currentTimeMS;
using WTF::getLocalTime;
using WTF::monotonicallyIncreasingTime;

#endif // CurrentTime_h
