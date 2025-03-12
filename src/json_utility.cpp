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
  size_t mismatch_index = 0;
  for (size_t i = 0; i < len; ++i, ++mismatch_index) {
    if (!reader->HasNextByte() ||
        reader->TestNextByteSkipWhitespace() != str[i]) {
      break;
    }

    reader->GetNextByteSkipWhitespace();
  }

  if (mismatch_index == 0) {
    return JsonParsingResult::kTypeMismatch;
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
  JsonParsingResult result = JsonNullParser::TryParseNull(reader);
  if (result != JsonParsingResult::kTypeMismatch) {
    return result;
  }

  result = JsonBooleanParser::TryParseBool(reader);
  if (result != JsonParsingResult::kTypeMismatch) {
    return result;
  }

  result = JsonNumberParser::TryParseNumber(reader);
  if (result != JsonParsingResult::kTypeMismatch) {
    return result;
  }

  result = JsonStringParser::TryParseString(reader);
  if (result != JsonParsingResult::kTypeMismatch) {
    return result;
  }

  result = JsonArrayParser::TryParseArray(reader);
  if (result != JsonParsingResult::kTypeMismatch) {
    return result;
  }

  result = JsonObjectParser::TryParseObject(reader);
  return result;
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
      reader->GetNextByteSkipWhitespace() != EOF) {
    result = JsonParsingResult::kInvalidTypeMatch;
  }

  return result;
}
