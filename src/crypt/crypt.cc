#include "crypt/crypt.h"

#include <string>

#include "third_party/mbedtls/include/mbedtls/cipher.h"
#include "third_party/mbedtls/include/mbedtls/error.h"
#include "third_party/mbedtls/include/mbedtls/md.h"
#include "third_party/mbedtls/include/mbedtls/pkcs5.h"

namespace buried {
std::string AESCrypt::GetKey(const std::string& salt,
                             const std::string password) {
  int32_t keylen = 32;
  uint32_t iterations = 1000;
  unsigned char key[32] = {0};
  mbedtls_md_context_t md_ctx;
  mbedtls_md_init(&md_ctx);
  const mbedtls_md_info_t* md_info =
      mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
  mbedtls_md_setup(&md_ctx, md_info, 1);
  mbedtls_md_starts(&md_ctx);
  int ret = mbedtls_pkcs5_pbkdf2_hmac(
      &md_ctx, reinterpret_cast<const unsigned char*>(password.data()),
      password.size(), reinterpret_cast<const unsigned char*>(salt.data()),
      salt.size(), iterations, keylen, key);
  mbedtls_md_free(&md_ctx);
  if (ret != 0) {
    return "";
  }
  return std::string((char*)key, keylen);
}

class AESImpl {
 public:
  explicit AESImpl(const std::string& key) { Init(key.data(), key.size()); }

  ~AESImpl() { UnInit(); }

  AESImpl(const AESImpl& other) = delete;
  AESImpl& operator=(const AESImpl& other) = delete;

  void Init(const char* key, size_t key_size);

  void UnInit();

  std::string Encrypt(const void* input, size_t input_size);

  std::string Decrypt(const void* input, size_t input_size);

 private:
  mbedtls_cipher_context_t encrypt_ctx_;
  mbedtls_cipher_context_t decrypt_ctx_;

  uint32_t encrypt_block_size_ = 0;
  uint32_t decrypt_block_size_ = 0;

  unsigned char iv_[16] = {0};
};

void AESImpl::Init(const char* key, size_t key_size) {
  mbedtls_cipher_init(&encrypt_ctx_);
  mbedtls_cipher_setup(
      &encrypt_ctx_, mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_256_CBC));
  mbedtls_cipher_set_padding_mode(&encrypt_ctx_, MBEDTLS_PADDING_PKCS7);
  mbedtls_cipher_setkey(&encrypt_ctx_,
                        reinterpret_cast<const unsigned char*>(key),
                        key_size * 8, MBEDTLS_ENCRYPT);

  encrypt_block_size_ = mbedtls_cipher_get_block_size(&encrypt_ctx_);

  mbedtls_cipher_init(&decrypt_ctx_);
  mbedtls_cipher_setup(
      &decrypt_ctx_, mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_256_CBC));
  mbedtls_cipher_set_padding_mode(&decrypt_ctx_, MBEDTLS_PADDING_PKCS7);
  mbedtls_cipher_setkey(&decrypt_ctx_,
                        reinterpret_cast<const unsigned char*>(key),
                        key_size * 8, MBEDTLS_DECRYPT);

  decrypt_block_size_ = mbedtls_cipher_get_block_size(&decrypt_ctx_);
}

void AESImpl::UnInit() {
  mbedtls_cipher_free(&encrypt_ctx_);
  mbedtls_cipher_free(&decrypt_ctx_);
}

std::string AESImpl::Encrypt(const void* input, size_t input_size) {
  mbedtls_cipher_set_iv(&encrypt_ctx_, iv_, sizeof(iv_));
  mbedtls_cipher_reset(&encrypt_ctx_);

  std::string output(input_size + encrypt_block_size_, 0);
  size_t olen = 0;
  int ret = mbedtls_cipher_update(
      &encrypt_ctx_, reinterpret_cast<const unsigned char*>(input), input_size,
      reinterpret_cast<unsigned char*>(output.data()), &olen);
  if (ret != 0) {
    return "";
  }
  size_t olen2 = 0;
  ret = mbedtls_cipher_finish(
      &encrypt_ctx_, reinterpret_cast<unsigned char*>(output.data()) + olen,
      &olen2);
  if (ret != 0) {
    return "";
  }
  output.resize(olen + olen2);
  return output;
}

std::string AESImpl::Decrypt(const void* input, size_t input_size) {
  mbedtls_cipher_set_iv(&decrypt_ctx_, iv_, sizeof(iv_));
  mbedtls_cipher_reset(&decrypt_ctx_);

  std::string output(input_size + decrypt_block_size_, 0);
  size_t olen = 0;
  int ret = mbedtls_cipher_update(
      &decrypt_ctx_, reinterpret_cast<const unsigned char*>(input), input_size,
      reinterpret_cast<unsigned char*>(output.data()), &olen);
  if (ret != 0) {
    return "";
  }
  size_t olen2 = 0;
  ret = mbedtls_cipher_finish(
      &decrypt_ctx_, reinterpret_cast<unsigned char*>(output.data()) + olen,
      &olen2);
  if (ret != 0) {
    return "";
  }
  output.resize(olen + olen2);
  return output;
}

AESCrypt::AESCrypt(const std::string& key)
    : impl_(std::make_unique<AESImpl>(key)) {}

AESCrypt::~AESCrypt() {}

std::string AESCrypt::Encrypt(const std::string& input) {
  return impl_->Encrypt(input.data(), input.size());
}

std::string AESCrypt::Decrypt(const std::string& input) {
  return impl_->Decrypt(input.data(), input.size());
}

std::string AESCrypt::Encrypt(const void* input, size_t input_size) {
  return impl_->Encrypt(input, input_size);
}

std::string AESCrypt::Decrypt(const void* input, size_t input_size) {
  return impl_->Decrypt(input, input_size);
}

}  // namespace buried
