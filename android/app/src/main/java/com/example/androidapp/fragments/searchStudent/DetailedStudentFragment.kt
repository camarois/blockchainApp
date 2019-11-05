package com.example.androidapp.fragments.searchStudent

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.example.androidapp.CourseResult
import com.example.androidapp.R
import com.example.androidapp.fragments.searchStudent.student.StudentItem

import kotlinx.android.synthetic.main.fragment_detailed_student.view.*

class DetailedStudentFragment(
    private val student: StudentItem,
    private val results: List<CourseResult>
) : Fragment() {

    private var columnCount = 1

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        val viewCreated = view.results_list
        viewCreated.adapter =
            StudentResultsRecyclerViewAdapter(
                results
            )
        super.onViewCreated(view, savedInstanceState)
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_detailed_student, container, false)
        view.student_code.text = student.code.toString()
        view.student_name.text = student.name

        // Set the adapter
        if (view is RecyclerView) {
            with(view) {
                layoutManager = when {
                    columnCount <= 1 -> LinearLayoutManager(context)
                    else -> GridLayoutManager(context, columnCount)
                }
                adapter =
                    StudentResultsRecyclerViewAdapter(
                        results
                    )
            }
        }
        return view
    }
}
