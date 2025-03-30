//
//  Copyright 2025 Amr Saqr
//

#include "json_object_parser.h"

#include "json_string_parser.h"
#include "json_utility.h"

JsonParsingResult JsonObjectParser::TryParseObject(HistoricalReader* reader) {
  // if stream ended, then there is nothing to read
  if (!reader->HasNextByte()) {
    return JsonParsingResult::kTypeMismatch;
  }

  // if we don't read an opening { character, then the next token can't be an
  // object
  char last_byte = reader->TestNextByte(true);
  if (last_byte != '{') {
    return JsonParsingResult::kTypeMismatch;
  }

  // at this point it has to be an object, either valid or invalid
  reader->GetNextByte(true);

  // read key/value pairs
  bool has_to_read_key = false;
  while (reader->HasNextByte()) {
    // this is an invalid object if:
    // 1.we read an invalid json key
    // 2.has_to_read_key flag was true and we couldn't read a valid json key
    JsonParsingResult result = JsonStringParser::TryParseString(reader);
    if (result == JsonParsingResult::kInvalidTypeMatch ||
        (has_to_read_key && result != JsonParsingResult::kValidTypeMatch)) {
      return JsonParsingResult::kInvalidTypeMatch;
    }

    // if we read a valid json key, then we have to read a colon followed by a
    // valid json value, otherwise this is an invalid object
    if (result == JsonParsingResult::kValidTypeMatch) {
      // read the colon
      if (!reader->HasNextByte() || reader->GetNextByte(true) != ':') {
        return JsonParsingResult::kInvalidTypeMatch;
      }

      // if we couldn't read a json value, then this is an invalid object
      if (result = JsonUtility::TryParseJsonValue(reader);
          result != JsonParsingResult::kValidTypeMatch) {
        return JsonParsingResult::kInvalidTypeMatch;
      }
    }

    // set has_to_read_key flag to false, whether we read a kv pair or not
    has_to_read_key = false;

    // at this point there are two possibilities
    // 1.we read a kv pair, then we should either read ',' or '}'
    // 2.we didn't read a kv pair, then we must read a '}'
    // otherwise, this is an invalid object
    bool read_a_kv = result == JsonParsingResult::kValidTypeMatch;
    last_byte = reader->GetNextByte(true);
    if ((read_a_kv && last_byte != ',' && last_byte != '}') ||
        (!read_a_kv && last_byte != '}')) {
      return JsonParsingResult::kInvalidTypeMatch;
    }

    // if we read a ',' set has_to_read_key flag to true
    // if we read a '}' then we reached the end of the object
    if (last_byte == ',') {
      has_to_read_key = true;
    } else {
      break;
    }
  }

  return last_byte == '}' ? JsonParsingResult::kValidTypeMatch
                          : JsonParsingResult::kInvalidTypeMatch;
}
