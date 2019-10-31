package controllers;

import javafx.scene.control.*;

import javafx.scene.layout.GridPane;
import javafx.util.Callback;
import models.LoginRequest;
import javafx.application.Platform;
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
            MenuBar menuBar = createMenuBar();
            rootNode.setTop(menuBar);

            Button btn = (Button) event.getSource();
            Scene scene = btn.getScene();
            scene.setRoot(rootNode);
        }
    }

    public MenuBar createMenuBar() {
        MenuBar menuBar = new MenuBar();
        Menu generalMenu = new Menu("Options");
        MenuItem changePasswordMenuItem = new MenuItem("Changer mot de passe");
        MenuItem createSupervisorMenuItem = new MenuItem("Créer un compte superviseur");
        MenuItem deleteSupervisorMenuItem = new MenuItem("Supprimer un compte superviseur");
        MenuItem logoutMenuItem = new MenuItem("Déconnexion");

        changePasswordMenuItem.setOnAction(actionEvent -> {
            try {
                Optional<PasswordRequest> request = showChangePasswordDialog();
                //RestService.getInstance().postChangePasswordAsync(request.get());
            } catch (Exception e) {
                e.printStackTrace();
            }
        });

        logoutMenuItem.setOnAction(actionEvent -> {
            try {
                //RestService.getInstance().postLogoutAsync();
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

        menuBar.getMenus().addAll(generalMenu);

        return menuBar;
    }

    public Optional<PasswordRequest> showChangePasswordDialog() {
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

    public void showErrorDialog(String message) {
        Platform.runLater(() -> {
            Alert alert = new Alert(Alert.AlertType.WARNING);
            alert.setTitle("Attention!");
            alert.setHeaderText(message);
            alert.showAndWait();
        });
    }
}
