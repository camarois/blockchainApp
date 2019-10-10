package com.example.androidapp.ui.fragments.search

import androidx.recyclerview.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageButton
import android.widget.TextView
import com.example.androidapp.R
import com.example.androidapp.ui.fragments.search.SearchFragment.OnListFragmentInteractionListener
import com.example.androidapp.ui.fragments.search.student.StudentItem
import kotlinx.android.synthetic.main.fragment_student.view.*

/**
 * [RecyclerView.Adapter] that can display a [StudentItem] and makes a call to the
 * specified [OnListFragmentInteractionListener].
 */
class MyStudentRecyclerViewAdapter(
    private val mValues: List<StudentItem>,
    private val mListener: OnListFragmentInteractionListener?
) : RecyclerView.Adapter<MyStudentRecyclerViewAdapter.ViewHolder>() {

    private val mOnClickListener: View.OnClickListener

    init {
        mOnClickListener = View.OnClickListener { v ->
            val item = v.tag as StudentItem
            // Notify the active callbacks interface (the activity, if the fragment is attached to
            // one) that an item has been selected.
            mListener?.onListFragmentInteraction(item)
        }
    }

    private fun toggleFavorite(button: ImageButton, studentItem: StudentItem) {
        // TODO: Actually save favorites
        if (studentItem.isFav) {
            button.setImageResource(R.drawable.ic_star_border_black_24dp)
        } else {
            button.setImageResource(R.drawable.ic_star_black_24dp)
        }
        studentItem.isFav = !studentItem.isFav
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view = LayoutInflater.from(parent.context)
            .inflate(R.layout.fragment_student, parent, false)
        return ViewHolder(view)
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        val item = mValues[position]
        holder.mNameView.text = item.name
        holder.mDegreeView.text = item.degree
        holder.mGPAView.text = item.gpa.toString()

        holder.mFavButton.setOnClickListener { toggleFavorite(holder.mFavButton, item) }

        with(holder.mView) {
            tag = item
            setOnClickListener(mOnClickListener)
        }
    }

    override fun getItemCount(): Int = mValues.size

    inner class ViewHolder(val mView: View) : RecyclerView.ViewHolder(mView) {
        val mNameView: TextView = mView.name
        val mDegreeView: TextView = mView.degree
        val mGPAView: TextView = mView.gpa
        val mFavButton: ImageButton = mView.fav_button
    }
}
