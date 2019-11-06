package com.example.androidapp.activities

import android.content.Intent
import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.android.volley.AuthFailureError
import com.crashlytics.android.Crashlytics
import com.example.androidapp.LoginRequest
import com.example.androidapp.R
import com.example.androidapp.services.RestRequestService
import io.fabric.sdk.android.Fabric
import kotlinx.android.synthetic.main.activity_main.*
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import org.koin.android.ext.android.get
import kotlin.coroutines.CoroutineContext
import android.content.pm.PackageManager
import android.Manifest.permission.READ_EXTERNAL_STORAGE
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat

class MainActivity : AppCompatActivity(), CoroutineScope {
    private lateinit var job: Job
    private var restService: RestRequestService = get()

    override val coroutineContext: CoroutineContext
        get() = job + Dispatchers.Main

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        requestStoragePermission()
        Fabric.with(this, Crashlytics())
        job = Job()
        setContentView(R.layout.activity_main)

        connection_button.setOnClickListener { launch { submitLogin() } }
        register_text_view.setOnClickListener { submitRegister() }
    }

    private suspend fun submitLogin() {
        try {
            val username = username_edit_text.text.toString()
            // restService.initServerUrl(username) // Activate this while developping
            val password = password_edit_text.text.toString()
            //val response = restService.postLoginAsync(LoginRequest(username, password))
            val intent = Intent(this@MainActivity, SidePanelActivity::class.java).apply {
                //putExtra("user", response.edition)
            }
            startActivity(intent)
        } catch (e: AuthFailureError) {
            password_edit_text.setText("")
            Toast.makeText(this, "Le nom d'usager et/ou le mot de passe est invalide",
                Toast.LENGTH_LONG).show()
        }
    }

    private fun submitRegister() {
        // TODO : Decide how we want people to register. Do they send a request? Does the PC admin adds them?
        // val intent = Intent(this@MainActivity, RegisterActivity::class.java).apply { }
        // startActivity(intent)
    }

    private fun requestStoragePermission() {
        if (ContextCompat.checkSelfPermission(
                this,
                READ_EXTERNAL_STORAGE
            ) == PackageManager.PERMISSION_GRANTED
        )
            return

        if (ActivityCompat.shouldShowRequestPermissionRationale(this, READ_EXTERNAL_STORAGE))
            Toast.makeText(this, "L'application utilise cette permission afin de lire des fichiers PDFs.", Toast.LENGTH_LONG).show()
    }
}
