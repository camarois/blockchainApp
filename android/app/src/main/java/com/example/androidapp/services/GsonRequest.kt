package com.example.androidapp.services

import android.content.Context
import android.util.Base64
import com.android.volley.*
import com.android.volley.toolbox.HttpHeaderParser
import com.google.gson.Gson
import com.google.gson.JsonSyntaxException
import java.io.UnsupportedEncodingException

// Inspired by : https://developer.android.com/training/volley/request-custom#example:-gsonrequest
class GsonRequest <T> (
    private val context: Context,
    private val credentialsManager: CredentialsManager,
    method: Int,
    url: String,
    private val body: Any = "",
    private val classOfT: Class<T>,
    private val isFile: Boolean = false,
    private val headers: MutableMap<String, String?>?,
    private val listener: Response.Listener<T>,
    errorListener: Response.ErrorListener?
) : Request<T>(method, url, errorListener
) {
    private val gson = Gson()

    override fun getHeaders(): MutableMap<String, String?> {
        val params = headers ?: super.getHeaders()
        params[CredentialsManager.HTTP_HEADER_AUTHORIZATION] ?: credentialsManager.getAuthToken(context)
        return params
    }

    override fun getBody(): ByteArray = gson.toJson(body).toByteArray()
    override fun deliverResponse(response: T) = listener.onResponse(response)

    override fun parseNetworkResponse(response: NetworkResponse?): Response<T> {
        return try {
            val json = String(
                response?.data ?: ByteArray(0),
                Charsets.UTF_8)
            val token = response?.headers?.get(CredentialsManager.HTTP_HEADER_AUTHORIZATION)
            credentialsManager.saveCredentials(context, token)

            val bodyParsed = if (isFile) Base64.decode(json, Base64.NO_WRAP) as T else gson.fromJson(json, classOfT)

            Response.success(
                bodyParsed,
                HttpHeaderParser.parseCacheHeaders(response))
        } catch (e: UnsupportedEncodingException) {
            Response.error(ParseError(e))
        } catch (e: JsonSyntaxException) {
            Response.error(ParseError(e))
        }
    }
}
