package com.example.androidapp.fragments.register

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast

import com.example.androidapp.R
import com.example.androidapp.services.Utils
import kotlinx.android.synthetic.main.fragment_get_course_info.*

class GetCourseInfoFragment : Fragment() {

    private val registerCourseFragment = RegisterCourseFragment()
    private val validateCodeRegex = Regex("^[A-Z]{3}[0-9]{4}[A-Z]?\$")

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.fragment_get_course_info, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        submitButton.setOnClickListener {
            Utils.debounce(submitButton)
            submit()
        }
        super.onViewCreated(view, savedInstanceState)
    }

    private fun submit() {
        if (sigle.text.isEmpty() || name.text.isEmpty() || trimester.text.isEmpty()) {
            Toast.makeText(activity, getString(R.string.info_missing_data), Toast.LENGTH_SHORT).show()
        } else {
            if (trimester.text.length != 5) {
                Toast.makeText(activity, getString(R.string.error_invalid_trimester), Toast.LENGTH_SHORT).show()
                return
            }

            if (!validateCodeRegex.matches(sigle.text)) {
                Toast.makeText(activity, getString(R.string.error_invalid_code), Toast.LENGTH_SHORT).show()
                return
            }
            val bundle = Bundle()
            bundle.putString("code", sigle.text.toString())
            bundle.putString("name", name.text.toString())
            bundle.putInt("trimester", trimester.text.toString().toInt())
            swapFragment(bundle)
        }
    }

    private fun swapFragment(bundle: Bundle) {
        val transaction = fragmentManager!!.beginTransaction()
        registerCourseFragment.arguments = bundle
        transaction.replace(R.id.fragment, registerCourseFragment)
        transaction.commit()
        course_info_fragment.visibility = View.GONE
    }
}
