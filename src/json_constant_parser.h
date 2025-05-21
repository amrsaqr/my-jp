//
//  Copyright 2025 Amr Saqr
//

#ifndef JSON_CONSTANT_PARSER_H_
#define JSON_CONSTANT_PARSER_H_

#include "json_parsing_result.h"
#include "limited_history_preserving_reader.h"

class JsonConstantParser {
 public:
  static JsonParsingResult TryParseNull(LimitedHistoryPreservingReader* reader);
  static JsonParsingResult TryParseBool(LimitedHistoryPreservingReader* reader);

 private:
  static JsonParsingResult TryParseJsonConstant(
      LimitedHistoryPreservingReader* reader, const char* str, size_t len);
};

#endif  // JSON_CONSTANT_PARSER_H_
