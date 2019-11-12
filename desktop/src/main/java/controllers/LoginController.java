package controllers;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import models.LoginRequest;
import models.LoginResponse;
import models.PasswordRequest;
import services.RestService;

import java.io.IOException;
import java.util.Objects;
import java.util.Optional;

public class LoginController {
    @FXML private TextField usernameTextField;
    @FXML private TextField passwordTextField;

    public void onClickLogin(ActionEvent event) {
        String username = usernameTextField.getText();
        String password = passwordTextField.getText();
        LoginRequest loginRequest = new LoginRequest(username, password);

        RestService.postRequestAsync(RestService.urls.getLogin(), loginRequest, LoginResponse.class, loginResponse -> {
            try {
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
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }, (e) -> showErrorDialog("Le nom d'utilisateur et/ou le mot de passe est invalide."));
    }

    private MenuBar createMenuBar() {
        Menu generalMenu = new Menu("Options");
        MenuItem changePasswordMenuItem = new MenuItem("Changer mot de passe");
        MenuItem createSupervisorMenuItem = new MenuItem("Creer un compte superviseur");
        MenuItem deleteSupervisorMenuItem = new MenuItem("Supprimer un compte superviseur");
        MenuItem logoutMenuItem = new MenuItem("Deconnexion");

        changePasswordMenuItem.setOnAction(actionEvent -> {
            Optional<PasswordRequest> request = showChangePasswordDialog();
            RestService.postRequestAsync(RestService.urls.getChangePassword(), request.get(), null, (e) -> {}, (e) -> {
                showErrorDialog("L'ancien mot de passe est invalide.");
            });
        });

        logoutMenuItem.setOnAction(actionEvent -> {
            try {
                RestService.postRequestAsync(RestService.urls.getLogout(), null);
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
        PasswordField textOldPassword = new PasswordField();
        PasswordField textNewPassword = new PasswordField();

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
