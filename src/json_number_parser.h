//
//  Copyright 2025 Amr Saqr
//

#ifndef JSON_NUMBER_PARSER_H_
#define JSON_NUMBER_PARSER_H_

#include "json_parsing_result.h"
#include "limited_history_preserving_reader.h"

class JsonNumberParser {
 public:
  static JsonParsingResult TryParseNumber(LimitedHistoryPreservingReader* reader);
};

#endif  // JSON_NUMBER_PARSER_H_
