#include "database/database.h"

#include "third_party/sqlite/sqlite_orm.h"

using namespace sqlite_orm;
namespace buried {

inline auto InitStorage(const std::string& path) {
  return make_storage(
      path, make_table("buried_data",
                       make_column("id", &BuriedDb::Data::id,
                                   primary_key().autoincrement()),
                       make_column("priority", &BuriedDb::Data::priority),
                       make_column("timestamp", &BuriedDb::Data::timestamp),
                       make_column("content", &BuriedDb::Data::content)));
}

class BuriedDbImpl {
 public:
  using DBStorage = decltype(InitStorage(""));

 public:
  BuriedDbImpl(std::string db_path) : db_path_(db_path) {
    storage_ = std::make_unique<DBStorage>(InitStorage(db_path_));
    storage_->sync_schema();
  }

  ~BuriedDbImpl() {}

  void InsertData(const BuriedDb::Data& data) {
    auto guard = storage_->transaction_guard();
    storage_->insert(data);
    guard.commit();
  }

  void DeleteData(const BuriedDb::Data& data) {
    auto guard = storage_->transaction_guard();
    storage_->remove_all<BuriedDb::Data>(
        where(c(&BuriedDb::Data::id) == data.id));
    guard.commit();
  }

  void DeleteDatas(const std::vector<BuriedDb::Data>& datas) {
    auto guard = storage_->transaction_guard();
    for (const auto& data : datas) {
      storage_->remove_all<BuriedDb::Data>(
          where(c(&BuriedDb::Data::id) == data.id));
    }
    guard.commit();
  }

  std::vector<BuriedDb::Data> QueryData(int32_t limit_size) {
    auto limited = storage_->get_all<BuriedDb::Data>(
        order_by(&BuriedDb::Data::priority).desc(), limit(limit_size));
    return limited;
  }

 private:
  std::string db_path_;

  std::unique_ptr<DBStorage> storage_;
};

BuriedDb::BuriedDb(std::string db_path)
    : impl_{std::make_unique<BuriedDbImpl>(std::move(db_path))} {}

BuriedDb::~BuriedDb() {}

void BuriedDb::InsertData(const Data& data) { impl_->InsertData(data); }

void BuriedDb::DeleteData(const Data& data) { impl_->DeleteData(data); }

void BuriedDb::DeleteDatas(const std::vector<Data>& datas) {
  impl_->DeleteDatas(datas);
}

std::vector<BuriedDb::Data> BuriedDb::QueryData(int32_t limit) {
  return impl_->QueryData(limit);
}

}  // namespace buried
