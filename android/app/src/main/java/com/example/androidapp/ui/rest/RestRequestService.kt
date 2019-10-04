package com.example.androidapp.ui.rest

import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.StringRequest
import org.json.JSONObject
import kotlin.coroutines.resume
import kotlin.coroutines.suspendCoroutine

class RestRequestService(private val httpClient: HTTPRestClient) {
    private lateinit var serverUrl: String

    init {
        initServerUrl("server") // TODO change with dev user with service.
    }

    private fun initServerUrl(user: String) {
        val baseUrl = "https://us-central1-projet3-46f1b.cloudfunctions.net/getServerURL?user=$user"
        val request = StringRequest(
            Request.Method.GET, baseUrl, {
                serverUrl = "http://$it:10000"
                println(serverUrl)
            }, {
                serverUrl = it.toString()
            })
        httpClient.addToRequestQueue(request)
    }

    suspend fun getLol(): String {
        return suspendCoroutine { continuation ->
            val request = StringRequest(
                Request.Method.GET, "$serverUrl/lol",
                Response.Listener<String> { response ->
                    continuation.resume(JSONObject(response).getInt("lol").toString())
                },
                Response.ErrorListener {})
            httpClient.addToRequestQueue(request)
        }
    }
}
