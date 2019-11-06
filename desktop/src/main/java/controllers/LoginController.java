package controllers;

import javafx.scene.control.TextField;
import javafx.scene.control.MenuItem;
import javafx.scene.control.Menu;
import javafx.scene.control.Button;
import javafx.scene.control.Alert;
import javafx.scene.control.MenuBar;
import javafx.scene.control.Dialog;
import javafx.scene.control.Label;
import javafx.scene.control.SeparatorMenuItem;
import javafx.scene.control.ButtonType;
import javafx.scene.control.ButtonBar;

import javafx.scene.layout.GridPane;
import models.LoginRequest;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import models.LoginResponse;
import models.PasswordRequest;
import services.RestService;

import java.util.Objects;
import java.util.Optional;

public class LoginController {
    @FXML private TextField usernameTextField;
    @FXML private TextField passwordTextField;

    public void onClickLogin(ActionEvent event) {
        String username = usernameTextField.getText();
        String password = passwordTextField.getText();
        LoginRequest loginRequest = new LoginRequest(username, password);

        try {
            LoginResponse loginResponse = (LoginResponse) RestService.postLoginAsync(loginRequest).get();
            if (loginResponse != null) {
                BorderPane rootNode = new BorderPane();
                Parent logsViewer = FXMLLoader.load(
                        Objects.requireNonNull(getClass().getClassLoader().getResource("views/LogsViewer.fxml"))
                );
                rootNode.setCenter(logsViewer);
                MenuBar menuBar = createMenuBar();
                rootNode.setTop(menuBar);

                Button btn = (Button) event.getSource();
                Scene scene = btn.getScene();
                scene.setRoot(rootNode);
            }
        } catch (Exception e) {
            showErrorDialog("Le nom d'utilisateur et/ou le mot de passe est invalide.");
        }
    }

    private MenuBar createMenuBar() {
        Menu generalMenu = new Menu("Options");
        MenuItem changePasswordMenuItem = new MenuItem("Changer mot de passe");
        MenuItem createSupervisorMenuItem = new MenuItem("Creer un compte superviseur");
        MenuItem deleteSupervisorMenuItem = new MenuItem("Supprimer un compte superviseur");
        MenuItem logoutMenuItem = new MenuItem("Deconnexion");

        changePasswordMenuItem.setOnAction(actionEvent -> {
            try {
                Optional<PasswordRequest> request = showChangePasswordDialog();
                RestService.postChangePasswordAsync(request.get()).get();
            } catch (Exception e) {
                showErrorDialog("L'ancien mot de passe est invalide.");
            }
        });

        logoutMenuItem.setOnAction(actionEvent -> {
            try {
                RestService.getInstance().postLogoutAsync();
                BorderPane rootNode = new BorderPane();
                Parent loginViewer = FXMLLoader.load(
                        Objects.requireNonNull(getClass().getClassLoader().getResource("views/Login.fxml"))
                );
                rootNode.setCenter(loginViewer);
                MenuItem menuItem = (MenuItem) actionEvent.getTarget();
                Scene scene = menuItem.getParentPopup().getOwnerWindow().getScene();
                scene.setRoot(rootNode);

            } catch (Exception e) {
                e.printStackTrace();
            }
        });

        generalMenu.getItems().addAll(changePasswordMenuItem, createSupervisorMenuItem, deleteSupervisorMenuItem,
                new SeparatorMenuItem(), logoutMenuItem);

        MenuBar menuBar = new MenuBar();
        menuBar.getMenus().addAll(generalMenu);

        return menuBar;
    }

    private Optional<PasswordRequest> showChangePasswordDialog() {
        Dialog<PasswordRequest> dialog = new Dialog<>();
        dialog.setHeaderText("Pour changer votre mot de passe, veuillez mettre l'ancien et le nouveau.");
        dialog.setResizable(true);

        Label oldPassword = new Label("Ancien: ");
        Label newPassword = new Label("Nouveau: ");
        TextField textOldPassword = new TextField();
        TextField textNewPassword = new TextField();

        GridPane grid = new GridPane();
        grid.add(oldPassword, 1, 1);
        grid.add(textOldPassword, 2, 1);
        grid.add(newPassword, 1, 3);
        grid.add(textNewPassword, 2, 3);
        dialog.getDialogPane().setContent(grid);

        ButtonType buttonTypeOk = new ButtonType("Envoyer", ButtonBar.ButtonData.OK_DONE);
        dialog.getDialogPane().getButtonTypes().add(buttonTypeOk);
        dialog.setResultConverter(b -> {
            if (b == buttonTypeOk) {
                return new PasswordRequest(textOldPassword.getText(), textNewPassword.getText());
            }
            return null;
        });

        return dialog.showAndWait();
    }

    private void showErrorDialog(String message) {
        Alert alert = new Alert(Alert.AlertType.WARNING);
        alert.setTitle("Attention!");
        alert.setHeaderText(message);
        alert.showAndWait();
    }
}
