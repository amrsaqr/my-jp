//
//  Copyright 2025 Amr Saqr
//

#include "json_parser.h"

#include <fstream>
#include <string>

#include "historical_reader.h"
#include "json_utility.h"
using std::ifstream;
using std::string;

JsonParser::JsonParser(const char* const file_path) : file_path_(file_path) {}

bool JsonParser::Parse(string* error_output) const {
  ifstream input_stream(file_path_);

  if (!input_stream.is_open()) {
    *error_output = "Could not open file " + file_path_;
    return false;
  }

  HistoricalReader reader(&input_stream);
  bool successful_parsing = JsonUtility::TryParseJsonRoot(&reader) ==
                            JsonParsingResult::kValidTypeMatch;

  if (!successful_parsing) {
    *error_output = reader.GetHistory();
  }

  return successful_parsing;
}
