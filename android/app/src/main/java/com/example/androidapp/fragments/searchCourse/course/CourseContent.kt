package com.example.androidapp.fragments.searchCourse.course

import com.example.androidapp.CourseItem
import java.util.ArrayList
import java.util.HashMap

/**
 * Helper class for providing sample content for user interfaces created by
 * Android template wizards.
 *
 */
object CourseContent {

    /**
     * An array of student items.
     */
    val items: MutableList<CourseItem> = ArrayList()

    /**
     * A map of student items, by ID.
     */
    private val item_map: MutableMap<String, CourseItem> = HashMap()

    init {
        // Add some sample items.
        // TODO: Fill with information from database
    }

    private fun addItem(item: CourseItem) {
        items.add(item)
        item_map[item.code] = item
    }
}
