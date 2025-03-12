//
//  Copyright 2025 Amr Saqr
//

#ifndef JSON_NUMBER_PARSER_H_
#define JSON_NUMBER_PARSER_H_

#include "historical_reader.h"
#include "json_parsing_result.h"

class JsonNumberParser {
 public:
  static JsonParsingResult TryParseNumber(HistoricalReader* reader);
};

#endif  // JSON_NUMBER_PARSER_H_
