package controllers;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.TableView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class LogsViewer {

    public static class Log {

        private String number;
        private String severity;
        private String date;
        private String provenance;
        private String message;

        void setNumber(int number) {
            this.number = Integer.toString(number);
        }

        void setSeverity(String severity) {
            this.severity = severity;
        }

        void setDate(Date date) {
            this.date = new SimpleDateFormat("hh:mm:ss").format(date);
        }

        void setProvenance(String provenance) {
            this.provenance = provenance;
        }

        void setMessage(String message) {
            this.message = message;
        }

        public String getNumber() {
            return number;
        }

        public String getSeverity() {
            return severity;
        }

        public String getDate() {
            return date;
        }

        public String getProvenance() {
            return provenance;
        }

        public String getMessage() {
            return message;
        }
    }

    private ObservableList<Log> logs;
    @FXML
    private TableView<Log> logTableView;

    @FXML
    public void initialize() {
        logs = FXCollections.observableArrayList();
        loadLogsFromTxt();
        logTableView.setItems(logs);
    }

    private void loadLogsFromTxt() { //TODO: Retiner le fichier texte quand on va avoir setup la BD
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
