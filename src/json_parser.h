//
//  Copyright 2025 Amr Saqr
//

#ifndef JSON_PARSER_H_
#define JSON_PARSER_H_

#include <list>
#include <string>

#include "json_parsing_result.h"
#include "limited_history_preserving_reader.h"

class JsonParser {
 public:
  bool Parse(const std::string& file_path, std::string* error_output);

 private:
  JsonParsingResult TryParseJsonRoot(
      LimitedHistoryPreservingReader* historical_reader);
  JsonParsingResult TryParseJsonValue(
      LimitedHistoryPreservingReader* historical_reader);
  JsonParsingResult TryParseObject(LimitedHistoryPreservingReader* reader);
  JsonParsingResult TryParseArray(LimitedHistoryPreservingReader* reader);
  void AddParsingError(const char* parsing_error);
  static constexpr size_t kMaxJsonDepth = 19;
  static constexpr const char* const kErrInvalidNull = "Invalid null constant";
  static constexpr const char* const kErrInvalidBool = "Invalid bool constant";
  static constexpr const char* const kErrInvalidNumber = "Invalid number value";
  static constexpr const char* const kErrInvalidString = "Invalid string value";
  static constexpr const char* const kErrInvalidArray =
      "Invalid array structure";
  static constexpr const char* const kErrArrayValueExpected =
      "A valid array value was expected";
  static constexpr const char* const kErrArrayCommaOrClosureExpected =
      "A comma or array closure was expected";
  static constexpr const char* const kErrArrayClosureExpected =
      "An array closure was expected";

  static constexpr const char* const kErrInvalidObject =
      "Invalid object structure";
  static constexpr const char* const kErrObjectKeyExpected =
      "A valid object key was expected";
  static constexpr const char* const kErrObjectValueExpected =
      "A valid object value was expected";
  static constexpr const char* const kErrObjectColonExpected =
      "A colon was expected";
  static constexpr const char* const kErrObjectCommaOrClosureExpected =
      "A comma or object closure was expected";
  static constexpr const char* const kErrObjectClosureExpected =
      "An object closure was expected";

  static constexpr const char* const kErrInvalidRoot =
      "A valid root object or root array was expected";
  static constexpr const char* const kErrAdditionalCharacters =
      "Additional characters after reading a valid JSON root";
  static constexpr const char* const kErrMaxDepthReached =
      "Max JSON depth reached";
  size_t json_depth_ = 0;
  std::list<std::string> parsing_errors_;
};

#endif  // JSON_PARSER_H_
