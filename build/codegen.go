// Copyright (C) 2016 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package art

// This file implements the "codegen" property to apply different properties based on the currently
// selected codegen arches, which defaults to all arches on the host and the primary and secondary
// arches on the device.

import (
	"android/soong/android"
	"sort"
	"strings"
)

func codegen(ctx android.LoadHookContext, c *codegenProperties, library bool) {
	var hostArches, deviceArches []string

	e := envDefault(ctx, "ART_HOST_CODEGEN_ARCHS", "")
	if e == "" {
		hostArches = supportedArches
	} else {
		hostArches = strings.Split(e, " ")
	}

	e = envDefault(ctx, "ART_TARGET_CODEGEN_ARCHS", "")
	if e == "" {
		deviceArches = defaultDeviceCodegenArches(ctx)
	} else {
		deviceArches = strings.Split(e, " ")
	}

	getCodegenArchProperties := func(archName string) *codegenArchProperties {
		var arch *codegenArchProperties
		switch archName {
		case "arm":
			arch = &c.Codegen.Arm
		case "arm64":
			arch = &c.Codegen.Arm64
		case "x86":
			arch = &c.Codegen.X86
		case "x86_64":
			arch = &c.Codegen.X86_64
		default:
			ctx.ModuleErrorf("Unknown codegen architecture %q", archName)
		}
		return arch
	}

	appendCodegenSourceArchProperties := func(p *CodegenSourceArchProperties, archName string) {
		arch := getCodegenArchProperties(archName)
		p.Srcs = append(p.Srcs, arch.CodegenSourceArchProperties.Srcs...)
	}

	addCodegenSourceArchProperties := func(host bool, p *CodegenSourceArchProperties) {
		type sourceProps struct {
			Target struct {
				Android *CodegenSourceArchProperties
				Host    *CodegenSourceArchProperties
			}
		}

		sp := &sourceProps{}
		if host {
			sp.Target.Host = p
		} else {
			sp.Target.Android = p
		}
		ctx.AppendProperties(sp)
	}

	addCodegenArchProperties := func(host bool, archName string) {
		type commonProps struct {
			Target struct {
				Android *CodegenCommonArchProperties
				Host    *CodegenCommonArchProperties
			}
		}

		type libraryProps struct {
			Target struct {
				Android *CodegenLibraryArchProperties
				Host    *CodegenLibraryArchProperties
			}
		}

		arch := getCodegenArchProperties(archName)

		cp := &commonProps{}
		lp := &libraryProps{}
		if host {
			cp.Target.Host = &arch.CodegenCommonArchProperties
			lp.Target.Host = &arch.CodegenLibraryArchProperties
		} else {
			cp.Target.Android = &arch.CodegenCommonArchProperties
			lp.Target.Android = &arch.CodegenLibraryArchProperties
		}

		ctx.AppendProperties(cp)
		if library {
			ctx.AppendProperties(lp)
		}
	}

	addCodegenProperties := func(host bool, arches []string) {
		sourceProps := &CodegenSourceArchProperties{}
		for _, arch := range arches {
			appendCodegenSourceArchProperties(sourceProps, arch)
			addCodegenArchProperties(host, arch)
		}
		sourceProps.Srcs = android.FirstUniqueStrings(sourceProps.Srcs)
		addCodegenSourceArchProperties(host, sourceProps)
	}

	addCodegenProperties(false /* host */, deviceArches)
	addCodegenProperties(true /* host */, hostArches)
}

// These properties are allowed to contain the same source file name in different architectures.
// They we will be deduplicated automatically.
type CodegenSourceArchProperties struct {
	Srcs []string
}

type CodegenCommonArchProperties struct {
	Cflags   []string
	Cppflags []string
}

type CodegenLibraryArchProperties struct {
	Static struct {
		Whole_static_libs []string
	}
	Shared struct {
		Shared_libs []string
	}
}

type codegenArchProperties struct {
	CodegenSourceArchProperties
	CodegenCommonArchProperties
	CodegenLibraryArchProperties
}

type codegenProperties struct {
	Codegen struct {
		Arm, Arm64, X86, X86_64 codegenArchProperties
	}
}

type codegenCustomizer struct {
	library           bool
	codegenProperties codegenProperties
}

func defaultDeviceCodegenArches(ctx android.LoadHookContext) []string {
	arches := make(map[string]bool)
	for _, a := range ctx.DeviceConfig().Arches() {
		s := a.ArchType.String()
		arches[s] = true
		if s == "arm64" {
			arches["arm"] = true
		} else if s == "x86_64" {
			arches["x86"] = true
		}
	}
	ret := make([]string, 0, len(arches))
	for a := range arches {
		ret = append(ret, a)
	}
	sort.Strings(ret)
	return ret
}

func installCodegenCustomizer(module android.Module, library bool) {
	c := &codegenProperties{}
	android.AddLoadHook(module, func(ctx android.LoadHookContext) { codegen(ctx, c, library) })
	module.AddProperties(c)
}
