package com.example.androidapp

import android.app.Application
import com.example.androidapp.di.appModules
import com.example.androidapp.ui.rest.restServiceModule
import org.koin.android.ext.koin.androidContext
import org.koin.android.ext.koin.androidLogger
import org.koin.core.context.startKoin

class App :Application(){
    override fun onCreate() {
        super.onCreate()
        startKoin{
            androidLogger()
            androidContext(this@App)
            modules(listOf(restServiceModule, appModules))
        }
    }
}
