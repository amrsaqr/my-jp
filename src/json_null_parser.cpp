//
//  Copyright 2025 Amr Saqr
//

#include "json_null_parser.h"

#include "json_utility.h"

JsonParsingResult JsonNullParser::TryParseNull(
    HistoricalReader* reader) {
  return JsonUtility::TryParseJsonConstant(reader, kJsonNull, kJsonNullLength);
}

const char* const JsonNullParser::kJsonNull = "null";
