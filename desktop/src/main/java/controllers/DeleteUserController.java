package controllers;

import javafx.beans.binding.Bindings;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.TableCell;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.layout.VBox;
import javafx.util.Callback;
import models.AllUsersRequest;
import models.DeleteUserRequest;
import models.User;
import services.RestService;

public class DeleteUserController {
    @FXML
    public VBox vboxDeleteUser;

    @FXML
    private TableView<User> usersTableView;

    @FXML
    public void initialize() {
        TableColumn colDelete = usersTableView.getColumns().get(3);
        usersTableView.setFixedCellSize(35);
        usersTableView.prefHeightProperty().bind(Bindings.size(usersTableView.getItems())
                .multiply(usersTableView.getFixedCellSize()).add(30));

        Callback<TableColumn<User, Boolean>, TableCell<User, Boolean>> cellFactory
                = new Callback<>() {
                    @Override
                    public TableCell call(final TableColumn<User, Boolean> param) {
                        final TableCell<User, Boolean> cell = new TableCell<>() {
                            Button btn = new Button("Supprimer");

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
                                        usersTableView.getItems().remove(user);
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

        usersTableView.setColumnResizePolicy(TableView.CONSTRAINED_RESIZE_POLICY);
        updateTbl();
    }

    public void updateTbl() {
        RestService.getRequestAsync(RestService.urls.getAllUsers(), AllUsersRequest.class, (allUsersRequest) -> {
            usersTableView.getItems().clear();
            usersTableView.getItems().addAll(allUsersRequest.getUsers());
        });
    }

    public void onClickRefresh(ActionEvent actionEvent) {
        updateTbl();
    }
}
