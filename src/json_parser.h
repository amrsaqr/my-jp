//
//  Copyright 2025 Amr Saqr
//

#ifndef JSON_PARSER_H_
#define JSON_PARSER_H_

#include <string>

#include "historical_reader.h"
#include "json_parsing_result.h"

class JsonParser {
 public:
  bool Parse(const std::string& file_path, std::string* error_output);

 private:
  JsonParsingResult TryParseJsonRoot(HistoricalReader* historical_reader);
  JsonParsingResult TryParseJsonValue(HistoricalReader* historical_reader);
  JsonParsingResult TryParseObject(HistoricalReader* reader);
  JsonParsingResult TryParseArray(HistoricalReader* reader);
  static constexpr size_t kMaxJsonDepth = 19;
  size_t json_depth_ = 0;
};

#endif  // JSON_PARSER_H_
