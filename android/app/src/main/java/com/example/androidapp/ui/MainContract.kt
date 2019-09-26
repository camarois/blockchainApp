package com.example.androidapp.ui

interface MainContract {
    interface View

    interface Controller {
        suspend fun onRefreshLolAsync(): String
    }
}
