#include <string>

std::string mxlBeg = R"(
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE score-partwise PUBLIC
    "-//Recordare//DTD MusicXML 3.1 Partwise//EN"
    "http://www.musicxml.org/dtds/partwise.dtd">
<score-partwise version="3.1">
  <part-list>
    <score-part id="P1">
      <part-name>DryadTest</part-name>
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
      )";

std::string mxlEnd = R"(
    </measure>
  </part>
</score-partwise>)";

auto genNote = [](std::string step, int oct, int dur) -> std::string
{
  std::string output = "\n<note>\n\t<pitch>\n\t\t<step>";
  output += step;
  output += "</step>\n\t\t<octave>";
  output += std::to_string(oct);
  output += "</octave>\n\t</pitch>\n\t<duration>";
  output += std::to_string(dur);
  output += "</duration>\n</note>";
  return output;
};

auto genBackup = [](int dur) -> std::string
{
  std::string output = "\n<backup>\n\t<duration>";
  output += std::to_string(dur);
  output += "</duration>\n</backup>";
  return output;
};


