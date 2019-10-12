package com.example.androidapp.ui.rest

import android.content.Context
import com.android.volley.Request
import com.android.volley.RequestQueue
import com.android.volley.toolbox.HurlStack
import com.android.volley.toolbox.Volley
import java.io.BufferedInputStream
import java.net.HttpURLConnection
import java.net.URL
import java.security.KeyStore
import java.security.cert.CertificateFactory
import java.security.cert.X509Certificate
import javax.net.ssl.HttpsURLConnection
import javax.net.ssl.SSLContext
import javax.net.ssl.TrustManagerFactory

class HTTPRestClient(private val context: Context) {

    private lateinit var hurlStack: HurlStack

    init {
        initHttps()
    }

    private fun initHttps() {
        val cf = CertificateFactory.getInstance("X.509")
        val caInput = BufferedInputStream(context.assets.open("rootCA.crt"))
        val ca = caInput.use {
            cf.generateCertificate(it) as X509Certificate
        }
        println("ca=" + ca.subjectDN)

        val keyStoreType = KeyStore.getDefaultType()
        val keyStore = KeyStore.getInstance(keyStoreType).apply {
            load(null, null)
            setCertificateEntry("ca", ca)
        }

        val tmfAlgorithm = TrustManagerFactory.getDefaultAlgorithm()
        val tmf = TrustManagerFactory.getInstance(tmfAlgorithm).apply {
            init(keyStore)
        }

        val context = SSLContext.getInstance("TLS").apply {
            init(null, tmf.trustManagers, null)
        }

        hurlStack = object : HurlStack() {
            override fun createConnection(url: URL): HttpURLConnection {
                val httpsURLConnection = super
                    .createConnection(url) as HttpsURLConnection
                httpsURLConnection.sslSocketFactory = context.socketFactory
                return httpsURLConnection
            }
        }
    }
    
    private val requestQueue: RequestQueue by lazy {
        Volley.newRequestQueue(context, hurlStack)
    }

    fun <T> addToRequestQueue(req: Request<T>) {
        requestQueue.add(req)
    }
}
