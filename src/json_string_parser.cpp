//
//  Copyright 2025 Amr Saqr
//

#include "json_string_parser.h"

JsonParsingResult JsonStringParser::TryParseString(LimitedHistoryPreservingReader* reader) {
  // if stream ended, then there is nothing to read
  if (!reader->HasNextByte()) {
    return JsonParsingResult::kTypeMismatch;
  }

  // if we don't read the double quotation mark, then the next token can't be a
  // string
  char last_byte = reader->TestNextByte(true);
  if (last_byte != '"') {
    return JsonParsingResult::kTypeMismatch;
  }

  // at this point, it has to be a string, either valid or invalid
  reader->GetNextByte(true);

  // read bytes
  bool found_closing_double_quote = false;
  bool currently_escaping = false;
  while (reader->HasNextByte()) {
    last_byte = reader->GetNextByte();

    // if we encounter a control character, that's an invalid byte
    if (iscntrl(last_byte)) {
      return JsonParsingResult::kInvalidTypeMatch;
    }

    if (currently_escaping) {
      switch (last_byte) {
        // special escaped characters
        case '"':
        case '\\':
        case '/':
        case 'b':
        case 'f':
        case 'n':
        case 'r':
        case 't':
          break;
        // the 'u' has to be handled in a special way, we need to make sure
        // exactly 4 hex digits follow it
        case 'u': {
          int hex_digits = 0;
          while (reader->HasNextByte() && hex_digits < 4) {
            last_byte = reader->GetNextByte();
            if (!ishexnumber(last_byte)) {
              return JsonParsingResult::kInvalidTypeMatch;
            }
            ++hex_digits;
          }

          if (hex_digits < 4) {
            return JsonParsingResult::kInvalidTypeMatch;
          }
          break;
        }
        // any other character that follows \ is invalid
        default:
          return JsonParsingResult::kInvalidTypeMatch;
      }

      currently_escaping = false;
    } else if (last_byte == '\\') {
      currently_escaping = true;
    } else if (last_byte == '"') {
      found_closing_double_quote = true;
      break;
    }
  }

  return found_closing_double_quote ? JsonParsingResult::kValidTypeMatch
                                    : JsonParsingResult::kInvalidTypeMatch;
}
