package services;

import com.google.gson.Gson;
import com.google.gson.JsonObject;
import constants.ServerUrls;
import javafx.concurrent.Task;
import javafx.concurrent.WorkerStateEvent;
import javafx.event.EventHandler;
import models.LogsResponse;
import models.LogsRequest;
import models.ChaineRequest;
import models.PasswordRequest;
import models.LoginRequest;
import models.LoginResponse;

import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManagerFactory;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URI;
import java.net.http.HttpClient;

import org.apache.http.HttpException;
import org.apache.http.HttpStatus;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.security.KeyStore;
import java.security.cert.Certificate;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.util.concurrent.Future;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ExecutionException;

import java.util.function.Consumer;
import java.lang.String;

public class RestService {
    private static final String HTTP_GET_METHOD = "GET";
    private static final String HTTP_POST_METHOD = "POST";
    private static String baseUrl = "";
    private static Gson gson;
    private static ExecutorService threadPool;
    private static HttpClient httpClient;
    public static ServerUrls urls;

    public static void init() {
        gson = new Gson();
        InputStreamReader reader = new InputStreamReader(
                RestService.class.getClassLoader().getResourceAsStream("values/strings.json")
        );
        urls = gson.fromJson(reader, ServerUrls.class);

        threadPool = Executors.newFixedThreadPool(2);
        httpClient = HttpClient.newHttpClient();
        initSslContext();
    }

    public static <T> Future postRequestAsync(String url, Object data, Class<T> classOfT,
                                              Consumer<T> onSucceed, Consumer<WorkerStateEvent> onFail) {
        Task<T> task = new Task<>() {
            @Override
            public T call() throws Exception {
                return postRequest(url, data, classOfT);
            }
        };
        task.setOnSucceeded(e -> onSucceed.accept(task.getValue()));
        task.setOnFailed(onFail::accept);
        return threadPool.submit(task);
    }

    public static <T> Future postRequestAsync(String url, Object data, Class<T> classOfT,
                                              Consumer<T> onSucceed) {
        return postRequestAsync(url, data, classOfT, onSucceed, (e) -> {});
    }

    public static <T> Future postRequestAsync(String url, Object data) {
        return postRequestAsync(url, data, null, (e) -> {}, (e) -> {});
    }

    public static <T> Future getRequestAsync(String url, Class<T> classOfT,
                                             Consumer<T> onSucceed, Consumer<WorkerStateEvent> onFail) {
        Task<T> task = new Task<>() {
            @Override
            public T call() throws Exception {
                return getRequest(url, classOfT);
            }
        };
        task.setOnSucceeded(e -> onSucceed.accept(task.getValue()));
        task.setOnFailed(onFail::accept);
        return threadPool.submit(task);
    }

    public static <T> Future getRequestAsync(String url, Class<T> classOfT, Consumer<T> onSucceed) {
        return getRequestAsync(url, classOfT, onSucceed, (e) -> {});
    }

    public static <T> Future getRequestAsync(String url) {
        return getRequestAsync(url, null, (e) -> {}, (e) -> {});
    }

    public static <T> T getRequest(String url, Class<T> classOfT) throws Exception {
        return gson.fromJson(baseRequest(HTTP_GET_METHOD, url, ""), classOfT);
    }

    public static <T> T postRequest(String url, Object data, Class<T> classOfT) throws Exception {
        return gson.fromJson(baseRequest(HTTP_POST_METHOD, url, data), classOfT);
    }

    private static String baseRequest(String method, String url, Object data) throws Exception {
        try {
            HttpRequest request = HttpRequest.newBuilder()
                    .uri(URI.create(baseUrl + url))
                    .headers("Authorization", CredentialsManager.getInstance().getAuthToken())
                    .method(method, HttpRequest.BodyPublishers.ofString(gson.toJson(data)))
                    .build();

            HttpResponse<String> response = httpClient.send(request, HttpResponse.BodyHandlers.ofString());
            if (response.statusCode() == HttpStatus.SC_OK) {
                String authToken = response.headers().allValues("Authorization").get(0);
                CredentialsManager.saveAuthToken(authToken);
                return response.body();
            } else {
                throw new Exception("Forbidden");
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        }
    }

    private static void initSslContext() {
        try {
            HttpRequest request = HttpRequest.newBuilder()
                    .uri(URI.create(urls.getFirebase() + "server"))
                    .build();

            HttpResponse<String> response = HttpClient.newHttpClient()
                    .send(request, HttpResponse.BodyHandlers.ofString());
            baseUrl = "https://" + response.body() + ":8080/";
            System.out.println("Connected to: " + baseUrl);

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
            System.out.println("Initialized secure connection");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
