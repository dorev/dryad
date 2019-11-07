// import { VexScore } from "./VexScore";
import { j2xParser, parse } from "fast-xml-parser";
import * as opensheetmusicdisplay from "opensheetmusicdisplay";

const xmlFile: string = `<score-partwise version="3.1">
<part-list>
  <score-part id="P1">
    <part-name>Music</part-name>
  </score-part>
</part-list>
<part id="P1">
  <measure number="1">
    <attributes>
      <divisions>1</divisions>
      <key>
        <fifths>0</fifths>
      </key>
      <time>
        <beats>4</beats>
        <beat-type>4</beat-type>
      </time>
      <clef>
        <sign>G</sign>
        <line>2</line>
      </clef>
    </attributes>
    <note>
      <pitch>
        <step>C</step>
        <octave>4</octave>
      </pitch>
      <duration>1</duration>
      <type>quarter</type>
    </note>
    <note>
      <pitch>
        <step>C</step>
        <octave>4</octave>
      </pitch>
      <duration>1</duration>
      <type>quarter</type>
    </note>
  </measure>
</part>
</score-partwise>`;

const optionsXmlToJson = {
  attributeNamePrefix : "", // "@_",
  attrNodeName: "attr", // default is 'false'
  textNodeName : "#text",
  ignoreAttributes : false,
  allowBooleanAttributes : true,
  parseNodeValue : true,
  parseAttributeValue : true,
  trimValues: true,
  arrayMode: false, // "strict"
};

const optionsJsonToXml = {
  attributeNamePrefix : "", // "@_",
  attrNodeName: "attr", // default is false
  textNodeName : "#text",
  ignoreAttributes : false,
  cdataTagName: "__cdata", // default is false
  cdataPositionChar: "\\c",
  format: false,
  indentBy: "  ",
  supressEmptyNode: true,
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

  console.log(text.value);
  const obj = parse(text.value, optionsXmlToJson);

  cleanObj(obj);

  console.log(obj);

  const parser = new j2xParser(optionsJsonToXml);
  const xmlReborn: string = `<?xml version="1.0" encoding="UTF-8" standalone="no"?>
  <!DOCTYPE score-partwise PUBLIC
  "-//Recordare//DTD MusicXML 3.1 Partwise//EN"
  "http://www.musicxml.org/dtds/partwise.dtd">
  ` + parser.parse(obj);

  console.log(xmlReborn);

  osmd.load(xmlReborn)
  .then(() => osmd.render())
  .catch((error) => {
    throw error;
  });
}

function cleanObj(obj: object): void {

  for (const item in obj) {

    if (item === null || item === undefined) {
      continue;
    }

    if (typeof((obj as any)[item]) === "object") {
      cleanObj((obj as any)[item] as any);
    }

    if ((obj as any)[item] === "") {
      delete (obj as any)[item];
    }
  }
}

input.click();
