package com.example.androidapp.ui.rest

import android.content.Context
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.StringRequest
import com.example.androidapp.R
import org.json.JSONObject
import kotlin.coroutines.resume
import kotlin.coroutines.suspendCoroutine

class RestRequestService(
    private val appCtx: Context,
    private val httpClient: HTTPRestClient
) {
     suspend fun getLol(): String {
         return suspendCoroutine { continuation ->
             val request = StringRequest(
                 Request.Method.GET, httpClient.getBaseURL() + "/lol",
                 Response.Listener<String> { response ->
                     continuation.resume(JSONObject(response).getInt("lol").toString())
                 },
                 Response.ErrorListener {
                     continuation.resume(appCtx.getString(R.string.error_message_unknown))
                 })
             httpClient.addToRequestQueue(request)
         }
    }
}
