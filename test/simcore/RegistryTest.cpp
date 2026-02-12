#include <simcore/data/Registry.h>
#include <gtest/gtest.h>

#include <sstream>

using sim::data::Registry;

// --- Test POD structs ---

struct TestState {
  double x = 0.0;
  double y = 0.0;
  double velocity = 0.0;
};

struct NonPodStruct {
  std::vector<double> data;
};

// --- Tests ---

TEST(RegistryTest, PublishAndGetScalar) {
  Registry reg;
  double value = 3.14;
  reg.publish("pi", &value);

  EXPECT_TRUE(reg.has("pi"));
  EXPECT_FALSE(reg.has("nonexistent"));
  EXPECT_DOUBLE_EQ(reg.get<double>("pi"), 3.14);
}

TEST(RegistryTest, MutableAccess) {
  Registry reg;
  double value = 1.0;
  reg.publish("val", &value);

  reg.get<double>("val") = 42.0;
  EXPECT_DOUBLE_EQ(value, 42.0);
}

TEST(RegistryTest, WriteThrough) {
  Registry reg;
  TestState s{10.0, 20.0, 30.0};
  reg.publish("state", &s);
  reg.publish("state.x", &s.x);

  // Modify via registry field access
  reg.get<double>("state.x") = 99.0;
  EXPECT_DOUBLE_EQ(s.x, 99.0);

  // Modify via struct access and verify field reflects it
  reg.get<TestState>("state").velocity = 55.0;
  EXPECT_DOUBLE_EQ(s.velocity, 55.0);
}

TEST(RegistryTest, UntypedAccess) {
  Registry reg;
  int val = 7;
  reg.publish("count", &val);

  void* ptr = reg.get("count");
  EXPECT_EQ(ptr, &val);
  EXPECT_EQ(*static_cast<int*>(ptr), 7);
}

TEST(RegistryTest, CaptureAndRestore) {
  Registry reg;
  TestState s{1.0, 2.0, 3.0};
  reg.publish("s", &s);

  auto snapshot = reg.capture();
  EXPECT_FALSE(snapshot.empty());

  // Modify state
  s.x = 100.0;
  s.y = 200.0;
  s.velocity = 300.0;
  EXPECT_DOUBLE_EQ(s.x, 100.0);

  // Restore
  reg.restore(snapshot);
  EXPECT_DOUBLE_EQ(s.x, 1.0);
  EXPECT_DOUBLE_EQ(s.y, 2.0);
  EXPECT_DOUBLE_EQ(s.velocity, 3.0);
}

TEST(RegistryTest, NonPodExcludedFromSnapshot) {
  Registry reg;
  double pod_val = 5.0;
  NonPodStruct non_pod{};
  non_pod.data = {1.0, 2.0, 3.0};

  reg.publish("pod", &pod_val);
  reg.publish("non_pod", &non_pod);

  auto snapshot = reg.capture();

  // Change both
  pod_val = 99.0;
  non_pod.data = {9.0};

  reg.restore(snapshot);

  // POD restored, non-POD unchanged
  EXPECT_DOUBLE_EQ(pod_val, 5.0);
  ASSERT_EQ(non_pod.data.size(), 1u);
  EXPECT_DOUBLE_EQ(non_pod.data[0], 9.0);
}

TEST(RegistryTest, MultipleEntriesSnapshot) {
  Registry reg;
  double a = 1.0, b = 2.0, c = 3.0;
  reg.publish("a", &a);
  reg.publish("b", &b);
  reg.publish("c", &c);

  auto snap = reg.capture();
  a = 10.0;
  b = 20.0;
  c = 30.0;

  reg.restore(snap);
  EXPECT_DOUBLE_EQ(a, 1.0);
  EXPECT_DOUBLE_EQ(b, 2.0);
  EXPECT_DOUBLE_EQ(c, 3.0);
}

TEST(RegistryTest, StreamToJson) {
  Registry reg;
  double val = 42.0;
  int count = 7;
  reg.publish("val", &val);
  reg.publish("count", &count);

  std::ostringstream os;
  reg.streamTo(os);

  auto j = nlohmann::json::parse(os.str());
  EXPECT_DOUBLE_EQ(j["val"].get<double>(), 42.0);
  EXPECT_EQ(j["count"].get<int>(), 7);
}

TEST(RegistryTest, ThrowsOnMissingEntry) {
  Registry reg;
  EXPECT_THROW(reg.get<double>("missing"), std::invalid_argument);
  EXPECT_THROW(reg.get("missing"), std::invalid_argument);
}

#ifndef NDEBUG
TEST(RegistryTest, ThrowsOnTypeMismatch) {
  Registry reg;
  double val = 1.0;
  reg.publish("val", &val);
  EXPECT_THROW(reg.get<int>("val"), std::logic_error);
}
#endif

TEST(RegistryTest, OverwriteEntry) {
  Registry reg;
  double a = 1.0, b = 2.0;
  reg.publish("x", &a);
  EXPECT_DOUBLE_EQ(reg.get<double>("x"), 1.0);

  reg.publish("x", &b);
  EXPECT_DOUBLE_EQ(reg.get<double>("x"), 2.0);
}

TEST(RegistryTest, SizeAndClear) {
  Registry reg;
  EXPECT_EQ(reg.size(), 0u);

  double a = 1.0, b = 2.0;
  reg.publish("a", &a);
  reg.publish("b", &b);
  EXPECT_EQ(reg.size(), 2u);

  reg.clear();
  EXPECT_EQ(reg.size(), 0u);
  EXPECT_FALSE(reg.has("a"));
}

TEST(RegistryTest, EntriesIteration) {
  Registry reg;
  double a = 1.0;
  int b = 2;
  reg.publish("a", &a);
  reg.publish("b", &b);

  auto& entries = reg.entries();
  EXPECT_EQ(entries.size(), 2u);
  EXPECT_TRUE(entries.count("a"));
  EXPECT_TRUE(entries.count("b"));
}
