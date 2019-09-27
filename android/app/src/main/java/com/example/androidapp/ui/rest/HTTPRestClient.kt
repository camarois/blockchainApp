package com.example.androidapp.ui.rest

import android.content.Context
import com.android.volley.Request
import com.android.volley.RequestQueue
import com.android.volley.toolbox.Volley

class HTTPRestClient(private val url: String, private val context: Context) {
    private val requestQueue: RequestQueue by lazy {
        Volley.newRequestQueue(context)
    }

    val baseURL: String
        get() = url

    fun <T> addToRequestQueue(req: Request<T>) {
        requestQueue.add(req)
    }
}
