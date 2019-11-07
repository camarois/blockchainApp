package com.example.androidapp.fragments.settings

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import com.android.volley.AuthFailureError
import com.example.androidapp.PasswordRequest
import com.example.androidapp.R
import com.example.androidapp.services.RestRequestService
import com.example.androidapp.services.Utils
import kotlinx.android.synthetic.main.activity_main.*
import kotlinx.android.synthetic.main.fragment_settings.*
import kotlinx.android.synthetic.main.fragment_settings.view.*
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Job
import kotlinx.coroutines.*
import org.koin.android.ext.android.get
import kotlin.coroutines.CoroutineContext

class SettingsFragment : Fragment(), CoroutineScope {

    private var restService: RestRequestService = get()
    private lateinit var job: Job

    override val coroutineContext: CoroutineContext
        get() = job + Dispatchers.Main

    private lateinit var settingsViewModel: SettingsViewModel

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        job = Job()
        settingsViewModel =
            ViewModelProviders.of(this).get(SettingsViewModel::class.java)
        val root = inflater.inflate(R.layout.fragment_settings, container, false)
        root.connection_button.setOnClickListener {
            Utils.preventTwoClick(root.connection_button)
            launch { submitNewPassword() }
        }

        return root
    }

    private suspend fun submitNewPassword() {
        try {
            val oldPassword = old_password_edit_text.text.toString()
            val newPassword = new_password_edit_text.text.toString()
            val response = restService.postChangePasswordAsync(PasswordRequest(oldPassword, newPassword))
            Toast.makeText(context, "Le mot de passe a été changé avec succès.",
                Toast.LENGTH_LONG).show()
        } catch (e: AuthFailureError) {
            password_edit_text.setText("")
            Toast.makeText(context, "L'ancien mot de passe est invalide",
                Toast.LENGTH_LONG).show()
        }
    }
}
