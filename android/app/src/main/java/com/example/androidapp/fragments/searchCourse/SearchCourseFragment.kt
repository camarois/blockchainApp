package com.example.androidapp.fragments.searchCourse

import android.content.Context
import android.os.Bundle
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.example.androidapp.CourseItem
import com.example.androidapp.R
import com.example.androidapp.services.RestRequestService
import com.example.androidapp.services.Utils
import kotlinx.android.synthetic.main.fragment_course_list.*
import kotlinx.android.synthetic.main.fragment_register_list.list
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import org.koin.android.ext.android.get
import java.util.ArrayList
import kotlin.coroutines.CoroutineContext

class SearchCourseFragment : Fragment(), CoroutineScope {

    private var columnCount = 1
    private lateinit var job: Job
    private var restService: RestRequestService = get()
    private val courses: MutableList<CourseItem> = ArrayList()
    private var listener: OnListFragmentInteractionListener? = null

    override val coroutineContext: CoroutineContext
        get() = job + Dispatchers.Main

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        list.adapter = CourseRecyclerViewAdapter(courses, listener)

        launch {
            Utils.processRequest(context!!) {
                val newCourses = restService.getClassListAsync()
                for (element in newCourses.listeClasses) {
                    courses.add(element)
                    list.adapter?.notifyItemInserted(courses.size - 1)
                }

                if (newCourses.listeClasses.isNotEmpty()) {
                    emptyList.visibility = View.GONE
                    list.visibility = View.VISIBLE
                } else {
                    emptyList.visibility = View.VISIBLE
                    list.visibility = View.GONE
                }
            }
        }

        super.onViewCreated(view, savedInstanceState)
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        job = Job()
        val view = inflater.inflate(R.layout.fragment_course_list, container, false)

        // Set the adapter
        if (view is RecyclerView) {
            with(view) {
                layoutManager = when {
                    columnCount <= 1 -> LinearLayoutManager(context)
                    else -> GridLayoutManager(context, columnCount)
                }
                adapter = CourseRecyclerViewAdapter(courses, listener)
            }
        }
        return view
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        if (context is OnListFragmentInteractionListener) {
            listener = context
        } else {
            throw RuntimeException("$context must implement OnListFragmentInteractionListener")
        }
    }

    override fun onDetach() {
        super.onDetach()
        listener = null
    }

    interface OnListFragmentInteractionListener {
        fun onListFragmentInteraction(course: CourseItem)
    }
}
