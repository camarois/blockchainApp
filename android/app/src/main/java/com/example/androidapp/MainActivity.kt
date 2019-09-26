package com.example.androidapp

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.JsonObjectRequest
import com.android.volley.toolbox.StringRequest
import com.android.volley.toolbox.Volley
import org.json.JSONObject
import com.android.volley.toolbox.RequestFuture
import android.support.v4.app.SupportActivity
import android.support.v4.app.SupportActivity.ExtraData
import android.support.v4.content.ContextCompat.getSystemService
import android.icu.lang.UCharacter.GraphemeClusterBreak.T
import java.util.concurrent.TimeUnit


class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        findViewById<TextView>(R.id.lolVar).text = "lol!"
        findViewById<Button>(R.id.refreshBtn).setOnClickListener { refreshLol() }
    }

    private fun refreshLol() {
        val textView = findViewById<TextView>(R.id.lolVar)
        // Instantiate the RequestQueue
        val queue = Volley.newRequestQueue(this)
        val baseUrl = "https://us-central1-projet3-46f1b.cloudfunctions.net/getServerURL?user=server"
        val stringRequest = StringRequest(
            Request.Method.GET, baseUrl, { serverUrl ->
                println(serverUrl)
                var url = "http://$serverUrl:10000/lol"
                val jsonObjectRequest = JsonObjectRequest(
                    Request.Method.GET, url, null, { lol ->
                        println(lol)
                        textView.text = lol.toString()
                        try {
                            // TODO Gson https://developer.android.com/training/volley/request-custom.html
                            textView.text = lol.getInt("lol").toString()
                        } catch (t: Throwable) {
                            textView.text = "Malformed response"
                        }
                    }, {
                        textView.text = "That didn't work! Lol error"
                    })
                queue.add(jsonObjectRequest)
            }, {
                textView.text = "That didn't work! Firebase error"
            })
        queue.add(stringRequest)
    }
}
