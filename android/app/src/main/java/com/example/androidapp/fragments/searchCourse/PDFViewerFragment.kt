package com.example.androidapp.fragments.searchCourse

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup

import com.example.androidapp.R
import kotlinx.android.synthetic.main.fragment_pdfviewer.*
import java.io.File

class PDFViewerFragment(file: File) : Fragment() {

    val pdf = file

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_pdfviewer, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        pdfView.fromFile(pdf)
            .swipeHorizontal(true)
            .pageSnap(true)
            .autoSpacing(true)
            .pageFling(true)
            .load()
    }
}
