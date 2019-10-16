package controllers;

import javafx.fxml.FXML;
import javafx.scene.control.Button;
import services.RestService;

public class Application {
    @FXML
    private Button pingButton;

    @FXML
    private void initialize() {
        pingButton.setOnAction(event -> {
            RestService.requestAsync(RestService.urls.ping, (resp) -> {
                System.out.println(resp);
            });
        });
    }
}
