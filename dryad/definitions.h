
#define INLINE_GET_SET(type, name, member, param) \
inline void Set##name(type value) { member = value; } \
inline type Get##name() const { return member; }