export class Fraction {

  public static reduce(fraction: Fraction, minimumDenom: number = 1): Fraction {

    let divisor: number = fraction.denom;
    minimumDenom = minimumDenom > fraction.denom
                 ? fraction.denom
                 : minimumDenom;

    for (let i = 1; i < fraction.denom; ++i) {

      if (fraction.denom / i < minimumDenom) {
        continue;
      }

      if (fraction.num % i === 0 && fraction.denom % i === 0) {
          divisor = i;
      }
    }
    return new Fraction(fraction.num / divisor, fraction.denom / divisor);
  }

  public num: number;
  public denom: number;

  constructor( numerator: number, denominator: number) {
    this.num = numerator;
    this.denom = denominator;
  }

}
