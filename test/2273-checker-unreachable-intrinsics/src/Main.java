/*
 * Copyright (C) 2024 The Android Open Source Project
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

public class Main {
    public static void main(String[] args) {
        assertEquals(false, $noinline$testIsEmpty("Hello"));
        assertEquals(true, $noinline$testIsEmpty(""));
    }

    private static boolean $noinline$testIsEmpty(String str) {
        return IsEmpty(str);
    }

    private static boolean IsEmpty(CharSequence chr) {
        return chr.isEmpty();
    }

    private static void assertEquals(boolean expected, boolean actual) {
        if (expected != actual) {
            throw new AssertionError("Wrong result: " + expected + " != " + actual);
        }
    }
}
