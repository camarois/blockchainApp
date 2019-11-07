package com.example.androidapp.fragments.searchStudent

import android.app.ProgressDialog
import android.content.Context
import android.os.Bundle
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import com.android.volley.TimeoutError
import com.example.androidapp.R
import com.example.androidapp.StudentItem
import com.example.androidapp.services.RestRequestService
import kotlinx.android.synthetic.main.fragment_register_list.*
import kotlinx.android.synthetic.main.fragment_student_list.view.*
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import org.koin.android.ext.android.get
import java.util.ArrayList
import kotlin.coroutines.CoroutineContext

class SearchStudentFragment : Fragment(), CoroutineScope {

    private var columnCount = 1
    private lateinit var job: Job
    private var restService: RestRequestService = get()
    private val students: MutableList<StudentItem> = ArrayList()
    private var listener: OnListFragmentInteractionListener? = null

    override val coroutineContext: CoroutineContext
        get() = job + Dispatchers.Main

    override fun onCreate(savedInstanceState: Bundle?) {
        job = Job()
        super.onCreate(savedInstanceState)

        arguments?.let {
            columnCount = it.getInt(ARG_COLUMN_COUNT)
        }
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        val viewCreated = view.list
        launch {
            val pd = ProgressDialog(context)
            pd.setMessage("En d'une réponse des mineurs...")
            pd.show()
            try {
                val newStudents = restService.getStudentListAsync()
                for (element in newStudents.listeEtudiant) {
                    students.add(element)
                    list.adapter?.notifyItemInserted(students.size - 1)
                }
            } catch (e: TimeoutError) {
                Toast.makeText(
                    context,
                    "Petit problème de connexion au serveur, veuillez réessayer!",
                    Toast.LENGTH_LONG
                ).show()
            }
            pd.dismiss()
        }
        viewCreated.adapter =
            StudentRecyclerViewAdapter(students, listener)
        super.onViewCreated(view, savedInstanceState)
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_student_list, container, false)

        // Set the adapter
        if (view is RecyclerView) {
            with(view) {
                layoutManager = when {
                    columnCount <= 1 -> LinearLayoutManager(context)
                    else -> GridLayoutManager(context, columnCount)
                }
                adapter =
                    StudentRecyclerViewAdapter(students, listener)
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

    /**
     * This interface must be implemented by activities that contain this
     * fragment to allow an interaction in this fragment to be communicated
     * to the activity and potentially other fragments contained in that
     * activity.
     *
     *
     * See the Android Training lesson
     * [Communicating with Other Fragments](http://developer.android.com/training/basics/fragments/communicating.html)
     * for more information.
     */
    interface OnListFragmentInteractionListener {
        fun onListFragmentInteraction(student: StudentItem)
    }

    companion object {
        const val ARG_COLUMN_COUNT = "column-count"

        @JvmStatic
        fun newInstance(columnCount: Int) =
            SearchStudentFragment().apply {
                arguments = Bundle().apply {
                    putInt(ARG_COLUMN_COUNT, columnCount)
                }
            }
    }
}
