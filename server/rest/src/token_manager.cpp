#include <common/database.hpp>
#include <common/models.hpp>
#include <gflags/gflags.h>
#include <rest/token_manager.hpp>

DECLARE_string(database);

namespace Rest {

TokenManager::TokenManager(const std::string& username, const std::string& password) { encode(username, password); }
TokenManager::TokenManager(const std::string& signature) : signature_(signature){};

jwt::jwt_object TokenManager::getToken() const { return token_; }

std::string TokenManager::getSignature() const { return signature_; }

void TokenManager::encode(const std::string& username, const std::string& password) {
  token_.add_claim("username", username)
      .add_claim("password", password)
      .add_claim("exp", std::chrono::system_clock::now() + std::chrono::seconds{kExpirationTimeZ_});
  signature_ = token_.signature(errCode_);
}

void TokenManager::refresh() {
  token_.remove_claim("exp");
  token_.add_claim("exp", std::chrono::system_clock::now() + std::chrono::seconds{kExpirationTimeMax2_});
  signature_ = token_.signature(errCode_);
}

void TokenManager::decode(const std::string& signature) {
  try {
    jwt::jwt_object decodedObj =
        jwt::decode(signature, jwt::params::algorithms({"hs256"}), errCode_, jwt::params::secret("inf3995"));

    if (errCode_.value() == static_cast<int>(jwt::VerificationErrc::TokenExpired)) {
      std::string username = decodedObj.payload().get_claim_value<std::string>(kUsername_);
      std::string password = decodedObj.payload().get_claim_value<std::string>(kPassword_);

      Common::Database db = Common::Database(FLAGS_database);
      auto user = db.getUser(username);

      if (user->username == username && user->password == password) {
        this->refresh();
      }
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

}  // namespace Rest
