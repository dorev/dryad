import { Beat } from "./Beat";
import { EventData, MusicalEvent } from "./Events";
import { InstantAddress } from "./Instant";
import { Note } from "./Note";
import { Score } from "./Score";

const score = new Score();
score.addEvent({ event : new MusicalEvent(), beat : new Beat(0)});
