package controllers;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.CheckBox;
import javafx.scene.control.TextField;
import models.CreateUserRequest;
import models.LoginRequest;
import models.RegisterResponse;
import services.RestService;

public class CreateUserController {
    @FXML
    private TextField newUsernameTextField;
    @FXML
    private TextField newPasswordTextField;
    @FXML
    private TextField newConfirmPasswordTextField;
    @FXML
    private CheckBox isEditorCheckBox;
    @FXML
    private CheckBox isAdminCheckBox;

    public void onClickCreate(ActionEvent event) {
        String username = newUsernameTextField.getText();
        String password = newPasswordTextField.getText();
        String conformPassword = newConfirmPasswordTextField.getText();
        Boolean isEditor = isEditorCheckBox.isSelected();
        Boolean isAdmin = isAdminCheckBox.isSelected();

        if (username.isEmpty() || password.isEmpty() || conformPassword.isEmpty()) {
            showWarningDialog("Le nom d'utilisateur ou le mot de passe est invalide");
        } else if (!password.equals(conformPassword)) {
            showWarningDialog("Les mots de passe ne sont pas identiques");
        } else {
            RestService.postRequestAsync(RestService.urls.getCreateUser(),
                    new CreateUserRequest(
                            new LoginRequest(
                                    username,
                                    password),
                            isEditor,
                            isAdmin),
                    RegisterResponse.class,
                registerResponse -> {
                    showConfirmationDialog("Creation du compte reussie.");
                },
                (e) -> showErrorDialog("Erreur dans la creation du compte")
            );
        }
    }

    private void showWarningDialog(String message) {
        Alert alert = new Alert(Alert.AlertType.WARNING);
        alert.setTitle("Attention!");
        alert.setHeaderText(message);
        alert.showAndWait();
    }

    private void showConfirmationDialog(String message) {
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle("Attention!");
        alert.setHeaderText(message);
        alert.showAndWait();
    }

    private void showErrorDialog(String message) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle("Attention!");
        alert.setHeaderText(message);
        alert.showAndWait();
    }
}
