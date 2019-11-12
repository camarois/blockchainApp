package com.example.androidapp.fragments.home

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.example.androidapp.AccountTypes
import com.example.androidapp.R
import com.example.androidapp.activities.SidePanelActivity
import kotlinx.android.synthetic.main.fragment_home.*

class HomeFragment : Fragment() {
    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.fragment_home, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        username.text = (activity!! as SidePanelActivity).username
        val type = (activity!! as SidePanelActivity).type

        description.text = if (type == AccountTypes.EDITION) resources.getString(R.string.edition_description) else resources.getString(R.string.consultation_description)
        super.onViewCreated(view, savedInstanceState)
    }
}
