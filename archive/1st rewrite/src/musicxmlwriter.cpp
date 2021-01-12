#include "dryadutils.h"
#include "musicxmlwriter.h"
#include "includes.h"
#include "phrase.h"

#include <pugixml/pugixml.hpp>

using namespace pugi;

struct xml_string_writer: xml_writer
{
    std::string result;

    virtual void write(const void* data, size_t size)
    {
        result.append(static_cast<const char*>(data), size);
    }

};

namespace dryad
{

std::string musicxml_writer::render(song_t* song)
{
    (void) song;

    xml_document root;

    xml_node score_partwise = root.append_child("score-partwise");

    score_partwise.append_child("work")
        .append_child("work-title")
        .append_child(node_pcdata).set_value("Dryad generation");

    xml_node part_list = score_partwise.append_child("part-list");
    
    xml_node score_part = part_list.append_child("score-part");
    score_part.append_attribute("id").set_value("P1");
    score_part.append_child("part-name").append_child(node_pcdata).set_value("Music");

    xml_node part = score_partwise.append_child("part");
    part.append_attribute("id").set_value("P1");

    int measure_id = 0;
    for (phrase_t& phrase : *song)
    {
        for (measure_t& measure : phrase.get_measures())
        {
            (void) measure;
            xml_node measure_node = part.append_child("measure");
            measure_node.append_attribute("number").set_value(++measure_id);

            if (measure_id == 1)
            {
                // set first measure attributes
                xml_node attributes = measure_node.append_child("attributes");
                attributes.append_child("divisions").append_child(node_pcdata).set_value(std::to_string(QUARTER).c_str());
                attributes.append_child("key").append_child("fifth").append_child(node_pcdata).set_value("0");
                
                xml_node time = attributes.append_child("time");
                time.append_child("beats").append_child(node_pcdata).set_value("4");
                time.append_child("beat-type").append_child(node_pcdata).set_value("4");
                
                xml_node clef = attributes.append_child("clef");
                clef.append_child("sign").append_child(node_pcdata).set_value("G");
                clef.append_child("line").append_child(node_pcdata).set_value("2");
            }

            for (note_t& note : measure.get_voice())
            {
                xml_node note_node = measure_node.append_child("note");

                xml_node pitch = note_node.append_child("pitch");
                pitch.append_child("step").append_child(node_pcdata).set_value(note.get_pitch().get_step());
                pitch.append_child("octave").append_child(node_pcdata).set_value(std::to_string(note.get_pitch().get_octave()).c_str());

                note_node.append_child("duration").append_child(node_pcdata).set_value(std::to_string(note.get_duration()).c_str());
                note_node.append_child("type").append_child(node_pcdata).set_value(duration_to_note_type(note.get_duration()));
            }
        }
    }

    xml_string_writer writer;
    root.save(writer);
    return writer.result;
}

}