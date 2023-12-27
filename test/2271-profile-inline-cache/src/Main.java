/*
 * Copyright (C) 2023 The Android Open Source Project
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

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.stream.Collectors;

public class Main {
    public static void main(String[] args) throws Exception {
        System.loadLibrary(args[0]);

        if (!hasJit()) {
            // Test requires JIT for creating profiling infos.
            return;
        }

        File file = null;
        try {
            file = createTempFile();
            String codePath = System.getenv("DEX_LOCATION") + "/2271-profile-inline-cache.jar";
            VMRuntime.registerAppInfo("test.app", file.getPath(), file.getPath(),
                    new String[] {codePath}, VMRuntime.CODE_PATH_TYPE_PRIMARY_APK);

            Derived1 derived1 = new Derived1();
            Derived2 derived2 = new Derived2();

            // This method is below the inline cache threshold.
            Method method1 = Main.class.getDeclaredMethod("$noinline$method1", Base.class);
            ensureJitBaselineCompiled(method1);
            $noinline$method1(derived1);
            for (int i = 0; i < 2998; i++) {
                $noinline$method1(derived2);
            }
            checkMethodHasNoInlineCache(file, method1);

            // This method is right on the inline cache threshold.
            Method method2 = Main.class.getDeclaredMethod("$noinline$method2", Base.class);
            ensureJitBaselineCompiled(method2);
            $noinline$method2(derived1);
            for (int i = 0; i < 2999; i++) {
                $noinline$method2(derived2);
            }
            checkMethodHasInlineCache(file, method2, Derived1.class, Derived2.class);

            // This method is above the inline cache threshold.
            Method method3 = Main.class.getDeclaredMethod("$noinline$method3", Base.class);
            ensureJitBaselineCompiled(method3);
            for (int i = 0; i < 10000; i++) {
                $noinline$method3(derived1);
            }
            for (int i = 0; i < 10000; i++) {
                $noinline$method3(derived2);
            }
            checkMethodHasInlineCache(file, method3, Derived1.class, Derived2.class);

            // This method is above the JIT threshold.
            Method method4 = Main.class.getDeclaredMethod("$noinline$method4", Base.class);
            ensureJitBaselineCompiled(method4);
            $noinline$method4(derived1);
            $noinline$method4(derived2);
            ensureMethodJitCompiled(method4);
            checkMethodHasInlineCache(file, method4, Derived1.class, Derived2.class);
        } finally {
            if (file != null) {
                file.delete();
            }
        }
    }

    public static void $noinline$method1(Base obj) {
        obj.f();
    }

    public static void $noinline$method2(Base obj) {
        obj.f();
    }

    public static void $noinline$method3(Base obj) {
        obj.f();
    }

    public static void $noinline$method4(Base obj) {
        obj.f();
    }

    public static class Base {
        public void f() {}
    }

    public static class Derived1 extends Base {
        @Override
        public void f() {}
    }

    public static class Derived2 extends Base {
        @Override
        public void f() {}
    }

    private static void checkMethodHasInlineCache(File file, Method m, Class<?>... targetTypes) {
        ensureProfileProcessing();
        if (!hasInlineCacheInProfile(file.getPath(), m, targetTypes)) {
            throw new RuntimeException("Expected method " + m
                    + " to have inline cache in the profile with target types "
                    + Arrays.stream(targetTypes)
                              .map(Class::getName)
                              .collect(Collectors.joining(", ")));
        }
    }

    private static void checkMethodHasNoInlineCache(File file, Method m) {
        ensureProfileProcessing();
        if (hasInlineCacheInProfile(file.getPath(), m)) {
            throw new RuntimeException(
                    "Expected method " + m + " not to have inline cache in the profile");
        }
    }

    public static void ensureJitBaselineCompiled(Method method) {
        ensureJitBaselineCompiled(method.getDeclaringClass(), method.getName());
    }
    public static native void ensureMethodJitCompiled(Method method);
    public static native void ensureJitBaselineCompiled(Class<?> cls, String methodName);
    public static native void ensureProfileProcessing();
    public static native boolean hasInlineCacheInProfile(
            String profile, Method method, Class<?>... targetTypes);
    public static native boolean hasJit();

    private static final String TEMP_FILE_NAME_PREFIX = "temp";
    private static final String TEMP_FILE_NAME_SUFFIX = "-file";

    private static File createTempFile() throws Exception {
        try {
            return File.createTempFile(TEMP_FILE_NAME_PREFIX, TEMP_FILE_NAME_SUFFIX);
        } catch (IOException e) {
            System.setProperty("java.io.tmpdir", "/data/local/tmp");
            try {
                return File.createTempFile(TEMP_FILE_NAME_PREFIX, TEMP_FILE_NAME_SUFFIX);
            } catch (IOException e2) {
                System.setProperty("java.io.tmpdir", "/sdcard");
                return File.createTempFile(TEMP_FILE_NAME_PREFIX, TEMP_FILE_NAME_SUFFIX);
            }
        }
    }

    private static class VMRuntime {
        public static final int CODE_PATH_TYPE_PRIMARY_APK = 1 << 0;
        private static final Method registerAppInfoMethod;

        static {
            try {
                Class<? extends Object> c = Class.forName("dalvik.system.VMRuntime");
                registerAppInfoMethod = c.getDeclaredMethod("registerAppInfo", String.class,
                        String.class, String.class, String[].class, int.class);
            } catch (Exception e) {
                throw new RuntimeException(e);
            }
        }

        public static void registerAppInfo(String packageName, String curProfile, String refProfile,
                String[] codePaths, int codePathsType) throws Exception {
            registerAppInfoMethod.invoke(
                    null, packageName, curProfile, refProfile, codePaths, codePathsType);
        }
    }
}
