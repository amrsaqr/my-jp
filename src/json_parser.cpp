//
//  Copyright 2025 Amr Saqr
//

#include "json_parser.h"

#include <fstream>
#include <string>

#include "json_constant_parser.h"
#include "json_number_parser.h"
#include "json_string_parser.h"
#include "limited_history_preserving_reader.h"
using std::ifstream;
using std::ios_base;
using std::string;

bool JsonParser::Parse(const string& file_path, string* error_output) {
  try {
    ifstream input_stream(file_path);
    input_stream.exceptions(std::ifstream::badbit);

    if (!input_stream.is_open()) {
      *error_output = "Failure opening the input file " + file_path;
      return false;
    }

    LimitedHistoryPreservingReader reader(&input_stream);
    bool successful_parsing =
        TryParseJsonRoot(&reader) == JsonParsingResult::kValidTypeMatch;

    if (!successful_parsing) {
      *error_output = reader.GetHistory();
    }

    return successful_parsing;
  } catch (const ios_base::failure& fail) {
    *error_output = "Failure reading the input file " + file_path +
                    "\nReason: " + fail.what();
    return false;
  }
}

JsonParsingResult JsonParser::TryParseJsonRoot(
    LimitedHistoryPreservingReader* reader) {
  // if stream ended, then there is nothing to read
  if (!reader->HasNextByte()) {
    return JsonParsingResult::kTypeMismatch;
  }

  // try json object and json array for the root
  JsonParsingResult result = TryParseObject(reader);
  if (result == JsonParsingResult::kTypeMismatch) {
    result = TryParseArray(reader);
  }

  // if it's a valid match, make sure there is no additional characters
  if (result == JsonParsingResult::kValidTypeMatch &&
      reader->GetNextByte(true) != EOF) {
    result = JsonParsingResult::kInvalidTypeMatch;
  }

  return result;
}

JsonParsingResult JsonParser::TryParseJsonValue(
    LimitedHistoryPreservingReader* reader) {
  // if stream ended, then there is nothing to read
  if (!reader->HasNextByte()) {
    return JsonParsingResult::kTypeMismatch;
  }

  // try all different json values
  // start with primitive values
  using ParsingFuncPtr = JsonParsingResult (*)(LimitedHistoryPreservingReader*);
  static ParsingFuncPtr parsers[] = {
      &JsonConstantParser::TryParseNull, &JsonConstantParser::TryParseBool,
      &JsonNumberParser::TryParseNumber, &JsonStringParser::TryParseString};

  for (ParsingFuncPtr parser : parsers) {
    JsonParsingResult result = parser(reader);
    if (result != JsonParsingResult::kTypeMismatch) {
      return result;
    }
  }

  // now that we're only left with either object or array, we need to check the
  // depth first
  if (json_depth_ >= kMaxJsonDepth) {
    return JsonParsingResult::kTypeMismatch;
  }

  // then try object and array
  using ThisParsingFuncPtr =
      JsonParsingResult (JsonParser::*)(LimitedHistoryPreservingReader*);
  static ThisParsingFuncPtr this_parsers[] = {&JsonParser::TryParseObject,
                                              &JsonParser::TryParseArray};

  for (ThisParsingFuncPtr parser : this_parsers) {
    JsonParsingResult result = (this->*parser)(reader);
    if (result != JsonParsingResult::kTypeMismatch) {
      return result;
    }
  }

  return JsonParsingResult::kTypeMismatch;
}

JsonParsingResult JsonParser::TryParseObject(
    LimitedHistoryPreservingReader* reader) {
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

  // since it has to be an object, we need to increase the depth
  ++json_depth_;

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
      if (result = TryParseJsonValue(reader);
          result != JsonParsingResult::kValidTypeMatch) {
        return JsonParsingResult::kInvalidTypeMatch;
      }
    }

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

  // since we're done parsing the object, we need to decrease the depth
  --json_depth_;

  return last_byte == '}' ? JsonParsingResult::kValidTypeMatch
                          : JsonParsingResult::kInvalidTypeMatch;
}

JsonParsingResult JsonParser::TryParseArray(
    LimitedHistoryPreservingReader* reader) {
  // if stream ended, then there is nothing to read
  if (!reader->HasNextByte()) {
    return JsonParsingResult::kTypeMismatch;
  }

  // if we don't read an opening [ character, then the next token can't be an
  // array
  char last_byte = reader->TestNextByte(true);
  if (last_byte != '[') {
    return JsonParsingResult::kTypeMismatch;
  }

  // at this point, it has to be an array, either valid or invalid
  reader->GetNextByte(true);

  // since it has to be an array, we need to increase the depth
  ++json_depth_;

  // read array items
  bool has_to_read_value = false;
  while (reader->HasNextByte()) {
    // this is an invalid array if:
    // 1.we read an invalid json value
    // 2.has_to_read_value flag was true and we couldn't read a valid json value
    JsonParsingResult result = TryParseJsonValue(reader);
    if (result == JsonParsingResult::kInvalidTypeMatch ||
        (has_to_read_value && result != JsonParsingResult::kValidTypeMatch)) {
      return JsonParsingResult::kInvalidTypeMatch;
    }

    // at this point there are two possibilities:
    // 1.we read a value, then we should either read ',' or ']'
    // 2.we didn't read a value, then we must read a ']'
    // otherwise, this is an invalid array
    bool read_a_value = result == JsonParsingResult::kValidTypeMatch;
    last_byte = reader->GetNextByte(true);
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

  // since we're done parsing the array, we need to decrease the depth
  --json_depth_;

  return last_byte == ']' ? JsonParsingResult::kValidTypeMatch
                          : JsonParsingResult::kInvalidTypeMatch;
}
