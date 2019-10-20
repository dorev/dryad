import { Beat } from "./Beat";
import { MusicalEvent } from "./Events";
import { Instant } from "./Instant";

export class Score {

    private score: Instant[];

    constructor() {
        //
    }

    public addEvent(...events: EventData[]): void {
        events.forEach((eventData) => {
            const searchResult: ScoreSearchResult = this.findBeat(eventData.beat);
            if (searchResult.found) {
                // add the event to that instant
                this.score[searchResult.index].events.push(eventData.event);
            } else {
                // insert instant with event in it
                this.score.splice(searchResult.index, 0, new Instant(eventData.event));
            }
        });
    }


    // Assumes that this.score is already sorted
    // Returns the index of identical beat
    // or the index of the beat which would be right after
    private findBeat(beat: Beat): ScoreSearchResult {
        let i = 0;
        for (; i < this.score.length; ++i) {
            const compare = Beat.compare(beat, this.score[i].offset);
            if (compare === 0) {
                return {found : true, index: i};
            } else if (compare === -1) {break; }
        }
        return {found : false, index: i};
    }


}

interface ScoreSearchResult {
    found: boolean;
    index: number;
}

interface EventData {
    event: MusicalEvent;
    beat: Beat;
}
