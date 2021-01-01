#pragma once

#include "dryadutils.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename dryad_object_type>
class dryad_info : crtp_helper<dryad_object_type, dryad_info>
{
public:

    inline const char* get_name() const     { return _name; }
    inline const char* get_desc() const     { return _desc; }
    inline int get_id() const               { return _id; }

    inline void set_name(const char* name)  { _name = name; }
    inline void set_desc(const char* desc)  { _desc = desc; }

protected:

    dryad_info(const char* name = "", const char* desc = "")
        : _name(name)
        , _desc(desc)
        , _id(++id_source)
    {
        _created++;
        _alive++;
    }

    dryad_info(const dryad_info& other)
        : _name(other._name)
        , _desc(other._desc)
        , _id(++id_source)
    {
        _created++;
        _alive++;
    }

    ~dryad_info()
    {
        _alive--;
    }

private:

    inline static std::atomic_int32_t id_source = 0;

    const char* _name;
    const char* _desc;
    int _id;

    static std::atomic_int32_t _created;
    static std::atomic_int32_t _alive;
};

template <typename dryad_object_type>
std::atomic_int32_t dryad_info<dryad_object_type>::_created = 0;

template <typename dryad_object_type>
std::atomic_int32_t dryad_info<dryad_object_type>::_alive = 0;

}