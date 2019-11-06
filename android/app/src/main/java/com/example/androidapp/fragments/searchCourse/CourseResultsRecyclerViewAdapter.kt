package com.example.androidapp.fragments.searchCourse

import androidx.recyclerview.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import com.example.androidapp.R
import com.example.androidapp.StudentItem
import kotlinx.android.synthetic.main.fragment_course_result.view.*

class CourseResultsRecyclerViewAdapter(
    private val mValues: List<StudentItem>
) : RecyclerView.Adapter<CourseResultsRecyclerViewAdapter.ViewHolder>() {

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view = LayoutInflater.from(parent.context)
            .inflate(R.layout.fragment_course_result, parent, false)
        return ViewHolder(view)
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        val item = mValues[position]
        holder.mNameView.text = item.firstName + item.lastName
        holder.mCode.text = item.code
        holder.mGrade.text = item.grade

        with(holder.mView) {
            tag = item
        }
    }

    override fun getItemCount(): Int = mValues.size

    inner class ViewHolder(val mView: View) : RecyclerView.ViewHolder(mView) {
        val mNameView: TextView = mView.student_name
        val mCode: TextView = mView.code
        val mGrade: TextView = mView.grade
    }
}
