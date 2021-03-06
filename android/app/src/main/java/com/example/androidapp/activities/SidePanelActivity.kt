package com.example.androidapp.activities

import android.content.Intent
import android.os.Bundle
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.navigateUp
import androidx.navigation.ui.setupActionBarWithNavController
import androidx.navigation.ui.setupWithNavController
import androidx.appcompat.app.AppCompatActivity
import android.view.Menu
import android.view.MenuItem
import com.example.androidapp.AccountTypes
import com.example.androidapp.CourseRequest
import com.example.androidapp.CourseItem
import com.example.androidapp.R
import com.example.androidapp.StudentItem
import com.example.androidapp.fragments.register.RegisterCourseFragment
import com.example.androidapp.StudentRequest
import com.example.androidapp.fragments.searchCourse.DetailedCourseFragment
import com.example.androidapp.fragments.searchStudent.SearchStudentFragment
import com.example.androidapp.fragments.searchCourse.SearchCourseFragment
import com.example.androidapp.fragments.searchStudent.DetailedStudentFragment
import com.example.androidapp.services.RestRequestService
import com.example.androidapp.services.Utils
import com.google.android.material.navigation.NavigationView
import kotlinx.android.synthetic.main.activity_side_panel.*
import kotlinx.android.synthetic.main.app_bar_side_panel.*
import kotlinx.android.synthetic.main.nav_header_side_panel.*
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import org.koin.android.ext.android.get
import kotlin.coroutines.CoroutineContext

class SidePanelActivity : AppCompatActivity(), CoroutineScope, SearchCourseFragment.OnListFragmentInteractionListener, SearchStudentFragment.OnListFragmentInteractionListener, RegisterCourseFragment.OnListFragmentInteractionListener {
    private lateinit var job: Job
    private lateinit var appBarConfiguration: AppBarConfiguration
    private var restService: RestRequestService = get()
    var username: String = ""
    lateinit var type: AccountTypes

    override val coroutineContext: CoroutineContext
        get() = job + Dispatchers.Main

    override fun onListFragmentInteraction(course: CourseItem) {
        launch {
            Utils.processRequest(this@SidePanelActivity) {
                val transaction = supportFragmentManager.beginTransaction()
                val response = restService.postCourseInfoAsync(
                    CourseRequest(
                        course.code,
                        course.trimester.toInt()
                    )
                )
                val frag = DetailedCourseFragment(course, response.students)
                transaction.replace(R.id.course_list_fragment, frag)
                transaction.commit()
            }
        }
    }

    override fun onListFragmentInteraction(student: StudentItem) {
        launch {
            Utils.processRequest(this@SidePanelActivity) {
                val transaction = supportFragmentManager.beginTransaction()
                val response =
                    restService.postStudentInfoAsync(StudentRequest("*", "*", student.code))
                val frag = DetailedStudentFragment(student, response.results)

                transaction.replace(R.id.student_list_fragment, frag)
                transaction.commit()
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        job = Job()
        setContentView(R.layout.activity_side_panel)
        setSupportActionBar(sidePanelToolbar)

        username = intent.getStringExtra("username")
        type = intent!!.getSerializableExtra("type") as AccountTypes

        val navController = findNavController(R.id.sidePanelFragment)
        appBarConfiguration = AppBarConfiguration(
            setOf(
                R.id.nav_home, R.id.nav_search, R.id.nav_course_search, R.id.nav_settings, R.id.nav_register
            ), sidePanelDrawerLayout
        )

        setupActionBarWithNavController(navController, appBarConfiguration)
        sidePanelNavigationView.setupWithNavController(navController)

        if (type == AccountTypes.CONSULTATION) {
            val v: NavigationView = findViewById(R.id.sidePanelNavigationView)
            val menu = v.menu
            menu.setGroupVisible(R.id.editionGroup, false)
        }
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        if (type == AccountTypes.EDITION) {
            textView.text = resources.getString(R.string.nav_header_subtitle_edition)
        } else {
            textView.text = resources.getString(R.string.nav_header_subtitle_consultation)
        }

        menuInflater.inflate(R.menu.side_panel, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        when (item.itemId) {
            R.id.action_logout -> {
                launch { logout() }
                return true
            }
            else -> return super.onOptionsItemSelected(item)
        }
    }
    private suspend fun logout() {
        Utils.processRequest(this@SidePanelActivity) {
            restService.postLogoutAsync()
            val intent = Intent(this@SidePanelActivity, MainActivity::class.java).apply { }
            startActivity(intent)
        }
    }

    override fun onSupportNavigateUp(): Boolean {
        val navController = findNavController(R.id.sidePanelFragment)
        return navController.navigateUp(appBarConfiguration) || super.onSupportNavigateUp()
    }
}
