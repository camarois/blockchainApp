package com.example.androidapp

import android.app.Application
import android.content.Intent
import com.example.androidapp.services.appModules
import org.koin.android.ext.koin.androidContext
import org.koin.android.ext.koin.androidLogger
import org.koin.core.context.startKoin
import android.os.Looper
import android.widget.Toast
import androidx.core.content.ContextCompat.getSystemService
import android.icu.lang.UCharacter.GraphemeClusterBreak.T
import com.example.androidapp.activities.MainActivity
import com.example.androidapp.activities.SidePanelActivity
import com.example.androidapp.fragments.home.HomeFragment
import androidx.core.content.ContextCompat.getSystemService
import android.icu.lang.UCharacter.GraphemeClusterBreak.T
import android.util.Log
import java.io.PrintWriter
import java.io.StringWriter
import kotlin.system.exitProcess


class App : Application() {
    override fun onCreate() {
        super.onCreate()
        startKoin {
            androidLogger()
            androidContext(this@App)
            modules(appModules)
        }
        Thread.setDefaultUncaughtExceptionHandler { _, paramThrowable ->
            val stackTrace = Log.getStackTraceString(paramThrowable)
            Log.e("FATAL", stackTrace)
            object : Thread() {
                override fun run() {
                    Looper.prepare()
                    // TODO create an activity to show error message
                    Toast.makeText(this@App, stackTrace, Toast.LENGTH_LONG).show()
                    Looper.loop()
                }
            }.start()
            Thread.sleep(4000)
            exitProcess(2)
        }
    }
}
