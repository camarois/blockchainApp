package com.example.androidapp.fragments.register

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.provider.DocumentsContract
import android.util.Base64
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageButton
import com.example.androidapp.R
import com.example.androidapp.ui.fragments.search.student.StudentItem
import kotlinx.android.synthetic.main.fragment_register_list.*
import kotlinx.android.synthetic.main.fragment_student_list.view.*
import java.util.ArrayList
import kotlinx.android.synthetic.main.add_student_bottom_panel.*
import android.widget.Toast
import com.example.androidapp.TransactionRequest
import com.google.android.material.bottomsheet.BottomSheetBehavior
import kotlinx.android.synthetic.main.add_student_bottom_panel.code
import kotlinx.android.synthetic.main.add_student_bottom_panel.grade
import kotlinx.android.synthetic.main.add_student_bottom_panel.name
import kotlinx.android.synthetic.main.fragment_graded_student.*
import java.io.File

/**
 * A fragment representing a list of Items.
 * Activities containing this fragment MUST implement the
 * [RegisterFragment.OnListFragmentInteractionListener] interface.
 */
class RegisterFragment : Fragment() {

    private var pdfFilePath: String = ""
    private var columnCount = 1
    private val registeredStudents: MutableList<StudentItem> = ArrayList()
    private var listener: OnListFragmentInteractionListener? = null
    private var bottomSheetBehavior: BottomSheetBehavior<View>? = null
    private val PDF_UPLOAD_CODE = 111

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        arguments?.let {
            columnCount = it.getInt(ARG_COLUMN_COUNT)
        }
    }

    private fun openBottomSheet() {
        if (bottomSheetBehavior != null) {
            if (bottomSheetBehavior!!.state != BottomSheetBehavior.STATE_EXPANDED) {
                addStudentButton.visibility = View.GONE
                bottomSheetBehavior!!.state = BottomSheetBehavior.STATE_EXPANDED
            } else {
                bottomSheetBehavior!!.state = BottomSheetBehavior.STATE_COLLAPSED
            }
        }
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        val viewCreated = view.list
        viewCreated.adapter = GradedStudentRecyclerViewAdapter(this@RegisterFragment, registeredStudents, listener)
        addStudentButton.setOnClickListener { openBottomSheet() }
        super.onViewCreated(view, savedInstanceState)
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_register_list, container, false)

        // Set the adapter
        if (view is RecyclerView) {
            with(view) {
                layoutManager = when {
                    columnCount <= 1 -> LinearLayoutManager(context)
                    else -> GridLayoutManager(context, columnCount)
                }
                adapter =
                    GradedStudentRecyclerViewAdapter(
                        this@RegisterFragment,
                        registeredStudents,
                        listener
                    )
            }
        }

        val bottomSheet: View = view.findViewById(R.id.student_bottom_panel)
        bottomSheetBehavior = BottomSheetBehavior.from(bottomSheet)

        bottomSheetBehavior!!.setBottomSheetCallback(object : BottomSheetBehavior.BottomSheetCallback() {
            override fun onStateChanged(bottomSheet: View, newState: Int) {
                // this part hides the button immediately and waits bottom sheet
                // to collapse to show
                if (BottomSheetBehavior.STATE_EXPANDED == newState) {
                    addStudentButton.visibility = View.GONE
                } else if (BottomSheetBehavior.STATE_COLLAPSED == newState) {
                    addStudentButton.animate().scaleX(1F).scaleY(1F).setDuration(0).start()
                    addStudentButton.visibility = View.VISIBLE
                } else if (BottomSheetBehavior.STATE_HIDDEN == newState) {
                    addStudentButton.animate().scaleX(1F).scaleY(1F).setDuration(0).start()
                    addStudentButton.visibility = View.VISIBLE
                }
            }

            override fun onSlide(bottomSheet: View, slideOffset: Float) {}
        })

        val createStudentButton: ImageButton = view.findViewById(R.id.createStudentButton)
        createStudentButton.setOnClickListener { createStudent() }
        return view
    }

    private fun createStudent() {
        val studentName = name.text.toString()
        val studentCode = code.text.toString()
        val studentGrade = grade.text.toString()
        if (studentName.isNotEmpty() && studentCode.isNotEmpty() && studentGrade.isNotEmpty()) {
            registeredStudents.add(registeredStudents.size, StudentItem(registeredStudents.size.toString(), name.text.toString(), code.text.toString().toInt(), grade.text.toString().toFloat()))
            list.adapter?.notifyItemInserted(registeredStudents.size - 1)
            list.smoothScrollToPosition(registeredStudents.size - 1)
            resetView()
        } else {
            Toast.makeText(activity, "Il manque des informations!", Toast.LENGTH_SHORT).show()
        }
    }

    private fun resetView() {
        name.setText("")
        code.setText("")
        grade.setText("")
    }

    fun uploadPDF(){
        val intent = Intent()
            .setType("application/pdf")
            .setAction(Intent.ACTION_GET_CONTENT)
        startActivityForResult(Intent.createChooser(intent, "Select a file"), PDF_UPLOAD_CODE)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (resultCode == Activity.RESULT_OK) {
            if (requestCode == PDF_UPLOAD_CODE) {
                if (data == null) { return }
                if (data.data.path == "") {
                    Toast.makeText(activity, "Le fichier sélectionné est invalide.", Toast.LENGTH_SHORT).show()
                } else {
                    pdfFilePath = getFilePath(data.data!!)
                }
            }
        }
    }

    private fun getFilePath(path: Uri): String {
        val id: String = DocumentsContract.getDocumentId(path)
        var ret = ""
        if (id.isNotEmpty()) {
            if (id.startsWith("raw:")) {
                ret = id.replaceFirst("raw:", "")
            }
        }
        return ret
    }

//            try {
//                final Uri contentUri = ContentUris.withAppendedId(
//                    Uri.parse("content://downloads/public_downloads"), Long.valueOf(id)
//                );
//                return getDataColumn(context, contentUri, null, null);
//            } catch (NumberFormatException e) {
//                Log.e(
//                    "FileUtils",
//                    "Downloads provider returned unexpected uri " + uri.toString(),
//                    e
//                );
//                return null;
//            }
//        }
//    }

    private fun convertToBase64(attachment: File): String {
        return Base64.encodeToString(attachment.readBytes(), Base64.NO_WRAP)
    }

    fun submit(values: List<StudentItem>) {
        if (pdfFilePath == "") {
            Toast.makeText(activity, "Veuillez choisir un fichier PDF valide!", Toast.LENGTH_SHORT).show()
            return
        }

        if(class_name.text.isEmpty()) {
            Toast.makeText(activity, "Veuillez entrer un sigle valide!", Toast.LENGTH_SHORT).show()
            return
        }

        val pdf = convertToBase64(File(pdfFilePath))
        val code = class_name.text.toString()
        val name = "UN NOM DE CLASSE"
        val trimester = "A2000"
        TransactionRequest(code, name, trimester, values, pdf)
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
        fun onListFragmentInteraction(gradedStudent: StudentItem)
    }

    companion object {

        // TODO: Customize parameter argument names
        const val ARG_COLUMN_COUNT = "column-count"

        // TODO: Customize parameter initialization
        @JvmStatic
        fun newInstance(columnCount: Int) =
            RegisterFragment().apply {
                arguments = Bundle().apply {
                    putInt(ARG_COLUMN_COUNT, columnCount)
                }
            }
    }
}
