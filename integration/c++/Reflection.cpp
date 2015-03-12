#include <Emscripten/LiveLog/Reflection.h>

using namespace Emscripten::LiveLog;

std::map<std::size_t, ReflObject*> ReflObject::_singleValueCache;

void ReflObject::addOtherReflection(const char* name, int offset, ReflObject* refl) {
    GroupMeta meta;
    meta.name = name;
    meta.obj = refl;
    meta.offset = offset;
    Groups.push_back(meta);
}

std::size_t ReflObject::sizeOf() {
    std::size_t size = 0;
    for(auto iter = Meta.begin(); iter != Meta.end(); iter++) {
        size += iter->size;
    }
    for(auto giter = Groups.begin(); giter != Groups.end(); giter++) {
        size += giter->obj->sizeOf();
    }
    return size;
}