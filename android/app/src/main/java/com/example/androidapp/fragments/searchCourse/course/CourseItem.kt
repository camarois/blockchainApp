package com.example.androidapp.fragments.searchCourse.course

/**
 * A course item representing a piece of content.
 */
data class CourseItem(val code: String, val name: String, val trimester: String) {
    override fun toString(): String = "Nom: $name, Sigle: $code, Name "
}