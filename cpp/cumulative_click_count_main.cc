#include <iostream>
#include <memory>
#include <cassert>
#include <stdint.h>
#include <chrono>
#include <sstream>
#include <limits>
#include <cassert>
#include <numeric>
#include <cstdlib>

#include <arrow/ipc/feather.h>
#include <arrow/io/file.h>
#include <arrow/status.h>
#include <arrow/table.h>
#include <arrow/array.h>
#include <arrow/type.h>
#include <arrow/builder.h>
#include <bitset>
#include <arrow/io/interfaces.h>
#include <unordered_map>
#include "feature_calculator.h"
using namespace std;

class CumulativeClickCount : public GroupedFeatureCalculator<uint16_t, arrow::UInt16Type> {
public:
  CumulativeClickCount(): GroupedFeatureCalculator() {}
  
  virtual string name() {
    return "CumulativeClickCount";
  }

  virtual vector<uint16_t> calculate_feature(const unordered_map<uint64_t, vector<size_t>> &grouped_click_times) {
    vector<uint16_t> feature(ip.size());
    for (const auto &entry : grouped_click_times) {
      const auto &group_click_times = entry.second;
      for (size_t index = 0; index < group_click_times.size(); index++) {
        feature[group_click_times[index]] = index;
      }
    }
    return feature;
  }
};

int main(int argc, char **argv)
{

  if (argc != 5) {
    fprintf(stderr, "Usage: %s (input_train_feather) (input_test_feather)"
            " (output_train_feather) (output_test_feather)\n", argv[0]);
    exit(-1);
  }
  
  CumulativeClickCount feature_calculator;
  arrow::Status status =feature_calculator.calculate(argv[1], argv[2], argv[3], argv[4]);
  assert(status.ok());
}
