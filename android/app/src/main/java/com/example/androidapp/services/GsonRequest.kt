package com.example.androidapp.services

import android.content.Context
import com.android.volley.NetworkResponse
import com.android.volley.ParseError
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.HttpHeaderParser
import com.google.gson.Gson
import com.google.gson.JsonSyntaxException
import java.io.UnsupportedEncodingException
import java.nio.charset.Charset

class GsonRequest <T> (
    private val context: Context,
    method: Int,
    url: String,
    private val body: String = "",
    private val classOfT: Class<T>,
    private val headers: MutableMap<String, String>?,
    private val listener: Response.Listener<T>,
    errorListener: Response.ErrorListener?
) : Request<T>(method, url, errorListener
) {
    private val gson = Gson()

    override fun getHeaders(): MutableMap<String, String> {
        val params = headers ?: super.getHeaders()
        params[CredentialsManager.HTTP_HEADER_AUTHORIZATION] ?: CredentialsManager.getAuthToken(context)
        return params
    }

    override fun getBody(): ByteArray = body.toByteArray()
    override fun deliverResponse(response: T) = listener.onResponse(response)

    override fun parseNetworkResponse(response: NetworkResponse?): Response<T> {
        return try {
            val json = String(
                response?.data ?: ByteArray(0),
                Charset.forName(HttpHeaderParser.parseCharset(response?.headers)))

            val token = response?.headers?.get(CredentialsManager.HTTP_HEADER_AUTHORIZATION)
            CredentialsManager.saveCredentials(context, token)
            Response.success(
                gson.fromJson(json, classOfT),
                HttpHeaderParser.parseCacheHeaders(response))
        } catch (e: UnsupportedEncodingException) {
            Response.error(ParseError(e))
        } catch (e: JsonSyntaxException) {
            Response.error(ParseError(e))
        }
    }
}
