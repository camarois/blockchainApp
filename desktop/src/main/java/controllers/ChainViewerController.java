package controllers;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.ComboBox;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import models.ChainRequest;
import models.ChainResponse;
import services.RestService;

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
    @FXML
    public ComboBox<String> minerChooser;
    @FXML
    public TableColumn verifNum;

    @FXML
    public void initialize() {
        fillCombobox();
        contentCol.prefWidthProperty().bind(
                chainTableView.widthProperty()
                        .subtract(idCol.widthProperty())
                        .subtract(noCol.widthProperty())
                        .subtract(hashCol.widthProperty())
        );
    }

    public void onClickRefresh(ActionEvent actionEvent) {
        String value = minerChooser.getValue();
        getChainDataFromMiner(value.charAt(value.length() - 1));
    }

    public void fillCombobox() {

        ObservableList<String> miners = FXCollections.observableArrayList();
        for (int i = 1; i <= 3; i++) {
            miners.add("Mineur " + i);
        }

        minerChooser.getItems().addAll(miners);
    }

    public void onComboBoxSelect(ActionEvent actionEvent) {
        String value = minerChooser.getValue();
        getChainDataFromMiner(value.charAt(value.length() - 1));
    }

    private void getChainDataFromMiner(char miner) {
        RestService.postRequestAsync(RestService.urls.getChain() + miner,
            new ChainRequest(10),
            ChainResponse.class,
            (e) -> {
                chainTableView.getItems().clear();
                chainTableView.getItems().addAll(e.getBlocks());
            });
    }
}
