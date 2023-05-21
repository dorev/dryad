#include "types.h"
#include "constants.h"

namespace Dryad
{
    namespace Authoring
    {
        constexpr Byte DryadCode[5] = { 'D', 'R', 'Y', 'A', 'D'};
        constexpr Byte GraphCode[5] = { 'G', 'R', 'A', 'P', 'H' };
        constexpr Byte MotifCode[5] = { 'M', 'O', 'T', 'I', 'F' };
        constexpr Byte ScaleCode[5] = { 'S', 'C', 'A', 'L', 'E' };

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
