package com.example.androidapp.di


import com.example.androidapp.ui.MainContract
import com.example.androidapp.ui.MainPresenter
import org.koin.dsl.module

val appModules = module {
    factory<MainContract.Presenter> { MainPresenter() }
}