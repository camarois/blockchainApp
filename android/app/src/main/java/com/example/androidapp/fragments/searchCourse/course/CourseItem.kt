package com.example.androidapp.fragments.searchCourse.course

import com.example.androidapp.StudentItem
import com.google.gson.annotations.SerializedName

/**
 * A course item representing a piece of content.
 */
data class CourseItem(val code: String, val name: String, val trimester: String, val students: List<StudentItem>) {
    override fun toString(): String = "Nom: $name, Sigle: $code, Name "
}