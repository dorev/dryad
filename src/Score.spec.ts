// npm install chai mocha ts-node @types/chai @types/mocha --save-dev
import { expect } from "chai";
import "mocha";
import { Score } from "./Score";


describe("Score.addEvent(eventArray)",  () => {

  it("should add single event", () => {
    const score = new Score();
    expect(false).to.equal(true);
  });

  it("should add multiple events", () => {
    const score = new Score();
    expect(false).to.equal(true);
  });

});

describe("Score.findBeat(beat)",  () => {

  it("should find an identical beat", () => {
    const score = new Score();
    expect(false).to.equal(true);
  });

  it("should target index 0 if the score is empty", () => {
    const score = new Score();
    expect(false).to.equal(true);
  });

  it("should return the index of the element that would be next if the beat is absent", () => {
    const score = new Score();
    expect(false).to.equal(true);
  });

});
