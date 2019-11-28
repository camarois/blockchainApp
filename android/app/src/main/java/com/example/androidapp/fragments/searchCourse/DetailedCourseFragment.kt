package com.example.androidapp.fragments.searchCourse

import android.os.Bundle
import android.os.Environment
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.example.androidapp.CourseItem
import com.example.androidapp.PdfFileRequest
import com.example.androidapp.R
import com.example.androidapp.StudentItem
import com.example.androidapp.services.RestRequestService
import com.example.androidapp.services.Utils
import kotlinx.android.synthetic.main.fragment_detailed_course.view.*
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import org.koin.android.ext.android.get
import kotlin.coroutines.CoroutineContext
import java.io.FileOutputStream
import java.io.File

class DetailedCourseFragment(
    private val course: CourseItem,
    private val students: List<StudentItem>
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
        view.trimester.text = course.trimester.toString()
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
        var filename = ""
        Utils.processRequest(context!!) {
            val response = restService.postPdfFileAsync(
                PdfFileRequest(
                    course.code,
                    course.trimester.toInt()
                )
            )
            filename = course.code + "_" + course.trimester
            val os =
                FileOutputStream(getReportPath(filename), false)
            os.write(response)
            os.flush()
            os.close()

            Toast.makeText(
                context, getString(R.string.info_pdf_success),
                Toast.LENGTH_LONG
            ).show()

            swapFragment(File(getReportPath(filename)))
        }
    }
    private fun getReportPath(filename: String): String {
        val file = File(Environment.getExternalStorageDirectory().path, "Classbook")
        if (!file.exists()) {
            file.mkdirs()
        }
        return file.absolutePath + "/" + filename + ".pdf"
    }

    private fun swapFragment(file: File) {
        val transaction = fragmentManager!!.beginTransaction()
        val frag = PDFViewerFragment(file)
        transaction.replace(R.id.detailed_course_fragment, frag)
        transaction.commit()
    }
}
