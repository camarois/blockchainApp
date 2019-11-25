package controllers;

import javafx.beans.binding.Bindings;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.VBox;
import models.AllUsersRequest;
import models.LogsRequest;
import models.LogsResponse;
import services.RestService;

import java.util.*;

public class LogsViewer {
    private List<String> provenance = List.of("serveurweb", "1", "2", "3");
    private ObservableList<LogsResponse.Log> logsList;
    @FXML public VBox vboxLogs;
    @FXML public TableColumn hourCol;
    @FXML public TableColumn noCol;
    @FXML public TableColumn severityCol;
    @FXML public TableColumn provenanceCol;
    @FXML public TableColumn messageCol;
    @FXML private TableView<LogsResponse.Log> logTableView;

    @FXML
    public void initialize() {
        logsList = FXCollections.observableArrayList();
        messageCol.prefWidthProperty().bind(
                logTableView.widthProperty()
                        .subtract(hourCol.widthProperty())
                        .subtract(noCol.widthProperty())
                        .subtract(severityCol.widthProperty())
                        .subtract(provenanceCol.widthProperty())
        );
        logTableView.setItems(logsList);
        updateTbl();
    }

    public void updateTbl() {
        try {
            for (String origin : provenance) {
                LogsRequest request = logsList.filtered(log ->
                        log.getProvenance().equals((origin))).isEmpty() ? new LogsRequest(0) :
                        new LogsRequest(logsList.stream().filter(log ->
                                log.getProvenance().equals(origin)
                        ).mapToInt(LogsResponse.Log::getNumber).max().getAsInt());
                RestService.postRequestAsync(RestService.urls.getLogs() + origin, request, LogsResponse.class,
                        logsResponse -> {
                            logsResponse.logs.forEach((log) -> log.setProvenance(origin));
                            logsList.addAll(logsResponse.logs);
                        }, (e) -> showErrorDialog("Le journal de l'application n'a pu etre mis a jour."));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void onClickRefresh(ActionEvent actionEvent) {
        updateTbl();
    }

    private void showErrorDialog(String message) {
        Alert alert = new Alert(Alert.AlertType.WARNING);
        alert.setTitle("Attention!");
        alert.setHeaderText(message);
        alert.showAndWait();
    }
}
