package com.example.androidapp.ui.rest

import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.StringRequest
import org.json.JSONObject
import kotlin.coroutines.resume
import kotlin.coroutines.suspendCoroutine

class RestRequestService(
    private val httpClient: HTTPRestClient
) {
    suspend fun getLol(): String {
        return suspendCoroutine { continuation ->
            val request = StringRequest(
                Request.Method.GET, httpClient.baseURL + "/lol",
                Response.Listener<String> { response ->
                    continuation.resume(JSONObject(response).getInt("lol").toString())
                },
                Response.ErrorListener {})
            httpClient.addToRequestQueue(request)
        }
    }
}
