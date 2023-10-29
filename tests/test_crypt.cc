#include "gtest/gtest.h"
#include "src/crypt/crypt.h"

TEST(CryptTest, BasicTest) {
  std::string key = buried::AESCrypt::GetKey("salt", "password");
  EXPECT_TRUE(!key.empty());

  buried::Crypt* crypt = new buried::AESCrypt(key);
  std::string input = "hello world";
  std::string encrypted = crypt->Encrypt(input);
  EXPECT_TRUE(!encrypted.empty());
  std::string decrypted = crypt->Decrypt(encrypted);
  EXPECT_TRUE(!decrypted.empty());
  EXPECT_EQ(input, decrypted);
  delete crypt;
}
