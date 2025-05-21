//
//  Copyright 2025 Amr Saqr
//

#ifndef JSON_STRING_PARSER_H_
#define JSON_STRING_PARSER_H_

#include "json_parsing_result.h"
#include "limited_history_preserving_reader.h"

class JsonStringParser {
 public:
  static JsonParsingResult TryParseString(LimitedHistoryPreservingReader* reader);
};

#endif  // JSON_STRING_PARSER_H_
