//
//  Copyright 2025 Amr Saqr
//

#ifndef JSON_ARRAY_PARSER_H_
#define JSON_ARRAY_PARSER_H_

#include "historical_reader.h"
#include "json_parsing_result.h"

class JsonArrayParser {
 public:
  static JsonParsingResult TryParseArray(HistoricalReader* reader);
};

#endif  // JSON_ARRAY_PARSER_H_
