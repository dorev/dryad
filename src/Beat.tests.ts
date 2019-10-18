import { expect } from "chai";
import "mocha";
import { Beat } from "./Beat";


describe("Beat add",  () => {

  it("should add beats correctly", () => {
    const beat = new Beat(1, 1, 4);
    expect(beat.beat).to.equal(1);
    expect(beat.num).to.equal(1);
    expect(beat.denom).to.equal(4);
  });

});
