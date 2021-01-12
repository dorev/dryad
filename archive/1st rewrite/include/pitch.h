#include "includes.h"
#include "dryadutils.h"

namespace dryad
{

class pitch_t
{
public:

    pitch_t(int pitch, accidental accidental_type = accidental::sharp);

    inline const char* get_step() const { return _step; }
    inline int get_alter() const { return _alter; }
    inline int get_octave() const { return _octave; }

private:

    const char* _step;
    int _alter;
    int _octave;

};

}