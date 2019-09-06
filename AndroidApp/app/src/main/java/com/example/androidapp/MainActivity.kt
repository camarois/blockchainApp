package com.example.androidapp

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.StringRequest
import com.android.volley.toolbox.Volley
import org.json.JSONObject

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        findViewById<TextView>(R.id.lolVar).text = "lol!"
        findViewById<Button>(R.id.refreshBtn).setOnClickListener{ refreshLol() }
    }

    private fun refreshLol() {
        val textView = findViewById<TextView>(R.id.lolVar)
        // Instantiate the RequestQueue
        val queue = Volley.newRequestQueue(this)
        val url = getString(R.string.host) + "/lol"

        // Request a string response from the provided URL.
        val stringRequest = StringRequest(
            Request.Method.GET, url,
            Response.Listener<String> { response ->
                // Display the response string.
                textView.text = response

                try {
                    val obj = JSONObject(response)
                    textView.text = obj.getInt("lol").toString()

                } catch (t: Throwable) {
                    textView.text = "Malformed response"
                }

            },
            Response.ErrorListener { textView.text = "That didn't work!" })

        // Add the request to the RequestQueue.
        queue.add(stringRequest)

    }
}
