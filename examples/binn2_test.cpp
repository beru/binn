
#include <stdio.h>

#include "../src/binn2.h"

using namespace binn2;

int main(int argc, char* argv[])
{
  manager mgr;
  object* o = mgr.new_object();
  o->set("hoge", 100);
  o->set("moge", 123.456);

  list* l = mgr.new_list();
  l->add(100.0);
  double d;
  bool r = l->get(1, d);

  double darr1[16];
  double darr2[32];
  for (size_t i=0; i<16; ++i) darr1[i] = i+1;
  for (size_t i=0; i<32; ++i) darr2[i] = i+2;

  o->set("darr1", &darr1[0], 16);
  o->set("darr2", darr2);

  int hoge;
  double moge;
  double darr1_2[16];
  double darr2_2[32];
  o->get("hoge", hoge);
  o->get("moge", moge);
  o->get("darr1", darr1_2, 16);
  o->get("darr2", darr2_2);
  return 0;
}