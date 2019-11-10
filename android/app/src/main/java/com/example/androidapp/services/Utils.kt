package com.example.androidapp.services
import android.app.ProgressDialog
import android.content.Context
import android.view.View
import android.widget.Toast
import com.android.volley.TimeoutError

object Utils {
    fun debounce(view: View) {
        view.isEnabled = false
        view.postDelayed({ view.isEnabled = true }, 500)
    }

    suspend fun processRequest(context: Context, request: suspend () -> Unit) {
        val pd = ProgressDialog(context)
        pd.setMessage("En attente d'une réponse des mineurs...")
        pd.setCancelable(false)
        pd.show()
        try {
            request()
        } catch (e: TimeoutError) {
            Toast.makeText(context, "Petit problème de connexion au serveur, veuillez réessayer!", Toast.LENGTH_LONG).show()
        }
        pd.dismiss()
    }
}
