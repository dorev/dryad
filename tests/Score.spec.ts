// npm install chai mocha ts-node @types/chai @types/mocha --save-dev
import { expect } from "chai";
import "mocha";
import { Beat } from "../src/Beat";
import { MusicalEvent } from "../src/MusicalEvent";
import { Score, ScoreSearchResult } from "../src/Score";

describe("Score.addEvent(eventArray)",  () => {

  it("should add single event", () => {
    const score = new Score();
    expect(score.score.length).to.equal(0, "Score should not contain anything at construction.");
    score.addEvent({event : new MusicalEvent(), beat : new Beat(0)});
    expect(score.score.length).to.equal(1, "Score should cointain the added event.");
  });

  it("should add multiple events", () => {
    const score = new Score();
    expect(score.score.length).to.equal(0, "Score should not contain anything at construction.");
    score.addEvent({ event : new MusicalEvent(), beat : new Beat(0)});
    score.addEvent({ event : new MusicalEvent(), beat : new Beat(1)});
    score.addEvent({ event : new MusicalEvent(), beat : new Beat(2)});
    expect(score.score.length).to.equal(3, "Score should cointain 3 events.");
  });

});

describe("Score.findBeat(beat)",  () => {

  it("should find an identical beat", () => {
    const score = new Score();
    score.addEvent({event : new MusicalEvent(), beat : new Beat(1, 3, 4)});
    const result: ScoreSearchResult = score.findBeat(new Beat(1, 3, 4));
    expect(result.found).to.equal(true);
    expect(result.index).to.equal(0);
  });

  it("should target index 0 if the score is empty", () => {
    const score = new Score();
    const result: ScoreSearchResult = score.findBeat(new Beat(1, 3, 4));
    expect(result.found).to.equal(false);
    expect(result.index).to.equal(0);
  });

  it("should return the index of the element that would be next if the beat is absent", () => {
    const score = new Score();
    score.addEvent({event : new MusicalEvent(), beat : new Beat(1, 3, 4)});
    score.addEvent({event : new MusicalEvent(), beat : new Beat(2, 1, 4)});
    score.addEvent({event : new MusicalEvent(), beat : new Beat(3)});
    const result: ScoreSearchResult = score.findBeat(new Beat(2, 3, 4));
    expect(result.found).to.equal(false);
    expect(result.index).to.equal(2);
  });

});
