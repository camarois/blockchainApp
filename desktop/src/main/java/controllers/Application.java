package controllers;

import com.google.gson.Gson;
import constants.ServerUrls;
import java.io.InputStreamReader;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.stage.Stage;
import services.RestService;

public class Application {
  @FXML
  private Button pingButton;

  @FXML
  private void initialize() {
    pingButton.setOnAction(t -> new Thread(new Runnable() {
      @Override
      public void run() {
        try {
          HttpClient client = HttpClient.newHttpClient();
          HttpRequest request = HttpRequest.newBuilder()
                  .uri(URI.create(RestService.urls.firebase + "server"))
                  .build();

          HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
          System.out.println(response.body());
        } catch (Exception e) {
          System.out.println("Exception occurred, http call didn't work");
        }
      }
    }).start());
  }

//  public void onClickHelloWorld(ActionEvent event) throws Exception {
//    try {
//      HttpClient client = HttpClient.newHttpClient();
//      HttpRequest request = HttpRequest.newBuilder()
//          .uri(URI.create(urls.firebase + "server"))
//          .build();
//
//      HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
//      System.out.println(response.body());
//    } catch (Exception e) {
//      System.out.println("Exception occurred, http call didn't work");
//    }
//  }
}
