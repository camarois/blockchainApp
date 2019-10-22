package com.example.androidapp.ui.fragments.search.student

import java.util.ArrayList
import java.util.HashMap

/**
 * Helper class for providing sample content for user interfaces created by
 * Android template wizards.
 *
 */
object StudentContent {

    /**
     * An array of student items.
     */
    val items: MutableList<StudentItem> = ArrayList()

    /**
     * A map of student items, by ID.
     */
    val item_map: MutableMap<String, StudentItem> = HashMap()

    private val COUNT = 25
    private val MAX_GPA = 4.0
    private val MIN_GRAD_YEAR = 2000

    init {
        // Add some sample items.
        for (i in 1..COUNT) {
            addItem(createStudentItem(i))
        }
    }

    private fun addItem(item: StudentItem) {
        items.add(item)
        item_map[item.id] = item
    }

    private fun createStudentItem(position: Int): StudentItem {
        return StudentItem(position.toString(),
            "Prenom Nom $position", "GENIE INFO", (position * MAX_GPA / COUNT).toFloat(), MIN_GRAD_YEAR + position, "notes.pdf")
    }
}
