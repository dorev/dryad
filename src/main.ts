import { Beat } from "./Beat";


const beat1 = new Beat(1, 3, 4);
const beat2 = new Beat(2, 5, 6);
const beat = beat1.add(beat2);

const num = beat.num;

const num2 = num + 2;
