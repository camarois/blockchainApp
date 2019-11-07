package com.example.androidapp.services

import android.content.Context
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.StringRequest
import com.android.volley.toolbox.Volley
import com.example.androidapp.*
import kotlin.coroutines.resume
import kotlin.coroutines.suspendCoroutine
import kotlin.coroutines.resumeWithException

class RestRequestService(private val httpClient: HTTPRestClient, private val context: Context, private val credentialsManager: CredentialsManager) {
    private lateinit var serverUrl: String

    init {
        initServerUrl("server")
    }

    fun initServerUrl(user: String) {
        val baseUrl = "https://us-central1-projet3-46f1b.cloudfunctions.net/getServerURL?user=$user"
        val request = StringRequest(
            Request.Method.GET, baseUrl, {
                serverUrl = "https://10.200.23.190:10000"
                httpClient.initHttps()
            }, {
                serverUrl = it.toString()
            })
        val requestQueue = Volley.newRequestQueue(context)
        requestQueue.add(request)
    }

    suspend fun getStudentListAsync(): ListStudent {
        return getAsync("info/listeEtudiants", ListStudent::class.java)
    }

    suspend fun getClassListAsync(): ListClass {
        return getAsync("info/listeCours", ListClass::class.java)
    }

    suspend fun postCourseInfoAsync(request: CourseRequest): CourseResponse {
        return postAsync("info/cours", request, CourseResponse::class.java)
    }

    suspend fun postStudentInfoAsync(request: StudentRequest): StudentResponse {
        return postAsync("info/etudiant", request, StudentResponse::class.java)
    }

    suspend fun postPdfFileAsync(request: PdfFileRequest): ByteArray {
        return postAsync("fichier/notes", request, ByteArray::class.java, true)
    }

    suspend fun postLoginAsync(request: LoginRequest): LoginResponse {
        return postAsync("usager/login", request, LoginResponse::class.java)
    }

    suspend fun postLogoutAsync(): String {
        return postAsync("usager/logout", "", String::class.java)
    }

    suspend fun postChangePasswordAsync(request: PasswordRequest): String {
        return postAsync("usager/motdepasse", request, String::class.java)
    }

    suspend fun postTransactionAsync(request: TransactionRequest): String {
        return postAsync("transaction", request, String::class.java)
    }

    private suspend fun <T> getAsync(url: String, classOfT: Class<T>, isFile: Boolean = false): T {
        return baseRequestAsync(Request.Method.GET, url, "", classOfT, isFile)
    }

    private suspend fun <T> postAsync(url: String, data: Any, classOfT: Class<T>, isFile: Boolean = false): T {
        return baseRequestAsync(Request.Method.POST, url, data, classOfT, isFile)
    }

    private suspend fun <T> baseRequestAsync(method: Int, url: String, data: Any, classOfT: Class<T>, isFile: Boolean): T {
        return suspendCoroutine { continuation ->
            val request = GsonRequest(context, credentialsManager, method, "$serverUrl/$url", data, classOfT, isFile,
                mutableMapOf(
                    CredentialsManager.HTTP_HEADER_AUTHORIZATION to credentialsManager.getAuthToken(context)
                ),
                Response.Listener { response ->
                    continuation.resume(response)
                },
                Response.ErrorListener { continuation.resumeWithException(it) }
            )
            httpClient.addToRequestQueue(request)
        }
    }
}
