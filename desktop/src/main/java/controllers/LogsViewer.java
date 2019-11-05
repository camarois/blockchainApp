package controllers;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.TableView;
import models.LogsRequest;
import models.LogsResponse;
import services.RestService;

import java.util.TimerTask;
import java.util.Timer;
import java.util.Collections;

public class LogsViewer {
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
                    LogsRequest request =  logsList.isEmpty() ? new LogsRequest(0) :
                            new LogsRequest(Collections.max(logsList, (log1, log2) -> {
                                if (log1.getNumber() > log2.getNumber()) {
                                    return 1;
                                } else if (log1.getNumber() < log2.getNumber()) {
                                    return -1;
                                }
                                return 0;
                            }).getNumber());
                    LogsResponse logsResponse = (LogsResponse) RestService.postLogsAsync("serveurweb", request)
                            .get();
                    logsList.addAll(logsResponse.logs);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }, 250, 10000);
    }
}
