import * as Vex from "vexflow";
import { VexScore } from "./VexProcessor";



const input = document.querySelector("#dryad-input-button") as HTMLElement;
const text = document.querySelector("#dryad-input") as HTMLTextAreaElement;
text.value = "{}";
input.addEventListener("click", crunchDryad);

function crunchDryad(): void {

  const json = JSON.parse(text.value);
  console.log(json);

  const vexScore = new VexScore(document.querySelector("#vex"), 420, 200, 400);

  const result: string = vexScore.render(json);

  if (result !== "all good") {
    input.setAttribute("value", result);
    setTimeout(() => {input.setAttribute("value", "crunch"); }, 1000);
  }



}



// Fill a score according to the content of the text
// Need a VexProcessor that can take a single object as input
// and completely render the staff





/*
//
// VEX SETUP
//

// Create an SVG renderer and attach it to the DIV element named "boo".
const VF = Vex.Flow;
const div = document.querySelector("#vex") as HTMLElement;
const renderer = new VF.Renderer(div, VF.Renderer.Backends.SVG);

// Size our svg:
renderer.resize(420, 200);

// And get a drawing context:
const context = renderer.getContext();

// Create a stave at position 10, 0 of width 400 on the canvas.
const stave = new VF.Stave(10, 0, 400);

// Add a clef and time signature.
stave.addClef("treble").addTimeSignature("4/4");

// Connect it to the rendering context and draw!
stave.setContext(context).draw();

const notes = [
  // A quarter-note C.
  new VF.StaveNote({clef: "treble", keys: ["c/4"], duration: "q" }),

  // A quarter-note D.
  new VF.StaveNote({clef: "treble", keys: ["d/4"], duration: "q" }),

  // A quarter-note rest. Note that the key (b/4) specifies the vertical
  // position of the rest.
  new VF.StaveNote({clef: "treble", keys: ["b/4"], duration: "qr" }),

  // A C-Major chord.
  new VF.StaveNote({clef: "treble", keys: ["c/4", "e/4", "g/4"], duration: "q" }),
];

// Create a voice in 4/4 and add above notes
const voice = new VF.Voice({num_beats: 4,  beat_value: 4});
voice.addTickables(notes);

// Format and justify the notes to 400 pixels.
const formatter = new VF.Formatter().joinVoices([voice]).format([voice], 400);

// Render voice
voice.draw(context, stave);
*/
