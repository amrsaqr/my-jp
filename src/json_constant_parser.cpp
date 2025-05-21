//
//  Copyright 2025 Amr Saqr
//

#include "json_constant_parser.h"

JsonParsingResult JsonConstantParser::TryParseNull(
    LimitedHistoryPreservingReader* reader) {
  return TryParseJsonConstant(reader, "null", 4);
}

JsonParsingResult JsonConstantParser::TryParseBool(
    LimitedHistoryPreservingReader* reader) {
  JsonParsingResult parsing_result = TryParseJsonConstant(reader, "true", 4);

  if (parsing_result == JsonParsingResult::kTypeMismatch) {
    parsing_result = TryParseJsonConstant(reader, "false", 5);
  }

  return parsing_result;
}

JsonParsingResult JsonConstantParser::TryParseJsonConstant(
    LimitedHistoryPreservingReader* reader, const char* const str,
    const size_t len) {
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
