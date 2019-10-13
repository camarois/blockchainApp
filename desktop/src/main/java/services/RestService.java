package services;

import com.google.gson.Gson;
import constants.ServerUrls;

import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManagerFactory;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.security.KeyStore;
import java.security.cert.Certificate;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.function.Consumer;

public class RestService {
    public static ServerUrls urls;
    private static String baseUrl = "";
    private static ExecutorService threadPool;
    private static HttpClient httpClient;

    public static void init() {
        InputStreamReader reader = new InputStreamReader(
                RestService.class.getClassLoader().getResourceAsStream("values/strings.json")
        );
        urls = new Gson().fromJson(reader, ServerUrls.class);

        threadPool = Executors.newFixedThreadPool(2);
        httpClient = HttpClient.newHttpClient();
        requestAsync(urls.firebase + "server", (resp) -> {
            baseUrl = "https://" + resp + ":10000/";
            System.out.println("Connected to: " + baseUrl);
        });
        initSSLContext();
    }

    public static void requestAsync(String url, Consumer<String> onResponse) {
        threadPool.submit(() -> {
            String resp = request(url);
            onResponse.accept(resp);
        });
    }

    public static Future<String> requestAsync(String url) {
        return threadPool.submit(() -> request(url));
    }

    public static String request(String url) {
        try {
            HttpRequest request = HttpRequest.newBuilder()
                    .uri(URI.create(baseUrl + url))
                    .build();

            HttpResponse<String> response = httpClient.send(request, HttpResponse.BodyHandlers.ofString());
            return response.body();
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    private static void initSSLContext() {
        try {
            InputStream caInput = RestService.class.getClassLoader().getResourceAsStream("cert/rootCA.crt");
            CertificateFactory cf = CertificateFactory.getInstance("X.509");

            Certificate ca = cf.generateCertificate(caInput);
            System.out.println("ca=" + ((X509Certificate) ca).getSubjectDN());
            caInput.close();

            String keyStoreType = KeyStore.getDefaultType();
            KeyStore keyStore = KeyStore.getInstance(keyStoreType);
            keyStore.load(null, null);
            keyStore.setCertificateEntry("ca", ca);

            String tmfAlgorithm = TrustManagerFactory.getDefaultAlgorithm();
            TrustManagerFactory tmf = TrustManagerFactory.getInstance(tmfAlgorithm);
            tmf.init(keyStore);

            SSLContext sslContext = SSLContext.getInstance("TLS");
            sslContext.init(null, tmf.getTrustManagers(), null);

            httpClient = HttpClient.newBuilder().sslContext(sslContext).build();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
