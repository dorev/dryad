#pragma once

#include "definitions.h"

enum class Lang
{
  en,
  fr
};

using LangText = std::pair<Lang, std::string>;

struct Text
{
  std::vector<LangText> _text;

  Text() : _text({{{}}})
  {
  }

  Text(std::vector<LangText> text) : _text(text)
  {
  }

  std::string operator[](Lang lang) const
  {
    auto result = std::find_if(ALL(_text), [&](LangText lt) { return lt.first == lang; });
    if (result != _text.end())
      return result->second;

    return "";
  }
};