package controllers;

import models.LoginRequest;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.scene.layout.BorderPane;
import models.LoginResponse;
import services.RestService;
import java.util.Objects;

public class LoginController {
    @FXML private TextField usernameTextField;
    @FXML private TextField passwordTextField;

    private static LoginController instance;

    public LoginController() {
        instance = this;
    }

    public static LoginController getInstance() {
        return instance;
    }

    public void onClickLogin(ActionEvent event) throws Exception {
        String username = usernameTextField.getText();
        String password = passwordTextField.getText();
        LoginRequest loginRequest = new LoginRequest(username, password);

        LoginResponse loginResponse = RestService.getInstance().postLoginAsync(loginRequest);
        if (loginResponse != null) {
            BorderPane rootNode = new BorderPane();
            Parent logsViewer = FXMLLoader.load(
                    Objects.requireNonNull(getClass().getClassLoader().getResource("views/LogsViewer.fxml"))
            );
            rootNode.setCenter(logsViewer);
            Button btn = (Button) event.getSource();
            Scene scene = btn.getScene();
            scene.setRoot(rootNode);
        }
    }

    /* This displays an alert message to the user */
    public void showErrorDialog(String message) {
        Platform.runLater(() -> {
            Alert alert = new Alert(Alert.AlertType.WARNING);
            alert.setTitle("Attention!");
            alert.setHeaderText(message);
            alert.showAndWait();
        });

    }
}
