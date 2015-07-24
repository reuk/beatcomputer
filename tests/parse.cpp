#include "parse_functions.h"

#include "gtest/gtest.h"

using namespace std;

TEST(parse_number, parse_number) {
    ASSERT_NO_THROW(parse_number("0"));
    ASSERT_NO_THROW(parse_number("1"));
    ASSERT_NO_THROW(parse_number("0x0"));
    ASSERT_NO_THROW(parse_number("0x1"));
    ASSERT_NO_THROW(parse_number("+1"));
    ASSERT_NO_THROW(parse_number("-1"));
    ASSERT_NO_THROW(parse_number("+0x1"));
    ASSERT_NO_THROW(parse_number("-0x1"));
    ASSERT_THROW(parse_number("xyz"), runtime_error);
    ASSERT_THROW(parse_number("0xfg"), runtime_error);
    ASSERT_THROW(parse_number("0x"), runtime_error);
    ASSERT_THROW(parse_number(""), runtime_error);
    ASSERT_THROW(parse_number("+-1"), runtime_error);
    ASSERT_THROW(parse_number("10a"), runtime_error);
}

TEST(parse_register, parse_register) {
    ASSERT_NO_THROW(parse_register("R0"));
    ASSERT_THROW(parse_register("xyz"), runtime_error);
    ASSERT_THROW(parse_register("r0"), runtime_error);
    ASSERT_THROW(parse_register("R-1"), runtime_error);
    ASSERT_THROW(parse_register("R32"), runtime_error);
    ASSERT_EQ(0, parse_register("R0"));
    ASSERT_EQ(1, parse_register("R1"));
}

TEST(parse_immediate, parse_immediate) {
    ASSERT_NO_THROW(parse_immediate("10"));
    ASSERT_NO_THROW(parse_immediate("0xabcd"));
    ASSERT_NO_THROW(parse_immediate("0123"));
    ASSERT_NO_THROW(parse_immediate("-40"));
    ASSERT_THROW(parse_immediate("xyz"), runtime_error);
    ASSERT_THROW(parse_immediate("0xefg"), runtime_error);
    ASSERT_THROW(parse_immediate("10abc"), runtime_error);
    ASSERT_EQ(0xabc, parse_immediate("0xabc"));
    ASSERT_EQ(0123, parse_immediate("0123"));
    ASSERT_EQ(56, parse_immediate("56"));
    ASSERT_EQ(-56, parse_immediate("-56"));
}

TEST(parse_address, parse_address) {
    ASSERT_NO_THROW(parse_address("10"));
    ASSERT_NO_THROW(parse_address("0xabcd"));
    ASSERT_NO_THROW(parse_address("0123"));
    ASSERT_THROW(parse_address("-40"), runtime_error);
    ASSERT_THROW(parse_address("xyz"), runtime_error);
    ASSERT_THROW(parse_address("0xefg"), runtime_error);
    ASSERT_THROW(parse_address("10abc"), runtime_error);
    ASSERT_EQ(0xabc, parse_address("0xabc"));
    ASSERT_EQ(0123, parse_address("0123"));
    ASSERT_EQ(56, parse_address("56"));
}
