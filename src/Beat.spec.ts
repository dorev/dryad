// npm install chai mocha ts-node @types/chai @types/mocha --save-dev
import { expect } from "chai";
import "mocha";
import { Beat } from "./Beat";

describe("Beat.add(otherBeat)",  () => {

  it("should add beats and fractions", () => {
    const beat1 = new Beat(1, 1, 4);
    const beat2 = new Beat(2, 1, 4);
    const beat = beat1.add(beat2);

    expect(beat.beat).to.equal(3, "beat value is wrong");
    expect(beat.num).to.equal(2, "numerator value is wrong");
    expect(beat.denom).to.equal(4, "denominator value is wrong");
  });


  it("should add beats and fractions resulting in whole fraction", () => {
    const beat1 = new Beat(1, 3, 4);
    const beat2 = new Beat(2, 1, 4);
    const beat = beat1.add(beat2);

    expect(beat.beat).to.equal(4, "beat value is wrong");
    expect(beat.num).to.equal(null, "numerator value is wrong");
    expect(beat.denom).to.equal(null, "denominator value is wrong");
  });

  it("should addbeat with fraction and one without", () => {
    const beat1 = new Beat(1, 1, 4);
    const beat2 = new Beat(2);
    const beat = beat1.add(beat2);

    expect(beat.beat).to.equal(3, "beat value is wrong");
    expect(beat.num).to.equal(1, "numerator value is wrong");
    expect(beat.denom).to.equal(4, "denominator value is wrong");
  });

  it("should add two beats without fractions", () => {
    const beat1 = new Beat(1);
    const beat2 = new Beat(2);
    const beat = beat1.add(beat2);

    expect(beat.beat).to.equal(3, "beat value is wrong");
    expect(beat.num).to.equal(null, "numerator value is wrong");
    expect(beat.denom).to.equal(null, "denominator value is wrong");
  });

  it("should add two beats with overflowing fractions with same denominator", () => {
    const beat1 = new Beat(1, 3, 4);
    const beat2 = new Beat(2, 3, 4);
    const beat = beat1.add(beat2);

    expect(beat.beat).to.equal(4, "beat value is wrong");
    expect(beat.num).to.equal(2, "numerator value is wrong");
    expect(beat.denom).to.equal(4, "denominator value is wrong");
  });

  it("should add two beats with different denominators", () => {
    const beat1 = new Beat(1, 1, 5);
    const beat2 = new Beat(2, 1, 4);
    const beat = beat1.add(beat2);

    expect(beat.beat).to.equal(3, "beat value is wrong");
    expect(beat.num).to.equal(9, "numerator value is wrong");
    expect(beat.denom).to.equal(20, "denominator value is wrong");
  });

  it("should add two beats with overflowing fractions with different denominator", () => {
      const beat1 = new Beat(1, 3, 4);
      const beat2 = new Beat(2, 5, 6);
      const beat = beat1.add(beat2);

      expect(beat.beat).to.equal(4, "beat value is wrong");
      expect(beat.num).to.equal(7, "numerator value is wrong");
      expect(beat.denom).to.equal(12, "denominator value is wrong");
    });

});


describe("Beat.substract(otherBeat)",  () => {

  it("should substract beats and fractions", () => {
    const beat1 = new Beat(3, 2, 4);
    const beat2 = new Beat(2, 1, 4);
    const beat = beat1.substract(beat2);

    expect(beat.beat).to.equal(1, "beat value is wrong");
    expect(beat.num).to.equal(1, "numerator value is wrong");
    expect(beat.denom).to.equal(4, "denominator value is wrong");
  });

  it("should substract with fraction and one without", () => {
    const beat1 = new Beat(2, 1, 4);
    const beat2 = new Beat(1);
    const beat = beat1.substract(beat2);

    expect(beat.beat).to.equal(1, "beat value is wrong");
    expect(beat.num).to.equal(1, "numerator value is wrong");
    expect(beat.denom).to.equal(4, "denominator value is wrong");
  });

  it("should substract two beats without fractions", () => {
    const beat1 = new Beat(5);
    const beat2 = new Beat(2);
    const beat = beat1.substract(beat2);

    expect(beat.beat).to.equal(3, "beat value is wrong");
    expect(beat.num).to.equal(null, "numerator value is wrong");
    expect(beat.denom).to.equal(null, "denominator value is wrong");
  });

  it("should substract two beats with underflowing fractions with same denominator", () => {
    const beat1 = new Beat(5, 1, 4);
    const beat2 = new Beat(2, 2, 4);
    const beat = beat1.substract(beat2);

    expect(beat.beat).to.equal(2, "beat value is wrong");
    expect(beat.num).to.equal(3, "numerator value is wrong");
    expect(beat.denom).to.equal(4, "denominator value is wrong");
  });

  it("should substract two beats with different denominators", () => {
    const beat1 = new Beat(4, 1, 4);
    const beat2 = new Beat(1, 1, 5);
    const beat = beat1.substract(beat2);

    expect(beat.beat).to.equal(3, "beat value is wrong");
    expect(beat.num).to.equal(1, "numerator value is wrong");
    expect(beat.denom).to.equal(20, "denominator value is wrong");
  });

  it("should substract two beats with underflowing fractions with different denominator", () => {
    const beat1 = new Beat(5, 1, 4);
    const beat2 = new Beat(2, 5, 6);
    const beat = beat1.substract(beat2);

    expect(beat.beat).to.equal(2, "beat value is wrong");
    expect(beat.num).to.equal(5, "numerator value is wrong");
    expect(beat.denom).to.equal(12, "denominator value is wrong");
  });

});
