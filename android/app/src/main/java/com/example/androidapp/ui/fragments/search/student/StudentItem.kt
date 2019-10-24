package com.example.androidapp.ui.fragments.search.student

/**
 * A student item representing a piece of content.
 */
data class StudentItem(val id: String, val name: String, val code: Int, val grade: Float? = null) {
    override fun toString(): String = "Nom: $name, Matricule: $code"
}
