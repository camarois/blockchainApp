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
    @SerializedName("resultats") val results: List<CourseResult>
)

data class CourseResult(
    @SerializedName("sigle") val acronym: String,
    @SerializedName("trimestre") val trimester: String,
    @SerializedName("note") val grade: String
)

data class CourseRequest(
    @SerializedName("trimestre")val trimester: String
)

data class CourseResponse(
    @SerializedName("etudiants") val students: List<StudentItem>
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

data class PdfFileRequest(
    @SerializedName("sigle") val acronym: String,
    @SerializedName("trimestre") val trimester: String
)