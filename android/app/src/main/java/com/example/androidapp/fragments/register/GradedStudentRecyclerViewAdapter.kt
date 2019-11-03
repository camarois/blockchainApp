package com.example.androidapp.fragments.register

import android.content.Intent
import android.util.Log
import androidx.recyclerview.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import com.example.androidapp.R
import com.example.androidapp.fragments.register.RegisterFragment.OnListFragmentInteractionListener
import com.example.androidapp.ui.fragments.search.student.StudentItem
import kotlinx.android.synthetic.main.fragment_gradedstudent.view.*
import android.widget.Toast
import androidx.core.app.ActivityCompat.startActivityForResult
import kotlinx.android.synthetic.main.bottom_button.view.*

/**
 * [RecyclerView.Adapter] that can display a [StudentItem] and makes a call to the
 * specified [OnListFragmentInteractionListener].
 */
class GradedStudentRecyclerViewAdapter(
    private val mValues: List<StudentItem>,
    private val mListener: OnListFragmentInteractionListener?
) : RecyclerView.Adapter<GradedStudentRecyclerViewAdapter.ViewHolder>() {

    private val mOnClickListener: View.OnClickListener

    init {
        mOnClickListener = View.OnClickListener { v ->
            val item = v.tag as StudentItem
            // Notify the active callbacks interface (the activity, if the fragment is attached to
            // one) that an item has been selected.
            mListener?.onListFragmentInteraction(item)
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        return if (viewType == R.layout.fragment_gradedstudent) {
            val view = LayoutInflater.from(parent.context).inflate(R.layout.fragment_gradedstudent, parent, false)
            GradedStudentViewHolder(view)
        } else {
            val view= LayoutInflater.from(parent.context).inflate(R.layout.bottom_button, parent, false)
            return ButtonsViewHolder(view)
        }
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        if (position == mValues.size) {
            (holder as ButtonsViewHolder).bind(holder.itemView)
        } else {
            val item = mValues[position]
            val studentHolder = holder as GradedStudentViewHolder
            studentHolder.bind(item, position)
            // TODO: Missing onclicklistener here
        }
    }

    override fun getItemViewType(position: Int): Int {
        return if (position == mValues.size) R.layout.bottom_button else R.layout.fragment_gradedstudent
    }

    override fun getItemCount(): Int = mValues.size + 1

    open inner class ViewHolder(mView: View): RecyclerView.ViewHolder(mView)

    inner class ButtonsViewHolder(mView: View) : ViewHolder(mView) {
        private val mPDFButton: Button = mView.uploadPDFBtn
        private val mSubmitButton: Button = mView.registerClassBtn

        fun bind(mView : View) {
            mPDFButton.setOnClickListener { uploadPDF(mView) }

            mSubmitButton.setOnClickListener { submit() }
        }

        private fun uploadPDF(view : View) {
            val intent = Intent()
                .setType("application/pdf")
                .setAction(Intent.ACTION_GET_CONTENT)

            view.context.startActivity(Intent.createChooser(intent, "Select a file"))
            mSubmitButton.isClickable = true
            mSubmitButton.isEnabled = true

            // todo - actually handle pfd uploading
        }

        private fun submit() {

        }

    }
    inner class GradedStudentViewHolder(mView: View) : ViewHolder(mView) {
        private val mName: TextView = mView.name
        private val mCode: TextView = mView.code
        private val mGrade: TextView = mView.grade
        private val mPos: TextView = mView.position

        fun bind(item: StudentItem, position: Int) {
            mName.text = item.name
            mCode.text = item.code.toString()
            mGrade.text = item.grade.toString()
            mPos.text = position.toString()
        }
    }
}
