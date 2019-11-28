package controllers;

import javafx.beans.binding.Bindings;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import models.Block;
import models.ChainRequest;
import models.ChainResponse;
import services.RestService;

import java.util.ArrayList;

public class ChainViewerController {
    @FXML
    public TableView<Block> chainTableView;
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
        //                        .subtract(verifNum.widthProperty())
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
            (chainResponse) -> {
                chainTableView.getItems().clear();
                if (chainResponse.getBlocks().isEmpty()) {
                    showInformationDialog("La chaine de blocs est vide");
                }
                chainTableView.setRowFactory(tv -> new TableRow<Block>() {
                    @Override
                    public void updateItem(Block item, boolean empty) {
                        super.updateItem(item, empty);
                        if (item == null) {
                            setStyle("");
                        } else if (item.getVerifNumber() == 1) {
                            setStyle("-fx-background-color: #ff7979; -fx-border-color: black; -fx-border-width: 1px;");
                        } else if (item.getVerifNumber() == 2) {
                            setStyle("-fx-background-color: #ffe55f; -fx-border-color: black; -fx-border-width: 1px;");
                        } else if (item.getVerifNumber() >= 3) {
                            setStyle("-fx-background-color: #7eff86; -fx-border-color: black; -fx-border-width: 1px;");
                        } else {
                            setStyle("");
                        }
                    }
                });

                int maxVal = 1;
                ArrayList<Block> blocks = chainResponse.getBlocks();
                for (int i = blocks.size() - 1; i >= 0; i--) {
                    if (blocks.get(i).getVerifNumber() > maxVal) {
                        maxVal = blocks.get(i).getVerifNumber();
                    } else if (blocks.get(i).getVerifNumber() < maxVal) {
                        blocks.get(i).setVerifNumber(maxVal);
                    }
                }

                chainTableView.getItems().addAll(chainResponse.getBlocks());

                chainTableView.setFixedCellSize(80);
                chainTableView.prefHeightProperty().bind(Bindings.size(chainTableView.getItems())
                        .multiply(chainTableView.getFixedCellSize()).add(40));

            }, (e) -> {
                chainTableView.getItems().clear();
                showErrorDialog("Le mineur est inaccessible");
            }
        );
    }

    private void showInformationDialog(String message) {
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
