// npm install chai mocha ts-node @types/chai @types/mocha --save-dev
import { expect } from "chai";
import "mocha";
import { Fraction } from "../src/Fraction";

describe("Fraction.reduce(fraction)",  () => {

  it("should reduce a fraction to its smallest denominator", () => {
    const fraction = new Fraction(5, 95);
    const reducedFraction = Fraction.reduce(fraction);

    expect(reducedFraction.num).to.equal(1, "Fraction numerator is not as small as it could be");
    expect(reducedFraction.denom).to.equal(19, "Fraction denominator is not as small as it could be");
  });

  it("should leave intact a fraction already reduced", () => {
    const fraction = new Fraction(2, 3);
    const reducedFraction = Fraction.reduce(fraction);

    expect(reducedFraction.num).to.equal(2, "Reduced fraction numerator should not have changed");
    expect(reducedFraction.denom).to.equal(3, "Reduced fraction denominator should not have changed");
  });

  it("should respect the minimal denominator limit provided", () => {
    const fraction = new Fraction(4, 12);
    const reducedFraction = Fraction.reduce(fraction, 6);

    expect(reducedFraction.num).to.equal(2, "Limited fraction reduction not respected");
    expect(reducedFraction.denom).to.equal(6, "Limited fraction reduction not respected");
  });

  it("should keep the fraction as is when the minimal denominator is bigger than the denominator", () => {
    const fraction = new Fraction(4, 12);
    const reducedFraction = Fraction.reduce(fraction, 24);

    expect(reducedFraction.num).to.equal(4, "Limited fraction reduction not respected");
    expect(reducedFraction.denom).to.equal(12, "Limited fraction reduction not respected");
  });




});
