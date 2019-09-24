package com.example.androidapp.ui.rest

import android.content.Context
import android.content.SharedPreferences
import com.android.volley.Request
import com.android.volley.RequestQueue
import com.android.volley.toolbox.Volley
import com.example.androidapp.R

class HTTPRestClient(private val preferences: SharedPreferences, private val context: Context) {
    private val requestQueue: RequestQueue by lazy {
        Volley.newRequestQueue(context.applicationContext)
    }

    fun <T> addToRequestQueue(req: Request<T>) {
        requestQueue.add(req)
    }

    fun getBaseURL(): String {
        // TODO: add sharedpreferences logic
        return context.getString(R.string.host)
    }
}
