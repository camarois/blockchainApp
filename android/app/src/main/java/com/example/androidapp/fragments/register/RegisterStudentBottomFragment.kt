package com.example.androidapp.fragments.register

import android.app.Dialog
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.EditText
import android.widget.ImageButton
import com.example.androidapp.R
import com.example.androidapp.ui.fragments.search.student.StudentItem
import com.google.android.material.bottomsheet.BottomSheetDialogFragment
import kotlinx.android.synthetic.main.add_student_bottom_panel.*
import kotlinx.android.synthetic.main.fragment_register_list.*

class RegisterStudentBottomFragment : BottomSheetDialogFragment() {

    var registerFragment: RegisterFragment? = null

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        registerFragment = (activity!!
            .supportFragmentManager
            .findFragmentById(R.id.register_fragment) as RegisterFragment)

        //createStudentButton.setOnClickListener { createStudent() }

        super.onViewCreated(view, savedInstanceState)
    }
}
