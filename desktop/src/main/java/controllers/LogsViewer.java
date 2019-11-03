package controllers;

import javafx.collections.FXCollections;
import javafx.collections.MapChangeListener;
import javafx.collections.ObservableList;
import javafx.collections.ObservableMap;
import javafx.fxml.FXML;
import javafx.scene.control.TableView;
import models.LogsRequest;
import models.LogsResponse;
import services.RestService;

import java.util.TimerTask;
import java.util.Timer;
import java.util.Collections;

public class LogsViewer {
    private ObservableMap<Integer, LogsResponse.Log> logs;
    private ObservableList<LogsResponse.Log> logsList;
    @FXML private TableView<LogsResponse.Log> logTableView;

    @FXML public void initialize() {
        logs = FXCollections.observableHashMap();
        logsList = FXCollections.observableArrayList();
        logTableView.setColumnResizePolicy(TableView.CONSTRAINED_RESIZE_POLICY);
        logTableView.setItems(logsList);
        startTask();

        logs.addListener((MapChangeListener<Integer, LogsResponse.Log>) mapChange -> {
            if (mapChange.wasAdded()) {
                logsList.add(mapChange.getValueAdded());
            }
            if (mapChange.wasRemoved()) {
                logsList.remove(mapChange.getValueRemoved());
            }
        });
    }

    private void startTask() {
        new Timer().scheduleAtFixedRate(new TimerTask() {
            public void run() {
                try {
                    LogsRequest request =  logs.keySet().isEmpty() ? new LogsRequest(0) :
                            new LogsRequest(Collections.max(logs.keySet()));
                    LogsResponse logsResponse =  RestService.postLogsAsync("serveurweb", request);

                    for (LogsResponse.Log log : logsResponse.logs) {
                        logs.put(log.getNumber(), log);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }, 0, 10000);
    }
}
