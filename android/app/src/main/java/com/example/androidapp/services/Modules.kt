package com.example.androidapp.services

import android.content.SharedPreferences
import android.preference.PreferenceManager
import org.koin.dsl.module

val appModules = module {
    single<SharedPreferences> { PreferenceManager.getDefaultSharedPreferences(get()) }
    single { RestRequestService(get(), get()) }
    single { HTTPRestClient(get()) }
}
