package com.example.androidapp.ui

import com.example.androidapp.ui.rest.RestRequestService
import org.koin.core.KoinComponent

class MainController(
    private val restService: RestRequestService
) : MainContract.Controller, KoinComponent {

    override suspend fun onRefreshLolAsync(): String {
        return restService.getLol()
    }
}
