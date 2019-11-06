package com.example.androidapp.fragments.searchCourse.course

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

    private const val COUNT = 25
    private const val BASE_COURSE_CODE = 1000
    private const val BASE_YEAR = 2000

    init {
        // Add some sample items.
        // TODO: Fill with information from database
        for (i in 1..COUNT) {
            addItem(createCourseItem(i))
        }
    }

    private fun addItem(item: CourseItem) {
        items.add(item)
        item_map[item.code] = item
    }

    private fun createCourseItem(position: Int): CourseItem {
        return CourseItem("MTH$BASE_COURSE_CODE$position", "Nom $position", "A$BASE_YEAR")
    }
}
