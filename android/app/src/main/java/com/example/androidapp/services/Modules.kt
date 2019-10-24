package com.example.androidapp.services

import org.koin.dsl.module

val appModules = module {
    single { RestRequestService(get(), get()) }
    single { HTTPRestClient(get()) }
}
