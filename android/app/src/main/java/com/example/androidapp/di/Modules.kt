package com.example.androidapp.di

import android.content.SharedPreferences
import android.preference.PreferenceManager
import com.example.androidapp.ui.MainContract
import com.example.androidapp.ui.MainController
import org.koin.dsl.module

val appModules = module {
    single<SharedPreferences> { PreferenceManager.getDefaultSharedPreferences(get()) }
    factory<MainContract.Controller> { MainController(get()) }
}
