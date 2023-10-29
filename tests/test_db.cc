#include <filesystem>

#include "gtest/gtest.h"
#include "src/database/database.h"

TEST(DbTest, DISABLED_BasicTest) {
  std::filesystem::path db_path("hello_world.db");

  if (std::filesystem::exists(db_path)) {
    std::filesystem::remove(db_path);
  }

  buried::BuriedDb db(db_path.string());
  {
    buried::BuriedDb::Data data{-1, 1, 2,
                                std::vector<char>{'h', 'e', 'l', 'l', 'o'}};
    db.InsertData(data);
  }
  auto datas = db.QueryData(10);
  EXPECT_EQ(datas.size(), 1);
  {
    buried::BuriedDb::Data data{-1, 2, 3,
                                std::vector<char>{'h', 'e', 'l', 'l', 'o'}};
    db.InsertData(data);
  }
  datas = db.QueryData(10);
  EXPECT_EQ(datas.size(), 2);
  {
    buried::BuriedDb::Data data{-1, 3, 4,
                                std::vector<char>{'h', 'e', 'l', 'l', 'o'}};
    db.InsertData(data);
  }
  datas = db.QueryData(10);
  EXPECT_EQ(datas.size(), 3);
  {
    buried::BuriedDb::Data data{-1, 4, 5,
                                std::vector<char>{'h', 'e', 'l', 'l', 'o'}};
    db.InsertData(data);
  }
  datas = db.QueryData(10);
  EXPECT_EQ(datas.size(), 4);
  EXPECT_EQ(datas[0].priority, 4);
  EXPECT_EQ(datas[1].priority, 3);
  EXPECT_EQ(datas[2].priority, 2);
  EXPECT_EQ(datas[3].priority, 1);

  db.DeleteData(datas[0]);
  datas = db.QueryData(10);
  EXPECT_EQ(datas.size(), 3);

  for (int i = 0; i < 100; ++i) {
    buried::BuriedDb::Data data{-1, i, i,
                                std::vector<char>{'h', 'e', 'l', 'l', 'o'}};
    db.InsertData(data);
  }

  datas = db.QueryData(10);
  EXPECT_EQ(datas.size(), 10);

  // remove db_path
  std::filesystem::remove(db_path);
}
