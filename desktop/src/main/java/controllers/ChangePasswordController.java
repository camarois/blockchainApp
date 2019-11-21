package controllers;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.ButtonBar;
import javafx.scene.control.ButtonType;
import javafx.scene.control.TextField;
import models.PasswordRequest;
import services.RestService;

public class ChangePasswordController {

    @FXML
    private TextField oldPwd;

    @FXML
    private TextField newPwd;

    @FXML
    private TextField confPwd;

    public void onClickSubmit(ActionEvent actionEvent) {
        String oldPwdValue = oldPwd.getText();
        String newPwdValue = newPwd.getText();
        String confPwdValue = confPwd.getText();
        if (oldPwdValue.equals("") || newPwdValue.equals("") || confPwdValue.equals("")) {
            showErrorDialog("Les mots de passe sont invalides.");
        } else if (!newPwdValue.equals(confPwdValue)) {
            showErrorDialog("Les nouveaux mots de passe ne sont pas identiques");
        } else {
            RestService.postRequestAsync(
                RestService.urls.getChangePassword(),
                new PasswordRequest(oldPwdValue, newPwdValue),
                null,
                (e) -> {},
                (e) -> {
                    showErrorDialog("L'ancien mot de passe est invalide");
                });
        }
    }

    private void showErrorDialog(String message) {
        Alert alert = new Alert(Alert.AlertType.WARNING);
        alert.setTitle("Attention!");
        alert.setHeaderText(message);
        alert.showAndWait();
    }
}
