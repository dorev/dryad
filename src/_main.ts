import { Beat } from "./Beat";
import { MusicalEvent } from "./MusicalEvent";
import { Score } from "./Score";
import { ScorePosition } from "./ScorePosition";

const score = new Score();
score.addEvent({ event : new MusicalEvent(), beat : new Beat(0)});
