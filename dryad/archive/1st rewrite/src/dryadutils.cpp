#include "dryadutils.h"

namespace dryad
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void uppercase(std::string& s)
{
    for (char& c : s)
    {
        c = static_cast<char>(std::toupper(c));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int step_down_duration(int duration, const std::vector<int>& duration_vector)
{
    int i = 0;

    for (i; i < duration_vector.size(); ++i)
    {
        if (duration == duration_vector[i])
        {
            break;
        }
    }

    /*
    if (i >= duration_vector.size())
    {
        CRASH("Illegal duration");
    }

    return duration_vector[i > 0 ? --i : 0];
    */

    int duration_index = i > 0
        ? --i 
        : 0;

    return duration_vector[duration_index];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int step_up_duration(int duration, const std::vector<int>& duration_vector)
{
    int i = 0;

    for (i; i < duration_vector.size(); ++i)
    {
        if (duration == duration_vector[i])
        {
            break;
        }
    }

    /*
    if (i >= duration_vector.size())
    {
        CRASH("Illegal duration");
    }
    
    return duration_vector[i < duration_vector.size() - 1 ? ++i : duration_vector.size() - 1];
    */
    
    int duration_index = i < duration_vector.size() - 1 
        ? ++i 
        : static_cast<int>(duration_vector.size() - 1);

    return duration_vector[duration_index];

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void integer_to_step_and_alteration(int integer, const char*& output_step, int& output_alteration, accidental accidental_type)
{
    static int notes[7] = {0, 2, 4, 5, 7, 9, 11 };
    int noteValue = integer % 12;

    switch (noteValue)
    {
        case 0: output_step = "C"; return;
        case 2: output_step = "D"; return;
        case 4: output_step = "E"; return;
        case 5: output_step = "F"; return;
        case 7: output_step = "G"; return;
        case 9: output_step = "A"; return;
        case 11: output_step = "B"; return;
        default:
            switch (accidental_type)
            {
                default:
                case accidental::sharp:
                case accidental::double_sharp:
                    switch (noteValue)
                    {
                        case 1: output_step =  "C"; break;
                        case 3: output_step = "D"; break;
                        case 6: output_step = "F"; break;
                        case 8: output_step = "G"; break;
                        case 10: output_step = "A"; break;
                        default: CRASH("Invalid sharp note value");
                    }

                    output_alteration = 1;
                    return;

                case accidental::flat:
                case accidental::double_flat:
                    switch (noteValue)
                    {
                        case 1: output_step = "D"; break;
                        case 3: output_step = "E"; break;
                        case 6: output_step = "G"; break;
                        case 8: output_step = "A"; break;
                        case 10: output_step = "B"; break;
                        default: CRASH("Invalid flat note value");
                    }

                    output_alteration = -1;
                    return;
            }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* duration_to_note_type(int duration)
{
    switch (duration)
    {
    default:
    case WHOLE:
        return "whole";
    case HALF_DOTTED:
    case HALF:
    case HALF_TRIPLET:
        return "half";
    case QUARTER_DOTTED:
    case QUARTER:
    case QUARTER_TRIPLET:
        return "quarter";
    case EIGHTH_DOTTED:
    case EIGHTH:
    case EIGHTH_TRIPLET:
        return "eighth";
    case SIXTEENTH:
    case SIXTEENTH_TRIPLET:
        return "16th";
    case THIRTYSECOND:
    case THIRTYSECOND_TRIPLET:
        return "32nd";
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

dryad_timer::dryad_timer()
    : _start(std::chrono::steady_clock::now())
{}

void dryad_timer::reset()
{
    _start = std::chrono::steady_clock::now();
}

std::string dryad_timer::stop()
{
    std::chrono::duration<double> diff = std::chrono::steady_clock::now() - _start;

    std::stringstream ss;
    ss << std::fixed << std::setprecision(6) << diff.count() * 1000 << " ms";

    return ss.str();
}

double dryad_timer::stop_ms()
{
    std::chrono::duration<double> diff = std::chrono::steady_clock::now() - _start;

    return diff.count() * 1000;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}