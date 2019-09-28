//package com.example.androidapp
//
//import android.app.AlertDialog
//import android.content.DialogInterface
//import android.support.v7.app.AppCompatActivity
//import android.os.Bundle
//import android.widget.Button
//import android.widget.EditText
//import android.widget.TextView
//import com.android.volley.Request
//import com.android.volley.toolbox.JsonObjectRequest
//import com.android.volley.toolbox.StringRequest
//import com.android.volley.toolbox.Volley
//
//class MainActivity : AppCompatActivity() {
//
//    var serverUrl: String = ""
//
//    override fun onCreate(savedInstanceState: Bundle?) {
//        super.onCreate(savedInstanceState)
//        setServerUrlWithDialog()
//        setContentView(R.layout.activity_main)
//        findViewById<TextView>(R.id.lolVar).text = "lol!"
//        findViewById<Button>(R.id.refreshBtn).setOnClickListener { refreshLol() }
//    }
//
//    private fun initServerUrl(user: String) {
//        val queue = Volley.newRequestQueue(this)
//        val baseUrl = "https://us-central1-projet3-46f1b.cloudfunctions.net/getServerURL?user=$user"
//        val stringRequest = StringRequest(
//            Request.Method.GET, baseUrl, {
//                serverUrl = "http://$it:10000/lol"
//                println(serverUrl)
//            }, {
//                serverUrl = it.toString()
//            })
//        queue.add(stringRequest)
//    }
//
//    private fun setServerUrlWithDialog() {
//        val userDialog: AlertDialog = this.let {
//            val builder = AlertDialog.Builder(it)
//            val input = EditText(this@MainActivity)
//            builder.apply {
//                setTitle("Entrez votre nom de dev")
//                setMessage("Ne rien mettre = server")
//                setPositiveButton("Se connecter") { dialog, _ ->
//                    initServerUrl(if (input.text.isNotEmpty()) input.text.toString() else "server")
//                    dialog.cancel()
//                }
//                setView(input)
//            }
//
//            builder.create()
//        }
//        userDialog.setCancelable(false)
//        userDialog.show()
//    }
//
//    private fun refreshLol() {
//        val textView = findViewById<TextView>(R.id.lolVar)
//        val queue = Volley.newRequestQueue(this)
//        val jsonObjectRequest = JsonObjectRequest(
//            Request.Method.GET, serverUrl, null, {
//                textView.text = it.toString()
//                try {
//                    // TODO Gson https://developer.android.com/training/volley/request-custom.html
//                    textView.text = it.getInt("lol").toString()
//                } catch (t: Throwable) {
//                    textView.text = "Malformed response: $t"
//                }
//            }, {
//                textView.text = "That didn't work! $it"
//            })
//        queue.add(jsonObjectRequest)
//    }
//}
