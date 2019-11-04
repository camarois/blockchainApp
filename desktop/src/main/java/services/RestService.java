package services;

import com.google.gson.Gson;
import com.google.gson.JsonObject;
import constants.ServerUrls;
import models.*;

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
import java.util.concurrent.*;

import java.util.function.Consumer;
import java.lang.String;

public class RestService {
    private static final String HTTP_POST_METHOD = "POST";
    private static ServerUrls urls;
    private static String baseUrl = "";
    private static Gson gson;
    private static ExecutorService threadPool;
    private static HttpClient httpClient;
    private static RestService instance;

    private static void init() {
        gson = new Gson();
        InputStreamReader reader = new InputStreamReader(
                RestService.class.getClassLoader().getResourceAsStream("values/strings.json")
        );
        urls = gson.fromJson(reader, ServerUrls.class);

        threadPool = Executors.newFixedThreadPool(2);
        httpClient = HttpClient.newHttpClient();
        consumeRequestAsync(urls.firebase + "server", (resp) -> {
            baseUrl = "https://" + resp + ":10000/";
            System.out.println("Connected to: " + baseUrl);
        });
        initSslContext();
    }

    public static RestService getInstance() {
        if (instance == null) {
            instance = new RestService();
            init();
        }
        return instance;
    }

    public static Future postLoginAsync(LoginRequest request) {
        return callRequestAsync(
                () -> {
                    String resp = baseAsyncRequest(HTTP_POST_METHOD, "admin/login", request);
                    return gson.fromJson(resp, LoginResponse.class);
                });
    }

    public Future postLogoutAsync() {
        return callRequestAsync(
                () -> baseAsyncRequest(HTTP_POST_METHOD, "admin/logout", null));
    }

    public static Future postChangePasswordAsync(PasswordRequest request) {
        return callRequestAsync(
                () -> baseAsyncRequest(HTTP_POST_METHOD, "admin/motdepasse", request));
    }

    public static Future getChaineAsync(ChaineRequest request, Integer miner) {
        return callRequestAsync(
                () -> {
                    String resp = baseAsyncRequest(HTTP_POST_METHOD, "admin/chaine/" + miner, request);
                    return gson.fromJson(resp, JsonObject.class);
                });
    }

    public static Future postLogsAsync(String origin, LogsRequest request) {
        return callRequestAsync(
                () -> {
                    String resp = baseAsyncRequest(HTTP_POST_METHOD, "admin/logs/" + origin, request);
                    LogsResponse logsResponse = gson.fromJson(resp, LogsResponse.class);
                    logsResponse.logs.forEach((log) -> log.setProvenance(origin));
                    return logsResponse;
                });
    }

    private static void consumeRequestAsync(String url, Consumer<String> onResponse) {
        threadPool.submit(() -> {
            String resp = getRequest(url);
            onResponse.accept(resp);
        });
    }

    private static Future callRequestAsync(Callable<?> onResponse) {
        return threadPool.submit(onResponse);
    }

    private static String getRequest(String url) {
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

    private static String baseAsyncRequest(String method, String url, Object data) throws IOException,
            InterruptedException, HttpException {
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
            }
            else {
                throw new HttpException("Forbiddent");
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
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
