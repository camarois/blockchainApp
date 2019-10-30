package services;

import controllers.LoginController;
import com.google.gson.Gson;
import constants.ServerUrls;
import controllers.LogsViewer;
import models.LoginRequest;
import models.LoginResponse;

import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManagerFactory;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.lang.reflect.Type;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.security.KeyStore;
import java.security.cert.Certificate;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.function.Consumer;

public class RestService {
    public static ServerUrls urls;
    private static String baseUrl = "";
    private static Gson gson;
    private static ExecutorService threadPool;
    private static HttpClient httpClient;
    private static RestService instance;

    public static void init() {
        gson = new Gson();
        InputStreamReader reader = new InputStreamReader(
                RestService.class.getClassLoader().getResourceAsStream("values/strings.json")
        );
        urls = gson.fromJson(reader, ServerUrls.class);

        threadPool = Executors.newFixedThreadPool(2);
        httpClient = HttpClient.newHttpClient();
        requestGetAsync(urls.firebase + "server", (resp) -> {
            baseUrl = "https://" + resp + ":10000/";
            System.out.println("Connected to: " + baseUrl);
        });
        initSslContext();
    }

    public static RestService getInstance() {
        if (instance == null) {
            instance = new RestService();
            instance.init();
        }
        return instance;
    }

    public LoginResponse postLoginAsync(LoginRequest request) {
        try {
            return (LoginResponse) requestPostAsync("usager/login", request, LoginResponse.class, System.out::println);
        } catch (Exception e) {
            LoginController.getInstance().showErrorDialog("Le nom d'utilisateur et/ou le mot de passe est invalide.");
            return null;
        }
    }

    public static void requestGetAsync(String url, Consumer<String> onResponse) {
        threadPool.submit(() -> {
            String resp = getRequest(url);
            onResponse.accept(resp);
        });
    }

    public static <T> Future<?> requestPostAsync(String url, Object data, T classOfT, Consumer<T> onResponse) {
        return threadPool.submit(() -> {
            String resp = postRequest(url, data);
            onResponse.accept(gson.fromJson(resp, (Type) classOfT));
        });
    }

    public static String getRequest(String url) {
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

    public static String postRequest(String url, Object data) {
        try {
            HttpRequest request = HttpRequest.newBuilder()
                    .uri(URI.create(baseUrl + url))
                    .headers("Authorization", "token")
                    .POST(HttpRequest.BodyPublishers.ofString(gson.toJson(data)))
                    .build();

            HttpResponse<String> response = httpClient.send(request, HttpResponse.BodyHandlers.ofString());
            return response.body();
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    private static void initSslContext() {
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
