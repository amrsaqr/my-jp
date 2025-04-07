//
//  Copyright 2025 Amr Saqr
//

#ifndef JSON_CONSTANT_PARSER_H_
#define JSON_CONSTANT_PARSER_H_

#include "historical_reader.h"
#include "json_parsing_result.h"

class JsonConstantParser {
 public:
  static JsonParsingResult TryParseNull(HistoricalReader* reader);
  static JsonParsingResult TryParseBool(HistoricalReader* reader);

 private:
  static JsonParsingResult TryParseJsonConstant(HistoricalReader* reader,
                                                const char* str, size_t len);
};

#endif  // JSON_CONSTANT_PARSER_H_
