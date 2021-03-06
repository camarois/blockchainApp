package com.example.androidapp.fragments.register

import androidx.recyclerview.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import android.widget.Toast
import com.example.androidapp.R
import com.example.androidapp.fragments.register.RegisterCourseFragment.OnListFragmentInteractionListener
import com.example.androidapp.StudentItem
import com.example.androidapp.services.Utils
import kotlinx.android.synthetic.main.fragment_graded_student.view.*
import kotlinx.android.synthetic.main.bottom_button.view.*
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import kotlin.coroutines.CoroutineContext

class GradedStudentRecyclerViewAdapter(
    private val mFragment: RegisterCourseFragment,
    private val mValues: List<StudentItem>,
    private val mListener: OnListFragmentInteractionListener?

) : RecyclerView.Adapter<GradedStudentRecyclerViewAdapter.ViewHolder>() {

    private val mOnClickListener: View.OnClickListener

    init {
        mOnClickListener = View.OnClickListener { v ->
            val item = v.tag as StudentItem
            mListener?.onListFragmentInteraction(item)
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        return if (viewType == R.layout.fragment_graded_student) {
            val view = LayoutInflater.from(parent.context).inflate(R.layout.fragment_graded_student, parent, false)
            GradedStudentViewHolder(view)
        } else {
            val view = LayoutInflater.from(parent.context).inflate(R.layout.bottom_button, parent, false)
            ButtonsViewHolder(view)
        }
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        if (position == mValues.size) {
            (holder as ButtonsViewHolder).bind(holder.itemView)
        } else {
            val item = mValues[position]
            val studentHolder = holder as GradedStudentViewHolder
            studentHolder.bind(item, position)
        }
    }

    override fun getItemViewType(position: Int): Int {
        return if (position == mValues.size) R.layout.bottom_button else R.layout.fragment_graded_student
    }

    override fun getItemCount(): Int = mValues.size + 1

    open inner class ViewHolder(mView: View) : RecyclerView.ViewHolder(mView)

    inner class ButtonsViewHolder(mView: View) : CoroutineScope, ViewHolder(mView) {
        private val mPDFButton: Button = mView.uploadPDFBtn
        private val mSubmitButton: Button = mView.registerClassBtn
        private lateinit var job: Job

        override val coroutineContext: CoroutineContext
            get() = job + Dispatchers.Main

        fun bind(mView: View) {
            job = Job()
            mPDFButton.setOnClickListener {
                Utils.debounce(mPDFButton)
                mFragment.uploadPDF()
            }
            mSubmitButton.setOnClickListener {
                Utils.debounce(mSubmitButton)
                launch { submit(mView) }
            }
        }

        private suspend fun submit(view: View) {
            if (mValues.isEmpty()) {
                Toast.makeText(view.context, view.context.getString(R.string.info_students_empty), Toast.LENGTH_SHORT).show()
                return
            }
            mFragment.submit(mValues)
        }
    }

    inner class GradedStudentViewHolder(mView: View) : ViewHolder(mView) {
        private val mName: TextView = mView.name
        private val mCode: TextView = mView.code
        private val mGrade: TextView = mView.grade
        private val mPos: TextView = mView.position

        fun bind(item: StudentItem, position: Int) {
            mName.text = "${item.firstName} ${item.lastName}"
            mCode.text = item.code
            mGrade.text = item.grade
            mPos.text = (position + 1).toString()
        }
    }
}
