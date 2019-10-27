package com.example.androidapp.services

import org.koin.dsl.module

val appModules = module {
    single { CredentialsManager() }
    single { RestRequestService(get(), get(), get()) }
    single { HTTPRestClient(get()) }
}
