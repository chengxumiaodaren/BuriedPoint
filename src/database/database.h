#pragma once

#include <stdint.h>

#include <memory>
#include <string>
#include <vector>

namespace buried {

class BuriedDbImpl;
class BuriedDb {
 public:
  struct Data {
    int32_t id;
    int32_t priority;
    uint64_t timestamp;
    std::string content;
  };

 public:
  BuriedDb(std::string db_path);

  ~BuriedDb();

  void InsertData(const Data& data);

  void DeleteData(const Data& data);

  void DeleteDatas(const std::vector<Data>& datas);

  std::vector<Data> QueryData(int32_t limit);

 private:
  std::unique_ptr<BuriedDbImpl> impl_;
};

}  // namespace buried
