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
