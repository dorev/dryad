import { Beat } from "./Beat";
import { EventData, MusicalEvent } from "./Events";
import { Instant } from "./Instant";

export class Score {

    public score: Instant[] = [];

    constructor() {
        //
    }

    public addEvent(eventData: EventData): void {

        if (this.score.length === 0) {
            this.score.push(new Instant(eventData.event, eventData.beat));
        }

        const searchResult: ScoreSearchResult = this.findBeat(eventData.beat);
        if (searchResult.found) {
            // add the event to that instant
            this.score[searchResult.index].events.push(eventData.event);
        } else {
            // insert instant with event in it
            this.score.splice(searchResult.index, 0, new Instant(eventData.event, eventData.beat));
        }
    }

    // Assumes that this.score is already sorted
    // Returns the index of identical beat
    // or the index of the beat which would be right after
    public findBeat(beat: Beat): ScoreSearchResult {
        let i = 0;
        for (; i < this.score.length; ++i) {

            const compare = Beat.compare(beat, this.score[i].beat);
            if (compare === 0) {
                return {found : true, index: i};
            } else if (compare === -1) { break; }
        }
        return {found : false, index: i};
    }
}

export interface ScoreSearchResult {
    found: boolean;
    index: number;
}

