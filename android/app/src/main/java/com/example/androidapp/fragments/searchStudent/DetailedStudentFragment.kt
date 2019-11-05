package com.example.androidapp.fragments.searchStudent

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.example.androidapp.R
import com.example.androidapp.fragments.searchStudent.student.StudentItem

import com.example.androidapp.services.RestRequestService
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import org.koin.android.ext.android.get
import kotlin.coroutines.CoroutineContext

class DetailedStudentFragment(
    private val student: StudentItem
) : Fragment(), CoroutineScope {

    private var restService: RestRequestService = get()
    private lateinit var job: Job

    override val coroutineContext: CoroutineContext
        get() = job + Dispatchers.Main

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        job = Job()
        return inflater.inflate(R.layout.fragment_detailed_student, container, false)
    }
}
