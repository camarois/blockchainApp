package com.example.androidapp.ui.rest

import android.content.Context
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.StringRequest
import com.android.volley.toolbox.Volley
import kotlin.coroutines.resume
import kotlin.coroutines.suspendCoroutine

class RestRequestService(private val httpClient: HTTPRestClient, private val context: Context) {
    private lateinit var serverUrl: String

    init {
        initServerUrl("server") // TODO change with dev user with service.
    }

    private fun initServerUrl(user: String) {
        val baseUrl = "https://us-central1-projet3-46f1b.cloudfunctions.net/getServerURL?user=$user"
        val request = StringRequest(
            Request.Method.GET, baseUrl, {
                serverUrl = "https://$it:10000"
                println(serverUrl)
            }, {
                serverUrl = it.toString()
            })
        val requestQueue = Volley.newRequestQueue(context)
        requestQueue.add(request)
    }

    suspend fun getLol(): String {
        return suspendCoroutine { continuation ->
            val request = StringRequest(
                Request.Method.GET, "$serverUrl/ping",
                Response.Listener<String> { response ->
                    continuation.resume(response.toString())
                },
                Response.ErrorListener {
                    continuation.resume(it.message.toString())
                })
            httpClient.addToRequestQueue(request)
        }
    }
}
