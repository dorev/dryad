#include "types.h"
#include "constants.h"

namespace Dryad
{
    namespace Authoring
    {
        constexpr Byte DryadCode[5] = { 'D', 'R', 'Y', 'A', 'D'};
        constexpr Byte GraphCode[4] = { 'G', 'R', 'P', 'H' };
        constexpr Byte MotifCode[4] = { 'M', 'T', 'I', 'F' };
        constexpr Byte InterludeCode[4] = { 'M', 'O', 'M', 'T' };

        void Serialize();
        void Deserialize();
        void ExportJson();
        void ImportJson();

        void StartLiveLink();
        void StopLiveLink();
    }
}

using namespace Dryad;

int main(int argc, char* argv[])
{
    float freq = MidiToFreq(Notes[G][8]);
}