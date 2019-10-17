class MusicalEvent
{
    private static nextId : number = 0;
    id : number;

    constructor()
    {
        this.id = MusicalEvent.nextId++;
    }
}

class Note
{
    readonly value : number;
    readonly duration : Beat;

    constructor(value : number, duration : Beat)
    {
        this.value = value;
        this.duration = duration;
    }
}

class NoteOn extends MusicalEvent
{
    readonly note : Note;  

    constructor(note : Note)
    {
        super();
        this.note = note;
    }  
}

class NoteOff extends MusicalEvent
{
    readonly noteRef : InstantAddress;

    constructor(noteRef : InstantAddress)
    {
        super();
        this.noteRef = noteRef;
    }  
}

interface Fraction
{
    num   : number;
    denom : number;
}

class Beat
{
    readonly beat  : number = 0;
    readonly num   : number = 0;
    readonly denom : number = 0;

    constructor(beat : number, num : number = 0, denom : number = 0)
    {
        this.beat = beat;
        this.num = num;
        this.denom = denom;
    }

    add(other : Beat): Beat
    {
        if(this.denom === other.denom)
        {
            let beat = this.beat + other.beat + Math.floor((this.num + other.num) / this.denom);
            let num = Math.floor((this.num + other.num) / this.denom) ? (this.num + other.num) % this.denom : this.num + other.num;
            return new Beat(beat, num, this.denom);
        }
        else
        {
            let beatFraction : Fraction = reduceFraction({num: this.num * other.denom + other.num * this.denom, denom : this.denom * other.denom});

            let beat = this.beat + other.beat + Math.floor(beatFraction.num / this.denom);
            let num = Math.floor(beatFraction.num / this.denom) ? beatFraction.num % this.denom : beatFraction.num;

            return new Beat(beat, num, beatFraction.denom);
        }
    }

}

class InstantAddress
{
    readonly offset : Beat;
    readonly id : number;
}

class Instant
{
    readonly offset : Beat;
    events : MusicalEvent[];
    notesResonating : Note[];
}

class Score
{
    private score : Instant[];

    constructor()
    {}
}

function reduceFraction(fraction : Fraction) : Fraction
{
    let divisor : number = fraction.denom;
    for (var i = 1; i <= fraction.denom; ++i) {

        if(fraction.num % i == 0 && fraction.denom % i == 0)
        {
            divisor = i;
        }
    }
    return { num: fraction.num/divisor, denom : fraction.denom/divisor};
}