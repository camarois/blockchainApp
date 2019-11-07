package com.example.androidapp.services
import android.view.View

object Utils {
    fun preventTwoClick(view: View) {
        view.isEnabled = false
        view.postDelayed({ view.isEnabled = true }, 500)
    }
}