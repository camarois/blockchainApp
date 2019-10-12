package controllers;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.control.Label;
import javafx.scene.control.ListCell;
import javafx.scene.control.ListView;
import javafx.scene.layout.HBox;
import javafx.util.Callback;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class LogsViewer {

  //Nested classes
  class Log {

    public void setNumber(int number) {
      this.number = number;
      this.logNum.setText(Integer.toString(number));
    }

    public void setSeverity(String severity) {
      this.severity = severity;
      this.logSev.setText(severity);
    }

    public void setDate(Date date) {
      this.date = date;
      SimpleDateFormat formatter = new SimpleDateFormat("hh:mm:ss");
      this.logDat.setText(formatter.format(date));
    }

    void setProvenance(String provenance) {
      this.provenance = provenance;
      this.logPro.setText(provenance);
    }

    public void setMessage(String message) {
      this.message = message;
      this.logMes.setText(message);
    }

    int number;
    String severity;
    Date date;
    String provenance;
    String message;

    @FXML
    HBox haBox;
    @FXML
    Label logNum;
    @FXML
    Label logSev;
    @FXML
    Label logDat;
    @FXML
    Label logPro;
    @FXML
    Label logMes;

    public Log() {
      FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("/views/LogEntry.fxml"));
      fxmlLoader.setController(this);
      try {
        fxmlLoader.load();
      } catch (IOException e) {
        throw new RuntimeException(e);
      }
    }

    public HBox getBox() {
      return haBox;
    }
  }

  public class ListViewCell extends ListCell<Log> {
    @Override
    public void updateItem(Log log, boolean empty) {
      super.updateItem(log, empty);
      if (log != null) {
        setGraphic(log.getBox());
      }
    }
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
    logListView.setCellFactory(new Callback<ListView<Log>, ListCell<Log>>() {
          @Override
          public ListCell<Log> call(ListView<Log> param) {
            return new ListViewCell();
          }
        }
    );
  }

  private void loadLogsFromTxt() {
    try {
      InputStream is = this.getClass().getClassLoader().getResourceAsStream("logs.txt");
      BufferedReader br = new BufferedReader(new InputStreamReader(is));
      String line;
      while ((line = br.readLine()) != null) {
        String[] items = line.split(":");
        Log log = new Log();
        log.setNumber(Integer.parseInt(items[0].split(" ")[0]));
        log.setSeverity(items[1].split(" ")[1]);
        log.setDate(new SimpleDateFormat(" yyyy-MM-dd hh-mm-ss ").parse(items[2]));
        log.setProvenance(items[3].split(" ")[1]);
        log.setMessage(items[4].substring(1));
        logs.add(log);
      }
    } catch (IOException | ParseException e) {
      e.printStackTrace();
    }

  }

}