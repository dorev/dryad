export interface Fraction {
  num: number;
  denom: number;
}

export function reduceFraction(fraction: Fraction): Fraction {

  let divisor: number = fraction.denom;

  for (let i = 1; i <= fraction.denom; ++i) {
    if (fraction.num % i === 0 && fraction.denom % i === 0) {
        divisor = i;
    }
  }

  return { num: fraction.num / divisor, denom : fraction.denom / divisor};

}
