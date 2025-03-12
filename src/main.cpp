//  Copyright 2025 Amr Saqr

#include <fstream>
#include <iostream>

#include "historical_reader.h"
#include "json_utility.h"
using std::cout;
using std::endl;
using std::ifstream;

int main(const int argc, const char* const argv[]) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <input_file_path>" << endl;
    return 1;
  }

  ifstream input_file(argv[1]);
  if (!input_file.is_open()) {
    cout << "Unable to open file " << argv[1] << endl;
    return 1;
  }

  HistoricalReader reader(&input_file);

  bool success = JsonUtility::TryParseJsonRoot(&reader) ==
                 JsonParsingResult::kValidTypeMatch;
  input_file.close();

  if (success) {
    cout << "Valid JSON" << endl;
  } else {
    cout << "Invalid JSON" << endl;
    cout << reader.GetHistory() << endl;
  }

  return success ? 0 : 1;
}
