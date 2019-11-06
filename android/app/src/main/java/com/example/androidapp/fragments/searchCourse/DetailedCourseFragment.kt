package com.example.androidapp.fragments.searchCourse

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.example.androidapp.R
import com.example.androidapp.Student
import com.example.androidapp.fragments.searchCourse.course.CourseItem
import kotlinx.android.synthetic.main.fragment_detailed_course.view.*

class DetailedCourseFragment(
    private val course: CourseItem,
    private val students: List<Student>
) : Fragment() {

    private var columnCount = 1

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        val viewCreated = view.students_list
        viewCreated.adapter =
            CourseResultsRecyclerViewAdapter(
                students
            )
        super.onViewCreated(view, savedInstanceState)
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_detailed_course, container, false)
        view.code.text = course.code
        view.trimester.text = course.trimester

        // Set the adapter
        if (view is RecyclerView) {
            with(view) {
                layoutManager = when {
                    columnCount <= 1 -> LinearLayoutManager(context)
                    else -> GridLayoutManager(context, columnCount)
                }
                adapter =
                    CourseResultsRecyclerViewAdapter(
                        students
                    )
            }
        }
        return view
    }
}
