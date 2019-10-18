import { Fraction, reduceFraction } from "./Utils";

export class Beat {

  public beat: number = 0;
  public num: number = null;
  public denom: number = null;

  constructor(beat: number, num: number = null, denom: number = null) {

    if (beat < 0 || beat === null || beat === undefined) {
      throw new Error("Beat value must be a positive integer");
    }

    this.beat = beat;

    if (!num || num === denom) {
      this.num = null;
      this.denom = null;
      return;
    } else if (!denom) {
      throw new Error("Beat fraction denominator cannot be null with non-null numerator");
    }

    this.num = num;
    this.denom = denom;
  }

  public add(other: Beat): Beat {

    let beat: number = 0;
    let num: number = 0;

    if (other.denom === null && this.denom === null) {
      return new Beat(this.beat + other.beat);
    }

    // Match denom if one is a round beat
    if (!this.denom) {
      this.denom = other.denom;
    } else if (!other.denom) {
      other.denom = this.denom;
    }

    if (this.denom === other.denom) {

      beat = this.beat + other.beat + Math.floor((this.num + other.num) / this.denom);
      num = ((this.num + other.num) / this.denom) > 1
            ? (this.num + other.num) % this.denom
            : this.num + other.num;

      return new Beat(beat, num, this.denom);

    } else {

      const beatFraction: Fraction = reduceFraction({
          denom : this.denom === other.denom ? this.denom : this.denom * other.denom,
          num   : this.denom === other.denom ? this.num + other.num : this.num * other.denom + other.num * this.denom,
      });

      beat = this.beat + other.beat + Math.floor(beatFraction.num / beatFraction.denom);
      num = (beatFraction.num / beatFraction.denom) > 1
            ? beatFraction.num % beatFraction.denom
            : beatFraction.num;

      return new Beat(beat, num, beatFraction.denom);
    }
  }

  public substract(other: Beat): Beat {

    let beat: number = 0;
    let num: number = 0;

    if (other.denom === null && this.denom === null) {
      return new Beat(this.beat - other.beat);
    }

    // Match denom if one is a round beat
    if (!this.denom) {
      this.denom = other.denom;
    } else if (!other.denom) {
      other.denom = this.denom;
    }

    if (this.denom === other.denom) {

      beat = this.beat - other.beat + Math.floor((this.num - other.num) / this.denom);

      num = ((this.num - other.num) / this.denom) < 0
            ? (this.num - other.num) % this.denom
            : this.num - other.num;

      return new Beat(beat, num, this.denom);

    } else {

      const beatFraction: Fraction = reduceFraction({
          denom : this.denom === other.denom ? this.denom : this.denom * other.denom,
          num   : this.denom === other.denom ? this.num - other.num : this.num * other.denom - other.num * this.denom,
      });

      beat = this.beat - other.beat - Math.floor(beatFraction.num / beatFraction.denom);
      num = Math.floor(beatFraction.num / beatFraction.denom) < 0
            ? beatFraction.num % beatFraction.denom
            : beatFraction.num;

      return new Beat(beat, num, beatFraction.denom);
    }
  }

} // end of Beat class
