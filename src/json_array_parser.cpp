//
//  Copyright 2025 Amr Saqr
//

#include "json_array_parser.h"

#include "json_utility.h"

JsonParsingResult JsonArrayParser::TryParseArray(HistoricalReader* reader) {
  // if stream ended, then there is nothing to read
  if (!reader->HasNextByte()) {
    return JsonParsingResult::kTypeMismatch;
  }

  // if we don't read an opening [ character, then the next token can't be an
  // array
  char last_byte = reader->TestNextByteSkipWhitespace();
  if (last_byte != '[') {
    return JsonParsingResult::kTypeMismatch;
  }

  // at this point, it has to be an array, either valid or invalid
  reader->GetNextByteSkipWhitespace();

  // read array items
  bool has_to_read_value = false;
  while (reader->HasNextByte()) {
    // this is an invalid array if:
    // 1.we read an invalid json value
    // 2.has_to_read_value flag was true and we couldn't read a valid json value
    JsonParsingResult result = JsonUtility::TryParseJsonValue(reader);
    if (result == JsonParsingResult::kInvalidTypeMatch ||
        (has_to_read_value && result != JsonParsingResult::kValidTypeMatch)) {
      return JsonParsingResult::kInvalidTypeMatch;
    }

    // set has_to_read_value flag to false, whether we read a value or not
    has_to_read_value = false;

    // at this point there are two possibilities:
    // 1.we read a value, then we should either read ',' or ']'
    // 2.we didn't read a value, then we must read a ']'
    // otherwise, this is an invalid array
    bool read_a_value = result == JsonParsingResult::kValidTypeMatch;
    last_byte = reader->GetNextByteSkipWhitespace();
    if ((read_a_value && last_byte != ',' && last_byte != ']') ||
        (!read_a_value && last_byte != ']')) {
      return JsonParsingResult::kInvalidTypeMatch;
    }

    // if we read a ',' set has_to_read_value flag to true
    // if we read a ']' then we reached the end of the array
    if (last_byte == ',') {
      has_to_read_value = true;
    } else {  // last_byte == ']'
      break;
    }
  }

  return last_byte == ']' ? JsonParsingResult::kValidTypeMatch
                          : JsonParsingResult::kInvalidTypeMatch;
}
