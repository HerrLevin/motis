#include "gtest/gtest.h"

#include "motis/parse_location.h"

using namespace motis;
using namespace date;
namespace n = nigiri;

using namespace std::chrono_literals;

TEST(motis, parse_location_with_level) {
  auto const parsed = parse_location("-123.1,44.2,-1.5");
  ASSERT_TRUE(parsed.has_value());
  EXPECT_EQ((osr::location{{-123.1, 44.2}, osr::to_level(-1.5F)}), *parsed);
}

TEST(motis, parse_location_no_level) {
  auto const parsed = parse_location("-23.1,45.2");
  ASSERT_TRUE(parsed.has_value());
  EXPECT_EQ((osr::location{{-23.1, 45.2}, osr::to_level(0.F)}), *parsed);
}

TEST(motis, parse_date_time) {
  auto const t = get_date_time("06-28-2024", "7:06 PM");
  EXPECT_EQ(sys_days{2024_y / June / 28} + 19h + 6min, t);
}

TEST(motis, parse_cursor_earlier) {
  auto const q = parse_cursor("EARLIER|1720036560");

  ASSERT_TRUE(
      std::holds_alternative<n::interval<n::unixtime_t>>(q.start_time_));

  auto const interval = std::get<n::interval<n::unixtime_t>>(q.start_time_);
  EXPECT_EQ(sys_days{2024_y / July / 3} + 17h + 56min, interval.from_);
  EXPECT_EQ(sys_days{2024_y / July / 3} + 19h + 56min, interval.to_);
}

TEST(motis, parse_cursor_later) {
  auto const q = parse_cursor("LATER|1720036560");

  ASSERT_TRUE(
      std::holds_alternative<n::interval<n::unixtime_t>>(q.start_time_));

  auto const interval = std::get<n::interval<n::unixtime_t>>(q.start_time_);
  EXPECT_EQ(sys_days{2024_y / July / 3} + 19h + 56min, interval.from_);
  EXPECT_EQ(sys_days{2024_y / July / 3} + 21h + 56min, interval.to_);
}