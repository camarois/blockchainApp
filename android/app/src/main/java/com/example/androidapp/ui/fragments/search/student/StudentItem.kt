package com.example.androidapp.ui.fragments.search.student

/**
 * A student item representing a piece of content.
 */
data class StudentItem(val id: String, val name: String, val degree: String, val gpa: Float, val grad: Int, val transcript: String) {
    override fun toString(): String = "Nom: $name, Programme: $degree, Cote: $gpa"
}
