package com.example.androidapp.fragments.register

import android.os.Bundle
import android.view.View
import com.example.androidapp.R
import com.google.android.material.bottomsheet.BottomSheetDialogFragment

class RegisterStudentBottomFragment : BottomSheetDialogFragment() {

    var registerFragment: RegisterFragment? = null

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        registerFragment = (activity!!
            .supportFragmentManager
            .findFragmentById(R.id.register_fragment) as RegisterFragment)
        super.onViewCreated(view, savedInstanceState)
    }
}
