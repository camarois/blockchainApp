package com.example.androidapp.fragments.register

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup

import com.example.androidapp.R
import kotlinx.android.synthetic.main.fragment_get_course_info.*

class GetCourseInfoFragment : Fragment() {

    private val registerCourseFragment = RegisterCourseFragment()
    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_get_course_info, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        submitButton.setOnClickListener{ submit() }
        super.onViewCreated(view, savedInstanceState)
    }

    override fun onResume() {
        super.onResume()
        course_info_fragment.visibility = View.VISIBLE
    }

    private fun submit() {
        val transaction = fragmentManager!!.beginTransaction()
        transaction.replace(R.id.fragment, registerCourseFragment)
        transaction.commit()
        course_info_fragment.visibility = View.GONE
    }
}
