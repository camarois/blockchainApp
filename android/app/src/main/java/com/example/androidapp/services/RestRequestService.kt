package com.example.androidapp.services

import android.content.Context
import com.android.volley.Request
import com.android.volley.toolbox.JsonObjectRequest
import com.android.volley.toolbox.StringRequest
import com.android.volley.toolbox.Volley
import com.example.androidapp.LoginRequest
import com.example.androidapp.LoginResponse
import org.json.JSONObject
import kotlin.coroutines.resume
import kotlin.coroutines.suspendCoroutine
import com.google.gson.Gson
import kotlin.coroutines.resumeWithException

class RestRequestService(private val httpClient: HTTPRestClient, private val context: Context) {
    private lateinit var serverUrl: String
    private var gson = Gson()

    init {
        initServerUrl("server")
    }

    fun initServerUrl(user: String) {
        val baseUrl = "https://us-central1-projet3-46f1b.cloudfunctions.net/getServerURL?user=$user"
        val request = StringRequest(
            Request.Method.GET, baseUrl, {
                serverUrl = "https://$it:10000"
                httpClient.initHttps()
            }, {
                serverUrl = it.toString()
            })
        val requestQueue = Volley.newRequestQueue(context)
        requestQueue.add(request)
    }

    suspend fun getPingAsync(): String {
        return getAsync("ping")
    }

    suspend fun postLoginAsync(request: LoginRequest): LoginResponse {
        return postAsync("usager/login", request, LoginResponse::class.java)
    }

    suspend fun getAsync(url: String): String {
        return suspendCoroutine { continuation ->
            val request = StringRequest("$serverUrl/$url",
                { response ->
                    continuation.resume(response)
                },
                {
                    continuation.resumeWithException(it)
                })
            httpClient.addToRequestQueue(request)
        }
    }

    suspend fun <T> getAsync(url: String, classOfT: Class<T>): T {
        return suspendCoroutine { continuation ->
            val request = StringRequest("$serverUrl/$url",
                { response ->
                    continuation.resume(gson.fromJson(response, classOfT))
                },
                {
                    continuation.resumeWithException(it)
                })
            httpClient.addToRequestQueue(request)
        }
    }

    suspend fun postAsync(url: String, data: Any): String {
        return suspendCoroutine { continuation ->
            val request = JsonObjectRequest(
                Request.Method.POST, "$serverUrl/$url", JSONObject(gson.toJson(data)),
                { response ->
                    continuation.resume(response.toString())
                },
                {
                    continuation.resumeWithException(it)
                })
            httpClient.addToRequestQueue(request)
        }
    }

    suspend fun <T> postAsync(url: String, data: Any, classOfT: Class<T>): T {
        return suspendCoroutine { continuation ->
            val request = JsonObjectRequest(
                Request.Method.POST, "$serverUrl/$url", JSONObject(gson.toJson(data)),
                { response ->
                    continuation.resume(gson.fromJson(response.toString(), classOfT))
                },
                {
                    continuation.resumeWithException(it)
                })
            httpClient.addToRequestQueue(request)
        }
    }
}
