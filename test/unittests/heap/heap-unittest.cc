// Copyright 2014 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <cmath>
#include <iostream>
#include <limits>

#include "src/objects.h"
#include "src/objects-inl.h"

#include "src/handles.h"
#include "src/handles-inl.h"

#include "src/heap/heap.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace v8 {
namespace internal {

double Round(double x) {
  // Round to three digits.
  return floor(x * 1000 + 0.5) / 1000;
}


void CheckEqualRounded(double expected, double actual) {
  expected = Round(expected);
  actual = Round(actual);
  EXPECT_DOUBLE_EQ(expected, actual);
}


TEST(Heap, HeapGrowingFactor) {
  CheckEqualRounded(Heap::kMaxHeapGrowingFactor,
                    Heap::HeapGrowingFactor(34, 1, 4.0));
  CheckEqualRounded(3.553, Heap::HeapGrowingFactor(45, 1, 4.0));
  CheckEqualRounded(2.830, Heap::HeapGrowingFactor(50, 1, 4.0));
  CheckEqualRounded(1.478, Heap::HeapGrowingFactor(100, 1, 4.0));
  CheckEqualRounded(1.193, Heap::HeapGrowingFactor(200, 1, 4.0));
  CheckEqualRounded(1.121, Heap::HeapGrowingFactor(300, 1, 4.0));
  CheckEqualRounded(Heap::HeapGrowingFactor(300, 1, 4.0),
                    Heap::HeapGrowingFactor(600, 2, 4.0));
  CheckEqualRounded(Heap::kMinHeapGrowingFactor,
                    Heap::HeapGrowingFactor(400, 1, 4.0));
}

TEST(Heap, MaxHeapGrowingFactor) {
  CheckEqualRounded(
      1.3, Heap::MaxHeapGrowingFactor(Heap::kMinOldGenerationSize * MB));
  CheckEqualRounded(
      1.600, Heap::MaxHeapGrowingFactor(Heap::kMaxOldGenerationSize / 2 * MB));
  CheckEqualRounded(
      1.999,
      Heap::MaxHeapGrowingFactor(
          (Heap::kMaxOldGenerationSize - Heap::kPointerMultiplier) * MB));
  CheckEqualRounded(4.0,
                    Heap::MaxHeapGrowingFactor(
                        static_cast<size_t>(Heap::kMaxOldGenerationSize) * MB));
}

TEST(Heap, SemiSpaceSize) {
  uint64_t configurations[][2] = {
      {0, i::Heap::kPointerMultiplier * (Page::kPageSize / KB)},
      {512 * i::MB, i::Heap::kPointerMultiplier * (Page::kPageSize / KB)},
      {1 * i::GB, i::Heap::kPointerMultiplier * (6 * Page::kPageSize / KB)},
      {2 * static_cast<uint64_t>(i::GB), i::Heap::kMaxSemiSpaceSizeInKB},
      {4 * static_cast<uint64_t>(i::GB), i::Heap::kMaxSemiSpaceSizeInKB},
      {8 * static_cast<uint64_t>(i::GB), i::Heap::kMaxSemiSpaceSizeInKB}};

  for (auto configuration : configurations) {
    ASSERT_EQ(configuration[1],
              static_cast<uint64_t>(
                  i::Heap::ComputeMaxSemiSpaceSize(configuration[0])));
  }
}

TEST(Heap, OldGenerationSize) {
  uint64_t configurations[][2] = {
      {0, i::Heap::kMinOldGenerationSize},
      {512, i::Heap::kMinOldGenerationSize},
      {1 * i::GB, 256 * i::Heap::kPointerMultiplier},
      {2 * static_cast<uint64_t>(i::GB), 512 * i::Heap::kPointerMultiplier},
      {4 * static_cast<uint64_t>(i::GB), i::Heap::kMaxOldGenerationSize},
      {8 * static_cast<uint64_t>(i::GB), i::Heap::kMaxOldGenerationSize}};

  for (auto configuration : configurations) {
    ASSERT_EQ(configuration[1],
              static_cast<uint64_t>(
                  i::Heap::ComputeMaxOldGenerationSize(configuration[0])));
  }
}

}  // namespace internal
}  // namespace v8
