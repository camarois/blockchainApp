package com.example.androidapp.services

import android.content.Context
import org.apache.commons.lang3.time.DateUtils
import io.jsonwebtoken.Jwts
import io.jsonwebtoken.SignatureAlgorithm
import java.util.*
import kotlin.collections.HashMap
import io.jsonwebtoken.security.Keys

class CredentialsManager {
    companion object {
        const val HTTP_HEADER_AUTHORIZATION = "Authorization"
        private const val AUTH_TOKEN = "authorization_token"
    }

    fun saveCredentials(context: Context, token: String?) {
        val sp = context.getSharedPreferences(
            HTTP_HEADER_AUTHORIZATION, Context.MODE_PRIVATE)

        sp!!.edit().putString(AUTH_TOKEN, token)
            .commit()
    }

    fun getAuthToken(context: Context): String? {
        val sp = context.getSharedPreferences(
            HTTP_HEADER_AUTHORIZATION, Context.MODE_PRIVATE)

        return sp!!.getString(AUTH_TOKEN, null)
    }

    fun saveFirstAuthToken(context: Context, username: String, password: String) {
        val claims: HashMap<String, Any?> = HashMap()
        claims["username"] = username
        claims["password"] = password

        val expiryTime: Date = DateUtils.addHours(Date(), 1)
        val key = Keys.hmacShaKeyFor((HTTP_HEADER_AUTHORIZATION + "inf3995" + AUTH_TOKEN).toByteArray())

        var token: String = Jwts.builder().setClaims(claims)
            .setExpiration(expiryTime)
            .signWith(key, SignatureAlgorithm.HS256)
            .compact()

        val sp = context.getSharedPreferences(
            HTTP_HEADER_AUTHORIZATION, Context.MODE_PRIVATE)

        sp!!.edit().putString(AUTH_TOKEN, token)
            .commit()
    }
}
