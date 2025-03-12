//
//  Copyright 2025 Amr Saqr
//

#ifndef JSON_OBJECT_PARSER_H_
#define JSON_OBJECT_PARSER_H_

#include "historical_reader.h"
#include "json_parsing_result.h"

class JsonObjectParser {
 public:
  static JsonParsingResult TryParseObject(HistoricalReader* reader);
};

#endif  // JSON_OBJECT_PARSER_H_
