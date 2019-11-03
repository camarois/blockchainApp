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

import java.util.Map;
import java.util.TimerTask;
import java.util.Timer;
import java.util.HashMap;
import java.util.Collections;

public class LogsViewer {
    private ObservableMap<Integer, LogsResponse.Log> logs;
    private ObservableList<LogsResponse.Log> keys = FXCollections.observableArrayList();
    @FXML private TableView<LogsResponse.Log> logTableView;

    @FXML public void initialize() {
        logs = FXCollections.observableHashMap();
        logTableView.setColumnResizePolicy(TableView.CONSTRAINED_RESIZE_POLICY);
        logTableView.setItems(keys);
        startTask();

        logs.addListener((MapChangeListener<Integer, LogsResponse.Log>) mapChange -> {
            if (mapChange.wasAdded()) {
                keys.add(mapChange.getValueAdded());
            }
            if (mapChange.wasRemoved()) {
                keys.remove(mapChange.getValueRemoved());
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

                    Map<Integer, LogsResponse.Log> map = new HashMap<>();
                    for (LogsResponse.Log log : logsResponse.logs) {
                        map.put(log.getNumber(), log);
                    }
                    logs.putAll(map);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }, 0, 10000);
    }
}
