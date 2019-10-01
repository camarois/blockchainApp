package com.example.androidapp.ui

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import com.crashlytics.android.Crashlytics
import com.example.androidapp.R
import io.fabric.sdk.android.Fabric
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlin.coroutines.CoroutineContext

class MainActivity : AppCompatActivity(), MainContract.View, CoroutineScope {
    private lateinit var job: Job

    override val coroutineContext: CoroutineContext
        get() = job + Dispatchers.Main

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Fabric.with(this, Crashlytics())
        job = Job()
        setContentView(R.layout.activity_main)

        findViewById<Button>(R.id.connectionBtn).setOnClickListener{ submitLogin() }
        findViewById<TextView>(R.id.registerBtn).setOnClickListener{ submitRegister() }
    }
    private fun submitLogin() {
        val username = findViewById<EditText>(R.id.usernameVal).text.toString()
        val password = findViewById<EditText>(R.id.passwordVal).text.toString()
        if(username == "rose" && password == "1234") {
            val user = "rose;rooose;1234;rose@email.com;10"
            val intent = Intent(this@MainActivity, SidePanelActivity::class.java).apply {
                putExtra("user", user)
            }
            startActivity(intent)
        }
    }

    private fun submitRegister() {
        // val intent = Intent(this@MainActivity, RegisterActivity::class.java).apply { }
        // startActivity(intent)
    }
}
