//
//  Copyright 2025 Amr Saqr
//

#ifndef JSON_PARSER_H_
#define JSON_PARSER_H_

#include <string>

#include "json_parsing_result.h"
#include "limited_history_preserving_reader.h"

class JsonParser {
 public:
  bool Parse(const std::string& file_path, std::string* error_output);

 private:
  JsonParsingResult TryParseJsonRoot(
      LimitedHistoryPreservingReader* historical_reader);
  JsonParsingResult TryParseJsonValue(
      LimitedHistoryPreservingReader* historical_reader);
  JsonParsingResult TryParseObject(LimitedHistoryPreservingReader* reader);
  JsonParsingResult TryParseArray(LimitedHistoryPreservingReader* reader);
  static constexpr size_t kMaxJsonDepth = 19;
  size_t json_depth_ = 0;
};

#endif  // JSON_PARSER_H_
