package controllers;

import com.google.gson.Gson;
import constants.ServerUrls;
import java.io.InputStreamReader;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import javafx.event.ActionEvent;

public class Application {
  /**
   * HelloWorld button event handler.
   * @param event click event.
   * @throws Exception Exception output.
   */
  public void onClickHelloWorld(ActionEvent event) throws Exception {
    InputStreamReader reader = new InputStreamReader(
        getClass().getClassLoader().getResourceAsStream("values/strings.json")
    );
    ServerUrls urls = new Gson().fromJson(reader, ServerUrls.class);

    try {
      HttpClient client = HttpClient.newHttpClient();
      HttpRequest request = HttpRequest.newBuilder()
          .uri(URI.create(urls.getHost() + urls.getLol()))
          .build();

      HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
      System.out.println(response.body());
    } catch (Exception e) {
      System.out.println("Exception occurred, http call didn't work");
    }
  }
}
