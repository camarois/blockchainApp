package com.example.androidapp.ui.rest

import org.koin.dsl.module

val restServiceModule = module {
    single { RestRequestService(get()) }
    factory { HTTPRestClient(get(), get()) }
}
