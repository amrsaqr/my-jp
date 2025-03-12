//
//  Copyright 2025 Amr Saqr
//

#ifndef JSON_UTILITY_H_
#define JSON_UTILITY_H_

#include "historical_reader.h"
#include "json_parsing_result.h"

class JsonUtility {
 public:
  static JsonParsingResult TryParseJsonConstant(HistoricalReader* reader,
                                                const char* str, size_t len);
  static JsonParsingResult TryParseJsonValue(HistoricalReader* reader);
  static JsonParsingResult TryParseJsonRoot(HistoricalReader* reader);
};

#endif  // JSON_UTILITY_H_
