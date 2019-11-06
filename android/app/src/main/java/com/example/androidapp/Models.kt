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

data class CourseResult(
    @SerializedName("sigle") val acronym: String,
    @SerializedName("trimestre") val trimester: String,
    @SerializedName("note") val grade: String
)

data class Student(
    @SerializedName("nom") val lastName: String,
    @SerializedName("prenom")val firstName: String,
    @SerializedName("matricule") val code: String,
    @SerializedName("note") val grade: String) {
    override fun toString(): String = "Nom de famille: $lastName, Prénom: $firstName, Matricule: $code"
}
