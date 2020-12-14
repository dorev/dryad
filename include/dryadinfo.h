#pragma once

#include "dryadutils.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class id_provider
{
public:

    static uint64_t new_id() { return static_cast<uint64_t>(_id++); }

private:

    inline static std::atomic_uint64_t _id = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename dryad_object_type>
class dryad_info : crtp_helper<dryad_object_type, dryad_info>
{
public:

    inline const char* get_name() const     { return _name; }
    inline const char* get_desc() const     { return _desc; }
    inline uint64_t get_id() const          { return _id; }

    inline void set_name(const char* name)  { _name = name; }
    inline void set_desc(const char* desc)  { _desc = desc; }

protected:

    dryad_info(const char* name = "", const char* desc = "")
        : _name(name)
        , _desc(desc)
        , _id(id_provider::new_id())
    {
        _created++;
        _alive++;
    }

    dryad_info(const dryad_info& other)
        : _name(other._name)
        , _desc(other._desc)
        , _id(id_provider::new_id())
    {
        _created++;
        _alive++;
    }

    ~dryad_info()
    {
        _alive--;
    }

private:

    const char* _name;
    const char* _desc;
    uint64_t _id;

    static std::atomic_uint64_t _created;
    static std::atomic_uint64_t _alive;
};

template <typename dryad_object_type>
std::atomic_uint64_t dryad_info<dryad_object_type>::_created = 0;

template <typename dryad_object_type>
std::atomic_uint64_t dryad_info<dryad_object_type>::_alive = 0;

}