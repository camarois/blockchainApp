package com.example.androidapp.fragments.settings

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel

class SettingsViewModel : ViewModel() {

    val text: LiveData<String> = MutableLiveData<String>().apply {
        value = "This is settings Fragment"
    }
}
