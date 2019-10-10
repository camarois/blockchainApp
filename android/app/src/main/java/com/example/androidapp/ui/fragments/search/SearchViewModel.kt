package com.example.androidapp.ui.fragments.search

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel

class SearchViewModel : ViewModel() {

    val text: LiveData<String> = MutableLiveData<String>().apply {
        value = "This is search Fragment"
    }
}
