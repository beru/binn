
#include <stdio.h>
#include <limits>

#include "../src/binn2.h"

using namespace binn2;

struct ChildStruct {
  int8_t s1;
  int16_t s2;
  int32_t s4;
  int64_t s8;
  uint8_t u1;
  uint16_t u2;
  uint32_t u4;
  uint64_t u8;
};

struct TestStruct {
  int a;
  float f;
  double d;
  ChildStruct s;
};

template <typename T, typename std::enable_if<std::is_same<T, std::true_type>::value>::type* = nullptr>
object_setter& create_object_getter_setter(binn& s) {
  create_object(s);
  return (object_setter&)s;
}
template <typename T, typename std::enable_if<std::is_same<T, std::false_type>::value>::type* = nullptr>
object_getter& create_object_getter_setter(binn& s) {
  return (object_getter&)s;
}

template <typename B>
bool serialize(binn& b, ChildStruct& v) {
  auto& s = create_object_getter_setter<B>(b);
  return true
    && s("s1", v.s1)
    && s("s2", v.s2)
    && s("s4", v.s4)
    && s("s8", v.s8)
    && s("u1", v.u1)
    && s("u2", v.u2)
    && s("u4", v.u4)
    && s("u8", v.u8)
  ;
}

template <typename B>
bool serialize(binn& b, TestStruct& v) {
  auto& s = create_object_getter_setter<B>(b);
  return true
    && s("a", v.a)
    && s("f", v.f)
    && s("d", v.d)
    && s("s", v.s)
  ;
}

int main(int argc, char* argv[])
{
  enum TestEnum {
    TestEnum_a,
    TestEnum_b,
    TestEnum_c,
  };

  TestEnum e = TestEnum_c;
  TestEnum e2;
  TestStruct s;
  s.a = 54321;
  s.f = 1.3142f;
  s.d = 0.9876;
  auto& c = s.s;
  c.s1 = std::numeric_limits<int8_t>::min();
  c.s2 = std::numeric_limits<int16_t>::min();
  c.s4 = std::numeric_limits<int32_t>::min();
  c.s8 = std::numeric_limits<int64_t>::min();
  c.u1 = std::numeric_limits<uint8_t>::max();
  c.u2 = std::numeric_limits<uint16_t>::max();
  c.u4 = std::numeric_limits<uint32_t>::max();
  c.u8 = std::numeric_limits<uint64_t>::max();
  TestStruct s2;
  TestStruct s3 = {0};

  bool r;
  int hoge;
  double d;
  double moge;
  double darr1[16];
  double darr2[32];
  double darr1_2[16];
  double darr2_2[32];
  for (size_t i=0; i<16; ++i) darr1[i] = i+1;
  for (size_t i=0; i<32; ++i) darr2[i] = i+2;
  s2 = s3;

  // list
  binn l;
  create_list(l);
  {
    list_adder& adder = (list_adder&)l;
    list_getter& getter = (list_getter&)l;
    adder(100.0);
    adder(e);
    adder(s);
    r = getter(1, d);
    getter(2, e2);
    getter(3, s2);
  }

  // map
  binn m;
  create_map(m);
  s2 = s3;
  {
    map_setter& setter = (map_setter&)m;
    map_getter& getter = (map_getter&)m;
    e2 = TestEnum_b;
    setter(1234, e);
    setter(4444, s);
    getter(1234, e2);
    getter(4444, s2);
  }

  // object
  binn o;
  create_object(o);
  s2 = s3;
  {
    object_setter& setter = (object_setter&)o;
    object_getter& getter = (object_getter&)o;
    setter("hoge", 100);
    setter("moge", 123.456);
    setter("darr1", &darr1[0], 16);
    setter("darr2", darr2);
    setter("e", e);
    setter("struct", s);
    getter("hoge", hoge);
    getter("moge", moge);
    getter("darr1", darr1_2, 16);
    getter("darr2", darr2_2);
    getter("e", e2);
    getter("struct", s2);
  }

  return 0;
}