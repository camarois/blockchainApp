package controllers;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.collections.ObservableMap;
import javafx.fxml.FXML;
import javafx.scene.control.TableView;
import models.LogsRequest;
import models.LogsResponse;
import services.RestService;

import java.util.*;

public class LogsViewer {
    private ObservableMap<Integer, LogsResponse.Log> logs;
    @FXML private TableView<LogsResponse.Log> logTableView;

    @FXML public void initialize() {
        logs = FXCollections.observableHashMap();
        logTableView.setColumnResizePolicy(TableView.CONSTRAINED_RESIZE_POLICY);
        logTableView.setItems((ObservableList<LogsResponse.Log>) logs.values());
        //startTask();
    }

    private void startTask() {
        new Timer().scheduleAtFixedRate(new TimerTask() {
            public void run() {
                try {
                    LogsResponse logsResponse =  RestService.postLogsAsync("serveurweb", new LogsRequest(Collections.max(logs.keySet())));
                    HashMap<Integer, LogsResponse.Log> map = new HashMap<>();
                    for (LogsResponse.Log log : logsResponse.logs) map.put(log.getNumber(), log);
                    logs.putAll(map);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }, 250, 10000);
    }
}
