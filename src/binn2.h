#pragma once

#include <stdint.h>
#include <type_traits>

#include "binn.h"

namespace binn2 {

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

struct list_adder : public binn {
  operator binn* () { return this; }
#undef X
#define X(arg_t, arg_t2, suffix) \
bool operator()(arg_t value) { return binn_list_add_ ## suffix(this, value); }
DEF_LIST(X)
#undef X
  // bool
  bool operator()(bool value) { return binn_list_add_bool(this, value); }
  // null
  bool operator()(void) { return binn_list_add_null(this); }
  // blob
  bool operator()(void* value, int size) { return binn_list_add_blob(this, value, size); }
  // value
  bool operator()(binn* value) { return binn_list_add_value(this, value); }
  // typed array
  template <typename T>
  bool operator()(T* values, int cnt);
  template <typename T, int CNT>
  bool operator()(T (&values)[CNT]) { return add(values, CNT); }
  // enum
  template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
  bool operator()(T value) { return (*this)((int32_t)value); }
  // class
  template <typename T, typename std::enable_if<std::is_class<T>::value>::type* = nullptr>
  bool operator()(T& value);
};

struct list_getter : public binn {
  operator binn* () { return this; }
#undef X
#define X(arg_t, arg_t2, suffix) \
bool operator()(int pos, arg_t& value) { return binn_list_get_ ## suffix(this, pos, (arg_t2)&value); }
DEF_LIST(X)
#undef X
  // bool
  bool operator()(int pos, bool& value) {
    BOOL b;
    BOOL r = binn_list_get_bool(this, pos, &b);
    value = (r != FALSE);
    return r;
  }
  // blob
  bool operator()(int pos, void*& value, int& size) { return binn_list_get_blob(this, pos, &value, &size); }
  // value
  bool operator()(int pos, binn* value) { return binn_list_get_value(this, pos, value); }
  // typed array
  template <typename T>
  bool operator()(int pos, T* values, int cnt);
  template <typename T, int CNT>
  bool operator()(int pos, T (&values)[CNT]) { return get(pos, values, CNT); }
  // enum
  template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
  bool operator()(int pos, T& value) { return (*this)(pos, (int32_t&)value); }
  // class
  template <typename T, typename std::enable_if<std::is_class<T>::value>::type* = nullptr>
  bool operator()(int pos, T& value);
};

struct map_setter : public binn {
  operator binn* () { return this; }
#define X(arg_t, arg_t2, suffix) \
bool operator()(int id, arg_t value) { return binn_map_set_ ## suffix(this, id, value); }
DEF_LIST(X)
#undef X
  // bool
  bool operator()(int id, bool value) {
    return binn_map_set_bool(this, id, value);
  }
  // null
  bool operator()(int id) { return binn_map_set_null(this, id); }
  // blob
  bool operator()(int id, void* value, int size) { return binn_map_set_blob(this, id, value, size); }
  // value
  bool operator()(int id, binn* value) { return binn_map_set_value(this, id, value); }
  // typed array
  template <typename T>
  bool operator()(int id, T* values, int cnt);
  template <typename T, int CNT>
  bool operator()(int id, T (&values)[CNT]) { return set(id, values, CNT); }
  // enum
  template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
  bool operator()(int id, T value) { return (*this)(id, (int32_t)value); }
  // class
  template <typename T, typename std::enable_if<std::is_class<T>::value>::type* = nullptr>
  bool operator()(int id, T& value);
};

struct map_getter : public binn {
  operator binn* () { return this; }
#define X(arg_t, arg_t2, suffix) \
bool operator()(int id, arg_t& value) { return binn_map_get_ ## suffix(this, id, (arg_t2)&value); }
DEF_LIST(X)
#undef X
  // bool
  bool operator()(int id, bool& value) {
    BOOL b;
    BOOL r = binn_map_get_bool(this, id, &b);
    value = (r != FALSE);
    return r;
  }
  // blob
  bool operator()(int id, void*& value, int& size) { return binn_map_get_blob(this, id, &value, &size); }  
  // value
  bool operator()(int id, binn* value) { return binn_map_get_value(this, id, value); }
  // typed array
  template <typename T>
  bool operator()(int id, T* values, int cnt);
  template <typename T, int CNT>
  bool operator()(int id, T (&values)[CNT]) { return get(id, values, CNT); }
  // enum
  template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
  bool operator()(int id, T& value) { return (*this)(id, (int32_t&)value); }
  // class
  template <typename T, typename std::enable_if<std::is_class<T>::value>::type* = nullptr>
  bool operator()(int id, T& value);
};

struct object_setter : public binn {
  operator binn* () { return this; }
#define X(arg_t, arg_t2, suffix) \
bool operator()(char* key, arg_t value) { return binn_object_set_ ## suffix(this, key, value); }
DEF_LIST(X)
#undef X
  // bool
  bool operator()(char* key, bool value) {
    return binn_object_set_bool(this, key, value);
  }
  // null
  bool operator()(char* key) { return binn_object_set_null(this, key); }
  // blob
  bool operator()(char* key, void* value, int size) { return binn_object_set_blob(this, key, value, size); }
  // value
  bool operator()(char* key, binn* value) { return binn_object_set_value(this, key, value); }
  // typed array
  template <typename T>
  bool operator()(char* key, T* values, int cnt);
  template <typename T, int CNT>
  bool operator()(char* key, T (&values)[CNT]) { return (*this)(key, values, CNT); }

  // enum
  template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
  bool operator()(char* key, T value) { return (*this)(key, (int32_t)value); }
  // class
  template <typename T, typename std::enable_if<std::is_class<T>::value>::type* = nullptr>
  bool operator()(char* key, T& value);
};

struct object_getter : public binn {
  operator binn* () { return this; }
#define X(arg_t, arg_t2, suffix) \
bool operator()(char* key, arg_t& value) { return binn_object_get_ ## suffix(this, key, (arg_t2)&value); }
DEF_LIST(X)
#undef X
  // bool
  bool operator()(char* key, bool& value) {
    BOOL b;
    BOOL r = binn_object_get_bool(this, key, &b);
    value = (r != FALSE);
    return r;
  }
  // blob
  bool operator()(char* key, void*& value, int& size) { return binn_object_get_blob(this, key, &value, &size); }  
  // value
  bool operator()(char* key, binn* value) { return binn_object_get_value(this, key, value); }
  // typed array
  template <typename T>
  bool operator()(char* key, T* values, int cnt);
  template <typename T, int CNT>
  bool operator()(char* key, T (&values)[CNT]) { return (*this)(key, values, CNT); }

  // enum
  template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
  bool operator()(char* key, T& value) { return (*this)(key, (int32_t&)value); }
  // class
  template <typename T, typename std::enable_if<std::is_class<T>::value>::type* = nullptr>
  bool operator()(char* key, T& value);
};

#undef DEF_LIST

inline bool create_list(binn& b) { return binn_create_list(&b); }
inline bool create_map(binn& b) { return binn_create_map(&b); }
inline bool create_object(binn& b) { return binn_create_object(&b); }

template <typename T>
inline bool to_list(binn& b, T* values, int cnt) {
  create_list(b);
  list_adder& adder = (list_adder&)b;
  for (int i=0; i<cnt; ++i) if (!adder(values[i])) return false;
  return true;
}

template <typename T>
inline bool from_list(binn& b, T* values, int cnt) {
  list_getter& getter = (list_getter&)b;
  for (int i=0; i<cnt; ++i) if (!getter(1+i, values[i])) return false;
  return true;
}

template <typename T>
bool list_adder::operator()(T* values, int cnt) {
  binn b;
  return to_list(b, value, cnt) & (*this)(&b);
}
template <typename T, typename std::enable_if<std::is_class<T>::value>::type*>
bool list_adder::operator()(T& value) {
  binn b;
  return serialize<std::true_type>(b, value) && (*this)(&b);
}

template <typename T>
bool list_getter::operator()(int pos, T* values, int cnt) {
  binn b;
  create_list(b);
  return (*this)(pos, &b) && from_list(b, value, cnt);
}
template <typename T, typename std::enable_if<std::is_class<T>::value>::type*>
bool list_getter::operator()(int pos, T& value) {
  binn b;
  create_list(b);
  return (*this)(pos, &b) && serialize<std::false_type>(b, value);
}

template <typename T>
bool map_setter::operator()(int id, T* values, int cnt) {
  binn b;
  return to_list(b, value, cnt) && (*this)(id, &b);
}
template <typename T, typename std::enable_if<std::is_class<T>::value>::type*>
bool map_setter::operator()(int id, T& value) {
  binn b;
  return serialize<std::true_type>(b, value) && (*this)(id, &b);
}

template <typename T>
bool map_getter::operator()(int id, T* values, int cnt) {
  binn* b;
  return (*this)(id, b) && from_list(*b, value, cnt);
}
template <typename T, typename std::enable_if<std::is_class<T>::value>::type*>
bool map_getter::operator()(int id, T& value) {
  binn b;
  create_list(b);
  return (*this)(id, &b) && serialize<std::false_type>(b, value);
}

template <typename T>
bool object_setter::operator()(char* key, T* values, int cnt) {
  binn b;
  return to_list(b, values, cnt) & (*this)(key, &b);
}
template <typename T, typename std::enable_if<std::is_class<T>::value>::type*>
bool object_setter::operator()(char* key, T& value) {
  binn b;
  return serialize<std::true_type>(b, value) && (*this)(key, &b);
}

template <typename T>
bool object_getter::operator()(char* key, T* values, int cnt) {
  binn b;
  create_list(b);
  return (*this)(key, &b) && from_list(b, values, cnt);
}
template <typename T, typename std::enable_if<std::is_class<T>::value>::type*>
bool object_getter::operator()(char* key, T& value) {
  binn b;
  create_list(b);
  return (*this)(key, &b) && serialize<std::false_type>(b, value);
}

} // namespace binn2
