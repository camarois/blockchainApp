package com.example.androidapp.services
import android.app.ProgressDialog
import android.content.Context
import android.view.View
import android.widget.Toast
import com.android.volley.NoConnectionError
import com.android.volley.TimeoutError
import com.example.androidapp.R

object Utils {
    fun debounce(view: View) {
        view.isEnabled = false
        view.postDelayed({ view.isEnabled = true }, 500)
    }

    suspend fun processRequest(context: Context, request: suspend () -> Unit) {
        val pd = ProgressDialog(context)
        pd.setMessage(context.getString(R.string.status_waiting))
        pd.setCancelable(false)
        pd.show()
        try {
            request()
        } catch (e: TimeoutError) {
            Toast.makeText(context, context.getString(R.string.error_timeout), Toast.LENGTH_LONG).show()
        } catch (e: NoConnectionError) {
            Toast.makeText(context, context.getString(R.string.error_no_internet), Toast.LENGTH_LONG).show()
        } catch (e: Error) {
            Toast.makeText(context, context.getString(R.string.error_general), Toast.LENGTH_LONG).show()
        }
        pd.dismiss()
    }
}
