package com.example.androidapp.fragments.searchStudent

import androidx.recyclerview.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import com.example.androidapp.CourseResult
import com.example.androidapp.R
import kotlinx.android.synthetic.main.fragment_student_result.view.*

class StudentResultsRecyclerViewAdapter(
    private val mValues: List<CourseResult>
) : RecyclerView.Adapter<StudentResultsRecyclerViewAdapter.ViewHolder>() {

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view = LayoutInflater.from(parent.context)
            .inflate(R.layout.fragment_student_result, parent, false)
        return ViewHolder(view)
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        val item = mValues[position]
        holder.mNameView.text = item.acronym
        holder.mTrimester.text = item.trimester
        holder.mResult.text = item.grade

        with(holder.mView) {
            tag = item
        }
    }

    override fun getItemCount(): Int = mValues.size

    inner class ViewHolder(val mView: View) : RecyclerView.ViewHolder(mView) {
        val mNameView: TextView = mView.sigle
        val mTrimester: TextView = mView.trimester
        val mResult: TextView = mView.result
    }
}
