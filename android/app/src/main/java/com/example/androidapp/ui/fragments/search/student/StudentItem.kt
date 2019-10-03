package com.example.androidapp.ui.fragments.search.student

/**
 * A student item representing a piece of content.
 */
data class StudentItem(val id: String, val prenom: String, val nom: String, val gpa: Float, val grad: Int, val transcript: String) {
    override fun toString(): String = "$prenom $nom, $gpa"
}