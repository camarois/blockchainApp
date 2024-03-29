package com.example.androidapp
import com.google.gson.annotations.SerializedName

data class LoginRequest(
    @SerializedName("usager") val username: String,
    @SerializedName("mot_de_passe") val password: String
)

data class LoginResponse(
    @SerializedName("edition") val edition: Boolean
)

data class PasswordRequest(
    @SerializedName("ancien") val oldPassword: String,
    @SerializedName("nouveau") val newPassword: String
)

data class StudentRequest(
    @SerializedName("sigle") val acronym: String,
    @SerializedName("trimestre") val trimester: String,
    @SerializedName("matricule") val code: String
)

data class StudentResponse(
    @SerializedName("resultatsEtudiant") val results: List<CourseResult>
)

data class CourseResult(
    @SerializedName("note") val grade: String,
    @SerializedName("sigle") val acronym: String,
    @SerializedName("trimestre") val trimester: Int
)

data class CourseRequest(
    @SerializedName("sigle") val acronym: String,
    @SerializedName("trimestre")val trimester: Int
)

data class CourseResponse(
    @SerializedName("resultats") val students: List<StudentItem>
)

data class TransactionRequest(
    @SerializedName("sigle") val classCode: String,
    @SerializedName("nom") val className: String,
    @SerializedName("trimestre") val trimester: Number,
    @SerializedName("resultats") val results: List<StudentItem>,
    @SerializedName("pdf") val pdf: String
)

data class StudentItem(
    @SerializedName("nom") val lastName: String,
    @SerializedName("prenom")val firstName: String,
    @SerializedName("matricule") val code: String,
    @SerializedName("note") val grade: String
)

data class CourseItem(
    @SerializedName("sigle") val code: String,
    @SerializedName("nom") val name: String,
    @SerializedName("trimestre") val trimester: Number
)

data class ListStudent(
    @SerializedName("listeEtudiant") val listeEtudiant: List<StudentItem>
)

data class ListClass(
    @SerializedName("listeClasses") val listeClasses: List<CourseItem>
)

data class PdfFileRequest(
    @SerializedName("sigle") val acronym: String,
    @SerializedName("trimestre") val trimester: Int
)

enum class AccountTypes {
    CONSULTATION, EDITION
}
