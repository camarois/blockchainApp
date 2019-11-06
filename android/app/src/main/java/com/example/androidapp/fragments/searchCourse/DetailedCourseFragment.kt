package com.example.androidapp.fragments.searchCourse

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.example.androidapp.PdfFileRequest
import com.example.androidapp.R
import com.example.androidapp.Student
import com.example.androidapp.fragments.searchCourse.course.CourseItem
import com.example.androidapp.services.RestRequestService
import kotlinx.android.synthetic.main.fragment_detailed_course.view.*
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import org.koin.android.ext.android.get
import java.lang.Exception
import kotlin.coroutines.CoroutineContext

class DetailedCourseFragment(
    private val course: CourseItem,
    private val students: List<Student>
) : Fragment(), CoroutineScope {

    private var columnCount = 1
    private var restService: RestRequestService = get()
    private lateinit var job: Job

    override val coroutineContext: CoroutineContext
        get() = job + Dispatchers.Main


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
        job = Job()
        val view = inflater.inflate(R.layout.fragment_detailed_course, container, false)
        view.code.text = course.code
        view.trimester.text = course.trimester
        view.viewPdfButton.setOnClickListener { launch { viewPdf() } }
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

    private suspend fun viewPdf() {
        try {
            //restService.getFichierNotes(PdfFileRequest(course.code, course.trimester))
            Toast.makeText(context, "View pdf",
                Toast.LENGTH_LONG).show()
        } catch (e: Exception) {
            Toast.makeText(context, "${getString(R.string.error_message_unknown)}: ${e.message}",
                Toast.LENGTH_LONG).show()
        }
    }
}
