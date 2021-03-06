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
import android.Manifest.permission.WRITE_EXTERNAL_STORAGE
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.example.androidapp.AccountTypes
import com.example.androidapp.services.Utils

class MainActivity : AppCompatActivity(), CoroutineScope {
    private lateinit var job: Job
    private var restService: RestRequestService = get()
    private val STORAGE_PERMISSION_CODE: Int = 123

    override val coroutineContext: CoroutineContext
        get() = job + Dispatchers.Main

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        requestStoragePermission()
        Fabric.with(this, Crashlytics())
        job = Job()
        setContentView(R.layout.activity_main)

        connection_button.setOnClickListener {
            Utils.debounce(connection_button)
            launch { submitLogin() }
        }
    }

    private suspend fun submitLogin() {
        Utils.processRequest(this@MainActivity) {
            val username = username_edit_text.text.toString()
            // restService.initServerUrl(username) // Activate this while developping
            val password = password_edit_text.text.toString()
            try {
                val response = restService.postLoginAsync(LoginRequest(username, password))
                val accountType = if (response.edition) AccountTypes.EDITION else AccountTypes.CONSULTATION
                val intent = Intent(this@MainActivity, SidePanelActivity::class.java).apply {
                    putExtra("username", username)
                    putExtra("type", accountType)
                }
                startActivity(intent)
            } catch (e: AuthFailureError) {
                password_edit_text.setText("")
                Toast.makeText(this@MainActivity, getString(R.string.error_auth),
                    Toast.LENGTH_LONG).show()
            }
        }
    }

    private fun requestStoragePermission() {
        if (ContextCompat.checkSelfPermission(
                this,
                WRITE_EXTERNAL_STORAGE
            ) == PackageManager.PERMISSION_GRANTED
        )
            return

        if (ActivityCompat.shouldShowRequestPermissionRationale(this, WRITE_EXTERNAL_STORAGE))
            Toast.makeText(this, getString(R.string.info_storage), Toast.LENGTH_LONG).show()
        ActivityCompat.requestPermissions(this, arrayOf(READ_EXTERNAL_STORAGE, WRITE_EXTERNAL_STORAGE), STORAGE_PERMISSION_CODE)
    }
}
