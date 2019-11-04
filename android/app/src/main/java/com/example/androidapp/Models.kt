package com.example.androidapp

import com.example.androidapp.ui.fragments.search.student.StudentItem
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

data class TransactionRequest(
    @SerializedName("sigle") val classCode: String,
    @SerializedName("nom") val className: String,
    @SerializedName("trimestre") val trimester: String,
    @SerializedName("resultats") val results: List<StudentItem>, // todo - figure it out
    @SerializedName("pdf") val pdf: String
)