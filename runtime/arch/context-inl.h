/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// This file is special-purpose for cases where you want a stack context. Most users should use
// Context::Create().

#include "arch/instruction_set.h"
#include "context.h"

#ifndef ART_RUNTIME_ARCH_CONTEXT_INL_H_
#define ART_RUNTIME_ARCH_CONTEXT_INL_H_

#include "arm/context_arm.h"
#include "arm64/context_arm64.h"
#include "riscv64/context_riscv64.h"
#include "x86/context_x86.h"
#include "x86_64/context_x86_64.h"

namespace art {

namespace detail {

template <InstructionSet>
struct ContextSelector;

template <>
struct ContextSelector<InstructionSet::kArm> { using type = arm::ArmContext; };
template <>
struct ContextSelector<InstructionSet::kArm64> { using type = arm64::Arm64Context; };
template <>
struct ContextSelector<InstructionSet::kRiscv64> { using type = riscv64::Riscv64Context; };
template <>
struct ContextSelector<InstructionSet::kX86> { using type = x86::X86Context; };
template <>
struct ContextSelector<InstructionSet::kX86_64> { using type = x86_64::X86_64Context; };

}  // namespace detail

template <InstructionSet Isa>
using RuntimeContextTypeArch = typename detail::ContextSelector<Isa>::type;
using RuntimeContextType = RuntimeContextTypeArch<kRuntimeQuickCodeISA>;

}  // namespace art

#endif  // ART_RUNTIME_ARCH_CONTEXT_INL_H_
