
#include <stdio.h>

#include "../src/binn2.h"

using namespace binn2;

int main(int argc, char* argv[])
{
  manager mgr;
  object* o = mgr.new_object();
  o->set("hoge", 100);
  o->set("moge", 123.456);

  int hoge;
  double moge;
  o->get("hoge", hoge);
  o->get("moge", moge);
  return 0;
}