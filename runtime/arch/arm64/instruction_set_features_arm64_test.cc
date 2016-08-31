/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include "instruction_set_features_arm64.h"

#include <gtest/gtest.h>

namespace art {

TEST(Arm64InstructionSetFeaturesTest, Arm64Features) {
  // Build features for an ARM64 processor.
  std::string error_msg;
  std::unique_ptr<const InstructionSetFeatures> arm64_default_features(
      InstructionSetFeatures::FromVariant(kArm64, "default", &error_msg));
  ASSERT_TRUE(arm64_default_features.get() != nullptr) << error_msg;
  EXPECT_EQ(arm64_default_features->GetInstructionSet(), kArm64);
  EXPECT_TRUE(arm64_default_features->Equals(arm64_default_features.get()));
  EXPECT_FALSE(arm64_default_features->AsArm64InstructionSetFeatures()->HasCRC32Instruction());
  EXPECT_STREQ("a53,-crc", arm64_default_features->GetFeatureString().c_str());
  EXPECT_EQ(arm64_default_features->AsBitmap(), 1U);

  std::unique_ptr<const InstructionSetFeatures> cortex_a53_features(
      InstructionSetFeatures::FromVariant(kArm64, "cortex-a53", &error_msg));
  ASSERT_TRUE(cortex_a53_features.get() != nullptr) << error_msg;
  EXPECT_EQ(cortex_a53_features->GetInstructionSet(), kArm64);
  EXPECT_TRUE(cortex_a53_features->Equals(cortex_a53_features.get()));
  EXPECT_FALSE(cortex_a53_features->AsArm64InstructionSetFeatures()->HasCRC32Instruction());
  EXPECT_STREQ("a53,-crc", cortex_a53_features->GetFeatureString().c_str());
  EXPECT_EQ(cortex_a53_features->AsBitmap(), 1U);

  std::unique_ptr<const Arm64InstructionSetFeatures> cortex_a53_crc_features(
      Arm64InstructionSetFeatures::FromVariant("cortex-a53", "armv8.1-a", &error_msg));
  ASSERT_TRUE(cortex_a53_crc_features.get() != nullptr) << error_msg;
  EXPECT_EQ(cortex_a53_crc_features->GetInstructionSet(), kArm64);
  EXPECT_TRUE(cortex_a53_crc_features->Equals(cortex_a53_crc_features.get()));
  EXPECT_TRUE(cortex_a53_crc_features->AsArm64InstructionSetFeatures()->HasCRC32Instruction());
  EXPECT_STREQ("a53,crc", cortex_a53_crc_features->GetFeatureString().c_str());
  EXPECT_EQ(cortex_a53_crc_features->AsBitmap(), 3U);


  std::unique_ptr<const InstructionSetFeatures> cortex_a57_features(
      InstructionSetFeatures::FromVariant(kArm64, "cortex-a57", &error_msg));
  ASSERT_TRUE(cortex_a57_features.get() != nullptr) << error_msg;
  EXPECT_EQ(cortex_a57_features->GetInstructionSet(), kArm64);
  EXPECT_TRUE(cortex_a57_features->Equals(cortex_a57_features.get()));
  EXPECT_FALSE(cortex_a57_features->AsArm64InstructionSetFeatures()->HasCRC32Instruction());
  EXPECT_STREQ("a53,-crc", cortex_a57_features->GetFeatureString().c_str());
  EXPECT_EQ(cortex_a57_features->AsBitmap(), 1U);

  std::unique_ptr<const Arm64InstructionSetFeatures> cortex_a57_crc_features(
      Arm64InstructionSetFeatures::FromVariant("cortex-a57", "armv8.1-a", &error_msg));
  ASSERT_TRUE(cortex_a57_crc_features.get() != nullptr) << error_msg;
  EXPECT_EQ(cortex_a57_crc_features->GetInstructionSet(), kArm64);
  EXPECT_TRUE(cortex_a57_crc_features->Equals(cortex_a57_crc_features.get()));
  EXPECT_TRUE(cortex_a57_crc_features->AsArm64InstructionSetFeatures()->HasCRC32Instruction());
  EXPECT_STREQ("a53,crc", cortex_a57_crc_features->GetFeatureString().c_str());
  EXPECT_EQ(cortex_a57_crc_features->AsBitmap(), 3U);

  std::unique_ptr<const InstructionSetFeatures> cortex_a73_features(
      InstructionSetFeatures::FromVariant(kArm64, "cortex-a73", &error_msg));
  ASSERT_TRUE(cortex_a73_features.get() != nullptr) << error_msg;
  EXPECT_EQ(cortex_a73_features->GetInstructionSet(), kArm64);
  EXPECT_TRUE(cortex_a73_features->Equals(cortex_a73_features.get()));
  EXPECT_STREQ("a53,-crc", cortex_a73_features->GetFeatureString().c_str());
  EXPECT_EQ(cortex_a73_features->AsBitmap(), 1U);

  std::unique_ptr<const Arm64InstructionSetFeatures> cortex_a73_crc_features(
      Arm64InstructionSetFeatures::FromVariant("cortex-a73", "armv8.1-a", &error_msg));
  ASSERT_TRUE(cortex_a73_crc_features.get() != nullptr) << error_msg;
  EXPECT_EQ(cortex_a73_crc_features->GetInstructionSet(), kArm64);
  EXPECT_TRUE(cortex_a73_crc_features->Equals(cortex_a73_crc_features.get()));
  EXPECT_TRUE(cortex_a73_crc_features->AsArm64InstructionSetFeatures()->HasCRC32Instruction());
  EXPECT_STREQ("a53,crc", cortex_a73_crc_features->GetFeatureString().c_str());
  EXPECT_EQ(cortex_a73_crc_features->AsBitmap(), 3U);

  std::unique_ptr<const InstructionSetFeatures> cortex_a35_features(
      InstructionSetFeatures::FromVariant(kArm64, "cortex-a35", &error_msg));
  ASSERT_TRUE(cortex_a35_features.get() != nullptr) << error_msg;
  EXPECT_EQ(cortex_a35_features->GetInstructionSet(), kArm64);
  EXPECT_TRUE(cortex_a35_features->Equals(cortex_a35_features.get()));
  EXPECT_STREQ("-a53,-crc", cortex_a35_features->GetFeatureString().c_str());
  EXPECT_EQ(cortex_a35_features->AsBitmap(), 0U);

  std::unique_ptr<const Arm64InstructionSetFeatures> cortex_a35_crc_features(
      Arm64InstructionSetFeatures::FromVariant("cortex-a35", "armv8.1-a", &error_msg));
  ASSERT_TRUE(cortex_a35_crc_features.get() != nullptr) << error_msg;
  EXPECT_EQ(cortex_a35_crc_features->GetInstructionSet(), kArm64);
  EXPECT_TRUE(cortex_a35_crc_features->Equals(cortex_a35_crc_features.get()));
  EXPECT_TRUE(cortex_a35_crc_features->AsArm64InstructionSetFeatures()->HasCRC32Instruction());
  EXPECT_STREQ("a53,crc", cortex_a35_crc_features->GetFeatureString().c_str());
  EXPECT_EQ(cortex_a35_crc_features->AsBitmap(), 3U);

  std::unique_ptr<const InstructionSetFeatures> kryo_features(
      InstructionSetFeatures::FromVariant(kArm64, "kryo", &error_msg));
  ASSERT_TRUE(kryo_features.get() != nullptr) << error_msg;
  EXPECT_EQ(kryo_features->GetInstructionSet(), kArm64);
  EXPECT_TRUE(kryo_features->Equals(kryo_features.get()));
  EXPECT_TRUE(kryo_features->Equals(cortex_a35_features.get()));
  EXPECT_FALSE(kryo_features->Equals(cortex_a57_features.get()));
  EXPECT_STREQ("-a53,-crc", kryo_features->GetFeatureString().c_str());
  EXPECT_EQ(kryo_features->AsBitmap(), 0U);
}

}  // namespace art
