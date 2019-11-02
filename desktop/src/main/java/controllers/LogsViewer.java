package controllers;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.collections.ObservableSet;
import javafx.fxml.FXML;
import javafx.scene.control.TableView;
import models.LogsResponse;
import services.RestService;

import java.util.Timer;
import java.util.TimerTask;

public class LogsViewer {
    private ObservableList<LogsResponse.Log> logs = FXCollections.observableArrayList();
    @FXML private TableView<LogsResponse.Log> logTableView;

    @FXML public void initialize() {
        logTableView.setItems(logs);
        startTask();
        RestService.executeScheduledRequest();
    }

    private void startTask() {
        new Timer().scheduleAtFixedRate(new TimerTask() {
            public void run() {
                try {
                    logs.addAll(RestService.getLogs());
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }, 0, 2000);
    }
}
