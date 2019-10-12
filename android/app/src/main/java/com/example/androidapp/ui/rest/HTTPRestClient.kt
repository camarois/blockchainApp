package com.example.androidapp.ui.rest

import android.content.Context
import com.android.volley.Request
import com.android.volley.RequestQueue
import com.android.volley.toolbox.Volley
import java.io.BufferedInputStream
import java.security.KeyStore
import java.security.cert.CertificateFactory
import java.security.cert.X509Certificate
import io.fabric.sdk.android.services.network.NetworkUtils.getSSLSocketFactory
import com.android.volley.toolbox.HurlStack
import android.icu.lang.UCharacter.GraphemeClusterBreak.T
import android.icu.lang.UCharacter.GraphemeClusterBreak.T
import javax.net.ssl.*
import io.fabric.sdk.android.services.network.NetworkUtils.getSSLSocketFactory
import android.icu.lang.UCharacter.GraphemeClusterBreak.T
import java.net.URL
import io.fabric.sdk.android.services.network.NetworkUtils.getSSLSocketFactory
import android.icu.lang.UCharacter.GraphemeClusterBreak.T
import java.net.HttpURLConnection


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
