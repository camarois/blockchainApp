package com.example.androidapp.services

import android.content.Context

object CredentialsManager {
    const val HTTP_header_authorization = "Authorization"
    private const val auth_token = "authorization_token"

    fun saveCredentials(context: Context, token: String?) {
        val sp = context.getSharedPreferences(
            HTTP_header_authorization, Context.MODE_PRIVATE)

        sp!!.edit().putString(auth_token, token)
            .apply()
    }

    fun getAuthToken(context: Context): String? {
        val sp = context.getSharedPreferences(
            HTTP_header_authorization, Context.MODE_PRIVATE)

        return sp!!.getString(auth_token, null)
    }
}
