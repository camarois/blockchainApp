package com.example.androidapp.ui

import android.content.Intent
import android.os.Bundle
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.navigateUp
import androidx.navigation.ui.setupActionBarWithNavController
import androidx.navigation.ui.setupWithNavController
import androidx.drawerlayout.widget.DrawerLayout
import com.google.android.material.navigation.NavigationView
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.Toolbar
import android.view.Menu
import com.example.androidapp.R
import com.example.androidapp.ui.fragments.StudentInfoFragment
import com.example.androidapp.ui.fragments.search.SearchFragment
import com.example.androidapp.ui.fragments.search.student.StudentItem


class SidePanelActivity : AppCompatActivity(), SearchFragment.OnListFragmentInteractionListener {

    override fun onListFragmentInteraction(item: StudentItem) {
        // StudentInfoFragment.beginTransaction().replace(R.id.action_container,StudentInfoFragment.getInstance()).commit()
        // TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }

    private lateinit var appBarConfiguration: AppBarConfiguration

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_side_panel)
        val toolbar: Toolbar = findViewById(R.id.toolbar)
        setSupportActionBar(toolbar)

        val drawerLayout: DrawerLayout = findViewById(R.id.drawer_layout)
        val navView: NavigationView = findViewById(R.id.nav_view)
        val navController = findNavController(R.id.nav_host_fragment)
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        appBarConfiguration = AppBarConfiguration(
            setOf(
                R.id.nav_home, R.id.nav_search, R.id.nav_register
            ), drawerLayout
        )

        setupActionBarWithNavController(navController, appBarConfiguration)
        navView.setupWithNavController(navController)
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // TODO: Add on call listener for logout btn but HOW TO ACCESS IT GRRR
        // Inflate the menu; this adds items to the action bar if it is present.
        menuInflater.inflate(R.menu.side_panel, menu)
        return true
    }

    private fun logout() {
        // TODO: HOW TO CALL THIS METHOD ;-;
        // TODO: Actually logout properly
        val intent = Intent(this@SidePanelActivity, MainActivity::class.java).apply { }
        startActivity(intent)
    }

    override fun onSupportNavigateUp(): Boolean {
        val navController = findNavController(R.id.nav_host_fragment)
        return navController.navigateUp(appBarConfiguration) || super.onSupportNavigateUp()
    }
}
