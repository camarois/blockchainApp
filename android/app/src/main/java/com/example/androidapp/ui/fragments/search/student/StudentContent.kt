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
    val ITEMS: MutableList<StudentItem> = ArrayList()

    /**
     * A map of student items, by ID.
     */
    val ITEM_MAP: MutableMap<String, StudentItem> = HashMap()

    private val COUNT = 25

    init {
        // Add some sample items.
        for (i in 1..COUNT) {
            addItem(createStudentItem(i))
        }
    }

    private fun addItem(item: StudentItem) {
        ITEMS.add(item)
        ITEM_MAP[item.id] = item
    }

    private fun createStudentItem(position: Int): StudentItem {
        return StudentItem(position.toString(),
            "Prenom Nom $position", "GENIE INFO", (position * 4/25).toFloat(), 2000+position, "notes.pdf")
    }
}
