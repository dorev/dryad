import { j2xParser, parse } from "fast-xml-parser";
import * as opensheetmusicdisplay from "opensheetmusicdisplay";

const xmlFile: string = `<score-partwise version="3.1"><part-list>  <score-part id="P1">    <part-name>Music</part-name>  </score-part></part-list><part id="P1">  <measure number="1">    <attributes>      <divisions>1</divisions>      <key>        <fifths>0</fifths>      </key>      <time>        <beats>4</beats>        <beat-type>4</beat-type>      </time>      <clef>        <sign>G</sign>        <line>2</line>      </clef>    </attributes>    <note>      <pitch>        <step>C</step>        <octave>4</octave>      </pitch>      <duration>1</duration>      <type>quarter</type>    </note>    <note>      <pitch>        <step>C</step>        <octave>4</octave>      </pitch>      <duration>1</duration>      <type>quarter</type>    </note>  </measure></part></score-partwise>`;

const optionsXmlToJson = {
  attributeNamePrefix : "",
  attrNodeName: "attr",
  textNodeName : "#text",
  ignoreAttributes : false,
  arrayMode: false,
};

const optionsJsonToXml = {
  attributeNamePrefix : "",
  attrNodeName: "attr",
  textNodeName : "#text",
  indentBy: "  ",
};

const input = document.querySelector("#dryad-input-button") as HTMLInputElement;
const text = document.querySelector("#dryad-input") as HTMLTextAreaElement;
text.value = xmlFile;

const osmd = new opensheetmusicdisplay.OpenSheetMusicDisplay("vex");
osmd.setOptions({
  drawingParameters: "compact",
});

input.addEventListener("click", crunchDryad);

function crunchDryad(): void {

  const obj = parse(text.value, optionsXmlToJson);
  cleanObj(obj);
  console.log(obj);

  const parser = new j2xParser(optionsJsonToXml);
  const xmlReborn: string = `<?xml version="1.0" encoding="UTF-8" standalone="no"?><!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 3.1 Partwise//EN" "http://www.musicxml.org/dtds/partwise.dtd">`
                          + parser.parse(obj);

  console.log(xmlReborn);

  osmd.load(xmlReborn)
  .then(() => osmd.render())
  .catch((error) => {
    throw error;
  });
}

function cleanObj(obj: object): void {

  if (obj == null) {
    return;
  }

  for (const item in obj) {

    if (item == null) {
      continue;
    }

    const o = (obj as any)[item];

    if (typeof(o) === "object") {
      cleanObj(o as any);
    }

    if (o === "") {
      delete (obj as any)[item];
    }
  }
}

input.click();
