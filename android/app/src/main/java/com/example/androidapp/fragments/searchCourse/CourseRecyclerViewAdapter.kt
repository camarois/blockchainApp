package com.example.androidapp.fragments.searchCourse

import androidx.recyclerview.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import com.example.androidapp.R
import com.example.androidapp.fragments.searchCourse.SearchCourseFragment.OnListFragmentInteractionListener
import com.example.androidapp.fragments.searchCourse.course.CourseItem
import kotlinx.android.synthetic.main.fragment_course.view.*

/**
 * [RecyclerView.Adapter] that can display a [CourseItem] and makes a call to the
 * specified [OnListFragmentInteractionListener].
 */
class CourseRecyclerViewAdapter(
    private val mValues: List<CourseItem>,
    private val mListener: OnListFragmentInteractionListener?
) : RecyclerView.Adapter<CourseRecyclerViewAdapter.ViewHolder>() {

    private val mOnClickListener: View.OnClickListener

    init {
        mOnClickListener = View.OnClickListener { v ->
            val item = v.tag as CourseItem
            // Notify the active callbacks interface (the activity, if the fragment is attached to
            // one) that an item has been selected.
            mListener?.onListFragmentInteraction(item)
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view = LayoutInflater.from(parent.context)
            .inflate(R.layout.fragment_course, parent, false)
        return ViewHolder(view)
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        val item = mValues[position]
        holder.mNameView.text = item.name
        holder.mCodeView.text = item.code
        holder.mTrimesterView.text = item.trimester.toString()

        with(holder.mView) {
            tag = item
            setOnClickListener(mOnClickListener)
        }
    }

    override fun getItemCount(): Int = mValues.size

    inner class ViewHolder(val mView: View) : RecyclerView.ViewHolder(mView) {
        val mNameView: TextView = mView.name
        val mCodeView: TextView = mView.code
        val mTrimesterView: TextView = mView.trimester

        override fun toString(): String {
            return super.toString() + "Name: $mNameView, Code: $mCodeView, Trimestre: $mTrimesterView"
        }
    }
}
