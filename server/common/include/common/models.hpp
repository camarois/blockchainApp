#ifndef COMMON_MODELS_HPP
#define COMMON_MODELS_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace Common {
namespace Models {

const std::string kUsername = "usager";
const std::string kPassword = "mot_de_passe";
const std::string kEdtion = "edition";
const std::string kOld = "ancien";
const std::string kNew = "nouveau";
const std::string kAcronym = "sigle";
const std::string kName = "nom";
const std::string kTrimester = "trimestre";
const std::string kResults = "resultats";
const std::string kFirstName = "prenom";
const std::string kId = "matricule";
const std::string kGrade = "note";

template <typename T>
inline T from_str(const std::string& str) {
  return nlohmann::json::parse(str);
}

template <typename T>
inline std::string to_str(const T& obj) {
  return ((nlohmann::json)obj).dump(4);
}

struct LoginRequest {
  std::string username;
  std::string password;
};

inline void to_json(nlohmann::json& j, const LoginRequest& obj) {
  j = nlohmann::json{{kUsername, obj.username}, {kPassword, obj.password}};
}

inline void from_json(const nlohmann::json& j, LoginRequest& obj) {
  j.at(kUsername).get_to(obj.username);
  j.at(kPassword).get_to(obj.password);
}

struct LoginResponse {
  bool edition;
};

inline void to_json(nlohmann::json& j, const LoginResponse& obj) {
  j = nlohmann::json{{kEdtion, obj.edition}};
}

inline void from_json(const nlohmann::json& j, LoginResponse& obj) {
  j.at(kEdtion).get_to(obj.edition);
}

struct PasswordRequest {
  std::string oldPwd;
  std::string newPwd;
};

inline void to_json(nlohmann::json& j, const PasswordRequest& obj) {
  j = {{kOld, obj.oldPwd}, {kNew, obj.newPwd}};
}

inline void from_json(const nlohmann::json& j, PasswordRequest& obj) {
  j.at(kOld).get_to(obj.oldPwd);
  j.at(kNew).get_to(obj.newPwd);
}

struct Result {
  std::string lastName;
  std::string firstName;
  std::string id;
  std::string grade;
};

inline void to_json(nlohmann::json& j, const Result& obj) {
  j = nlohmann::json{
      {kName, obj.lastName}, {kFirstName, obj.firstName}, {kId, obj.id}, {kGrade, obj.grade}};
}

inline void from_json(const nlohmann::json& j, Result& obj) {
  j[kFirstName].get_to(obj.firstName);
  j[kName].get_to(obj.lastName);
  j[kId].get_to(obj.id);
  j[kGrade].get_to(obj.grade);
}

struct TransactionRequest {
  std::string acronym;
  std::string name;
  int trimester;
  // std::vector<Result> results;
};

inline void to_json(nlohmann::json& j, const TransactionRequest& obj) {
  j = nlohmann::json{{kAcronym, obj.acronym},
  	     {kName, obj.name},
  	     {kTrimester, obj.trimester}/*,
         {kResults, obj.results}*/};
}

inline void from_json(const nlohmann::json& j, TransactionRequest& obj) {
  j[kAcronym].get_to(obj.acronym);
  j[kName].get_to(obj.name);
  j[kTrimester].get_to(obj.trimester);
  // j[kResults].get_to(obj.results);
}

}  // namespace Models
}  // namespace Common

#endif  // COMMON_MODELS_HPP
