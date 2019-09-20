package com.example.androidapp

import android.app.Application
import com.example.androidapp.di.appModules
import org.koin.core.context.startKoin

class App :Application(){
    override fun onCreate() {
        super.onCreate()
        startKoin{
            modules(appModules)
        }
    }
}