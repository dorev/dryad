import { Fraction, reduceFraction } from "./Utils";


export class Note {
  public readonly value: number;
  public readonly duration: Beat;

  constructor(value: number, duration: Beat) {
    this.value = value;
    this.duration = duration;
  }
}


// tslint:disable-next-line:
export class Beat {

  public readonly beat: number = 0;
  public readonly num: number = 0;
  public readonly denom: number = 0;

  constructor(beat: number, num: number = 0, denom: number = 0) {
    this.beat = beat;
    this.num = num;
    this.denom = denom;
  }

  public add(other: Beat): Beat {
    if (this.denom === other.denom) {

      const beat = this.beat + other.beat + Math.floor((this.num + other.num) / this.denom);
      const num = Math.floor((this.num + other.num) / this.denom)
                  ? (this.num + other.num) % this.denom
                  : this.num + other.num;
      return new Beat(beat, num, this.denom);

    } else {

      const beatFraction: Fraction = reduceFraction(
        {
          denom : this.denom * other.denom,
          num   : this.num * other.denom + other.num * this.denom,
        });
      const beat = this.beat + other.beat + Math.floor(beatFraction.num / this.denom);
      const num = Math.floor(beatFraction.num / this.denom) ? beatFraction.num % this.denom : beatFraction.num;
      return new Beat(beat, num, beatFraction.denom);

    }
  }
}
