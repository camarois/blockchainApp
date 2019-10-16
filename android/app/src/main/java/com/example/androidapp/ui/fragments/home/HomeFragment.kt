package com.example.androidapp.ui.fragments.home

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import com.example.androidapp.R
import com.example.androidapp.ui.MainController
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Job
import kotlinx.coroutines.*
import org.koin.android.ext.android.get
import kotlin.coroutines.CoroutineContext

class HomeFragment : Fragment(), CoroutineScope {

    private var controller: MainController = get()
    private lateinit var textView: TextView
    private lateinit var job: Job

    override val coroutineContext: CoroutineContext
        get() = job + Dispatchers.Main

    private lateinit var homeViewModel: HomeViewModel

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        job = Job()
        homeViewModel =
            ViewModelProviders.of(this).get(HomeViewModel::class.java)
        val root = inflater.inflate(R.layout.fragment_home, container, false)
        textView = root.findViewById(R.id.lolVar)
        textView.text = "lol!"
        root.findViewById<Button>(R.id.refreshBtn).setOnClickListener { launch {
            try {
                textView.text = controller.onRefreshLolAsync()
            } catch (e: Exception) {
                textView.text = "${getString(R.string.error_message_unknown)}: ${e.message}"
            }
        } }

        return root
    }
}
