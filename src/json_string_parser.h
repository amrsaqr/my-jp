//
//  Copyright 2025 Amr Saqr
//

#ifndef JSON_STRING_PARSER_H_
#define JSON_STRING_PARSER_H_

#include "historical_reader.h"
#include "json_parsing_result.h"

class JsonStringParser {
 public:
  static JsonParsingResult TryParseString(HistoricalReader* reader);
};

#endif  // JSON_STRING_PARSER_H_
