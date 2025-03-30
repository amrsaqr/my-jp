//
//  Copyright 2025 Amr Saqr
//

#include "json_utility.h"

#include "json_array_parser.h"
#include "json_boolean_parser.h"
#include "json_null_parser.h"
#include "json_number_parser.h"
#include "json_object_parser.h"
#include "json_string_parser.h"

JsonParsingResult JsonUtility::TryParseJsonConstant(HistoricalReader* reader,
                                                    const char* const str,
                                                    const size_t len) {
  if (len <= 0) {
    return JsonParsingResult::kValidTypeMatch;
  }

  if (!reader->HasNextByte() || reader->TestNextByte(true) != str[0]) {
    return JsonParsingResult::kTypeMismatch;
  }
  reader->GetNextByte(true);

  size_t mismatch_index = 1;
  for (size_t i = 1; i < len; ++i, ++mismatch_index) {
    if (!reader->HasNextByte() || reader->TestNextByte() != str[i]) {
      break;
    }

    reader->GetNextByte();
  }

  return mismatch_index == len ? JsonParsingResult::kValidTypeMatch
                               : JsonParsingResult::kInvalidTypeMatch;
}

JsonParsingResult JsonUtility::TryParseJsonValue(HistoricalReader* reader) {
  // if stream ended, then there is nothing to read
  if (!reader->HasNextByte()) {
    return JsonParsingResult::kTypeMismatch;
  }

  // try all different json values
  using ParsingFuncPtr = JsonParsingResult (*)(HistoricalReader*);
  static ParsingFuncPtr parsers[] = {
      &JsonNullParser::TryParseNull,     &JsonBooleanParser::TryParseBool,
      &JsonNumberParser::TryParseNumber, &JsonStringParser::TryParseString,
      &JsonArrayParser::TryParseArray,   &JsonObjectParser::TryParseObject,
  };

  for (ParsingFuncPtr parser : parsers) {
    JsonParsingResult result = parser(reader);
    if (result != JsonParsingResult::kTypeMismatch) {
      return result;
    }
  }

  return JsonParsingResult::kTypeMismatch;
}

JsonParsingResult JsonUtility::TryParseJsonRoot(HistoricalReader* reader) {
  // if stream ended, then there is nothing to read
  if (!reader->HasNextByte()) {
    return JsonParsingResult::kTypeMismatch;
  }

  // try json object and json array for the root
  JsonParsingResult result = JsonObjectParser::TryParseObject(reader);
  if (result == JsonParsingResult::kTypeMismatch) {
    result = JsonArrayParser::TryParseArray(reader);
  }

  // if it's a valid match, make sure there is no additional characters
  if (result == JsonParsingResult::kValidTypeMatch &&
      reader->GetNextByte(true) != EOF) {
    result = JsonParsingResult::kInvalidTypeMatch;
  }

  return result;
}
