package com.example.androidapp.fragments.searchCourse

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.example.androidapp.R
import com.example.androidapp.fragments.searchCourse.course.CourseItem
import kotlinx.android.synthetic.main.fragment_detailed_course.view.*

class DetailedCourseFragment(
    private val course: CourseItem
) : Fragment() {

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view =  inflater.inflate(R.layout.fragment_detailed_course, container, false)
        view.code.setText(course.code)
        view.trimester.setText(course.trimester)

        return view
    }
}
