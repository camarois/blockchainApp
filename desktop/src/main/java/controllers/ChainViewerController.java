package controllers;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;

public class ChainViewerController {
    @FXML
    public TableView chainTableView;
    @FXML
    public TableColumn idCol;
    @FXML
    public TableColumn noCol;
    @FXML
    public TableColumn hashCol;
    @FXML
    public TableColumn contentCol;


    public void onClickRefresh(ActionEvent actionEvent) {
    }

}
