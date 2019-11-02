package controllers;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.TableView;
import models.LogsRequest;
import models.LogsResponse;
import services.RestService;

import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Collections;
import java.util.Comparator;

public class LogsViewer {
    private ObservableList<LogsResponse.Log> logs;
    @FXML private TableView<LogsResponse.Log> logTableView;

    @FXML public void initialize() {
        logs = FXCollections.observableArrayList();
        logTableView.setColumnResizePolicy(TableView.CONSTRAINED_RESIZE_POLICY);
        logTableView.setItems(logs);
        startTask();
    }

    private void startTask() {
        new Timer().scheduleAtFixedRate(new TimerTask() {
            public void run() {
                try {
                    LogsResponse logsResponse =  RestService.postLogsAsync("serveurweb", new LogsRequest(0));
                    logsResponse.logs.forEach((log) -> {
                        if (!logs.contains(log)) {
                            logs.add(log);
                        }
                    });
                    Collections.sort(logs, Comparator.comparing(LogsResponse.Log::getHour));
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }, 250, 30000);
    }
}
