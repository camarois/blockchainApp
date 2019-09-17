package controllers;

import com.google.gson.Gson;
import com.google.gson.JsonObject;
import javafx.event.ActionEvent;

import java.io.InputStreamReader;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

public class Application {
    public void onClickHelloWorld(ActionEvent event) throws Exception {
        InputStreamReader reader = new InputStreamReader(getClass().getClassLoader().getResourceAsStream("values/strings.json"));
        JsonObject urls  = new Gson().fromJson(reader, JsonObject.class);

        try {
            HttpClient client = HttpClient.newHttpClient();
            HttpRequest request = HttpRequest.newBuilder()
                    .uri(URI.create(urls.get("host").getAsString() + "/lol"))
                    .build();

            HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
            System.out.println(response.body());
        } catch (Exception e) {
            System.out.println("Exception occurred, http call didn't work");
        }
    }
}