//
//  Copyright 2025 Amr Saqr
//

#include "json_boolean_parser.h"

#include "json_utility.h"

JsonParsingResult JsonBooleanParser::TryParseBool(
    HistoricalReader* reader) {
  JsonParsingResult parsing_result =
      JsonUtility::TryParseJsonConstant(reader, kJsonTrue, kJsonTrueLength);

  if (parsing_result == JsonParsingResult::kTypeMismatch) {
    parsing_result =
        JsonUtility::TryParseJsonConstant(reader, kJsonFalse, kJsonFalseLength);
  }

  return parsing_result;
}

const char* const JsonBooleanParser::kJsonTrue = "true";
const char* const JsonBooleanParser::kJsonFalse = "false";
