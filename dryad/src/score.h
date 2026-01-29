#pragma once

#include "graph.h"
#include "voice.h"
#include "chord.h"
#include "events.h"

namespace Dryad
{

    class MotifNote;
    class Scale;
    class Score;

    class ScoreFrame : public Node
    {
    public:
        DRYAD_NODE_CLASS_ID(ScoreFrame);

        ScoreFrame(Time position = 0);

        // Comparison helper to automate the ordering in a set
        struct CompareByPosition
        {
            bool operator()(const ScoreFrame* a, const ScoreFrame* b) const
            {
                return a->position < b->position;
            }
        };

        Score* getScore();
        NoteValue getCurrentRoot();
        Scale* getCurrentScale();
        Chord getCurrentChord();
        Error addMotifNote(MotifNote* motifNote);

        Time position;
        bool isCommitted;
        bool isEmitted;
    };

    class Progression;
    class ProgressionNode;
    class Scale;
    struct SerializedScore;

    class Score : public Graph
    {
    public:
        DRYAD_NODE_CLASS_ID(Score);

        Score();

        Voice* addVoice(int id, String name);
        Error addMotifInstance(Voice* voice, Motif* motif, Time position, MotifInstance*& instance);

        // Calling this will set in stone the notes within the specified duration,
        // effectively appending 'real' notes to the score.
        //
        // The steps of this process are:
        //
        // - Check that all motifs in all voices are generated at least until the end of
        //   the committed duration (include the duration appended by this call)
        //
        // - Each newly committed frame will then evaluate its notes based on the associated
        //   motif parameters and considering the scale and progression chord of the frame
        Error commitDuration(Time duration);
        Error tick(Time delta, Vector<ScoreEvent>& outEvents);

        Error dump(SerializedScore& serializedScore);
        ScoreFrame* getOrCreateFrame(Time position);
        ScoreFrame* findFrameAtPosition(Time position);
        ScoreFrame* findLastCommittedFrame();

        NoteValue currentRoot;
        Progression* currentProgression;
        Scale* currentScale;
        Set<Voice*, Voice::CompareByID> voices;
        Set<ScoreFrame*, ScoreFrame::CompareByPosition> frames;

        ProgressionNode* progressionCursor;
        Time remainingChordDuration;
    };

} // namespace Dryad
