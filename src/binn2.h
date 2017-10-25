#pragma once

#include <stdint.h>

#include "binn.h"

namespace binn2 {

struct list;
struct map;
struct object;

#undef DEF_LIST
#define DEF_LIST(X) \
  X(int8_t, int8_t*, int8) \
  X(int16_t, int16_t*, int16) \
  X(int32_t, int32_t*, int32) \
  X(int64_t, int64_t*, int64) \
  X(uint8_t, uint8_t*, uint8) \
  X(uint16_t, uint16_t*, uint16) \
  X(uint32_t, uint32_t*, uint32) \
  X(uint64_t, uint64_t*, uint64) \
  X(float, float*, float) \
  X(double, double*, double) \
  X(char*, char**, str) \
  X(list*, void**, list) \
  X(map*, void**, map) \
  X(object*, void**, object) \


struct list : public binn {
#undef X
#define X(arg_t, arg_t2, suffix) \
bool add(arg_t value) { return binn_list_add_ ## suffix(this, value); } \
bool get(int pos, arg_t& value) { return binn_list_get_ ## suffix(this, pos, (arg_t2)&value); }
DEF_LIST(X)
#undef X
  // bool
  bool add(bool value) {
    return binn_list_add_bool(this, value);
  }
  bool get(int pos, bool& value) {
    BOOL b;
    BOOL r = binn_list_get_bool(this, pos, &b);
    value = (r != FALSE);
    return r;
  }
  // null
  bool add(void) { return binn_list_add_null(this); }
  // blob
  bool add(void* value, int size) { return binn_list_add_blob(this, value, size); }
  bool get(int pos, void*& value, int& size) { return binn_list_get_blob(this, pos, &value, &size); }
};

struct map : public binn {
#define X(arg_t, arg_t2, suffix) \
bool set(int id, arg_t value) { return binn_map_set_ ## suffix(this, id, value); } \
bool get(int id, arg_t& value) { return binn_map_get_ ## suffix(this, id, (arg_t2)&value); }
DEF_LIST(X)
#undef X
  // bool
  bool set(int id, bool value) {
    return binn_map_set_bool(this, id, value);
  }
  bool get(int id, bool& value) {
    BOOL b;
    BOOL r = binn_map_get_bool(this, id, &b);
    value = (r != FALSE);
    return r;
  }
  // null
  bool set(int id) { return binn_map_set_null(this, id); }
  // blob
  bool set(int id, void* value, int size) { return binn_map_set_blob(this, id, value, size); }
  bool get(int id, void*& value, int& size) { return binn_map_get_blob(this, id, &value, &size); }  
};

struct object : public binn {
#define X(arg_t, arg_t2, suffix) \
bool set(char* key, arg_t value) { return binn_object_set_ ## suffix(this, key, value); } \
bool get(char* key, arg_t& value) { return binn_object_get_ ## suffix(this, key, (arg_t2)&value); }
DEF_LIST(X)
#undef X
  // bool
  bool set(char* key, bool value) {
    return binn_object_set_bool(this, key, value);
  }
  bool get(char* key, bool& value) {
    BOOL b;
    BOOL r = binn_object_get_bool(this, key, &b);
    value = (r != FALSE);
    return r;
  }
  // null
  bool set(char* key) { return binn_object_set_null(this, key); }
  // blob
  bool set(char* key, void* value, int size) { return binn_object_set_blob(this, key, value, size); }
  bool get(char* key, void*& value, int& size) { return binn_object_get_blob(this, key, &value, &size); }  
};

#undef DEF_LIST

class manager {
public:
  manager()
    :
    cnt(0)
  {
  }
  
  ~manager() {
    for (size_t i=0; i<cnt; ++i) {
      binn_free(records[cnt - 1 - i]);
    }
    cnt = 0;
  }

  list* new_list() {
    binn* r = binn_list();
    rec(r);
    return (list*)r;
  }
  
  map* new_map() {
    binn* r = binn_map();
    rec(r);
    return (map*)r;
  }

  object* new_object() {
    binn* r = binn_object();
    rec(r);
    return (object*)r;
  }
  
private:
  void rec(binn* r) {
    if (r) records[cnt++] = r;
  }
  
  size_t cnt;
  binn* records[64];
};

} // namespace binn2
