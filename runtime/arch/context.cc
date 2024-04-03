/*
 * Copyright (C) 2011 The Android Open Source Project
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

#include "context-inl.h"

namespace art {

Context* Context::Create() {
  return new RuntimeContextType;
}

// Copy the GPRs and FPRs from the given thread's context to the given buffers. This function
// expects that a long jump (art_quick_do_long_jump) is called afterwards.
extern "C" void artContextCopyForLongJump(Context* context, uintptr_t* gprs, uintptr_t* fprs) {
  context->CopyContextTo(gprs, fprs);
  // Once the context has been copied, it is no longer needed.
  // The context pointer is passed via hand-written assembly stubs, otherwise we'd take the
  // context argument as a `std::unique_ptr<>` to indicate the ownership handover.
  delete context;
}

}  // namespace art
