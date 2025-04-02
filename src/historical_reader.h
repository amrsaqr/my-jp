//
//  Copyright 2025 Amr Saqr
//

#ifndef HISTORICAL_READER_H_
#define HISTORICAL_READER_H_

#include <fstream>
#include <list>
#include <string>

class HistoricalReader {
 public:
  explicit HistoricalReader(std::ifstream* input_stream);

  bool HasNextByte() const;

  char TestNextByte(bool skip_white_space = false);

  char GetNextByte(bool skip_white_space = false);

  std::string GetHistory() const;

  virtual ~HistoricalReader();

 private:
  void SkipWhitespace();
  void UpdateHistory(char byte);

  std::ifstream* input_stream_;
  std::list<char> history_;
  size_t history_length_no_ws = 0;
  const size_t kMaxHistoryLength = 21;
};

#endif  // HISTORICAL_READER_H_
