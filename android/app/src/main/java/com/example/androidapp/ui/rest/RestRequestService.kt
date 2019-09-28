package com.example.androidapp.ui.rest

import android.app.AlertDialog
import android.content.Context
import android.widget.EditText
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.StringRequest
import org.json.JSONObject
import kotlin.coroutines.resume
import kotlin.coroutines.suspendCoroutine

class RestRequestService(private val httpClient: HTTPRestClient, private val context: Context) {
    private lateinit var serverUrl: String

    init {
        setServerUrlWithDialog()
    }

    private fun setServerUrlWithDialog() {
        val userDialog: AlertDialog = context.let {
            val builder = AlertDialog.Builder(it)
            val input = EditText(context)
            builder.apply {
                setTitle("Entrez votre nom de dev")
                setMessage("Ne rien mettre = server")
                setPositiveButton("Se connecter") { dialog, _ ->
                    initServerUrl(if (input.text.isNotEmpty()) input.text.toString() else "server")
                    dialog.cancel()
                }
                setView(input)
            }

            builder.create()
        }
        userDialog.setCancelable(false)
        userDialog.show()
    }

    private fun initServerUrl(user: String) {
        val baseUrl = "https://us-central1-projet3-46f1b.cloudfunctions.net/getServerURL?user=$user"
        val request = StringRequest(
            Request.Method.GET, baseUrl, {
                serverUrl = "http://$it:10000/lol"
                println(serverUrl)
            }, {
                serverUrl = it.toString()
            })
        httpClient.addToRequestQueue(request)
    }

    suspend fun getLol(): String {
        return suspendCoroutine { continuation ->
            val request = StringRequest(
                Request.Method.GET, "http://$serverUrl:10000/lol",
                Response.Listener<String> { response ->
                    continuation.resume(JSONObject(response).getInt("lol").toString())
                },
                Response.ErrorListener {})
            httpClient.addToRequestQueue(request)
        }
    }
}
