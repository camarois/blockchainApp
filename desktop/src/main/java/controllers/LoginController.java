package controllers;

import javafx.beans.property.SimpleBooleanProperty;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.stage.Window;
import javafx.util.Callback;
import models.*;
import services.RestService;

import java.io.IOException;
import java.util.Objects;
import java.util.Optional;

public class LoginController {
    @FXML
    private TextField usernameTextField;
    @FXML
    private TextField passwordTextField;

    public void onClickLogin(ActionEvent event) {
        String username = usernameTextField.getText();
        String password = passwordTextField.getText();
        LoginRequest loginRequest = new LoginRequest(username, password);

        RestService.postRequestAsync(RestService.urls.getLogin(), loginRequest, LoginResponse.class, loginResponse -> {
            TabPane tabPane = createTabMenu();
            Button btn = (Button) event.getSource();
            Scene scene = btn.getScene();
            scene.setRoot(tabPane);
        }, (e) -> showErrorDialog("Le nom d'utilisateur et/ou le mot de passe est invalide."));
    }

    private TabPane createTabMenu() {
        TabPane tabPane = new TabPane();
        try {
            Parent logsViewer = FXMLLoader.load(
                    Objects.requireNonNull(getClass().getClassLoader().getResource("views/LogsViewer.fxml"))
            );
            Tab logTab = new Tab("Logs", logsViewer);
            logTab.setClosable(false);
            tabPane.getTabs().add(logTab);

            Parent createUser = FXMLLoader.load(
                    Objects.requireNonNull(getClass().getClassLoader().getResource("views/CreateUserTab.fxml"))
            );
            BorderPane createUserBp = new BorderPane();
            createUserBp.setCenter(createUser);
            Tab tabCreateUser = new Tab("Nouvel utilisateur", createUserBp);
            tabCreateUser.setClosable(false);
            tabPane.getTabs().add(tabCreateUser);

            Parent deleteUser = FXMLLoader.load(
                    Objects.requireNonNull(getClass().getClassLoader().getResource("views/DeleteUsersTab.fxml"))
            );
            Tab tabDeleteUser = new Tab("Supprimer un utilisateur", deleteUser);
            tabDeleteUser.setClosable(false);
            tabPane.getTabs().add(tabDeleteUser);

            Parent changePwd = FXMLLoader.load(
                    Objects.requireNonNull(getClass().getClassLoader().getResource("views/ChangePasswordView.fxml"))
            );
            BorderPane changePwdBp = new BorderPane();
            changePwdBp.setCenter(changePwd);
            Tab tabChangePwd = new Tab("Changer de mot de passe", changePwdBp);
            tabChangePwd.setClosable(false);
            tabPane.getTabs().add(tabChangePwd);
        } catch (IOException ex) {
            ex.printStackTrace();
        }

        return tabPane;
    }

    private void showErrorDialog(String message) {
        Alert alert = new Alert(Alert.AlertType.WARNING);
        alert.setTitle("Attention!");
        alert.setHeaderText(message);
        alert.showAndWait();
    }
}
