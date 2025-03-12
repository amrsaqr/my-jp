//
//  Copyright 2025 Amr Saqr
//

#ifndef JSON_NULL_PARSER_H_
#define JSON_NULL_PARSER_H_

#include "historical_reader.h"
#include "json_parsing_result.h"

class JsonNullParser {
 public:
  static JsonParsingResult TryParseNull(HistoricalReader* reader);

 private:
  static const char* const kJsonNull;
  static const size_t kJsonNullLength = 4;
};

#endif  // JSON_NULL_PARSER_H_
