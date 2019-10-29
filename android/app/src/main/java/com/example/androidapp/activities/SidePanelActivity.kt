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
import com.example.androidapp.R
import com.example.androidapp.fragments.search.SearchFragment
import com.example.androidapp.services.RestRequestService
import com.example.androidapp.ui.fragments.search.student.StudentItem
import kotlinx.android.synthetic.main.activity_side_panel.*
import kotlinx.android.synthetic.main.app_bar_side_panel.*
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import org.koin.android.ext.android.get
import kotlin.coroutines.CoroutineContext





class SidePanelActivity : AppCompatActivity(), CoroutineScope, SearchFragment.OnListFragmentInteractionListener {

    override fun onListFragmentInteraction(item: StudentItem) {
        // TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }

    private lateinit var job: Job
    private lateinit var appBarConfiguration: AppBarConfiguration
    private var restService: RestRequestService = get()

    override val coroutineContext: CoroutineContext
        get() = job + Dispatchers.Main

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        job = Job()
        setContentView(R.layout.activity_side_panel)
        setSupportActionBar(sidePanelToolbar)

        val navController = findNavController(R.id.sidePanelFragment)
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        appBarConfiguration = AppBarConfiguration(
            setOf(
                R.id.nav_home, R.id.nav_search, R.id.nav_register
            ), sidePanelDrawerLayout
        )

        setupActionBarWithNavController(navController, appBarConfiguration)
        sidePanelNavigationView.setupWithNavController(navController)
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // TODO: Add on call listener for logout btn but HOW TO ACCESS IT GRRR
        // Inflate the menu; this adds items to the action bar if it is present.
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
        restService.postLogoutAsync()
        val intent = Intent(this@SidePanelActivity, MainActivity::class.java).apply { }
        startActivity(intent)
    }

    override fun onSupportNavigateUp(): Boolean {
        val navController = findNavController(R.id.sidePanelFragment)
        return navController.navigateUp(appBarConfiguration) || super.onSupportNavigateUp()
    }
}
