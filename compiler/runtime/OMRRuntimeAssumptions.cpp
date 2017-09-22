/*******************************************************************************
 * Copyright (c) 2000, 2017 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
 *******************************************************************************/

#include "runtime/OMRRuntimeAssumptions.hpp"
#include "env/jittypes.h"

#if defined(__IBMCPP__) && !defined(AIXPPC) && !defined(LINUXPPC)
#define ASM_CALL __cdecl
#else
#define ASM_CALL
#endif

#if defined(TR_HOST_S390) || defined(TR_HOST_X86) || defined(TR_HOST_ARM)
// on these platforms, _patchVirtualGuard is written in C++
extern "C" void _patchVirtualGuard(uint8_t *locationAddr, uint8_t *destinationAddr, int32_t smpFlag);
#else
extern "C" void ASM_CALL _patchVirtualGuard(uint8_t*, uint8_t*, uint32_t);
#endif

TR::PatchNOPedGuardSiteOnMethodBreakPoint* TR::PatchNOPedGuardSiteOnMethodBreakPoint::make(
      TR_FrontEnd *fe, TR_PersistentMemory * pm, TR_OpaqueMethodBlock *method, uint8_t *location, uint8_t *destination,
      OMR::RuntimeAssumption **sentinel)
   {
   TR::PatchNOPedGuardSiteOnMethodBreakPoint *result = new (pm) TR::PatchNOPedGuardSiteOnMethodBreakPoint(pm, method, location, destination);
   result->addToRAT(pm, RuntimeAssumptionOnMethodBreakPoint, fe, sentinel);
   return result;
   }
 

void TR::PatchNOPedGuardSite::compensate(bool isSMP, uint8_t *location, uint8_t *destination)
   {
   _patchVirtualGuard(location, destination, isSMP);
   }
