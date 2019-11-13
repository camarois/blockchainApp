package controllers;

import javafx.beans.property.SimpleBooleanProperty;
import javafx.beans.value.ObservableValue;
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
        MenuItem changePasswordMenuItem = new MenuItem("Changer mot de passe");
        changePasswordMenuItem.setOnAction(actionEvent -> {
            Optional<PasswordRequest> request = showChangePasswordDialog();
            RestService.postRequestAsync(RestService.urls.getChangePassword(), request.get(), null, (e) -> {
            }, (e) -> {
                    showErrorDialog("L'ancien mot de passe est invalide.");
                });
        });

        MenuItem createSupervisorMenuItem = new MenuItem("Creer un compte utilisateur");
        createSupervisorMenuItem.setOnAction(actionEvent -> {
            Optional<CreateUserRequest> request = showCreateUserDialog();
            RestService.postRequestAsync(RestService.urls.getCreateUser(), request.get());
        });

        MenuItem deleteSupervisorMenuItem = new MenuItem("Supprimer un compte superviseur");
        deleteSupervisorMenuItem.setOnAction(actionEvent -> {
            Optional<DeleteUserRequest> request = showDeleteUserDialog();
            RestService.postRequestAsync(RestService.urls.getDeleteUser(), request.get());
        });

        MenuItem logoutMenuItem = new MenuItem("Deconnexion");
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

        Menu generalMenu = new Menu("Options");
        generalMenu.getItems().addAll(changePasswordMenuItem, createSupervisorMenuItem, deleteSupervisorMenuItem,
                new SeparatorMenuItem(), logoutMenuItem);

        MenuBar menuBar = new MenuBar();
        menuBar.getMenus().addAll(generalMenu);

        return menuBar;
    }

    private Optional<CreateUserRequest> showCreateUserDialog() {
        Dialog<CreateUserRequest> dialog = new Dialog<>();
        dialog.setHeaderText(
                "Pour créer un compte utilisateur, veuillez rentrer les informations correspondantes ci-dessous.");
        dialog.setResizable(true);

        Label username = new Label("Nom d'utilisateur: ");
        Label password = new Label("Mot de passe: ");
        Label editor = new Label("Compte éditeur: ");
        Label admin = new Label("Compte administrateur: ");

        TextField textUsername = new TextField();
        PasswordField textPassword = new PasswordField();
        CheckBox isEditor = new CheckBox("Pour uploader des cours sur l'application android");
        CheckBox isAdmin = new CheckBox("Pour accéder à ce logiciel");

        GridPane grid = new GridPane();
        grid.add(username, 1, 1);
        grid.add(textUsername, 2, 1);
        grid.add(password, 1, 3);
        grid.add(textPassword, 2, 3);
        grid.add(editor, 1, 5);
        grid.add(isEditor, 2, 5);
        grid.add(admin, 1, 7);
        grid.add(isAdmin, 2, 7);
        dialog.getDialogPane().setContent(grid);

        ButtonType buttonTypeOk = new ButtonType("Envoyer", ButtonBar.ButtonData.OK_DONE);
        dialog.getDialogPane().getButtonTypes().add(buttonTypeOk);
        dialog.setResultConverter(b -> {
            if (b == buttonTypeOk) {
                return new CreateUserRequest(
                        new LoginRequest(
                                textUsername.getText(),
                                textPassword.getText()),
                        isEditor.isSelected(),
                        isAdmin.isSelected());
            }
            return null;
        });

        return dialog.showAndWait();
    }

    private Optional<DeleteUserRequest> showDeleteUserDialog() {
        Dialog<DeleteUserRequest> dialog = new Dialog<>();
        dialog.setHeaderText("Attention! Les suppressions sont permanentes.");
        dialog.setResizable(true);

        BorderPane root = new BorderPane();
        dialog.getDialogPane().setContent(root);
        TableView tblUsers = new TableView();

        TableColumn<String, User> colUsername = new TableColumn<>("Nom d'utilisateur");
        colUsername.setCellValueFactory(new PropertyValueFactory<>("username"));
        tblUsers.getColumns().add(colUsername);
        TableColumn<String, User> colAdmin = new TableColumn<>("Est admin");
        colAdmin.setCellValueFactory(new PropertyValueFactory<>("isAdmin"));
        tblUsers.getColumns().add(colAdmin);
        TableColumn<String, User> colEditor = new TableColumn<>("Est editeur");
        colEditor.setCellValueFactory(new PropertyValueFactory<>("isEditor"));
        tblUsers.getColumns().add(colEditor);
        TableColumn<User, Boolean> colDelete = new TableColumn<>("Supprimer");
        tblUsers.getColumns().add(colDelete);

        colDelete.setCellValueFactory(
                new Callback<>() {
                    @Override
                    public ObservableValue<Boolean> call(TableColumn.CellDataFeatures<User, Boolean> param) {
                        return new SimpleBooleanProperty(param.getValue() != null);
                    }
                }
        );

        Callback<TableColumn<User, Boolean>, TableCell<User, Boolean>> cellFactory
                = new Callback<TableColumn<User, Boolean>, TableCell<User, Boolean>>() {
                    @Override
                    public TableCell call(final TableColumn<User, Boolean> param) {
                        final TableCell<User, Boolean> cell = new TableCell<User, Boolean>() {
                            final Button btn = new Button("Supprimer");

                            @Override
                            public void updateItem(Boolean item, boolean empty) {
                                super.updateItem(item, empty);
                                if (empty) {
                                    setGraphic(null);
                                    setText(null);
                                } else {
                                    btn.setOnAction(event -> {
                                        User user = getTableView().getItems().get(getIndex());
                                        RestService.postRequestAsync(
                                                RestService.urls.getDeleteUser(),
                                                new DeleteUserRequest(user.getUsername()));
                                        tblUsers.getItems().remove(user);
                                    });
                                    setGraphic(btn);
                                    setText(null);
                                }
                            }
                        };
                        return cell;
                    }
                };

        colDelete.setCellFactory(cellFactory);
        root.setCenter(tblUsers);
        Window window = dialog.getDialogPane().getScene().getWindow();
        window.setOnCloseRequest(event -> window.hide());

        try {
            RestService.getRequestAsync(RestService.urls.getAllUsers(), AllUsersRequest.class, (allUsersRequest) -> {
                for (User user : allUsersRequest.getUsers()) {
                    tblUsers.getItems().add(user);
                }
            });
        } catch (Exception e) {
            System.out.println(e);
        }

        return dialog.showAndWait();
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
