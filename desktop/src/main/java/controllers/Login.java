package controllers;

import javafx.event.ActionEvent;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.BorderPane;

import java.util.Objects;

public class Login {

    public void onClickLogin(ActionEvent event) throws Exception {
        BorderPane rootNode = new BorderPane();
        Parent logsViewer = FXMLLoader.load(
                Objects.requireNonNull(getClass().getClassLoader().getResource("views/LogsViewer.fxml"))
        );
        rootNode.setCenter(logsViewer);
        Button btn = (Button) event.getSource();
        Scene scene = btn.getScene();
        scene.setRoot(rootNode);
    }
}