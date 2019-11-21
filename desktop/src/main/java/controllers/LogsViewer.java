package controllers;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.TableView;
import models.LogsRequest;
import models.LogsResponse;
import services.RestService;

import java.util.*;

public class LogsViewer {
    private int provenanceIndex = 0;
    private List<String> provenance = List.of("serveurweb", "1", "2", "3");
    private ObservableList<LogsResponse.Log> logsList;
    @FXML private TableView<LogsResponse.Log> logTableView;

    @FXML public void initialize() {
        logsList = FXCollections.observableArrayList();
        logTableView.setColumnResizePolicy(TableView.CONSTRAINED_RESIZE_POLICY);
        logTableView.setItems(logsList);
        startTask();
    }
    private void startTask() {
        new Timer().scheduleAtFixedRate(new TimerTask() {
            public void run() {
                try {
                    String origin = provenance.get(provenanceIndex);
                    LogsRequest request =  logsList.filtered(log -> log.getProvenance().equals((origin))).isEmpty() ? new LogsRequest(0) :
                            new LogsRequest(logsList.stream().filter(log -> log.getProvenance().equals(origin)).mapToInt(LogsResponse.Log::getNumber).max().getAsInt());
                    LogsResponse logsResponse = RestService.postRequest(RestService.urls.getLogs() + origin,
                            request, LogsResponse.class);
                    logsResponse.logs.forEach((log) -> log.setProvenance(origin));
                    logsList.addAll(logsResponse.logs);
                    provenanceIndex  = provenanceIndex == 3 ? 0 : provenanceIndex++;
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }, 250, 10000);
    }
}
