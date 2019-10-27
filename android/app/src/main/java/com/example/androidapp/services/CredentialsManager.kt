package com.example.androidapp.services

import android.content.Context

object CredentialsManager {
    const val HTTP_HEADER_AUTHORIZATION = "Authorization"
    private const val AUTH_TOKEN = "authorization_token"

    fun saveCredentials(context: Context, token: String?) {
        val sp = context.getSharedPreferences(
            HTTP_HEADER_AUTHORIZATION, Context.MODE_PRIVATE)

        sp!!.edit().putString(AUTH_TOKEN, token)
            .apply()
    }

    fun getAuthToken(context: Context): String {
        val sp = context.getSharedPreferences(
            HTTP_HEADER_AUTHORIZATION, Context.MODE_PRIVATE)

        return sp!!.getString(AUTH_TOKEN, null)
    }
}
