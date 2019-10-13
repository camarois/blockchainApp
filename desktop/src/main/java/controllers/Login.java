package controllers;

import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXMLLoader;
import javafx.scene.Group;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;

import java.util.Objects;

public class Login {
  /**
   * Login btn event handler.
   * @param event event
   * @throws Exception Exception
   */
  public void onClickLogin(ActionEvent event) throws Exception {
    Button btn = (Button) event.getSource();
    Scene scene = btn.getScene();
    Group rootNode = new Group();
    ObservableList<javafx.scene.Node> children = rootNode.getChildren();
    Parent topMenu = FXMLLoader.load(
        Objects.requireNonNull(getClass().getClassLoader().getResource("views/TopMenuBar.fxml"))
    );
    Parent logsViewer = FXMLLoader.load(
            Objects.requireNonNull(getClass().getClassLoader().getResource("views/LogsViewer.fxml"))
    );
    children.add(topMenu);
    children.add(logsViewer);
    scene.setRoot(rootNode);
  }
}
