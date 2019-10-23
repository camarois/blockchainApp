#include <common/models.hpp>
#include <rest/token_manager.hpp>

namespace Rest {

TokenManager::TokenManager(const Pistache::Rest::Request& request){ encode(request); }
TokenManager::TokenManager(const std::string signature) : signature_(signature) {};

jwt::jwt_object TokenManager::token() const { return token_; }

std::string TokenManager::signature() const { return signature_; }

void TokenManager::encode(const Pistache::Rest::Request& request){
    Common::Models::LoginRequest loginRequest = nlohmann::json::parse(request.body());
    token_.add_claim("username", loginRequest.username)
          .add_claim("password", loginRequest.password)
          .add_claim("exp", std::chrono::system_clock::now() + std::chrono::seconds{kExpiration_});
    signature_ = token_.signature();
}

void TokenManager::decode(std::string signature) const {
    try{
        auto dec_obj = jwt::decode(signature, jwt::params::algorithms({"hs256"}), jwt::params::secret("inf3995"), jwt::params::verify(true));
    } catch (const jwt::TokenExpiredError& e) {
    //Handle Token expired exception here
    //...
    } catch (const jwt::SignatureFormatError& e) {
    //Handle invalid signature format error
    //...
    } catch (const jwt::DecodeError& e) {
    //Handle all kinds of other decode errors
    //... 
    } catch (const jwt::VerificationError& e) {
    // Handle the base verification error.
    //NOTE: There are other derived types of verification errors
    // which will be discussed in next topic.
    } catch (...) {
        std::cerr << "Caught unknown exception\n";
    }

}

}  // namespace Rest
