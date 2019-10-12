package controllers;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.ListView;
import javafx.scene.control.SelectionMode;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.List;

public class LogsViewer {

  //Nested classes
  class Log {
    public int number;
    public String severity;
    public Date date;
    public String message;
  }

  //Attributes
  private ObservableList<Log> logs;
  @FXML
  ListView<Log> logListView;

  /**
   * Initializer.
   */
  @FXML
  public void initialize() {
    logs = FXCollections.observableArrayList();
    loadLogsFromTxt();

    logListView.setItems(logs);
  }

  private void loadLogsFromTxt() {

    try {
      InputStream is = this.getClass().getClassLoader().getResourceAsStream("logs.txt");
      BufferedReader br = new BufferedReader(new InputStreamReader(is));
      String line;
      while ((line = br.readLine()) != null) {
        String[] items = line.split(":");
        Log log = new Log();
        log.number = Integer.parseInt(items[0].split(" ")[0]);
        log.severity = items[1].split(" ")[1];
        log.date = new SimpleDateFormat(" yyyy-MM-dd hh-mm-ss ").parse(items[2]);
        log.message = items[3].substring(1);
        logs.add(log);
      }
    } catch (IOException | ParseException e) {
      e.printStackTrace();
    }

  }

}