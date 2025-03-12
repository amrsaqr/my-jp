//
//  Copyright 2025 Amr Saqr
//

#ifndef JSON_BOOLEAN_PARSER_H_
#define JSON_BOOLEAN_PARSER_H_

#include "historical_reader.h"
#include "json_parsing_result.h"

class JsonBooleanParser {
 public:
  static JsonParsingResult TryParseBool(HistoricalReader* reader);

 private:
  static const char* const kJsonTrue;
  static const size_t kJsonTrueLength = 4;
  static const char* const kJsonFalse;
  static const size_t kJsonFalseLength = 5;
};

#endif  // JSON_BOOLEAN_PARSER_H_
