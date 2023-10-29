#pragma once
#include <memory>
#include <string>

namespace buried {

class Crypt {
 public:
  virtual ~Crypt() = default;

  virtual std::string Encrypt(const std::string& input) = 0;

  virtual std::string Decrypt(const std::string& input) = 0;

  virtual std::string Encrypt(const void* input, size_t input_size) = 0;

  virtual std::string Decrypt(const void* input, size_t input_size) = 0;
};

class AESImpl;
// non thread safe
class AESCrypt : public Crypt {
 public:
  static std::string GetKey(const std::string& salt,
                            const std::string password);

  explicit AESCrypt(const std::string& key);

  ~AESCrypt();

  AESCrypt(const AESCrypt& other) = delete;

  AESCrypt& operator=(const AESCrypt& other) = delete;

  std::string Encrypt(const std::string& input) override;

  std::string Decrypt(const std::string& input) override;

  std::string Encrypt(const void* input, size_t input_size) override;

  std::string Decrypt(const void* input, size_t input_size) override;

 private:
  std::unique_ptr<AESImpl> impl_;
};
}  // namespace buried
